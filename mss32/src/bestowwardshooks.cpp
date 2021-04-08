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

#include "bestowwardshooks.h"
#include "attack.h"
#include "attacksourcecat.h"
#include "batattackbestowwards.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "dynamiccast.h"
#include "game.h"
#include "globaldata.h"
#include "idvector.h"
#include "immunecat.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "umunit.h"
#include "unitmodifier.h"
#include "ussoldier.h"
#include "visitors.h"

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

    char flag = 1 << gameFunctions().attackSourceToNumber(&attackSource);
    unitInfo->attackSourceImmunityStatuses &= ~flag;
}

void resetUnitAttackClassWard(game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId,
                              game::CUmUnit* modifier)
{
    using namespace game;

    LAttackClass attackClass{};
    getModifierAttackClass(modifier, &attackClass);

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(battleMsgData, unitId);

    int flag = 1 << gameFunctions().attackClassToNumber(&attackClass);
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

bool addModifierInfo(game::CBatAttackBestowWards* thisptr,
                     game::BattleMsgData* battleMsgData,
                     game::CMidUnit* targetUnit,
                     const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    auto unitInfo = battle.getUnitInfoById(battleMsgData, &thisptr->unitId);

    size_t unitIndex = -1;
    auto& modifiedUnits = unitInfo->modifiedUnits;
    for (size_t i = 0; i < sizeof(modifiedUnits) / sizeof(*modifiedUnits); i++) {
        if (modifiedUnits[i].unitId == invalidId) {
            unitIndex = i;
            break;
        }
    }
    if (unitIndex == -1)
        return false;

    auto targetUnitInfo = battle.getUnitInfoById(battleMsgData, &targetUnit->unitId);

    size_t modifierIndex = -1;
    auto& modifiers = targetUnitInfo->modifierIds;
    for (size_t i = 0; i < sizeof(modifiers) / sizeof(*modifiers); i++) {
        if (modifiers[i] == invalidId) {
            modifierIndex = i;
            break;
        }
    }
    if (modifierIndex == -1)
        return false;

    modifiedUnits[unitIndex].unitId = targetUnit->unitId;
    modifiedUnits[unitIndex].modifierId = *modifierId;
    modifiers[modifierIndex] = *modifierId;
    return true;
}

bool applyModifier(game::CBatAttackBestowWards* thisptr,
                   game::BattleMsgData* battleMsgData,
                   game::CMidUnit* targetUnit,
                   const game::CMidgardID* modifierId)
{
    using namespace game;

    if (!addModifierInfo(thisptr, battleMsgData, targetUnit, modifierId))
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

bool canHeal(game::IAttack* attack,
             game::IMidgardObjectMap* objectMap,
             game::BattleMsgData* battleMsgData,
             game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal <= 0)
        return false;

    return BattleMsgDataApi::get().unitCanBeHealed(objectMap, battleMsgData, targetUnitId);
}

int heal(game::IMidgardObjectMap* objectMap,
         game::BattleMsgData* battleMsgData,
         game::CMidUnit* targetUnit,
         int qtyHeal)
{
    using namespace game;

    int hpBefore = targetUnit->currentHp;

    const auto& visitors = VisitorApi::get();
    visitors.changeUnitHp(&targetUnit->unitId, qtyHeal, objectMap, 1);

    int hpAfter = targetUnit->currentHp;
    BattleMsgDataApi::get().setUnitHp(battleMsgData, &targetUnit->unitId, hpAfter);

    return hpAfter - hpBefore;
}

bool canPerformSecondaryAttack(game::CBatAttackBestowWards* thisptr,
                               game::IMidgardObjectMap* objectMap,
                               game::BattleMsgData* battleMsgData,
                               game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    if (thisptr->attackImplMagic == -1)
        thisptr->attackImplMagic = fn.getAttackImplMagic(objectMap, &thisptr->attackImplUnitId, 0);

    if (thisptr->attackImplMagic <= 1)
        return false;

    if (thisptr->attackNumber != 1)
        return false;

    if (CMidgardIDApi::get().getType(&thisptr->attackImplUnitId) == IdType::Item)
        return false;

    if (!thisptr->attack2Initialized) {
        thisptr->attack2Impl = fn.getAttackById(objectMap, &thisptr->attackImplUnitId,
                                                thisptr->attackNumber + 1, true);
        thisptr->attack2Initialized = true;
    }

    if (thisptr->attack2Impl == nullptr)
        return false;

    const auto attack2 = thisptr->attack2Impl;
    const auto attack2Class = attack2->vftable->getAttackClass(attack2);
    const auto batAttack2 = fn.createBatAttack(objectMap, battleMsgData, &thisptr->unitId,
                                               &thisptr->attackImplUnitId,
                                               thisptr->attackNumber + 1, attack2Class, false);

    const auto& battle = BattleMsgDataApi::get();
    bool result = battle.canPerformAttackOnUnitWithStatusCheck(objectMap, battleMsgData, batAttack2,
                                                               targetUnitId);
    batAttack2->vftable->destructor(batAttack2, true);
    return result;
}

bool __fastcall bestowWardsAttackCanPerformHooked(game::CBatAttackBestowWards* thisptr,
                                                  int /*%edx*/,
                                                  game::IMidgardObjectMap* objectMap,
                                                  game::BattleMsgData* battleMsgData,
                                                  game::CMidgardID* targetUnitId)
{
    using namespace game;

    CMidgardID targetStackId{};
    thisptr->vftable->getTargetStackId(thisptr, &targetStackId, battleMsgData);

    CMidgardID targetUnitStackId{};
    gameFunctions().getAllyOrEnemyStackId(&targetUnitStackId, battleMsgData, targetUnitId, true);

    if (targetUnitStackId != targetStackId)
        return false;

    if (canHeal(thisptr->attackImpl, objectMap, battleMsgData, targetUnitId))
        return true;

    if (canApplyAnyModifier(thisptr->attackImpl, objectMap, battleMsgData, targetUnitId))
        return true;

    return canPerformSecondaryAttack(thisptr, objectMap, battleMsgData, targetUnitId);
}

void __fastcall bestowWardsAttackOnHitHooked(game::CBatAttackBestowWards* thisptr,
                                             int /*%edx*/,
                                             game::IMidgardObjectMap* objectMap,
                                             game::BattleMsgData* battleMsgData,
                                             game::CMidgardID* targetUnitId,
                                             game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    auto targetUnit = static_cast<CMidUnit*>(
        objectMap->vftable->findScenarioObjectByIdForChange(objectMap, targetUnitId));

    int qtyHealed = 0;
    if (BattleMsgDataApi::get().unitCanBeHealed(objectMap, battleMsgData, targetUnitId)) {
        const auto attack = thisptr->attackImpl;
        const auto qtyHeal = attack->vftable->getQtyHeal(attack);
        if (qtyHeal > 0)
            qtyHealed = heal(objectMap, battleMsgData, targetUnit, qtyHeal);
    }

    if (unitCanBeModified(battleMsgData, targetUnitId)) {
        const auto attack = thisptr->attackImpl;
        const auto wards = attack->vftable->getWards(attack);
        for (const CMidgardID* modifierId = wards->bgn; modifierId != wards->end; modifierId++) {
            if (canApplyModifier(battleMsgData, targetUnit, modifierId)) {
                if (!applyModifier(thisptr, battleMsgData, targetUnit, modifierId))
                    break;
            }
        }
    }

    BattleAttackUnitInfo info{};
    info.unitId = targetUnit->unitId;
    info.unitImplId = targetUnit->unitImpl->unitId;
    info.damage = qtyHealed;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);
}

bool __fastcall bestowWardsMethod15Hooked(game::CBatAttackBestowWards* thisptr,
                                          int /*%edx*/,
                                          game::BattleMsgData* battleMsgData)
{
    return true;
}

} // namespace hooks
