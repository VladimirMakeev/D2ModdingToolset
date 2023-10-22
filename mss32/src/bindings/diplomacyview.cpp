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

#include "diplomacyview.h"
#include "diplomacyhooks.h"
#include "racecategory.h"
#include <sol/sol.hpp>

namespace bindings {

static const game::LRaceCategory* getRaceById(game::RaceId categoryId)
{
    using namespace game;

    const auto& races = RaceCategories::get();

    if (races.human->id == categoryId) {
        return races.human;
    }

    if (races.dwarf->id == categoryId) {
        return races.dwarf;
    }

    if (races.heretic->id == categoryId) {
        return races.heretic;
    }

    if (races.undead->id == categoryId) {
        return races.undead;
    }

    if (races.neutral->id == categoryId) {
        return races.neutral;
    }

    if (races.elf->id == categoryId) {
        return races.elf;
    }

    return nullptr;
}

DiplomacyView::DiplomacyView(const game::CMidDiplomacy* diplomacy)
    : diplomacy{diplomacy}
{}

void DiplomacyView::bind(sol::state& lua)
{
    // clang-format off
    lua.new_enum("Relation",
        "War", RelationType::War,
        "Neutral", RelationType::Neutral,
        "Peace", RelationType::Peace
    );
    // clang-format on

    auto view = lua.new_usertype<DiplomacyView>("DiplomacyView");
    view["getCurrentRelation"] = &DiplomacyView::getCurrentRelation;
    view["getPreviousRelation"] = &DiplomacyView::getPreviousRelation;
    view["getAlliance"] = &DiplomacyView::getAlliance;
    view["getAllianceTurn"] = &DiplomacyView::getAllianceTurn;
    view["getAlwaysAtWar"] = &DiplomacyView::getAlwaysAtWar;
    view["getAiCouldNotBreakAlliance"] = &DiplomacyView::getAiCouldNotBreakAlliance;
    view["getRelationType"] = &DiplomacyView::getRelationType;
}

std::uint32_t DiplomacyView::getCurrentRelation(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    std::uint32_t relation = 0u;
    hooks::getCurrentRelationHooked(diplomacy, 0, &relation, race1, race2);
    return relation;
}

std::uint32_t DiplomacyView::getPreviousRelation(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    std::uint32_t relation = 0u;
    hooks::getPreviousRelationHooked(diplomacy, 0, &relation, race1, race2);
    return relation;
}

bool DiplomacyView::getAlliance(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    return hooks::areAlliesHooked(diplomacy, 0, race1, race2);
}

std::uint32_t DiplomacyView::getAllianceTurn(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    std::uint32_t turn = 0u;
    hooks::getAllianceTurnHooked(diplomacy, 0, &turn, race1, race2);
    return turn;
}

bool DiplomacyView::getAlwaysAtWar(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    return hooks::areAlwaysAtWarHooked(diplomacy, 0, race1, race2);
}

bool DiplomacyView::getAiCouldNotBreakAlliance(int race1CategoryId, int race2CategoryId) const
{
    using namespace game;

    const LRaceCategory* race1 = getRaceById(static_cast<RaceId>(race1CategoryId));
    const LRaceCategory* race2 = getRaceById(static_cast<RaceId>(race2CategoryId));

    return hooks::aiCouldNotBreakAllianceHooked(diplomacy, 0, race1, race2);
}

int DiplomacyView::getRelationType(std::uint32_t relations) const
{
    using namespace hooks;

    if (areRelationsAtWarHooked(&relations)) {
        return static_cast<int>(RelationType::War);
    }

    if (areRelationsNeutralHooked(&relations)) {
        return static_cast<int>(RelationType::Neutral);
    }

    if (areRelationsPeacefulHooked(&relations)) {
        return static_cast<int>(RelationType::Peace);
    }

    // This should never happen
    // Assume war relations
    return static_cast<int>(RelationType::War);
}

}