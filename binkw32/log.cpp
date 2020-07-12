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

#include "log.h"
#include "settings.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>

namespace hooks {

static void logAction(const std::string& logFile, const std::string& message)
{
    using namespace std::chrono;

    std::ofstream file(logFile.c_str(), std::ios_base::app);
    const std::time_t time{std::time(nullptr)};
    const std::tm tm = *std::localtime(&time);

    file << "[" << std::put_time(&tm, "%c") << "] " << message << "\n";
}

void logDebug(const std::string& logFile, const std::string& message)
{
    if (userSettings().debugMode) {
        logAction(logFile, message);
    }
}

void logError(const std::string& logFile, const std::string& message)
{
    logAction(logFile, message);
}

} // namespace hooks
