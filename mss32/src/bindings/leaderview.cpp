/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "leaderview.h"
#include "game.h"
#include "groundcat.h"
#include "leaderabilitycat.h"
#include "leadercategory.h"
#include "midunit.h"
#include "usleader.h"
#include "usstackleader.h"
#include <sol/sol.hpp>

namespace bindings {

LeaderView::LeaderView(const game::CMidUnit* unit)
    : UnitView(unit)
{ }

void LeaderView::bind(sol::state& lua)
{
    auto leader = lua.new_usertype<LeaderView>("LeaderView", sol::base_classes,
                                               sol::bases<UnitView>());
    leader["type"] = sol::property(&LeaderView::getCategory);
    leader["movement"] = sol::property(&LeaderView::getMovement);
    leader["scout"] = sol::property(&LeaderView::getScout);
    leader["leadership"] = sol::property(&LeaderView::getLeadership);
    leader["hasAbility"] = &LeaderView::hasAbility;
    leader["hasMoveBonus"] = &LeaderView::hasMoveBonus;
}

int LeaderView::getCategory() const
{
    auto leader{game::gameFunctions().castUnitImplToLeader(unit->unitImpl)};
    if (!leader) {
        return 0;
    }

    auto category{leader->vftable->getCategory(leader)};
    if (!category) {
        return 0;
    }

    return static_cast<int>(category->id);
}

int LeaderView::getMovement() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(unit->unitImpl)};

    return leader ? leader->vftable->getMovement(leader) : 0;
}

int LeaderView::getScout() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(unit->unitImpl)};

    return leader ? leader->vftable->getScout(leader) : 0;
}

int LeaderView::getLeadership() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(unit->unitImpl)};

    return leader ? leader->vftable->getLeadership(leader) : 0;
}

bool LeaderView::hasAbility(int abilityId) const
{
    using namespace game;

    auto leader{game::gameFunctions().castUnitImplToStackLeader(unit->unitImpl)};
    if (!leader) {
        return false;
    }

    auto leaderHasAbility{leader->vftable->hasAbility};
    const auto& abilities{LeaderAbilityCategories::get()};

    switch (static_cast<LeaderAbilityId>(abilityId)) {
    case LeaderAbilityId::Incorruptible:
        return leaderHasAbility(leader, abilities.incorruptible);
    case LeaderAbilityId::WeaponMaster:
        return leaderHasAbility(leader, abilities.weaponMaster);
    case LeaderAbilityId::WandScrollUse:
        return leaderHasAbility(leader, abilities.wandScrollUse);
    case LeaderAbilityId::WeaponArmorUse:
        return leaderHasAbility(leader, abilities.weaponArmorUse);
    case LeaderAbilityId::BannerUse:
        return leaderHasAbility(leader, abilities.bannerUse);
    case LeaderAbilityId::JewelryUse:
        return leaderHasAbility(leader, abilities.jewelryUse);
    case LeaderAbilityId::Rod:
        return leaderHasAbility(leader, abilities.rod);
    case LeaderAbilityId::OrbUse:
        return leaderHasAbility(leader, abilities.orbUse);
    case LeaderAbilityId::TalismanUse:
        return leaderHasAbility(leader, abilities.talismanUse);
    case LeaderAbilityId::TravelItemUse:
        return leaderHasAbility(leader, abilities.travelItemUse);
    case LeaderAbilityId::CriticalHit:
        return leaderHasAbility(leader, abilities.criticalHit);
    }

    return false;
}

bool LeaderView::hasMoveBonus(int groundId) const
{
    using namespace game;

    auto leader{game::gameFunctions().castUnitImplToStackLeader(unit->unitImpl)};
    if (!leader) {
        return false;
    }

    auto hasBonus{leader->vftable->hasMovementBonus};
    const auto& grounds{GroundCategories::get()};

    switch (static_cast<GroundId>(groundId)) {
    case GroundId::Plain:
        return hasBonus(leader, grounds.plain);
    case GroundId::Forest:
        return hasBonus(leader, grounds.forest);
    case GroundId::Water:
        return hasBonus(leader, grounds.water);
    case GroundId::Mountain:
        return hasBonus(leader, grounds.mountain);
    }

    return false;
}

} // namespace bindings
