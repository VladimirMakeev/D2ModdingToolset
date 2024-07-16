/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef TIMER_H
#define TIMER_H

#include "log.h"
#include <chrono>
#include <fmt/format.h>
#include <string_view>

namespace hooks {

// #define D2_MEASURE_EVENTS_TIME

/** Measures scope execution time and writes it to the log file. */
class ScopedTimer
{
    using Clock = std::chrono::high_resolution_clock;

public:
    ScopedTimer(std::string_view description, std::string_view log)
        : description{description}
        , log{log}
        , start{Clock::now()}
    { }

    ~ScopedTimer()
    {
        const auto elapsed{Clock::now() - start};
        const auto us{std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()};
        logDebug(log, fmt::format("{:s} time {:d} us", description, us));
    }

private:
    const std::string_view description;
    const std::string_view log;
    const Clock::time_point start;
};

/** Measures scope execution time and accumulates it using provided reference. */
class ScopedValueTimer
{
    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::microseconds;
    using Type = Duration::rep;

public:
    ScopedValueTimer(Type& value)
        : value{value}
        , start{Clock::now()}
    { }

    ~ScopedValueTimer()
    {
        const auto elapsed{Clock::now() - start};
        const auto us{std::chrono::duration_cast<Duration>(elapsed).count()};
        value += us;
    }

private:
    Type& value;
    const Clock::time_point start;
};

} // namespace hooks

#endif // TIMER_H
