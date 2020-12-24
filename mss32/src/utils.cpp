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

#include "utils.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace hooks {

std::string trimSpaces(const std::string& str)
{
    const auto begin = str.find_first_not_of(" ");
    if (begin == std::string::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" ");
    return str.substr(begin, end - begin + 1);
}

const std::filesystem::path& gameFolder()
{
    static std::filesystem::path folder{};

    if (folder.empty()) {
        folder = exePath();
        folder.remove_filename();
    }

    return folder;
}

const std::filesystem::path& exePath()
{
    static std::filesystem::path exe{};

    if (exe.empty()) {
        HMODULE module = GetModuleHandle(NULL);
        std::string moduleName(MAX_PATH, '\0');
        GetModuleFileName(module, &moduleName[0], MAX_PATH - 1);

        exe = std::filesystem::path(moduleName);
    }

    return exe;
}

} // namespace hooks
