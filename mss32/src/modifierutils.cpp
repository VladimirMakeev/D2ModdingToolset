/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "modifierutils.h"
#include "attack.h"
#include "attackmodified.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "custommodifier.h"
#include "dynamiccast.h"
#include "game.h"
#include "globaldata.h"
#include "idlistutils.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "modifgroup.h"
#include "settings.h"
#include "umattackhooks.h"
#include "umunit.h"
#include "unitmodifier.h"
#include "ussoldier.h"

namespace hooks {

bool unitCanBeModified(game::BattleMsgData* battleMsgData, game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    if (battle.getUnitStatus(battleMsgData, targetUnitId, BattleStatus::XpCounted)
        || battle.getUnitStatus(battleMsgData, targetUnitId, BattleStatus::Dead)
        || battle.getUnitStatus(battleMsgData, targetUnitId, BattleStatus::Retreat)) {
        return false;
    }

    return true;
}

game::CUmUnit* castUmModifierToUmUnit(game::CUmModifier* modifier)
{
    using namespace game;

    if (castModifierToCustomModifier(modifier))
        return nullptr; // CCustomModifier inherits type info from CUmUnit

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (CUmUnit*)dynamicCast(modifier, 0, rtti.CUmModifierType, rtti.CUmUnitType, 0);
}

game::CUmAttack* castUmModifierToUmAttack(game::CUmModifier* modifier)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (CUmAttack*)dynamicCast(modifier, 0, rtti.CUmModifierType, rtti.CUmAttackType, 0);
}

game::CUmStack* castUmModifierToUmStack(game::CUmModifier* modifier)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (CUmStack*)dynamicCast(modifier, 0, rtti.CUmModifierType, rtti.CUmStackType, 0);
}

game::IUsUnit* castUmModifierToUnit(game::CUmModifier* modifier)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (IUsUnit*)dynamicCast(modifier, 0, rtti.CUmModifierType, rtti.IUsUnitType, 0);
}

game::CUmModifier* castUnitToUmModifier(const game::IUsUnit* unit)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (CUmModifier*)dynamicCast(unit, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
}

game::CUmModifier* getFirstUmModifier(const game::IUsUnit* unit)
{
    using namespace game;

    CUmModifier* result = nullptr;
    for (auto curr = unit; curr; curr = result->data->prev) {
        auto modifier = castUnitToUmModifier(curr);
        if (!modifier)
            break;

        result = modifier;
    }

    return result;
}

const game::TUnitModifier* getUnitModifier(const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto modifiers = (*global.getGlobalData())->modifiers;
    const TUnitModifier* unitModifier = (TUnitModifier*)global.findById(modifiers, modifierId);

    return unitModifier;
}

void getModifierAttackSource(game::CUmUnit* modifier, game::LAttackSource* value)
{
    using namespace game;

    const int attackSourceId = (int)modifier->data->immunity.data->id;
    const auto attackSourceTable = (*GlobalDataApi::get().getGlobalData())->attackSources;
    LAttackSourceTableApi::get().findCategoryById(attackSourceTable, value, &attackSourceId);
}

void getModifierAttackClass(game::CUmUnit* modifier, game::LAttackClass* value)
{
    using namespace game;

    const int attackClassId = (int)modifier->data->immunityC.data->id;
    const auto attackClassTable = (*GlobalDataApi::get().getGlobalData())->attackClasses;
    LAttackClassTableApi::get().findCategoryById(attackClassTable, value, &attackClassId);
}

bool isUnitAttackSourceWardRemoved(game::AttackSourceImmunityStatusesPatched immunityStatuses,
                                   const game::LAttackSource* attackSource)
{
    using namespace game;

    std::uint32_t flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(attackSource);
    return immunityStatuses.patched & flag;
}

void resetUnitAttackSourceWard(game::BattleMsgData* battleMsgData,
                               const game::CMidgardID* unitId,
                               game::CUmUnit* modifier)
{
    using namespace game;

    LAttackSource attackSource{};
    getModifierAttackSource(modifier, &attackSource);

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(battleMsgData, unitId);

    std::uint32_t flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(&attackSource);
    unitInfo->attackSourceImmunityStatuses.patched &= ~flag;
}

std::vector<game::AttackSourceId> getRemovedAttackSourceWards(
    game::AttackSourceImmunityStatusesPatched immunityStatuses)
{
    using namespace game;

    std::vector<AttackSourceId> result;
    const auto& sourceCategories = AttackSourceCategories::get();
    for (auto source : {
             sourceCategories.weapon,
             sourceCategories.mind,
             sourceCategories.life,
             sourceCategories.death,
             sourceCategories.fire,
             sourceCategories.water,
             sourceCategories.earth,
             sourceCategories.air,
         }) {
        if (isUnitAttackSourceWardRemoved(immunityStatuses, source)) {
            result.push_back(source->id);
        }
    }
    for (const auto& custom : getCustomAttacks().sources) {
        if (isUnitAttackSourceWardRemoved(immunityStatuses, &custom.source)) {
            result.push_back(custom.source.id);
        }
    }

    return result;
}

bool isUnitAttackClassWardRemoved(std::uint32_t immunityStatuses,
                                  const game::LAttackClass* attackClass)
{
    using namespace game;

    std::uint32_t flag = 1 << gameFunctions().getAttackClassWardFlagPosition(attackClass);
    return immunityStatuses & flag;
}

void resetUnitAttackClassWard(game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId,
                              game::CUmUnit* modifier)
{
    using namespace game;

    LAttackClass attackClass{};
    getModifierAttackClass(modifier, &attackClass);

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(battleMsgData, unitId);

    std::uint32_t flag = 1 << gameFunctions().getAttackClassWardFlagPosition(&attackClass);
    unitInfo->attackClassImmunityStatuses &= ~flag;
}

std::vector<game::AttackClassId> getRemovedAttackClassWards(std::uint32_t immunityStatuses)
{
    using namespace game;

    std::vector<AttackClassId> result;
    const auto& classCategories = AttackClassCategories::get();
    for (auto class_ : {
             classCategories.paralyze,
             classCategories.petrify,
             classCategories.damage,
             classCategories.drain,
             classCategories.heal,
             classCategories.fear,
             classCategories.boostDamage,
             classCategories.lowerDamage,
             classCategories.lowerInitiative,
             classCategories.poison,
             classCategories.frostbite,
             classCategories.revive,
             classCategories.drainOverflow,
             classCategories.cure,
             classCategories.summon,
             classCategories.drainLevel,
             classCategories.giveAttack,
             classCategories.doppelganger,
             classCategories.transformSelf,
             classCategories.transformOther,
             classCategories.blister,
             classCategories.bestowWards,
             classCategories.shatter,
         }) {
        if (isUnitAttackClassWardRemoved(immunityStatuses, class_)) {
            result.push_back(class_->id);
        }
    }

    return result;
}

bool canApplyImmunityModifier(game::BattleMsgData* battleMsgData,
                              const game::CMidUnit* targetUnit,
                              game::CUmUnit* modifier,
                              game::ImmuneId immuneId)
{
    using namespace game;

    LAttackSource attackSource{};
    getModifierAttackSource(modifier, &attackSource);

    const auto targetSoldier = gameFunctions().castUnitImplToSoldier(targetUnit->unitImpl);
    const auto immuneCat = targetSoldier->vftable->getImmuneByAttackSource(targetSoldier,
                                                                           &attackSource);

    if (immuneCat->id == ImmuneCategories::get().notimmune->id) {
        return true;
    } else if (immuneCat->id == ImmuneCategories::get().once->id) {
        if (immuneId == ImmuneId::Always)
            return true;

        const auto& battle = BattleMsgDataApi::get();
        if (battle.isUnitAttackSourceWardRemoved(battleMsgData, &targetUnit->id, &attackSource))
            return true;
    }

    return false;
}

bool canApplyImmunityclassModifier(game::BattleMsgData* battleMsgData,
                                   const game::CMidUnit* targetUnit,
                                   game::CUmUnit* modifier,
                                   game::ImmuneId immuneId)
{
    using namespace game;

    LAttackClass attackClass{};
    getModifierAttackClass(modifier, &attackClass);

    const auto targetSoldier = gameFunctions().castUnitImplToSoldier(targetUnit->unitImpl);
    const auto immuneCat = targetSoldier->vftable->getImmuneByAttackClass(targetSoldier,
                                                                          &attackClass);

    if (immuneCat->id == ImmuneCategories::get().notimmune->id) {
        return true;
    } else if (immuneCat->id == ImmuneCategories::get().once->id) {
        if (immuneId == ImmuneId::Always)
            return true;

        const auto& battle = BattleMsgDataApi::get();
        if (battle.isUnitAttackClassWardRemoved(battleMsgData, &targetUnit->id, &attackClass))
            return true;
    }

    return false;
}

bool canApplyModifier(game::BattleMsgData* battleMsgData,
                      const game::CMidUnit* targetUnit,
                      const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    if (battle.unitHasModifier(battleMsgData, modifierId, &targetUnit->id))
        return false;

    CUmModifier* modifier = getUnitModifier(modifierId)->data->modifier;

    CUmUnit* umUnit = castUmModifierToUmUnit(modifier);
    if (umUnit) {
        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::Hp)
            || modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::Regeneration)
            || modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::Armor))
            return true;

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityOnce)
            && canApplyImmunityModifier(battleMsgData, targetUnit, umUnit, ImmuneId::Once))
            return true;

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityAlways)
            && canApplyImmunityModifier(battleMsgData, targetUnit, umUnit, ImmuneId::Always))
            return true;

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityclassOnce)
            && canApplyImmunityclassModifier(battleMsgData, targetUnit, umUnit, ImmuneId::Once))
            return true;

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityclassAlways)
            && canApplyImmunityclassModifier(battleMsgData, targetUnit, umUnit, ImmuneId::Always))
            return true;

        return false;
    }

    return true;
}

bool canApplyAnyModifier(game::IAttack* attack,
                         game::IMidgardObjectMap* objectMap,
                         game::BattleMsgData* battleMsgData,
                         game::CMidgardID* targetUnitId)
{
    using namespace game;

    if (!unitCanBeModified(battleMsgData, targetUnitId))
        return false;

    const CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);

    const auto wards = attack->vftable->getWards(attack);
    for (const CMidgardID* modifierId = wards->bgn; modifierId != wards->end; modifierId++) {
        if (canApplyModifier(battleMsgData, targetUnit, modifierId))
            return true;
    }

    return false;
}

game::ModifiedUnitInfo* getModifiedUnits(game::UnitInfo* unitInfo, game::ModifiedUnitInfo** end)
{
    using namespace game;

    auto& units = unitInfo->modifiedUnits;
    if (userSettings().unrestrictedBestowWards) {
        *end = units.patched + ModifiedUnitCountPatched;
        return units.patched;
    } else {
        *end = units.original + std::size(units.original);
        return units.original;
    }
}

void resetModifiedUnitsInfo(game::UnitInfo* unitInfo)
{
    using namespace game;

    game::ModifiedUnitInfo* end;
    for (auto info = getModifiedUnits(unitInfo, &end); info < end; info++) {
        info->unitId = invalidId;
        info->modifierId = invalidId;
    }
}

bool addUnitModifierInfo(game::BattleMsgData* battleMsgData,
                         game::CMidUnit* targetUnit,
                         const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    auto& modifierIds = battle.getUnitInfoById(battleMsgData, &targetUnit->id)->modifierIds;
    for (auto& id : modifierIds) {
        if (id == invalidId) {
            id = *modifierId;
            return true;
        }
    }

    return false;
}

bool addModifiedUnitInfo(const game::CMidgardID* unitId,
                         game::BattleMsgData* battleMsgData,
                         game::CMidUnit* targetUnit,
                         const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    auto unitInfo = battle.getUnitInfoById(battleMsgData, unitId);

    game::ModifiedUnitInfo* end;
    for (auto info = getModifiedUnits(unitInfo, &end); info < end; info++) {
        if (info->unitId == invalidId) {
            if (!addUnitModifierInfo(battleMsgData, targetUnit, modifierId))
                return false;
            info->unitId = targetUnit->id;
            info->modifierId = *modifierId;
            return true;
        }
    }

    return false;
}

bool applyModifier(const game::CMidgardID* unitId,
                   game::BattleMsgData* battleMsgData,
                   game::CMidUnit* targetUnit,
                   const game::CMidgardID* modifierId)
{
    using namespace game;

    if (!addModifiedUnitInfo(unitId, battleMsgData, targetUnit, modifierId))
        return false;

    CMidUnitApi::get().addModifier(targetUnit, modifierId);

    // Fixes modifiers getting lost after modified unit is untransformed
    if (targetUnit->transformed)
        addUniqueIdToList(targetUnit->origModifiers, modifierId);

    CUmModifier* modifier = getUnitModifier(modifierId)->data->modifier;

    // No ward reset in case of custom modifier because we don't know if it grants it or not
    CUmUnit* umUnit = castUmModifierToUmUnit(modifier);
    if (umUnit) {
        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityOnce))
            resetUnitAttackSourceWard(battleMsgData, &targetUnit->id, umUnit);

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityclassOnce))
            resetUnitAttackClassWard(battleMsgData, &targetUnit->id, umUnit);
    }

    return true;
}

void removeModifier(game::BattleMsgData* battleMsgData,
                    game::CMidUnit* unit,
                    const game::CMidgardID* modifierId)
{
    using namespace game;

    if (unit == nullptr) // Prevents the same crash with summoners that appears in removeModifier
        return;

    CMidUnitApi::get().removeModifier(unit, modifierId);

    // Fixes modifiers becoming permanent after modified unit is transformed
    removeIdFromList(unit->origModifiers, modifierId);

    BattleMsgDataApi::get().resetUnitModifierInfo(battleMsgData, &unit->id, modifierId);
}

void removeModifiers(game::BattleMsgData* battleMsgData,
                     game::IMidgardObjectMap* objectMap,
                     game::UnitInfo* unitInfo,
                     const game::CMidgardID* modifiedUnitId)
{
    using namespace game;

    auto modifiedUnit = static_cast<CMidUnit*>(
        objectMap->vftable->findScenarioObjectByIdForChange(objectMap, modifiedUnitId));

    auto unitModifierIds = getUnitModifierIds(unitInfo, modifiedUnitId);
    for (auto it = unitModifierIds.begin(); it != unitModifierIds.end(); it++)
        removeModifier(battleMsgData, modifiedUnit, &(*it));
}

game::CMidgardID validateId(game::CMidgardID src)
{
    using namespace game;

    CMidgardID value;
    const auto& id = CMidgardIDApi::get();
    id.validateId(&value, src);
    return value;
}

std::set<game::CMidgardID> getModifiedUnitIds(game::UnitInfo* unitInfo)
{
    using namespace game;

    std::set<CMidgardID> result;

    game::ModifiedUnitInfo* end;
    for (auto info = getModifiedUnits(unitInfo, &end); info < end; info++) {
        if (info->unitId != invalidId)
            result.insert(validateId(info->unitId));
    }

    return result;
}

std::set<game::CMidgardID> getUnitModifierIds(game::UnitInfo* unitInfo,
                                              const game::CMidgardID* modifiedUnitId)
{
    using namespace game;

    std::set<CMidgardID> result;

    game::ModifiedUnitInfo* end;
    for (auto info = getModifiedUnits(unitInfo, &end); info < end; info++) {
        if (info->unitId == *modifiedUnitId)
            result.insert(validateId(info->modifierId));
    }

    return result;
}

game::IAttack* wrapAltAttack(const game::IUsUnit* unit, game::IAttack* attack)
{
    using namespace game;

    const auto& attackModifiedApi = CAttackModifiedApi::get();

    auto result = attack;
    for (CUmModifier* curr = getFirstUmModifier(unit); curr; curr = curr->data->next) {
        auto umAttack = castUmModifierToUmAttack(curr);
        if (umAttack) {
            auto altAttackModified = getAltAttackModified(umAttack);
            attackModifiedApi.wrap(altAttackModified, result);
            result = altAttackModified;
        } else {
            auto custom = castModifierToCustomModifier(curr);
            if (custom) {
                result = custom->wrapAltAttack(result);
            }
        }
    }

    return result;
}

void getEditorModifiers(const game::CMidUnit* unit, game::IdList* value)
{
    using namespace game;

    const auto& unitApi = CMidUnitApi::get();
    const auto& idListApi = IdListApi::get();
    const auto& idApi = CMidgardIDApi::get();

    unitApi.getModifiers(value, unit);

    for (auto it = value->begin(); it != value->end();) {
        auto typeIndex = idApi.getTypeIndex(&(*it));
        if (0x9000 <= typeIndex && typeIndex <= 0x9999) { // Scenario Editor modifiers
            ++it;
        } else {
            idListApi.erase(value, it++);
        }
    }
}

int applyModifiers(int base, const game::IdList& modifiers, game::ModifierElementTypeFlag type)
{
    using namespace game;

    auto& globalApi{GlobalDataApi::get()};
    auto globalData{*globalApi.getGlobalData()};

    int result = base;
    for (const auto& modifier : modifiers) {
        auto unitModifier{(TUnitModifier*)globalApi.findById(globalData->modifiers, &modifier)};
        auto umModifier{unitModifier->data->modifier};

        if (!umModifier->vftable->hasElement(umModifier, type)) {
            continue;
        }

        bool percent = false;
        switch (type) {
        case ModifierElementTypeFlag::QtyDamage:
        case ModifierElementTypeFlag::Power:
        case ModifierElementTypeFlag::Initiative:
            percent = true;
            break;
        case ModifierElementTypeFlag::Hp:
            auto umUnit = castUmModifierToUmUnit(umModifier);
            percent = umUnit && umUnit->data->isPercentHp;
            break;
        }

        int value = umModifier->vftable->getFirstElementValue(umModifier);
        if (percent) {
            result += result * value / 100;
        } else {
            result += value;
        }
    }

    return result;
}

bool isImmunityModifier(const game::CMidgardID* modifierId,
                        const game::LAttackSource* source,
                        game::ImmuneId immuneId)
{
    using namespace game;

    auto modifier = getUnitModifier(modifierId)->data->modifier;
    switch (immuneId) {
    case ImmuneId::Once:
        if (!modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityOnce)) {
            return false;
        }
        break;
    case ImmuneId::Always:
        if (!modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityAlways)) {
            return false;
        }
        break;
    default:
        return false;
    }

    auto umUnit = castUmModifierToUmUnit(modifier);
    if (!umUnit) {
        return false;
    }

    LAttackSource modifierSource{};
    getModifierAttackSource(umUnit, &modifierSource);

    return modifierSource.id == source->id;
}

bool isImmunityclassModifier(const game::CMidgardID* modifierId,
                             const game::LAttackClass* class_,
                             game::ImmuneId immuneId)
{
    using namespace game;

    auto modifier = getUnitModifier(modifierId)->data->modifier;
    switch (immuneId) {
    case ImmuneId::Once:
        if (!modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityclassOnce)) {
            return false;
        }
        break;
    case ImmuneId::Always:
        if (!modifier->vftable->hasElement(modifier,
                                           ModifierElementTypeFlag::ImmunityclassAlways)) {
            return false;
        }
        break;
    default:
        return false;
    }

    auto umUnit = castUmModifierToUmUnit(modifier);
    if (!umUnit) {
        return false;
    }

    LAttackClass modifierClass{};
    getModifierAttackClass(umUnit, &modifierClass);

    return modifierClass.id == class_->id;
}

void notifyModifiersChanged(const game::IUsUnit* unitImpl)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    CUmModifier* modifier = nullptr;
    for (auto curr = unitImpl; curr; curr = modifier->data->prev) {
        modifier = (CUmModifier*)dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
        if (!modifier) {
            break;
        }

        auto customModifier = castModifierToCustomModifier(modifier);
        if (customModifier) {
            customModifier->notifyModifiersChanged();
        }
    }
}

bool addModifier(game::CMidUnit* unit, const game::CMidgardID* modifierId, bool checkCanApply)
{
    using namespace game;

    const auto unitModifier = getUnitModifier(modifierId);
    if (!unitModifier) {
        return false;
    }

    if (checkCanApply && !unitModifier->vftable->canApplyToUnit(unitModifier, unit->unitImpl)) {
        return false;
    }

    auto modifier = unitModifier->vftable->createModifier(unitModifier);

    auto prevModifier = castUnitToUmModifier(unit->unitImpl);
    if (prevModifier)
        prevModifier->data->next = modifier;

    CUmModifierApi::get().setPrev(modifier, unit->unitImpl);

    auto customModifier = castModifierToCustomModifier(modifier);
    if (customModifier)
        customModifier->setUnit(unit);

    unit->unitImpl = castUmModifierToUnit(modifier);

    if (userSettings().modifiers.notifyModifiersChanged) {
        notifyModifiersChanged(unit->unitImpl);
    }

    return true;
}

bool hasModifier(const game::IUsUnit* unitImpl, const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    const CUmModifier* modifier = nullptr;
    for (auto curr = unitImpl; curr; curr = modifier->data->prev) {
        modifier = static_cast<const CUmModifier*>(
            dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0));
        if (!modifier)
            break;

        if (modifier->data->modifierId == *modifierId) {
            return true;
        }
    }

    return false;
}

} // namespace hooks
