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
#include "battlemsgdata.h"
#include "dynamiccast.h"
#include "game.h"
#include "globaldata.h"
#include "idlistutils.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "settings.h"
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

    std::uint32_t flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(&attackSource);
    unitInfo->attackSourceImmunityStatuses.patched &= ~flag;
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
    auto& modifierIds = battle.getUnitInfoById(battleMsgData, &targetUnit->unitId)->modifierIds;
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
            info->unitId = targetUnit->unitId;
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

    // Fixes modifiers becoming permanent after modified unit is transformed
    removeIdFromList(unit->origModifiers, modifierId);

    BattleMsgDataApi::get().resetUnitModifierInfo(battleMsgData, &unit->unitId, modifierId);
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

} // namespace hooks
