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

#include "mapgen.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fmt/format.h>

namespace hooks {

/** Converts WinApi error code returned from GetLastError into readable string. */
static std::string winApiErrorToString(DWORD errorCode)
{
    LPVOID messageBuffer;
    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                           | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPTSTR)&messageBuffer, 0, NULL)) {
        // Failed to format message string, return error code as is
        return fmt::format("%d", errorCode);
    }

    std::string message((LPCSTR)messageBuffer);
    LocalFree(messageBuffer);

    return message;
}

bool showMapGeneratorDialog(std::string& errorMessage)
{
    static const char libraryName[] = "GeneratorSupportLib.dll";
    HMODULE library = LoadLibrary(libraryName);
    if (!library) {
        errorMessage = fmt::format("Failed to load {:s}.\nError: {:s}", libraryName,
                                   winApiErrorToString(GetLastError()));
        return false;
    }

    static const char functionName[] = "test";
    using DllInstance = HWND (*)(void);
    DllInstance instance = (DllInstance)GetProcAddress(library, functionName);
    if (!instance) {
        errorMessage = fmt::format("Could not find symbol '{:s}' in {:s}.", functionName,
                                   libraryName);
        return false;
    }

    HWND handle = instance();
    if (!handle) {
        errorMessage = fmt::format("Function '{:s}' from {:s} returned invalid window handle.",
                                   functionName, libraryName);
        return false;
    }

    // 'MQ_UIManager' is a name used to register window class
    HWND currentHandle = FindWindowEx(nullptr, nullptr, "MQ_UIManager", nullptr);
    if (!currentHandle) {
        errorMessage = fmt::format("Could not find game window handle.");
        return false;
    }

    SetParent(handle, currentHandle);
    ShowWindow(handle, SW_SHOWMAXIMIZED);
    return true;
}

} // namespace hooks
