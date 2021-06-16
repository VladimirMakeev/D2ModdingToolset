/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "customattackhooks.h"
#include "attackclasscat.h"
#include "attackimpl.h"
#include "attackutils.h"
#include "customattack.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "dbf/dbffile.h"
#include "dbtable.h"
#include "dynamiccast.h"
#include "game.h"
#include "gameutils.h"
#include "globaldata.h"
#include "idlistutils.h"
#include "immunecat.h"
#include "itembattle.h"
#include "log.h"
#include "mempool.h"
#include "midunitgroup.h"
#include "originalfunctions.h"
#include "scripts.h"
#include "targetslistutils.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>
#include <limits>

namespace hooks {

game::LAttackClass customAttackClass{};
static bool customAttackExists{false};

game::LAttackClassTable* __fastcall attackClassTableCtorHooked(game::LAttackClassTable* thisptr,
                                                               int /*%edx*/,
                                                               const char* globalsFolderPath,
                                                               void* codeBaseEnvProxy)
{
    static const char dbfFileName[] = "LAttC.dbf";
    static const char customCategoryName[] = "L_CUSTOM";

    logDebug("newAttackType.log", "LAttackClassTable c-tor hook started");

    {
        utils::DbfFile dbf;
        std::filesystem::path globals{globalsFolderPath};
        if (!dbf.open(globals / dbfFileName)) {
            logError("mssProxyError.log", fmt::format("Could not open {:s}", dbfFileName));
        } else {
            utils::DbfRecord record;
            if (dbf.recordsTotal() > 26 && dbf.record(record, 26)) {
                std::string categoryName;
                if (record.value(categoryName, "TEXT")
                    && trimSpaces(categoryName) == customCategoryName) {
                    customAttackExists = true;
                    logDebug("newAttackType.log", "Found custom attack category");
                }
            }
        }
    }

    using namespace game;
    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LAttackClassTableApi::vftable();

    const auto& table = LAttackClassTableApi::get();
    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);

    auto& categories = AttackClassCategories::get();
    table.readCategory(categories.paralyze, thisptr, "L_PARALYZE", dbfFileName);
    table.readCategory(categories.petrify, thisptr, "L_PETRIFY", dbfFileName);
    table.readCategory(categories.damage, thisptr, "L_DAMAGE", dbfFileName);
    table.readCategory(categories.drain, thisptr, "L_DRAIN", dbfFileName);
    table.readCategory(categories.heal, thisptr, "L_HEAL", dbfFileName);
    table.readCategory(categories.fear, thisptr, "L_FEAR", dbfFileName);
    table.readCategory(categories.boostDamage, thisptr, "L_BOOST_DAMAGE", dbfFileName);
    table.readCategory(categories.lowerDamage, thisptr, "L_LOWER_DAMAGE", dbfFileName);
    table.readCategory(categories.lowerInitiative, thisptr, "L_LOWER_INITIATIVE", dbfFileName);
    table.readCategory(categories.poison, thisptr, "L_POISON", dbfFileName);
    table.readCategory(categories.frostbite, thisptr, "L_FROSTBITE", dbfFileName);
    table.readCategory(categories.revive, thisptr, "L_REVIVE", dbfFileName);
    table.readCategory(categories.drainOverflow, thisptr, "L_DRAIN_OVERFLOW", dbfFileName);
    table.readCategory(categories.cure, thisptr, "L_CURE", dbfFileName);
    table.readCategory(categories.summon, thisptr, "L_SUMMON", dbfFileName);
    table.readCategory(categories.drainLevel, thisptr, "L_DRAIN_LEVEL", dbfFileName);
    table.readCategory(categories.giveAttack, thisptr, "L_GIVE_ATTACK", dbfFileName);
    table.readCategory(categories.doppelganger, thisptr, "L_DOPPELGANGER", dbfFileName);
    table.readCategory(categories.transformSelf, thisptr, "L_TRANSFORM_SELF", dbfFileName);
    table.readCategory(categories.transformOther, thisptr, "L_TRANSFORM_OTHER", dbfFileName);
    table.readCategory(categories.blister, thisptr, "L_BLISTER", dbfFileName);
    table.readCategory(categories.bestowWards, thisptr, "L_BESTOW_WARDS", dbfFileName);
    table.readCategory(categories.shatter, thisptr, "L_SHATTER", dbfFileName);

    if (customAttackExists) {
        table.readCategory(&customAttackClass, thisptr, customCategoryName, dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newAttackType.log", "LAttackClassTable c-tor hook finished");
    return thisptr;
}

game::LAttackSourceTable* __fastcall attackSourceTableCtorHooked(game::LAttackSourceTable* thisptr,
                                                                 int /*%edx*/,
                                                                 const char* globalsFolderPath,
                                                                 void* codeBaseEnvProxy)
{
    using namespace game;
    logDebug("customAttacks.log", "LAttackSourceTable c-tor hook started");

    static const char dbfFileName[] = "LAttS.dbf";
    fillCustomAttackSources(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LAttackSourceTableApi::vftable();

    const auto& table = LAttackSourceTableApi::get();
    auto& sources = AttackSourceCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(sources.weapon, thisptr, "L_WEAPON", dbfFileName);
    table.readCategory(sources.mind, thisptr, "L_MIND", dbfFileName);
    table.readCategory(sources.life, thisptr, "L_LIFE", dbfFileName);
    table.readCategory(sources.death, thisptr, "L_DEATH", dbfFileName);
    table.readCategory(sources.fire, thisptr, "L_FIRE", dbfFileName);
    table.readCategory(sources.water, thisptr, "L_WATER", dbfFileName);
    table.readCategory(sources.air, thisptr, "L_AIR", dbfFileName);
    table.readCategory(sources.earth, thisptr, "L_EARTH", dbfFileName);

    for (auto& custom : getCustomAttacks().sources) {
        logDebug("customAttacks.log",
                 fmt::format("Reading custom attack source {:s}", custom.text));
        table.readCategory(&custom.source, thisptr, custom.text.c_str(), dbfFileName);
    }

    table.initDone(thisptr);
    logDebug("customAttacks.log", "LAttackSourceTable c-tor hook finished");
    return thisptr;
}

game::LAttackReachTable* __fastcall attackReachTableCtorHooked(game::LAttackReachTable* thisptr,
                                                               int /*%edx*/,
                                                               const char* globalsFolderPath,
                                                               void* codeBaseEnvProxy)
{
    using namespace game;
    logDebug("customAttacks.log", "LAttackReachTable c-tor hook started");

    static const char dbfFileName[] = "LAttR.dbf";
    fillCustomAttackReaches(std::filesystem::path(globalsFolderPath) / dbfFileName);

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LAttackReachTableApi::vftable();

    const auto& table = LAttackReachTableApi::get();
    const auto& reaches = AttackReachCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(reaches.all, thisptr, "L_ALL", dbfFileName);
    table.readCategory(reaches.any, thisptr, "L_ANY", dbfFileName);
    table.readCategory(reaches.adjacent, thisptr, "L_ADJACENT", dbfFileName);

    for (auto& custom : getCustomAttacks().reaches) {
        logDebug("customAttacks.log", fmt::format("Reading custom attack reach {:s}", custom.text));
        table.readCategory(&custom.reach, thisptr, custom.text.c_str(), dbfFileName);
    }

    table.initDone(thisptr);
    logDebug("customAttacks.log", "LAttackReachTable c-tor hook finished");
    return thisptr;
}

game::CAttackImpl* __fastcall attackImplCtorHooked(game::CAttackImpl* thisptr,
                                                   int /*%edx*/,
                                                   const game::CDBTable* dbTable,
                                                   const game::GlobalData** globalData)
{
    using namespace game;

    const auto& attackImpl = CAttackImplApi::get();

    thisptr->data = (CAttackImplData*)Memory::get().allocate(sizeof(CAttackImplData));

    attackImpl.initData(thisptr->data);
    thisptr->vftable = CAttackImplApi::vftable();

    const auto& db = CDBTableApi::get();
    db.readId(&thisptr->attackId, dbTable, "ATT_ID");

    auto gData = *globalData;
    auto data = thisptr->data;
    db.readText(&data->name, dbTable, "NAME_TXT", gData->text);
    db.readText(&data->description, dbTable, "DESC_TXT", gData->text);
    db.findAttackClass(&data->attackClass, dbTable, "CLASS", gData->attackClasses);
    db.findAttackSource(&data->attackSource, dbTable, "SOURCE", gData->attackSources);
    db.findAttackReach(&data->attackReach, dbTable, "REACH", gData->attackReach);
    db.readInitiative(&data->initiative, dbTable, "INITIATIVE", &thisptr->attackId);

    const auto& categories = AttackClassCategories::get();
    const auto id = thisptr->data->attackClass.id;

    if (attackHasPower(&thisptr->data->attackClass)) {
        db.readPower(&data->power, &data->power, dbTable, "POWER", &thisptr->attackId);
    }

    if (id == categories.damage->id || id == categories.drain->id
        || id == categories.drainOverflow->id || id == categories.poison->id
        || id == categories.frostbite->id || id == categories.blister->id
        || id == categories.shatter->id) {
        db.readDamage(&data->qtyDamage, dbTable, "QTY_DAM", &thisptr->attackId);
    } else {
        data->qtyDamage = 0;
    }

    if (id == categories.heal->id || id == categories.revive->id) {
        db.readHeal(&data->qtyHeal, dbTable, "QTY_HEAL", &thisptr->attackId);
    } else if (id == categories.bestowWards->id) {
        data->qtyHeal = 0;
        db.readIntWithBoundsCheck(&data->qtyHeal, dbTable, "QTY_HEAL", 0,
                                  std::numeric_limits<int>::max());
    } else {
        data->qtyHeal = 0;
    }

    if (id == categories.boostDamage->id || id == categories.lowerDamage->id
        || id == categories.lowerInitiative->id) {
        db.readAttackLevel(&data->level, dbTable, "LEVEL", &thisptr->attackId, &data->attackClass);
    } else {
        data->level = -1;
    }

    if (id == categories.paralyze->id || id == categories.petrify->id
        || id == categories.boostDamage->id || id == categories.lowerDamage->id
        || id == categories.lowerInitiative->id || id == categories.poison->id
        || id == categories.frostbite->id || id == categories.blister->id
        || id == categories.transformOther->id) {
        db.readInfinite(&data->infinite, dbTable, "INFINITE");
    } else {
        data->infinite = false;
    }

    if (id == categories.transformSelf->id || id == categories.doppelganger->id) {
        db.readId(&data->altAttack, dbTable, "ALT_ATTACK");
    } else {
        data->altAttack = emptyId;
    }

    if (id == categories.bestowWards->id) {
        int count{0};
        db.readIntWithBoundsCheck(&count, dbTable, "QTY_WARDS", std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::max());

        for (int i = 0; i < count; ++i) {
            CMidgardID wardId = invalidId;
            db.readId(&wardId, dbTable, fmt::format("WARD{:d}", i + 1).c_str());
            IdVectorApi::get().pushBack(&data->wards, &wardId);
        }
    }

    if (id == categories.damage->id) {
        db.readBool(&data->critHit, dbTable, "CRIT_HIT");
    } else {
        data->critHit = false;
    }

    if (getCustomAttacks().customizeDamageRatio) {
        int damageRatio;
        db.readIntWithBoundsCheck(&damageRatio, dbTable, damageRatioColumnName, 0, 255);
        if (damageRatio == 0)
            damageRatio = 100;
        data->damageRatio = (std::uint8_t)damageRatio;

        data->damageRatioPerTarget = false;
        db.readBool(&data->damageRatioPerTarget, dbTable, damageRatioPerTargetColumnName);
    }

    return thisptr;
}

game::IBatAttack* __stdcall createBatAttackHooked(game::IMidgardObjectMap* objectMap,
                                                  game::BattleMsgData* battleMsgData,
                                                  const game::CMidgardID* id1,
                                                  const game::CMidgardID* id2,
                                                  int attackNumber,
                                                  const game::LAttackClass* attackClass,
                                                  bool a7)
{
    using namespace game;

    if (attackClass->id == customAttackClass.id) {
        CustomAttack* customAttack = (CustomAttack*)Memory::get().allocate(sizeof(CustomAttack));
        customAttackCtor(customAttack, objectMap, id1, id2, attackNumber);

        return customAttack;
    }

    return getOriginalFunctions().createBatAttack(objectMap, battleMsgData, id1, id2, attackNumber,
                                                  attackClass, a7);
}

std::uint32_t __stdcall getAttackClassWardFlagPositionHooked(const game::LAttackClass* attackClass)
{
    if (attackClass->id == customAttackClass.id) {
        return 23;
    }

    return getOriginalFunctions().getAttackClassWardFlagPosition(attackClass);
}

const char* __stdcall attackClassToStringHooked(const game::LAttackClass* attackClass)
{
    if (attackClass->id == customAttackClass.id) {
        return "DAMA";
    }

    return getOriginalFunctions().attackClassToString(attackClass);
}

void __stdcall getUnitAttackSourceImmunitiesHooked(const game::LImmuneCat* immuneCat,
                                                   const game::CMidUnit* unit,
                                                   game::LinkedList<game::LAttackSource>* value)
{
    using namespace game;

    const auto soldier = gameFunctions().castUnitImplToSoldier(unit->unitImpl);
    getSoldierAttackSourceImmunities(immuneCat, soldier, value);
}

void __stdcall getSoldierAttackSourceImmunitiesHooked(const game::IUsSoldier* soldier,
                                                      bool alwaysImmune,
                                                      game::LinkedList<game::LAttackSource>* value)
{
    using namespace game;

    const auto& immunities = ImmuneCategories::get();
    getSoldierAttackSourceImmunities(alwaysImmune ? immunities.always : immunities.once, soldier,
                                     value);
}

double __stdcall getSoldierImmunityAiRatingHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    double result = getOriginalFunctions().getSoldierImmunityAiRating(soldier);

    const auto& immunities = ImmuneCategories::get();
    for (const auto& custom : getCustomAttacks().sources) {
        auto immuneCat = soldier->vftable->getImmuneByAttackSource(soldier, &custom.source);
        if (immuneCat->id != immunities.notimmune->id)
            result += custom.immunityAiRating;
    }

    return result;
}

double __stdcall getAttackReachAiRatingHooked(const game::IUsSoldier* soldier, int targetCount)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    auto attack = soldier->vftable->getAttackById(soldier);
    auto attackReach = attack->vftable->getAttackReach(attack);

    if (attackReach->id == reaches.all->id) {
        int targetFactor = targetCount - 1;
        return 1.0 + 0.4 * targetFactor;
    } else if (attackReach->id == reaches.any->id) {
        return 1.5;
    } else if (attackReach->id != reaches.adjacent->id) {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (attackReach->id == custom.reach.id) {
                int count = std::min(targetCount, (int)custom.maxTargets);
                if (count == 1 && !custom.melee)
                    return 1.5;
                else
                    return 1.0 + 0.4 * (computeTotalDamageRatio(attack, count) - 1);
            }
        }
    }

    return 1.0;
}

std::uint32_t __stdcall getAttackSourceWardFlagPositionHooked(
    const game::LAttackSource* attackSource)
{
    for (const auto& custom : getCustomAttacks().sources) {
        if (custom.source.id == attackSource->id)
            return custom.wardFlagPosition;
    }

    return getOriginalFunctions().getAttackSourceWardFlagPosition(attackSource);
}

bool __fastcall isUnitAttackSourceWardRemovedHooked(game::BattleMsgData* thisptr,
                                                    int /*%edx*/,
                                                    const game::CMidgardID* unitId,
                                                    const game::LAttackSource* attackSource)
{
    using namespace game;

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(thisptr, unitId);
    if (unitInfo == nullptr)
        return false;

    std::uint32_t flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(attackSource);
    return (unitInfo->attackSourceImmunityStatuses.patched & flag) != 0;
}

void __fastcall removeUnitAttackSourceWardHooked(game::BattleMsgData* thisptr,
                                                 int /*%edx*/,
                                                 const game::CMidgardID* unitId,
                                                 const game::LAttackSource* attackSource)
{
    using namespace game;

    auto unitInfo = BattleMsgDataApi::get().getUnitInfoById(thisptr, unitId);
    if (unitInfo == nullptr)
        return;

    std::uint32_t flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(attackSource);
    unitInfo->attackSourceImmunityStatuses.patched |= flag;
}

void __stdcall addUnitToBattleMsgDataHooked(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidUnitGroup* group,
                                            const game::CMidgardID* unitId,
                                            char attackerFlags,
                                            game::BattleMsgData* battleMsgData)
{
    using namespace game;

    for (auto& unitsInfo : battleMsgData->unitsInfo) {
        if (unitsInfo.unitId1 == invalidId) {
            unitsInfo.attackClassImmunityStatuses = 0;
            unitsInfo.attackSourceImmunityStatuses.patched = 0;

            getOriginalFunctions().addUnitToBattleMsgData(objectMap, group, unitId, attackerFlags,
                                                          battleMsgData);
            return;
        }
    }

    logError("mssProxyError.log",
             fmt::format("Could not find a free slot for a new unit {:s} in battle msg data",
                         hooks::idToString(unitId)));
}

void __stdcall getTargetsToAttackHooked(game::IdList* value,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::IAttack* attack,
                                        const game::IBatAttack* batAttack,
                                        const game::LAttackReach* attackReach,
                                        const game::BattleMsgData* battleMsgData,
                                        game::BattleAction action,
                                        const game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& reaches = AttackReachCategories::get();

    if (action == BattleAction::Attack || action == BattleAction::UseItem) {
        CMidgardID targetGroupId{};
        batAttack->vftable->getTargetGroupId(batAttack, &targetGroupId, battleMsgData);

        if (attackReach->id == reaches.all->id) {
            getTargetsToAttackForAllAttackReach(objectMap, battleMsgData, attack, batAttack,
                                                &targetGroupId, targetUnitId, value);
        } else if (attackReach->id == reaches.any->id) {
            addUniqueIdToList(*value, targetUnitId);
        } else if (attackReach->id == reaches.adjacent->id) {
            addUniqueIdToList(*value, targetUnitId);
        } else {
            for (const auto& custom : getCustomAttacks().reaches) {
                if (attackReach->id == custom.reach.id) {
                    // This is a hack. Every attack in the game has its unitId as a first field,
                    // but its not a part of CBatAttackBase.
                    CMidgardID* unitId = (CMidgardID*)(batAttack + 1);

                    CMidgardID unitGroupId{};
                    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

                    getTargetsToAttackForCustomAttackReach(objectMap, battleMsgData, batAttack,
                                                           &targetGroupId, targetUnitId,
                                                           &unitGroupId, unitId, custom, value);
                    return;
                }
            }
        }
    } else {
        addUniqueIdToList(*value, targetUnitId);
    }
}

void __stdcall fillTargetsListHooked(const game::IMidgardObjectMap* objectMap,
                                     const game::BattleMsgData* battleMsgData,
                                     const game::IBatAttack* batAttack,
                                     const game::CMidgardID* unitId,
                                     const game::CMidgardID* attackUnitOrItemId,
                                     bool targetAllies,
                                     game::TargetsList* value,
                                     bool checkTransformed)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();
    const auto& reaches = AttackReachCategories::get();

    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    CMidgardID targetGroupId{};
    fn.getAllyOrEnemyGroupId(&targetGroupId, battleMsgData, unitId, targetAllies);

    IAttack* attack = fn.getAttackById(objectMap, attackUnitOrItemId, 1, checkTransformed);
    LAttackReach* attackReach = attack->vftable->getAttackReach(attack);
    if (attackReach->id == reaches.all->id) {
        battle.fillTargetsListForAllAttackReach(objectMap, battleMsgData, batAttack, &targetGroupId,
                                                value);
    } else if (attackReach->id == reaches.any->id) {
        battle.fillTargetsListForAnyAttackReach(objectMap, battleMsgData, batAttack, &targetGroupId,
                                                value);
    } else if (attackReach->id == reaches.adjacent->id) {
        battle.fillTargetsListForAdjacentAttackReach(objectMap, battleMsgData, batAttack,
                                                     &targetGroupId, &unitGroupId, unitId, value);
    } else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (attackReach->id == custom.reach.id) {
                fillTargetsListForCustomAttackReach(objectMap, battleMsgData, batAttack,
                                                    &targetGroupId, &unitGroupId, unitId, custom,
                                                    value);
                break;
            }
        }
    }

    if (shouldExcludeImmuneTargets(objectMap, battleMsgData, unitId)) {
        excludeImmuneTargets(objectMap, battleMsgData, attack, &unitGroupId, &targetGroupId, value);
    }
}

void __stdcall fillEmptyTargetsListHooked(const game::IMidgardObjectMap* objectMap,
                                          const game::BattleMsgData* battleMsgData,
                                          const game::IBatAttack* batAttack,
                                          const game::CMidgardID* unitId,
                                          const game::CMidgardID* attackUnitOrItemId,
                                          bool targetAllies,
                                          game::TargetsList* value)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();
    const auto& reaches = AttackReachCategories::get();

    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    CMidgardID targetGroupId{};
    fn.getAllyOrEnemyGroupId(&targetGroupId, battleMsgData, unitId, targetAllies);

    IAttack* attack = fn.getAttackById(objectMap, attackUnitOrItemId, 1, true);
    LAttackReach* attackReach = attack->vftable->getAttackReach(attack);
    if (attackReach->id == reaches.all->id) {
        battle.fillEmptyTargetsListForAllAttackReach(objectMap, &targetGroupId, value);
    } else if (attackReach->id == reaches.any->id) {
        battle.fillEmptyTargetsListForAnyAttackReach(objectMap, &targetGroupId, value);
    } else if (attackReach->id == reaches.adjacent->id) {
        battle.fillEmptyTargetsListForAdjacentAttackReach(objectMap, battleMsgData, batAttack,
                                                          &targetGroupId, &unitGroupId, unitId,
                                                          value);
    } else {
        // Do nothing - custom attack reaches process empty targets in fillTargetsListHooked
    }
}

bool __stdcall isGroupSuitableForAiNobleMisfitHooked(const game::IMidgardObjectMap* objectMap,
                                                     const game::CMidUnitGroup* group)
{
    using namespace game;

    const auto& fn = gameFunctions();

    const auto& unitIds = group->units;
    if (unitIds.end - unitIds.bgn < 2)
        return false;

    for (const CMidgardID* unitId = unitIds.bgn; unitId != unitIds.end; ++unitId) {
        auto unit = static_cast<const CMidUnit*>(
            objectMap->vftable->findScenarioObjectById(objectMap, unitId));

        auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
        if (!soldier->vftable->getSizeSmall(soldier))
            continue;

        auto attack = soldier->vftable->getAttackById(soldier);
        if (!isMeleeAttack(attack))
            return true;
    }

    return false;
}

bool __stdcall isUnitSuitableForAiNobleDuelHooked(const game::IMidgardObjectMap* objectMap,
                                                  const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& attackClasses = AttackClassCategories::get();
    const auto& attackReaches = AttackReachCategories::get();

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));

    if (unit->currentHp >= 100)
        return false;

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    auto attack = soldier->vftable->getAttackById(soldier);

    auto attackClassId = attack->vftable->getAttackClass(attack)->id;
    if (attackClassId == attackClasses.damage->id) {
        auto attackDamage = attack->vftable->getQtyDamage(attack);

        auto attackReach = attack->vftable->getAttackReach(attack);
        if (attackReach->id == attackReaches.all->id) {
            return attackDamage < 40;
        } else {
            for (auto& custom : getCustomAttacks().reaches) {
                if (attackReach->id == custom.reach.id) {
                    auto attackInitiative = attack->vftable->getInitiative(attack);
                    return attackDamage < 40 && attackInitiative <= 50;
                }
            }
        }
    } else if (attackClassId == attackClasses.heal->id
               || attackClassId == attackClasses.boostDamage->id
               || attackClassId == attackClasses.lowerDamage->id
               || attackClassId == attackClasses.lowerInitiative->id
               || attackClassId == attackClasses.revive->id
               || attackClassId == attackClasses.cure->id
               || attackClassId == attackClasses.bestowWards->id
               || attackClassId == attackClasses.shatter->id
               || attackClassId == attackClasses.giveAttack->id) {
        return true;
    }

    return false;
}

bool __stdcall findAttackTargetHooked(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId,
                                      const game::IAttack* attack,
                                      const game::CMidUnitGroup* targetGroup,
                                      const game::TargetsList* targets,
                                      const game::BattleMsgData* battleMsgData,
                                      game::CMidgardID* targetUnitId)
{
    using namespace game;

    if (getOriginalFunctions().findAttackTarget(objectMap, unitId, attack, targetGroup, targets,
                                                battleMsgData, targetUnitId)) {
        return true;
    }

    const auto& battle = BattleMsgDataApi::get();

    auto attackClass = attack->vftable->getAttackClass(attack);
    const auto& attackCategories = AttackClassCategories::get();
    if (attackClass->id == attackCategories.lowerDamage->id) {
        return battle.findBoostAttackTarget(objectMap, battleMsgData, targetGroup, targets,
                                            targetUnitId);
    } else if (attackClass->id == attackCategories.lowerInitiative->id) {
        return battle.findFearAttackTarget(objectMap, battleMsgData, targetGroup, targets,
                                           targetUnitId);
    }

    return false;
}

bool __stdcall findDoppelgangerAttackTargetHooked(const game::IMidgardObjectMap* objectMap,
                                                  const game::CMidgardID* unitId,
                                                  const game::BattleMsgData* battleMsgData,
                                                  const game::CMidUnitGroup* targetGroup,
                                                  const game::TargetsList* targets,
                                                  game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& groupApi = CMidUnitGroupApi::get();
    const auto& listApi = TargetsListApi::get();

    auto unit = static_cast<CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));

    CMidUnitGroup* enemyGroup = getAllyOrEnemyGroup(objectMap, battleMsgData, unitId, false);

    int unitPosition = groupApi.getUnitPosition(targetGroup, unitId);

    int primaryXpKilled = 0;
    int secondaryXpKilled = 0;
    CMidUnit* primaryTarget = nullptr;
    CMidUnit* secondaryTarget = nullptr;
    TargetsListIterator it, end;
    listApi.end(targets, &end);
    for (listApi.begin(targets, &it); !listApi.equals(&it, &end); listApi.preinc(&it)) {
        int targetPosition = *listApi.dereference(&it);

        auto targetId = getTargetUnitId(targetPosition, targetGroup, enemyGroup);
        auto targetUnit = static_cast<CMidUnit*>(
            objectMap->vftable->findScenarioObjectById(objectMap, &targetId));

        auto soldier = fn.castUnitImplToSoldier(getGlobalUnitImpl(targetUnit));

        auto attack = soldier->vftable->getAttackById(soldier);
        if (!fn.isAttackEffectiveAgainstGroup(objectMap, attack, enemyGroup))
            continue;

        auto xpKilled = soldier->vftable->getXpKilled(soldier);
        if (isMeleeAttack(attack) == (unitPosition % 2 == 0)) {
            if (isGreaterPickRandomIfEqual(xpKilled, primaryXpKilled)) {
                primaryXpKilled = xpKilled;
                primaryTarget = targetUnit;
            }
        } else if (primaryTarget == nullptr) {
            if (isGreaterPickRandomIfEqual(xpKilled, secondaryXpKilled)) {
                secondaryXpKilled = xpKilled;
                secondaryTarget = targetUnit;
            }
        }
    }

    if (primaryTarget == nullptr)
        primaryTarget = secondaryTarget;
    if (primaryTarget == nullptr)
        return false;

    *targetUnitId = primaryTarget->unitId;
    return true;
}

bool __stdcall findDamageAttackTargetWithNonAllReachHooked(const game::IMidgardObjectMap* objectMap,
                                                           const game::IAttack* attack,
                                                           int damage,
                                                           const game::CMidUnitGroup* targetGroup,
                                                           const game::TargetsList* targets,
                                                           const game::BattleMsgData* battleMsgData,
                                                           game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    auto attackSource = attack->vftable->getAttackSource(attack);
    auto attackClass = attack->vftable->getAttackClass(attack);
    if (isMeleeAttack(attack)) {
        auto id = battle.findDamageAttackTargetWithAdjacentReach(targetUnitId, objectMap,
                                                                 targetGroup, targets,
                                                                 battleMsgData, attackSource,
                                                                 attackClass);
        return *id != emptyId;
    } else {
        return battle.findDamageAttackTargetWithAnyReach(objectMap, targetGroup, targets, damage,
                                                         battleMsgData, attackClass, attackSource,
                                                         0, targetUnitId);
    }
}

bool __stdcall isAttackBetterThanItemUsageHooked(const game::IItem* item,
                                                 const game::IUsSoldier* soldier,
                                                 const game::CMidgardID* unitImplId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& attackClasses = AttackClassCategories::get();

    auto itemBattle = (CItemBattle*)dynamicCast(item, 0, rtti.IItemType, rtti.CItemBattleType, 0);

    auto itemAttack = getAttack(&itemBattle->attackId);
    if (itemAttack->vftable->getQtyDamage(itemAttack) == 0)
        return false;

    auto attackClass = itemAttack->vftable->getAttackClass(itemAttack);
    if (attackClass->id == attackClasses.shatter->id)
        return false;

    auto itemDamage = computeAverageTotalDamage(itemAttack,
                                                itemAttack->vftable->getQtyDamage(itemAttack));

    auto soldierDamage = fn.computeAttackDamageCheckTransformed(soldier, unitImplId, nullptr,
                                                                &emptyId);
    soldierDamage = computeAverageTotalDamage(soldier->vftable->getAttackById(soldier),
                                              soldierDamage);

    return soldierDamage >= itemDamage;
}

game::CMidgardID* __stdcall getSummonUnitImplIdByAttackHooked(game::CMidgardID* summonImplId,
                                                              const game::CMidgardID* attackId,
                                                              int position,
                                                              bool smallUnit)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& listApi = IdListApi::get();
    const auto& global = GlobalDataApi::get();

    IdList summonImplIds{};
    listApi.constructor(&summonImplIds);

    auto globalData = *global.getGlobalData();
    const auto transf = globalData->transf;
    fn.fillAttackTransformIdList(transf, &summonImplIds, attackId, smallUnit);

    if (summonImplIds.length == 0) {
        *summonImplId = emptyId;
    } else if (summonImplIds.length == 1) {
        *summonImplId = *listApi.front(&summonImplIds);
    } else {
        IdListIterator randomIt;
        listApi.begin(&summonImplIds, &randomIt);

        int random = fn.generateRandomNumberStd(summonImplIds.length);
        for (int i = 0; i < random; i++) {
            listApi.preinc(&randomIt);
        }

        if (position % 2 == 0) {
            *summonImplId = *listApi.dereference(&randomIt);
        } else {
            IdListIterator end;
            listApi.end(&summonImplIds, &end);

            IdListIterator it = randomIt;
            do {
                auto unitImpl = static_cast<TUsUnitImpl*>(
                    global.findById(globalData->units, listApi.dereference(&it)));

                const auto soldier = fn.castUnitImplToSoldier(unitImpl);
                const auto attack = soldier->vftable->getAttackById(soldier);
                if (!isMeleeAttack(attack))
                    break;

                listApi.preinc(&it);
                if (listApi.equals(&it, &end)) {
                    listApi.begin(&summonImplIds, &it);
                }
            } while (!listApi.equals(&it, &randomIt));

            *summonImplId = *listApi.dereference(&it);
        }
    }

    listApi.destructor(&summonImplIds);
    return summonImplId;
}

bool __stdcall isSmallMeleeFighterHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    if (!soldier->vftable->getSizeSmall(soldier))
        return false;

    const auto attack = soldier->vftable->getAttackById(soldier);
    if (attack->vftable->getQtyDamage(attack) == 0)
        return false;

    return isMeleeAttack(attack);
}

} // namespace hooks
