/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma comment(lib, "detours.lib")

#include "customattacks.h"
#include "hooks.h"
#include "log.h"
#include "restrictions.h"
#include "settings.h"
#include "unitsforhire.h"
#include "utils.h"
#include "version.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>
#include <fmt/format.h>
#include <string>

static HMODULE library{};
static void* registerInterface{};
static void* unregisterInterface{};

extern "C" __declspec(naked) void __stdcall RIB_register_interface(void)
{
    __asm {
        jmp registerInterface;
    }
}

extern "C" __declspec(naked) void __stdcall RIB_unregister_interface(void)
{
    __asm {
        jmp unregisterInterface;
    }
}

template <typename T>
static void writeProtectedMemory(T* address, T value)
{
    DWORD oldProtection{};
    if (VirtualProtect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtection)) {
        *address = value;
        VirtualProtect(address, sizeof(T), oldProtection, &oldProtection);
        return;
    }

    hooks::logError("mssProxyError.log",
                    fmt::format("Failed to change memory protection for {:p}", (void*)address));
}

template <typename T>
static void adjustRestrictionMax(game::Restriction<T>* restriction,
                                 const T& value,
                                 const char* name)
{
    if (value >= restriction->min) {
        hooks::logDebug("restrictions.log", fmt::format("Set '{:s}' to {:d}", name, value));
        writeProtectedMemory(&restriction->max, value);
        return;
    }

    hooks::logError(
        "mssProxyError.log",
        fmt::format(
            "User specified '{:s}' value of {:d} is less than minimum value allowed in game ({:d}). "
            "Change rejected.",
            name, value, restriction->min));
}

static void adjustGameRestrictions()
{
    using namespace hooks;

    auto& restrictions = game::gameRestrictions();
    // Allow game to load and scenario editor to create scenarios with maximum allowed spells level
    // set to zero, disabling usage of magic in scenario
    writeProtectedMemory(&restrictions.spellLevel->min, 0);
    // Allow using units with tier higher than 5
    writeProtectedMemory(&restrictions.unitLevel->max, 10);

    if (userSettings().unitMaxDamage != baseSettings().unitMaxDamage) {
        adjustRestrictionMax(restrictions.unitDamage, userSettings().unitMaxDamage,
                             "UnitMaxDamage");
    }

    if (userSettings().unitMaxArmor != baseSettings().unitMaxArmor) {
        adjustRestrictionMax(restrictions.unitArmor, userSettings().unitMaxArmor, "UnitMaxArmor");
    }

    if (userSettings().stackScoutRangeMax != baseSettings().stackScoutRangeMax) {
        adjustRestrictionMax(restrictions.stackScoutRange, userSettings().stackScoutRangeMax,
                             "StackMaxScoutRange");
    }

    if (executableIsGame()) {
        if (userSettings().criticalHitDamage != baseSettings().criticalHitDamage) {
            logDebug("restrictions.log", fmt::format("Set 'criticalHitDamage' to {:d}",
                                                     (int)userSettings().criticalHitDamage));
            writeProtectedMemory(restrictions.criticalHitDamage, userSettings().criticalHitDamage);
        }

        if (userSettings().mageLeaderAttackPowerReduction
            != baseSettings().mageLeaderAttackPowerReduction) {
            logDebug("restrictions.log",
                     fmt::format("Set 'mageLeaderPowerReduction' to {:d}",
                                 (int)userSettings().mageLeaderAttackPowerReduction));
            writeProtectedMemory(restrictions.mageLeaderAttackPowerReduction,
                                 userSettings().mageLeaderAttackPowerReduction);
        }
    }
}

static bool setupHook(hooks::HookInfo& hook)
{
    hooks::logDebug("mss32Proxy.log", fmt::format("Try to attach hook. Function {:p}, hook {:p}.",
                                                  hook.target, hook.hook));

    // hook.original is an optional field that can point to where the new address of the original
    // function should be placed.
    void** pointer = hook.original ? hook.original : (void**)&hook.original;
    *pointer = hook.target;

    auto result = DetourAttach(pointer, hook.hook);
    if (result != NO_ERROR) {
        const std::string msg{
            fmt::format("Failed to attach hook. Function {:p}, hook {:p}. Error code: {:d}.",
                        hook.target, hook.hook, result)};

        hooks::logError("mssProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "mss32.dll proxy", MB_OK);
        return false;
    }

    return true;
}

static bool setupHooks()
{
    auto hooks{hooks::getHooks()};

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    for (auto& hook : hooks) {
        if (!setupHook(hook)) {
            return false;
        }
    }

    const auto result = DetourTransactionCommit();
    if (result != NO_ERROR) {
        const std::string msg{
            fmt::format("Failed to commit detour transaction. Error code: {:d}.", result)};

        hooks::logError("mssProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "mss32.dll proxy", MB_OK);
        return false;
    }

    hooks::logDebug("mss32Proxy.log", "All hooks are set");
    return true;
}

static void setupVftableHooks()
{
    for (const auto& hook : hooks::getVftableHooks()) {
        void** target = (void**)hook.target;
        if (hook.original)
            *hook.original = *target;

        writeProtectedMemory(target, hook.hook);
    }

    hooks::logDebug("mss32Proxy.log", "All vftable hooks are set");
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_DETACH) {
        FreeLibrary(library);
        return TRUE;
    }

    if (reason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    library = LoadLibrary("Mss23.dll");
    if (!library) {
        MessageBox(NULL, "Failed to load Mss23.dll", "mss32.dll proxy", MB_OK);
        return FALSE;
    }

    registerInterface = GetProcAddress(library, "RIB_register_interface");
    unregisterInterface = GetProcAddress(library, "RIB_unregister_interface");
    if (!registerInterface || !unregisterInterface) {
        MessageBox(NULL, "Could not load Mss23.dll addresses", "mss32.dll proxy", MB_OK);
        return FALSE;
    }

    DisableThreadLibraryCalls(hDll);

    const auto error = hooks::determineGameVersion(hooks::exePath());
    if (error || hooks::gameVersion() == hooks::GameVersion::Unknown) {
        const std::string msg{
            fmt::format("Failed to determine target exe type.\nReason: {:s}.", error.message())};

        hooks::logError("mssProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "mss32.dll proxy", MB_OK);
        return FALSE;
    }

    if (hooks::executableIsGame() && !hooks::loadUnitsForHire(hooks::gameFolder())) {
        MessageBox(NULL, "Failed to load new units. Check error log for details.",
                   "mss32.dll proxy", MB_OK);
        return FALSE;
    }

    hooks::initializeCustomAttacks();

    adjustGameRestrictions();
    setupVftableHooks();
    return setupHooks();
}
