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

#include "unitimplview.h"
#include "attackview.h"
#include "dynupgradeview.h"
#include "game.h"
#include "globaldata.h"
#include "groundcat.h"
#include "leaderabilitycat.h"
#include "leadercategory.h"
#include "log.h"
#include "midsubrace.h"
#include "racecategory.h"
#include "racetype.h"
#include "unitutils.h"
#include "usleader.h"
#include "ussoldier.h"
#include "usstackleader.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

UnitImplView::UnitImplView(const game::IUsUnit* unitImpl)
    : impl(unitImpl)
{ }

void UnitImplView::bind(sol::state& lua)
{
    auto impl = lua.new_usertype<UnitImplView>("UnitImpl");
    impl["id"] = sol::property(&UnitImplView::getId);
    impl["level"] = sol::property(&UnitImplView::getLevel);
    impl["xpNext"] = sol::property(&UnitImplView::getXpNext);
    impl["xpKilled"] = sol::property(&UnitImplView::getXpKilled);
    impl["hitPoint"] = sol::property(&UnitImplView::getHitPoint);
    impl["armor"] = sol::property(&UnitImplView::getArmor);
    impl["regen"] = sol::property(&UnitImplView::getRegen);
    impl["race"] = sol::property(&UnitImplView::getRace);
    impl["subrace"] = sol::property(&UnitImplView::getSubRace);
    impl["small"] = sol::property(&UnitImplView::isSmall);
    impl["male"] = sol::property(&UnitImplView::isMale);
    impl["waterOnly"] = sol::property(&UnitImplView::isWaterOnly);
    impl["attacksTwice"] = sol::property(&UnitImplView::attacksTwice);
    impl["type"] = sol::property(&UnitImplView::getUnitCategory);
    impl["dynUpgLvl"] = sol::property(&UnitImplView::getDynUpgLevel);
    impl["dynUpg1"] = sol::property(&UnitImplView::getDynUpgrade1);
    impl["dynUpg2"] = sol::property(&UnitImplView::getDynUpgrade2);
    impl["attack1"] = sol::property(&UnitImplView::getAttack);
    impl["attack2"] = sol::property(&UnitImplView::getAttack2);

    impl["leaderType"] = sol::property(&UnitImplView::getLeaderCategory);
    impl["movement"] = sol::property(&UnitImplView::getMovement);
    impl["scout"] = sol::property(&UnitImplView::getScout);
    impl["leadership"] = sol::property(&UnitImplView::getLeadership);
    impl["hasAbility"] = &UnitImplView::hasAbility;
    impl["hasMoveBonus"] = &UnitImplView::hasMoveBonus;
}

std::string UnitImplView::getId() const
{
    auto id = impl ? impl->id : game::invalidId;
    return hooks::idToString(&id);
}

int UnitImplView::getLevel() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getLevel(soldier) : 0;
}

int UnitImplView::getXpNext() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getXpNext(soldier) : 0;
}

int UnitImplView::getDynUpgLevel() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getDynUpgLvl(soldier) : 0;
}

int UnitImplView::getXpKilled() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getXpKilled(soldier) : 0;
}

int UnitImplView::getHitPoint() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getHitPoints(soldier) : 0;
}

int UnitImplView::getArmor() const
{
    int armor;
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? *soldier->vftable->getArmor(soldier, &armor) : 0;
}

int UnitImplView::getRegen() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? *soldier->vftable->getRegen(soldier) : 0;
}

int UnitImplView::getRace() const
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    if (!soldier)
        return 0;

    auto raceId = soldier->vftable->getRaceId(soldier);
    auto races = (*globalApi.getGlobalData())->races;
    auto race = (TRaceType*)globalApi.findById(races, raceId);
    if (!race)
        return 0;

    return (int)race->data->raceType.id;
}

int UnitImplView::getSubRace() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? (int)soldier->vftable->getSubrace(soldier)->id : 0;
}

bool UnitImplView::isSmall() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getSizeSmall(soldier) : false;
}

bool UnitImplView::isMale() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getSexM(soldier) : false;
}

bool UnitImplView::isWaterOnly() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getWaterOnly(soldier) : false;
}

bool UnitImplView::attacksTwice() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? soldier->vftable->getAttackTwice(soldier) : false;
}

int UnitImplView::getUnitCategory() const
{
    using namespace game;

    auto category{impl->vftable->getCategory(impl)};
    if (!category) {
        return emptyCategoryId;
    }

    return static_cast<int>(category->id);
}

int UnitImplView::getLeaderCategory() const
{
    using namespace game;

    auto leader{gameFunctions().castUnitImplToLeader(impl)};
    if (!leader) {
        return emptyCategoryId;
    }

    auto category{leader->vftable->getCategory(leader)};
    if (!category) {
        return emptyCategoryId;
    }

    return static_cast<int>(category->id);
}

int UnitImplView::getMovement() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(impl)};

    return leader ? leader->vftable->getMovement(leader) : 0;
}

int UnitImplView::getScout() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(impl)};

    return leader ? leader->vftable->getScout(leader) : 0;
}

int UnitImplView::getLeadership() const
{
    auto leader{game::gameFunctions().castUnitImplToStackLeader(impl)};

    return leader ? leader->vftable->getLeadership(leader) : 0;
}

bool UnitImplView::hasAbility(int abilityId) const
{
    using namespace game;

    auto leader{gameFunctions().castUnitImplToStackLeader(impl)};
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

bool UnitImplView::hasMoveBonus(int groundId) const
{
    using namespace game;

    auto leader{gameFunctions().castUnitImplToStackLeader(impl)};
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

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade1() const
{
    return getDynUpgrade(1);
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade2() const
{
    return getDynUpgrade(2);
}

std::optional<AttackView> UnitImplView::getAttack() const
{
    auto soldier{hooks::castUnitImplToSoldierWithLogging(impl)};
    if (!soldier) {
        return std::nullopt;
    }

    auto attack{soldier->vftable->getAttackById(soldier)};
    if (!attack) {
        return std::nullopt;
    }

    return AttackView{attack};
}

std::optional<AttackView> UnitImplView::getAttack2() const
{
    auto soldier{hooks::castUnitImplToSoldierWithLogging(impl)};
    if (!soldier) {
        return std::nullopt;
    }

    auto secondaryAttack{soldier->vftable->getSecondAttackById(soldier)};
    if (!secondaryAttack) {
        return std::nullopt;
    }

    return AttackView{secondaryAttack};
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade(int upgradeNumber) const
{
    using namespace game;

    if (upgradeNumber != 1 && upgradeNumber != 2)
        return std::nullopt;

    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    if (!soldier)
        return std::nullopt;

    auto id = upgradeNumber == 1 ? soldier->vftable->getDynUpg1(soldier)
                                 : soldier->vftable->getDynUpg2(soldier);
    if (!id) {
        hooks::logError("mssProxyError.log",
                        fmt::format("Dyn upgrade {:d} id is null, unit impl {:s}", upgradeNumber,
                                    hooks::idToString(&impl->id)));
        return std::nullopt;
    }

    const auto& globalApi = GlobalDataApi::get();
    auto globalData = *globalApi.getGlobalData();

    auto upgrade = globalApi.findDynUpgradeById(globalData->dynUpgrade, id);
    if (!upgrade) {
        hooks::logError("mssProxyError.log", fmt::format("Could not find dyn upgrade {:d} {:s}",
                                                         upgradeNumber, hooks::idToString(id)));
        return std::nullopt;
    }

    return {upgrade};
}

} // namespace bindings
