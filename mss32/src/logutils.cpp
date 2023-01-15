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

namespace hooks {

std::string getNetPlayerIdDesc(int netPlayerId)
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

std::string getMidgardIdTypeDesc(const game::CMidgardID* id)
{
    using namespace game;

    auto type = CMidgardIDApi::get().getType(id);

#define RETURN_TYPE_DESC(TYPE)                                                                     \
    if (type == IdType::##TYPE)                                                                    \
        return #TYPE;

    RETURN_TYPE_DESC(Empty)
    RETURN_TYPE_DESC(ApplicationText)
    RETURN_TYPE_DESC(Building)
    RETURN_TYPE_DESC(Race)
    RETURN_TYPE_DESC(Lord)
    RETURN_TYPE_DESC(Spell)
    RETURN_TYPE_DESC(UnitGlobal)
    RETURN_TYPE_DESC(UnitGenerated)
    RETURN_TYPE_DESC(UnitModifier)
    RETURN_TYPE_DESC(Attack)
    RETURN_TYPE_DESC(TextGlobal)
    RETURN_TYPE_DESC(LandmarkGlobal)
    RETURN_TYPE_DESC(ItemGlobal)
    RETURN_TYPE_DESC(NobleAction)
    RETURN_TYPE_DESC(DynamicUpgrade)
    RETURN_TYPE_DESC(DynamicAttack)
    RETURN_TYPE_DESC(DynamicAltAttack)
    RETURN_TYPE_DESC(DynamicAttack2)
    RETURN_TYPE_DESC(DynamicAltAttack2)
    RETURN_TYPE_DESC(CampaignFile)
    RETURN_TYPE_DESC(Plan)
    RETURN_TYPE_DESC(ObjectCount)
    RETURN_TYPE_DESC(ScenarioFile)
    RETURN_TYPE_DESC(Map)
    RETURN_TYPE_DESC(MapBlock)
    RETURN_TYPE_DESC(ScenarioInfo)
    RETURN_TYPE_DESC(SpellEffects)
    RETURN_TYPE_DESC(Fortification)
    RETURN_TYPE_DESC(Player)
    RETURN_TYPE_DESC(PlayerKnownSpells)
    RETURN_TYPE_DESC(Fog)
    RETURN_TYPE_DESC(PlayerBuildings)
    RETURN_TYPE_DESC(Road)
    RETURN_TYPE_DESC(Stack)
    RETURN_TYPE_DESC(Unit)
    RETURN_TYPE_DESC(Landmark)
    RETURN_TYPE_DESC(Item)
    RETURN_TYPE_DESC(Bag)
    RETURN_TYPE_DESC(Site)
    RETURN_TYPE_DESC(Ruin)
    RETURN_TYPE_DESC(Tomb)
    RETURN_TYPE_DESC(Rod)
    RETURN_TYPE_DESC(Crystal)
    RETURN_TYPE_DESC(Diplomacy)
    RETURN_TYPE_DESC(SpellCast)
    RETURN_TYPE_DESC(Location)
    RETURN_TYPE_DESC(StackTemplate)
    RETURN_TYPE_DESC(Event)
    RETURN_TYPE_DESC(StackDestroyed)
    RETURN_TYPE_DESC(TalismanCharges)
    RETURN_TYPE_DESC(Mountains)
    RETURN_TYPE_DESC(SubRace)
    RETURN_TYPE_DESC(QuestLog)
    RETURN_TYPE_DESC(TurnSummary)
    RETURN_TYPE_DESC(ScenarioVariable)
    RETURN_TYPE_DESC(Invalid)

    return "";
}

} // namespace hooks
