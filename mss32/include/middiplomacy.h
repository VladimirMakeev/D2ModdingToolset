/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef MIDDIPLOMACY_H
#define MIDDIPLOMACY_H

#include "midscenarioobject.h"
#include "d2vector.h"
#include <cstdint>

namespace game {

struct LRaceCategory;

/*
 Relation contents:
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 | B| W|  |  |  |  |   turn when alliance was made  | previous relation  |  current relation  | A|
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0

 A - races are in alliance
 W - races are always at war
 B - AI can not break alliance
*/

struct DiplomacyRelation
{
    std::uint8_t race1CategoryId;
    std::uint8_t race2CategoryId;
    char padding[2];
    std::uint32_t relation;
};

assert_size(DiplomacyRelation, 8);

struct CMidDiplomacy : public IMidScenarioObject
{
    Vector<DiplomacyRelation> relations;
};

assert_size(CMidDiplomacy, 24);

namespace CMidDiplomacyApi {

struct Api
{
    using GetRelation = std::uint32_t*(__thiscall*)(const CMidDiplomacy* thisptr,
                                                    std::uint32_t* relation,
                                                    const LRaceCategory* race1,
                                                    const LRaceCategory* race2);
    /** Returns current diplomacy relation in range [0 : 100] between two races. */
    GetRelation getCurrentRelation;
    /** Returns previous diplomacy relation in range [0 : 100] between two races. */
    GetRelation getPreviousRelation;

    using GetBool = bool(__thiscall*)(const CMidDiplomacy* thisptr,
                                      const LRaceCategory* race1,
                                      const LRaceCategory* race2);
    /** Returns true if race1 and race2 are in alliance. */         
    GetBool areAllies;
    /** Returns turn number when two races made an alliance. */
    using GetAllianceTurn = std::uint32_t*(__thiscall*)(const CMidDiplomacy* thisptr,
                                                        std::uint32_t* turn,
                                                        const LRaceCategory* race1,
                                                        const LRaceCategory* race2);
    GetAllianceTurn getAllianceTurn;
    /** Returns true if race1 and race2 are always at war. */
    GetBool areAlwaysAtWar;
    /** Returns true if diplomacy relations prohibit AI-controlled race from breaking alliance. */
    GetBool aiCouldNotBreakAlliance;

    using CheckRelationType = bool(__stdcall*)(const std::uint32_t* relation);
    /** Returns true if relation value is in range [0 : D_WAR]. */
    CheckRelationType areRelationsAtWar;
    /** Returns true if relation value is in range (D_WAR : D_NEUTRAL]. */
    CheckRelationType areRelationsNeutral;
    /** Returns true if relation value is in range (D_NEUTRAL : 100]. */
    CheckRelationType areRelationsPeaceful;
};

Api& get();

}

}

#endif // MIDDIPLOMACY_H