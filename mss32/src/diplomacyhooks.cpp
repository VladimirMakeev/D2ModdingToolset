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

#include "diplomacyhooks.h"
#include "globaldata.h"
#include "globalvariables.h"
#include "middiplomacy.h"
#include "racecategory.h"

namespace hooks {

static const game::DiplomacyRelation* findRelation(
    const game::Vector<game::DiplomacyRelation>& relations,
    const game::LRaceCategory* race1,
    const game::LRaceCategory* race2)
{
    if (!race1 || !race2) {
        return nullptr;
    }

    using namespace game;

    const std::uint8_t id1 = static_cast<std::uint8_t>(race1->id);
    const std::uint8_t id2 = static_cast<std::uint8_t>(race2->id);

    for (const DiplomacyRelation* i = relations.bgn; i != relations.end; ++i) {
        if (i->race1CategoryId == id1 && i->race2CategoryId == id2) {
            return i;
        }

        if (i->race1CategoryId == id2 && i->race2CategoryId == id1) {
            return i;
        }
    }

    return nullptr;
}

std::uint32_t* __fastcall getCurrentRelationHooked(const game::CMidDiplomacy* thisptr,
                                                   int /*%edx*/,
                                                   std::uint32_t* current,
                                                   const game::LRaceCategory* race1,
                                                   const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        *current = 0u;
    } else {
        *current = (relation->relation >> 1) & 0x7fu;
    }

    return current;
}

std::uint32_t* __fastcall getPreviousRelationHooked(const game::CMidDiplomacy* thisptr,
                                                    int /*%edx*/,
                                                    std::uint32_t* previous,
                                                    const game::LRaceCategory* race1,
                                                    const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        *previous = 0u;
    } else {
        *previous = (relation->relation >> 8) & 0x7fu;
    }

    return previous;
}

bool __fastcall areAlliesHooked(const game::CMidDiplomacy* thisptr,
                                int /*%edx*/,
                                const game::LRaceCategory* race1,
                                const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        return false;
    }

    return relation->relation & 1u;
}

std::uint32_t* __fastcall getAllianceTurnHooked(const game::CMidDiplomacy* thisptr,
                                                int /*%edx*/,
                                                std::uint32_t* turn,
                                                const game::LRaceCategory* race1,
                                                const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        *turn = 0u;
    } else {
        *turn = (relation->relation << 2u) >> 17u;
    }

    return turn;
}

bool __fastcall areAlwaysAtWarHooked(const game::CMidDiplomacy* thisptr,
                                     int /*%edx*/,
                                     const game::LRaceCategory* race1,
                                     const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        return false;
    }

    return (relation->relation >> 30u) & 1u;
}

bool __fastcall aiCouldNotBreakAllianceHooked(const game::CMidDiplomacy* thisptr,
                                              int /*%edx*/,
                                              const game::LRaceCategory* race1,
                                              const game::LRaceCategory* race2)
{
    const auto* relation = findRelation(thisptr->relations, race1, race2);
    if (!relation) {
        return false;
    }

    return relation->relation >> 31u;
}

bool __stdcall areRelationsAtWarHooked(const std::uint32_t* relation)
{
    using namespace game;

    const GlobalData* data = *GlobalDataApi::get().getGlobalData();
    const GlobalVariables* variables = *data->globalVariables;

    return *relation <= variables->diplomacyWar;
}

bool __stdcall areRelationsNeutralHooked(const std::uint32_t* relation)
{
    using namespace game;

    const GlobalData* data = *GlobalDataApi::get().getGlobalData();
    const GlobalVariables* variables = *data->globalVariables;

    return *relation > variables->diplomacyWar && *relation <= variables->diplomacyNeutral;
}

bool __stdcall areRelationsPeacefulHooked(const std::uint32_t* relation)
{
    using namespace game;

    const GlobalData* data = *GlobalDataApi::get().getGlobalData();
    const GlobalVariables* variables = *data->globalVariables;

    return *relation > variables->diplomacyNeutral;
}

}