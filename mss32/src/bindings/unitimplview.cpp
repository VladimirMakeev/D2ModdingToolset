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
#include "attackclasscat.h"
#include "attacksourcecat.h"
#include "attackview.h"
#include "customattacks.h"
#include "dynupgradeview.h"
#include "game.h"
#include "globaldata.h"
#include "groundcat.h"
#include "leaderabilitycat.h"
#include "leadercategory.h"
#include "log.h"
#include "midsubrace.h"
#include "modifierutils.h"
#include "modifierview.h"
#include "racecategory.h"
#include "racetype.h"
#include "ummodifier.h"
#include "unitutils.h"
#include "usleader.h"
#include "ussoldier.h"
#include "usstackleader.h"
#include "usunitimpl.h"
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
    impl["hp"] = sol::property(&UnitImplView::getHitPoint);
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
    impl["altAttack"] = sol::property(&UnitImplView::getAltAttack);
    impl["altAttack2"] = sol::property(&UnitImplView::getAltAttack2);
    impl["base"] = sol::property(&UnitImplView::getBaseUnit);

    impl["global"] = sol::property(&UnitImplView::getGlobal);
    impl["generated"] = sol::property(&UnitImplView::getGenerated);
    impl["modifiers"] = sol::property(&UnitImplView::getModifiers);
    impl["hasModifier"] = sol::overload<>(&UnitImplView::hasModifier,
                                          &UnitImplView::hasModifierById);

    impl["leaderType"] = sol::property(&UnitImplView::getLeaderCategory);
    impl["movement"] = sol::property(&UnitImplView::getMovement);
    impl["scout"] = sol::property(&UnitImplView::getScout);
    impl["leadership"] = sol::property(&UnitImplView::getLeadership);
    impl["hasAbility"] = &UnitImplView::hasAbility;
    impl["hasMoveBonus"] = &UnitImplView::hasMoveBonus;
    impl["getImmuneToAttackClass"] = &UnitImplView::getImmuneToAttackClass;
    impl["getImmuneToAttackSource"] = &UnitImplView::getImmuneToAttackSource;
}

IdView UnitImplView::getId() const
{
    return IdView{impl->id};
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
        return emptyCategoryId;

    auto raceId = soldier->vftable->getRaceId(soldier);
    auto races = (*globalApi.getGlobalData())->races;
    auto race = (TRaceType*)globalApi.findById(races, raceId);
    if (!race)
        return emptyCategoryId;

    return (int)race->data->raceType.id;
}

int UnitImplView::getSubRace() const
{
    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    return soldier ? (int)soldier->vftable->getSubrace(soldier)->id : game::emptyCategoryId;
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

std::optional<UnitImplView> UnitImplView::getBaseUnit() const
{
    using namespace game;

    auto soldier = hooks::castUnitImplToSoldierWithLogging(impl);
    if (!soldier)
        return std::nullopt;

    auto baseUnitImplId = soldier->vftable->getBaseUnitImplId(soldier);
    if (*baseUnitImplId == emptyId)
        return std::nullopt;

    auto globalUnitImpl = hooks::getUnitImpl(baseUnitImplId);
    if (!globalUnitImpl)
        return std::nullopt;

    return {globalUnitImpl};
}

std::optional<UnitImplView> UnitImplView::getGlobal() const
{
    return {hooks::getGlobalUnitImpl(&impl->id)};
}

std::optional<UnitImplView> UnitImplView::getGenerated() const
{
    return {hooks::getUnitImpl(&impl->id)};
}

std::vector<ModifierView> UnitImplView::getModifiers() const
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    std::vector<ModifierView> result;

    CUmModifier* modifier = nullptr;
    for (auto curr = impl; curr; curr = modifier->data->prev) {
        modifier = (CUmModifier*)dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
        if (!modifier)
            break;

        result.push_back(ModifierView{modifier});
    }

    std::reverse(result.begin(), result.end());
    return result;
}

bool UnitImplView::hasModifier(const std::string& id) const
{
    return hasModifierById(IdView{id});
}

bool UnitImplView::hasModifierById(const IdView& id) const
{
    return hooks::hasModifier(impl, &id.id);
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

std::optional<AttackView> UnitImplView::getAltAttack() const
{
    auto altAttack = hooks::getAltAttack(impl, true);
    if (!altAttack) {
        return std::nullopt;
    }

    return AttackView{altAttack};
}

std::optional<AttackView> UnitImplView::getAltAttack2() const
{
    auto altAttack = hooks::getAltAttack(impl, false);
    if (!altAttack) {
        return std::nullopt;
    }

    return AttackView{altAttack};
}

int UnitImplView::getImmuneToAttackClass(int attackClassId) const
{
    using namespace game;

    auto soldier{hooks::castUnitImplToSoldierWithLogging(impl)};
    if (!soldier) {
        return emptyCategoryId;
    }

    const auto& attackClasses{AttackClassCategories::get()};
    auto getImmune{soldier->vftable->getImmuneByAttackClass};
    const LImmuneCat* immuneCategory = nullptr;

    switch (static_cast<AttackClassId>(attackClassId)) {
    default:
        return emptyCategoryId;
    case AttackClassId::Damage:
        immuneCategory = getImmune(soldier, attackClasses.damage);
        break;
    case AttackClassId::Drain:
        immuneCategory = getImmune(soldier, attackClasses.drain);
        break;
    case AttackClassId::Paralyze:
        immuneCategory = getImmune(soldier, attackClasses.paralyze);
        break;
    case AttackClassId::Heal:
        immuneCategory = getImmune(soldier, attackClasses.heal);
        break;
    case AttackClassId::Fear:
        immuneCategory = getImmune(soldier, attackClasses.fear);
        break;
    case AttackClassId::BoostDamage:
        immuneCategory = getImmune(soldier, attackClasses.boostDamage);
        break;
    case AttackClassId::Petrify:
        immuneCategory = getImmune(soldier, attackClasses.petrify);
        break;
    case AttackClassId::LowerDamage:
        immuneCategory = getImmune(soldier, attackClasses.lowerDamage);
        break;
    case AttackClassId::LowerInitiative:
        immuneCategory = getImmune(soldier, attackClasses.lowerInitiative);
        break;
    case AttackClassId::Poison:
        immuneCategory = getImmune(soldier, attackClasses.poison);
        break;
    case AttackClassId::Frostbite:
        immuneCategory = getImmune(soldier, attackClasses.frostbite);
        break;
    case AttackClassId::Revive:
        immuneCategory = getImmune(soldier, attackClasses.revive);
        break;
    case AttackClassId::DrainOverflow:
        immuneCategory = getImmune(soldier, attackClasses.drainOverflow);
        break;
    case AttackClassId::Cure:
        immuneCategory = getImmune(soldier, attackClasses.cure);
        break;
    case AttackClassId::Summon:
        immuneCategory = getImmune(soldier, attackClasses.summon);
        break;
    case AttackClassId::DrainLevel:
        immuneCategory = getImmune(soldier, attackClasses.drainLevel);
        break;
    case AttackClassId::GiveAttack:
        immuneCategory = getImmune(soldier, attackClasses.giveAttack);
        break;
    case AttackClassId::Doppelganger:
        immuneCategory = getImmune(soldier, attackClasses.doppelganger);
        break;
    case AttackClassId::TransformSelf:
        immuneCategory = getImmune(soldier, attackClasses.transformSelf);
        break;
    case AttackClassId::TransformOther:
        immuneCategory = getImmune(soldier, attackClasses.transformOther);
        break;
    case AttackClassId::Blister:
        immuneCategory = getImmune(soldier, attackClasses.blister);
        break;
    case AttackClassId::BestowWards:
        immuneCategory = getImmune(soldier, attackClasses.bestowWards);
        break;
    case AttackClassId::Shatter:
        immuneCategory = getImmune(soldier, attackClasses.shatter);
        break;
    }

    return immuneCategory ? static_cast<int>(immuneCategory->id) : emptyCategoryId;
}

int UnitImplView::getImmuneToAttackSource(int attackSourceId) const
{
    using namespace game;

    auto soldier{hooks::castUnitImplToSoldierWithLogging(impl)};
    if (!soldier) {
        return emptyCategoryId;
    }

    const auto& attackSources{AttackSourceCategories::get()};
    auto getImmune{soldier->vftable->getImmuneByAttackSource};
    const LImmuneCat* immuneCategory = nullptr;

    const AttackSourceId srcId{static_cast<AttackSourceId>(attackSourceId)};
    switch (srcId) {
    default: {
        // Check custom attack sources first
        const auto& customSources = hooks::getCustomAttacks().sources;
        for (const auto& customSource : customSources) {
            if (customSource.source.id == srcId) {
                immuneCategory = getImmune(soldier, &customSource.source);
                break;
            }
        }

        if (immuneCategory == nullptr) {
            // Could not find source id even in custom sources
            return emptyCategoryId;
        }

        break;
    }
    case AttackSourceId::Weapon:
        immuneCategory = getImmune(soldier, attackSources.weapon);
        break;
    case AttackSourceId::Mind:
        immuneCategory = getImmune(soldier, attackSources.mind);
        break;
    case AttackSourceId::Life:
        immuneCategory = getImmune(soldier, attackSources.life);
        break;
    case AttackSourceId::Death:
        immuneCategory = getImmune(soldier, attackSources.death);
        break;
    case AttackSourceId::Fire:
        immuneCategory = getImmune(soldier, attackSources.fire);
        break;
    case AttackSourceId::Water:
        immuneCategory = getImmune(soldier, attackSources.water);
        break;
    case AttackSourceId::Earth:
        immuneCategory = getImmune(soldier, attackSources.earth);
        break;
    case AttackSourceId::Air:
        immuneCategory = getImmune(soldier, attackSources.air);
        break;
    }

    return immuneCategory ? static_cast<int>(immuneCategory->id) : emptyCategoryId;
}

std::optional<DynUpgradeView> UnitImplView::getDynUpgrade(int upgradeNumber) const
{
    auto upgrade = hooks::getDynUpgrade(impl, upgradeNumber);
    if (!upgrade) {
        return std::nullopt;
    }

    return DynUpgradeView{upgrade};
}

} // namespace bindings
