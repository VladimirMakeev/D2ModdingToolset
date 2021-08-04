/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef MIDEVENT_H
#define MIDEVENT_H

#include "d2vector.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "sortedracelist.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct CMidEvCondition;
struct CMidEvEffect;

/** Holds event related data in scenario file and game. */
struct CMidEvent : public IMidScenarioObject
{
    CMidgardID eventId;
    StringAndId name;
    SortedRaceList affectedRaces; /**< Races to which conditions and effects can apply. */
    /** Races that can trigger event. 'Verified races' in game terms. */
    SortedRaceList racesCanTrigger;
    bool enabled;
    bool occurOnce;
    char padding[2];
    int order; /**< Event evaluation order. */
    int chance;
    Vector<CMidEvCondition*> conditions;
    Vector<CMidEvEffect*> effects;
};

static_assert(sizeof(CMidEvent) == 120, "Size of CMidEvent structure must be exactly 120 bytes");

static_assert(offsetof(CMidEvent, affectedRaces) == 20,
              "CMidEvent::affectedRaces offset must be 20 bytes");

static_assert(offsetof(CMidEvent, racesCanTrigger) == 48,
              "CMidEvent::racesCanTrigger offset must be 48 bytes");

static_assert(offsetof(CMidEvent, chance) == 84, "CMidEvent::chance offset must be 84 bytes");

} // namespace game

#endif // MIDEVENT_H
