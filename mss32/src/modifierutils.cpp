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
#include "dynamiccast.h"
#include "game.h"
#include "globaldata.h"
#include "midgardobjectmap.h"

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

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (CUmUnit*)dynamicCast(modifier, 0, rtti.CUmModifierType, rtti.CUmUnitType, 0);
}

game::CUmModifier* getModifier(const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();
    const auto modifiers = (*global.getGlobalData())->modifiers;
    const TUnitModifier* unitModifier = (TUnitModifier*)global.findById(modifiers, modifierId);

    return unitModifier->group->modifier;
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

void resetUnitAttackSourceWard(game::BattleMsgData* battleMsgData,
                               const game::CMidgardID* unitId,
                               game::CUmUnit* modifier)
{
    using namespace game;

    LAttackSource attackSource{};
    getModifierAttackSource(modifier, &attackSource);

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(battleMsgData, unitId);

    int flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(&attackSource);
    unitInfo->attackSourceImmunityStatusesPatched &= ~flag;
}

void resetUnitAttackClassWard(game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId,
                              game::CUmUnit* modifier)
{
    using namespace game;

    LAttackClass attackClass{};
    getModifierAttackClass(modifier, &attackClass);

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(battleMsgData, unitId);

    int flag = 1 << gameFunctions().getAttackClassWardFlagPosition(&attackClass);
    unitInfo->attackClassImmunityStatuses &= ~flag;
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
        if (battle.isUnitAttackSourceWardRemoved(battleMsgData, &targetUnit->unitId, &attackSource))
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
        if (battle.isUnitAttackClassWardRemoved(battleMsgData, &targetUnit->unitId, &attackClass))
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
    if (battle.unitHasModifier(battleMsgData, modifierId, &targetUnit->unitId))
        return false;

    CUmModifier* modifier = getModifier(modifierId);

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

bool isOriginalModifiedUnits(const game::UnitInfo* unitInfo)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();

    // Assume original layout for empty struct (invalidId)
    CMidgardID testId = unitInfo->modifiedUnits.patched[0].modifierId;
    return id.getType(&testId) != IdType::UnitModifier;
}

void resetModifiedUnitsInfo(game::ModifiedUnitsPatched& modifiedUnits)
{
    using namespace game;

    // Regardless of the layout (original vs patched) the reset method is the same.
    // See ResetModifiedUnitsInfo impl for reference.
    for (size_t i = 0; i < std::size(modifiedUnits.original); i++) {
        modifiedUnits.original[i].unitId = invalidId;
        modifiedUnits.original[i].modifierId = invalidId;
    }
}

bool addModifiedUnitInfo(game::ModifiedUnitInfoPatched& info,
                         game::CMidgardID unitId,
                         game::CMidgardID modifierId)
{
    using namespace game;

    if (info.modifierId == invalidId)
        info.modifierId = modifierId;
    else if (info.modifierId != modifierId)
        return false;

    for (size_t i = 0; i < std::size(info.unitIds); i++) {
        if (info.unitIds[i] == invalidId) {
            info.unitIds[i] = unitId;
            return true;
        }
        if (info.unitIds[i] == unitId) // Just in case, should not happen
            return true;
    }

    return false;
}

bool addModifiedUnitInfo(game::ModifiedUnitsPatched& modifiedUnits,
                         game::CMidgardID unitId,
                         game::CMidgardID modifierId)
{
    using namespace game;

    for (size_t i = 0; i < std::size(modifiedUnits.original); i++) {
        if (modifiedUnits.original[i].unitId == invalidId) {
            modifiedUnits.original[i].unitId = unitId;
            modifiedUnits.original[i].modifierId = modifierId;
            return true;
        }
    }

    return false;
}

bool addModifiedUnitInfoPatched(game::ModifiedUnitsPatched& modifiedUnits,
                                game::CMidgardID unitId,
                                game::CMidgardID modifierId)
{
    for (size_t i = 0; i < std::size(modifiedUnits.patched); i++) {
        if (addModifiedUnitInfo(modifiedUnits.patched[i], unitId, modifierId))
            return true;
    }

    return false;
}

bool switchToPatchedModifiedUnits(game::UnitInfo* unitInfo)
{
    using namespace game;

    ModifiedUnitsPatched result;
    resetModifiedUnitsInfo(result);

    const auto& modifiedUnits = unitInfo->modifiedUnits;
    for (size_t i = 0; i < std::size(modifiedUnits.original); i++) {
        auto& info = modifiedUnits.original[i];
        if (info.unitId == invalidId || info.modifierId == invalidId)
            continue;

        if (!addModifiedUnitInfoPatched(result, info.unitId, info.modifierId))
            return false;
    }

    unitInfo->modifiedUnits = result;
    return true;
}

size_t findFreeUnitModifierIndex(const game::UnitInfo* unitInfo)
{
    using namespace game;

    auto& modifiers = unitInfo->modifierIds;
    for (size_t i = 0; i < std::size(modifiers); i++) {
        if (modifiers[i] == invalidId)
            return i;
    }

    return -1;
}

bool addModifiedUnitInfo(const game::CMidgardID* unitId,
                         game::BattleMsgData* battleMsgData,
                         game::CMidUnit* targetUnit,
                         const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    auto targetUnitInfo = battle.getUnitInfoById(battleMsgData, &targetUnit->unitId);
    size_t modifierIndex = findFreeUnitModifierIndex(targetUnitInfo);
    if (modifierIndex == -1)
        return false;

    auto unitInfo = battle.getUnitInfoById(battleMsgData, unitId);
    auto& modifiedUnits = unitInfo->modifiedUnits;
    if (isOriginalModifiedUnits(unitInfo)) {
        if (addModifiedUnitInfo(modifiedUnits, targetUnit->unitId, *modifierId)) {
            targetUnitInfo->modifierIds[modifierIndex] = *modifierId;
            return true;
        }

        if (!switchToPatchedModifiedUnits(unitInfo))
            return false;
    }

    if (addModifiedUnitInfoPatched(modifiedUnits, targetUnit->unitId, *modifierId)) {
        targetUnitInfo->modifierIds[modifierIndex] = *modifierId;
        return true;
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

    CUmModifier* modifier = getModifier(modifierId);

    CUmUnit* umUnit = castUmModifierToUmUnit(modifier);
    if (umUnit) {
        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityOnce))
            resetUnitAttackSourceWard(battleMsgData, &targetUnit->unitId, umUnit);

        if (modifier->vftable->hasElement(modifier, ModifierElementTypeFlag::ImmunityclassOnce))
            resetUnitAttackClassWard(battleMsgData, &targetUnit->unitId, umUnit);
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

    auto& mods = unit->origModifiers;
    for (auto mod = mods.head->next; mod != mods.head; mod = mod->next) {
        if (mod->data == *modifierId) {
            const auto& list = IdListApi::get();
            list.remove(&mods, 0, mod, 0);
            break;
        }
    }

    const auto& battle = BattleMsgDataApi::get();
    battle.resetUnitModifierInfo(battleMsgData, &unit->unitId, modifierId);
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

std::set<game::CMidgardID> getModifiedUnitIds(const game::UnitInfo* unitInfo)
{
    using namespace game;

    std::set<CMidgardID> result;

    const auto& modifiedUnits = unitInfo->modifiedUnits;
    if (isOriginalModifiedUnits(unitInfo)) {
        for (size_t i = 0; i < std::size(modifiedUnits.original); i++) {
            const auto& info = modifiedUnits.original[i];
            if (info.unitId != invalidId)
                result.insert(validateId(info.unitId));
        }
    } else {
        for (size_t i = 0; i < std::size(modifiedUnits.patched); i++) {
            const auto& info = modifiedUnits.patched[i];
            for (size_t j = 0; j < std::size(info.unitIds); j++) {
                if (info.unitIds[j] != invalidId)
                    result.insert(validateId(info.unitIds[j]));
            }
        }
    }

    return result;
}

std::set<game::CMidgardID> getUnitModifierIds(const game::UnitInfo* unitInfo,
                                              const game::CMidgardID* modifiedUnitId)
{
    using namespace game;

    std::set<CMidgardID> result;

    const auto& modifiedUnits = unitInfo->modifiedUnits;
    if (isOriginalModifiedUnits(unitInfo)) {
        for (size_t i = 0; i < std::size(modifiedUnits.original); i++) {
            const auto& info = modifiedUnits.original[i];
            if (info.unitId == *modifiedUnitId)
                result.insert(validateId(info.modifierId));
        }
    } else {
        for (size_t i = 0; i < std::size(modifiedUnits.patched); i++) {
            const auto& info = modifiedUnits.patched[i];
            for (size_t j = 0; j < std::size(info.unitIds); j++) {
                if (info.unitIds[j] == *modifiedUnitId)
                    result.insert(validateId(info.modifierId));
            }
        }
    }

    return result;
}

} // namespace hooks
