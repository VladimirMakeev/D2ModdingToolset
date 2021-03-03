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
    auto& restrictions = game::gameRestrictions();
    // Allow game to load and scenario editor to create scenarios with maximum allowed spells level
    // set to zero, disabling usage of magic in scenario
    writeProtectedMemory(&restrictions.spellLevel->min, 0);
    // Allow using units with tier higher than 5
    writeProtectedMemory(&restrictions.unitLevel->max, 10);

    auto& settings = hooks::userSettings();

    if (settings.unitMaxDamage > 0) {
        adjustRestrictionMax(restrictions.unitDamage, settings.unitMaxDamage, "UnitMaxDamage");
    }

    if (settings.unitMaxArmor > 0) {
        adjustRestrictionMax(restrictions.unitArmor, settings.unitMaxArmor, "UnitMaxArmor");
    }

    if (settings.stackScoutRangeMax > 0) {
        adjustRestrictionMax(restrictions.stackScoutRange, settings.stackScoutRangeMax,
                             "StackMaxScoutRange");
    }

    if (hooks::executableIsGame() && settings.criticalHitDamage > 0) {
        hooks::logDebug("restrictions.log", fmt::format("Set 'criticalHitDamage' to {:d}",
                                                        (int)settings.criticalHitDamage));
        writeProtectedMemory(restrictions.criticalHitDamage, settings.criticalHitDamage);
    }
}

static bool setupHook(const hooks::HookInfo& hook)
{
    hooks::logDebug("mss32Proxy.log", fmt::format("Try to attach hook. Function {:p}, hook {:p}.",
                                                  *hook.first, hook.second));

    auto result = DetourAttach(hook.first, hook.second);
    if (result != NO_ERROR) {
        const std::string msg{
            fmt::format("Failed to attach hook. Function {:p}, hook {:p}. Error code: {:d}.",
                        *hook.first, hook.second, result)};

        hooks::logError("mssProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "mss32.dll proxy", MB_OK);
        return false;
    }

    return true;
}

static bool setupHooks()
{
    const auto hooks{hooks::getHooks()};

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    for (const auto& hook : hooks) {
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

    adjustGameRestrictions();
    return setupHooks();
}
