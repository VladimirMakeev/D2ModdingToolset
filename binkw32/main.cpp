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
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <detours.h>
#include <fmt/format.h>
#include <string>

static void setupHooks()
{
    DetourAttach((PVOID*)&game::respopupInit, (PVOID)hooks::RespopupInitHooked);
    DetourAttach((PVOID*)&game::toggleShowBannersInit, (PVOID)hooks::ToggleShowBannersInitHooked);
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hDll);

        hooks::readUserSettings();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        setupHooks();

        const auto result = DetourTransactionCommit();
        if (result != NO_ERROR) {
            const std::string msg{
                fmt::format("Failed to hook game functions. Error code: {:d}", result)};

            hooks::logError("binkwProxyError.log", msg);
            MessageBox(NULL, msg.c_str(), "binkw32.dll proxy", MB_OK);
        }
    }

    return TRUE;
}
