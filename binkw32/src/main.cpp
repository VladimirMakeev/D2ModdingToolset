/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#include "game.h"
#include "hooks.h"
#include "log.h"
#include "settings.h"
#include "version.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>
#include <filesystem>
#include <fmt/format.h>
#include <string>

template <typename T>
static void writeProtectedMemory(T* address, T value)
{
    DWORD oldProtection{};
    if (VirtualProtect(address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtection)) {
        *address = value;
        VirtualProtect(address, sizeof(T), oldProtection, &oldProtection);
        return;
    }

    hooks::logError("binkwProxyError.log",
                    fmt::format("Failed to change memory protection for {:p}", (void*)address));
}

static void adjustGameRestrictions()
{
    auto& variables = game::gameVariables();
    // Allow game to load scenarios with maximum allowed spells level set to zero,
    // disabling usage of magic in scenario
    writeProtectedMemory(variables.spellMinLevel, 0);
}

static void setupHooks()
{
    auto& fn = game::gameFunctions();

    DetourAttach((PVOID*)&fn.respopupInit, (PVOID)hooks::respopupInitHooked);
    DetourAttach((PVOID*)&fn.toggleShowBannersInit, (PVOID)hooks::toggleShowBannersInitHooked);
    DetourAttach((PVOID*)&fn.processUnitModifiers, (PVOID)hooks::processUnitModifiersHooked);
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD reason, LPVOID reserved)
{
    if (reason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    DisableThreadLibraryCalls(hDll);

    HMODULE module = GetModuleHandle(NULL);
    std::string moduleName(MAX_PATH, '\0');

    GetModuleFileName(module, &moduleName[0], MAX_PATH - 1);

    std::filesystem::path exeFilePath{moduleName};

    const std::error_code error = hooks::determineGameVersion(exeFilePath);
    if (error || hooks::gameVersion() == hooks::GameVersion::Unknown) {
        const std::string msg{
            fmt::format("Failed to determine game version.\nReason: {:s}.", error.message())};

        hooks::logError("binkwProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "binkw32.dll proxy", MB_OK);
        return FALSE;
    }

    exeFilePath.remove_filename();
    hooks::readUserSettings(exeFilePath / "disciple.ini");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    adjustGameRestrictions();
    setupHooks();

    const auto result = DetourTransactionCommit();
    if (result != NO_ERROR) {
        const std::string msg{
            fmt::format("Failed to hook game functions. Error code: {:d}.", result)};

        hooks::logError("binkwProxyError.log", msg);
        MessageBox(NULL, msg.c_str(), "binkw32.dll proxy", MB_OK);
        return FALSE;
    }

    return TRUE;
}
