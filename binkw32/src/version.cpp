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

#include "version.h"

namespace hooks {

static GameVersion version{GameVersion::Unknown};

GameVersion gameVersion()
{
    return version;
}

std::error_code determineGameVersion(const std::filesystem::path& exeFilePath)
{
    std::error_code errorCode;
    const auto exeFileSize = std::filesystem::file_size(exeFilePath, errorCode);
    if (errorCode) {
        return errorCode;
    }

    // Determine game version by executable file size
    switch (exeFileSize) {
    case 3907200:
        version = GameVersion::Akella;
        break;

    case 4187648:
        version = GameVersion::Russobit;
        break;

    case 4474880:
        version = GameVersion::Gog;
        break;

    default:
        version = GameVersion::Unknown;
        break;
    }

    return {};
}

} // namespace hooks
