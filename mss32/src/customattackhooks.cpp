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
#include "attacksourcelist.h"
#include "attackutils.h"
#include "customattack.h"
#include "customattacks.h"
#include "dbf/dbffile.h"
#include "dbtable.h"
#include "game.h"
#include "globaldata.h"
#include "immunecat.h"
#include "log.h"
#include "mempool.h"
#include "originalfunctions.h"
#include "settings.h"
#include "ussoldier.h"
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

static void fillCustomAttackSources(const std::filesystem::path& dbfFilePath)
{
    using namespace game;

    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const int lastBaseSourceWardFlagPosition = 7;
    static const std::array<const char*, 8> baseSources = {
        {"L_WEAPON", "L_MIND", "L_LIFE", "L_DEATH", "L_FIRE", "L_WATER", "L_AIR", "L_EARTH"}};

    auto& customSources = getCustomAttacks().sources;
    int wardFlagPosition = lastBaseSourceWardFlagPosition;
    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string text;
        record.value(text, "TEXT");
        text = trimSpaces(text);

        if (std::none_of(std::begin(baseSources), std::end(baseSources),
                         [&text](const char* baseText) { return text == baseText; })) {
            std::string nameId;
            record.value(nameId, "NAME_TXT");

            int immunityPower = 5; // 5 is the default
            record.value(immunityPower, "IMMUNE_POW");

            logDebug(
                "customAttacks.log",
                fmt::format("Found custom attack source {:s}, name id {:s}, immunity power {:d}",
                            text, nameId, immunityPower));

            customSources.push_back(
                {LAttackSource{AttackSourceCategories::vftable(), nullptr, (AttackSourceId)-1},
                 text, nameId, (double)immunityPower, ++wardFlagPosition});
        }
    }
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
        if (userSettings().unrestrictedBestowWards != baseSettings().unrestrictedBestowWards) {
            // Allow Bestow Wards to read QTY_HEAL values from DB
            db.readIntWithBoundsCheck(&data->qtyHeal, dbTable, "QTY_HEAL", 0,
                                      std::numeric_limits<int>::max());
        }
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
        db.readCriticalHit(&data->critHit, dbTable, "CRIT_HIT");
    } else {
        data->critHit = false;
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

int __stdcall getAttackClassWardFlagPositionHooked(const game::LAttackClass* attackClass)
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

static void getSoldierAttackSourceImmunities(const game::LImmuneCat* immuneCat,
                                             const game::IUsSoldier* soldier,
                                             game::LinkedList<game::LAttackSource>* value)
{
    using namespace game;

    const auto& sources = AttackSourceCategories::get();
    const auto& sourceListApi = AttackSourceListApi::get();
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.weapon)->id)
        sourceListApi.add(value, sources.weapon);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.mind)->id)
        sourceListApi.add(value, sources.mind);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.life)->id)
        sourceListApi.add(value, sources.life);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.death)->id)
        sourceListApi.add(value, sources.death);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.fire)->id)
        sourceListApi.add(value, sources.fire);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.water)->id)
        sourceListApi.add(value, sources.water);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.air)->id)
        sourceListApi.add(value, sources.air);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.earth)->id)
        sourceListApi.add(value, sources.earth);

    for (const auto& custom : getCustomAttacks().sources) {
        if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, &custom.source)->id)
            sourceListApi.add(value, &custom.source);
    }
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

double __stdcall getSoldierImmunityPowerHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    double result = getOriginalFunctions().getSoldierImmunityPower(soldier);

    const auto& immunities = ImmuneCategories::get();
    for (const auto& custom : getCustomAttacks().sources) {
        auto immuneCat = soldier->vftable->getImmuneByAttackSource(soldier, &custom.source);
        if (immuneCat->id != immunities.notimmune->id)
            result += custom.immunityPower;
    }

    return result;
}

int __stdcall getAttackSourceWardFlagPositionHooked(const game::LAttackSource* attackSource)
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

    int flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(attackSource);
    return (unitInfo->attackSourceImmunityStatusesPatched & flag) != 0;
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

    int flag = 1 << gameFunctions().getAttackSourceWardFlagPosition(attackSource);
    unitInfo->attackSourceImmunityStatusesPatched |= flag;
}

} // namespace hooks
