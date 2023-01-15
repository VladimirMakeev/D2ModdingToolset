/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "logutils.h"
#include "midgardid.h"
#include "netmsg.h"
#include <array>

namespace hooks {

const char* getNetPlayerIdDesc(int netPlayerId)
{
    using namespace game;

    switch (netPlayerId) {
    case broadcastNetPlayerId:
        return "broadcast";
    case serverNetPlayerId:
        return "server";
    default:
        return "client";
    }
}

const char* getMidgardIdTypeDesc(const game::CMidgardID* id)
{
    using namespace game;

    static std::array<const char*, (size_t)IdType::Invalid + 1> descriptions = {{
        "Empty",
        "ApplicationText",
        "Building",
        "Race",
        "Lord",
        "Spell",
        "UnitGlobal",
        "UnitGenerated",
        "UnitModifier",
        "Attack",
        "TextGlobal",
        "LandmarkGlobal",
        "ItemGlobal",
        "NobleAction",
        "DynamicUpgrade",
        "DynamicAttack",
        "DynamicAltAttack",
        "DynamicAttack2",
        "DynamicAltAttack2",
        "CampaignFile",
        "CW",
        "CO",
        "Plan",
        "ObjectCount",
        "ScenarioFile",
        "Map",
        "MapBlock",
        "ScenarioInfo",
        "SpellEffects",
        "Fortification",
        "Player",
        "PlayerKnownSpells",
        "Fog",
        "PlayerBuildings",
        "Road",
        "Stack",
        "Unit",
        "Landmark",
        "Item",
        "Bag",
        "Site",
        "Ruin",
        "Tomb",
        "Rod",
        "Crystal",
        "Diplomacy",
        "SpellCast",
        "Location",
        "StackTemplate",
        "Event",
        "StackDestroyed",
        "TalismanCharges",
        "MT",
        "Mountains",
        "SubRace",
        "BR",
        "QuestLog",
        "TurnSummary",
        "ScenarioVariable",
        "Invalid",
    }};

    auto type = (std::uint32_t)CMidgardIDApi::get().getType(id);
    return type < descriptions.size() ? descriptions[type] : "UNKNOWN ID TYPE";
}

} // namespace hooks
