/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "custommodifier.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "dynamiccast.h"
#include "game.h"
#include "immunecat.h"
#include "leaderview.h"
#include "mempool.h"
#include "restrictions.h"
#include "scripts.h"
#include "unitcat.h"
#include "utils.h"

namespace hooks {

/**
 * Script functions always use LeaderView so Lua can bind access to its properties.
 * If a unit is not a leader, LeaderView methods will return stub values.
 */
using GetString = std::function<std::string(const bindings::LeaderView&, const std::string&)>;
using GetInteger = std::function<int(const bindings::LeaderView&, int)>;
using GetIntegerIntParam = std::function<int(const bindings::LeaderView&, int, int)>;

static struct
{
    game::RttiInfo<game::IUsUnitVftable> usUnit;
    game::RttiInfo<game::IUsSoldierVftable> usSoldier;
    game::RttiInfo<game::CUmModifierVftable> umModifier;
    game::RttiInfo<game::IUsStackLeaderVftable> usStackLeader;
    game::RttiInfo<game::IAttackVftable> attack;
    game::RttiInfo<game::IAttackVftable> attack2;
} rttiInfo;

void initRttiInfo();
void initVftable(CCustomModifier* thisptr);

static inline CCustomModifier* castUnitToCustomModifier(const game::IUsUnit* unit)
{
    return (CCustomModifier*)unit;
}

static inline CCustomModifier* castSoldierToCustomModifier(const game::IUsSoldier* soldier)
{
    if (soldier == nullptr)
        return nullptr;

    return reinterpret_cast<CCustomModifier*>((uintptr_t)soldier
                                              - offsetof(CCustomModifier, usSoldier));
}

CCustomModifier* castModifierToCustomModifier(const game::CUmModifier* modifier)
{
    if (!modifier || modifier->vftable != &rttiInfo.umModifier.vftable)
        return nullptr;

    return reinterpret_cast<CCustomModifier*>((uintptr_t)modifier
                                              - offsetof(CCustomModifier, umModifier));
}

static inline CCustomModifier* castStackLeaderToCustomModifier(
    const game::IUsStackLeader* stackLeader)
{
    if (stackLeader == nullptr)
        return nullptr;

    return reinterpret_cast<CCustomModifier*>((uintptr_t)stackLeader
                                              - offsetof(CCustomModifier, usStackLeader));
}

CCustomModifier* castAttackToCustomModifier(const game::IAttack* attack)
{
    if (attack == nullptr)
        return nullptr;

    if (attack->vftable == &rttiInfo.attack.vftable) {
        return reinterpret_cast<CCustomModifier*>((uintptr_t)attack
                                                  - offsetof(CCustomModifier, attack));
    } else if (attack->vftable == &rttiInfo.attack2.vftable) {
        return reinterpret_cast<CCustomModifier*>((uintptr_t)attack
                                                  - offsetof(CCustomModifier, attack2));
    }

    return nullptr;
}

game::IUsUnit* CCustomModifier::getPrev()
{
    return umModifier.data->prev;
}

game::IUsSoldier* CCustomModifier::getPrevSoldier()
{
    return game::gameFunctions().castUnitImplToSoldier(getPrev());
}

game::IUsStackLeader* CCustomModifier::getPrevStackLeader()
{
    return game::gameFunctions().castUnitImplToStackLeader(getPrev());
}

game::IAttack* CCustomModifier::getPrevAttack(const game::IAttack* current)
{
    auto prev = getPrevSoldier();
    if (!prev)
        return nullptr;

    if (current == &attack) {
        return prev->vftable->getAttackById(prev);
    } else if (current == &attack2) {
        return prev->vftable->getSecondAttackById(prev);
    }

    return nullptr;
}

game::IAttack* CCustomModifier::getAttack(bool primary)
{
    // TODO: script functions to return attack id, differentiate between primary and secondary
    auto result = primary ? &attack : &attack2;
    auto prev = getPrevAttack(result);
    if (!prev)
        return nullptr;

    result->id = prev->id;
    return result;
}

CustomAttackData CCustomModifier::getCustomAttackData(const game::IAttack* current)
{
    // TODO: script functions, differentiate between primary and secondary
    auto prev = getPrevAttack(current);
    return hooks::getCustomAttackData(prev);
}

void CCustomModifier::setUnit(const game::CMidUnit* value)
{
    unit = value;
}

const char* CCustomModifier::getGlobalTextById(const char* functionName, const char* prev)
{
    std::optional<sol::environment> env;
    auto f = getScriptFunction<GetString>(modifiersFolder() / script, functionName, env);
    try {
        if (f) {
            bindings::LeaderView unitView{unit, getPrev()};
            auto textId = (*f)(unitView, prev);
            return getGlobalText(textId);
        }
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        functionName, e.what()));
    }

    return prev;
}

int CCustomModifier::getInteger(const char* functionName, int prev)
{
    std::optional<sol::environment> env;
    auto f = getScriptFunction<GetInteger>(modifiersFolder() / script, functionName, env);
    try {
        if (f) {
            bindings::LeaderView unitView{unit, getPrev()};
            return (*f)(unitView, prev);
        }
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        functionName, e.what()));
    }

    return prev;
}

int CCustomModifier::getIntegerIntParam(const char* functionName, int param, int prev)
{
    std::optional<sol::environment> env;
    auto f = getScriptFunction<GetIntegerIntParam>(modifiersFolder() / script, functionName, env);
    try {
        if (f) {
            bindings::LeaderView unitView{unit, getPrev()};
            return (*f)(unitView, param, prev);
        }
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        functionName, e.what()));
    }

    return prev;
}

CCustomModifier* customModifierCtor(CCustomModifier* thisptr,
                                    const char* script,
                                    const game::CMidgardID* id,
                                    const game::GlobalData** globalData)
{
    using namespace game;

    // Lazy init makes sure that vftable hooks (if any) are already applied
    initRttiInfo();

    thisptr->usUnit.id = emptyId;
    CUmModifierApi::get().constructor(&thisptr->umModifier, id, globalData);
    thisptr->unit = nullptr;
    thisptr->lastElementQuery = ModifierElementTypeFlag::None;
    new (&thisptr->script) std::string(script);
    initVftable(thisptr);

    return thisptr;
}

CCustomModifier* customModifierCopyCtor(CCustomModifier* thisptr, const CCustomModifier* src)
{
    using namespace game;

    thisptr->usUnit.id = src->usUnit.id;
    CUmModifierApi::get().copyConstructor(&thisptr->umModifier, &src->umModifier);
    thisptr->unit = src->unit;
    thisptr->lastElementQuery = src->lastElementQuery;
    new (&thisptr->script) std::string(src->script);
    initVftable(thisptr);

    return thisptr;
}

void customModifierDtor(CCustomModifier* thisptr, char flags)
{
    using namespace game;

    thisptr->script.~basic_string();

    CUmModifierApi::get().destructor(&thisptr->umModifier);

    if (flags & 1) {
        Memory::get().freeNonZero(thisptr);
    }
}

void __fastcall unitDtor(game::IUsUnit* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castUnitToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

game::IUsUnitExtension* __fastcall unitCast(const game::IUsUnit* thisptr,
                                            int /*%edx*/,
                                            const char* rawTypeName)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto& rttiApi = RttiApi::get();
    auto& typeInfoRawName = *rttiApi.typeInfoRawName;

    auto thiz = castUnitToCustomModifier(thisptr);
    auto prev = thiz->getPrev();

    if (!strcmp(rawTypeName, typeInfoRawName(rtti.IUsSoldierType))) {
        return (IUsUnitExtension*)&thiz->usSoldier;
    } else if (!strcmp(rawTypeName, typeInfoRawName(rtti.IUsStackLeaderType))) {
        auto prevStackLeader = prev->vftable->cast(prev, rawTypeName);
        return prevStackLeader ? (IUsUnitExtension*)&thiz->usStackLeader : nullptr;
    }

    return prev->vftable->cast(prev, rawTypeName);
}

const game::LUnitCategory* __fastcall unitGetCategory(const game::IUsUnit* thisptr, int /*%edx*/)
{
    auto prev = castUnitToCustomModifier(thisptr)->getPrev();
    return prev->vftable->getCategory(prev);
}

void __fastcall soldierDtor(game::IUsSoldier* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

const char* __fastcall soldierGetName(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    return thiz->getGlobalTextById("getNameTxt", prev->vftable->getName(prev));
}

const char* __fastcall soldierGetDescription(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    return thiz->getGlobalTextById("getDescTxt", prev->vftable->getDescription(prev));
}

const game::CMidgardID* __fastcall soldierGetRaceId(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getRaceId(prev);
}

const game::LSubRaceCategory* __fastcall soldierGetSubrace(const game::IUsSoldier* thisptr,
                                                           int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getSubrace(prev);
}

const game::LUnitBranch* __fastcall soldierGetBranch(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getBranch(prev);
}

bool __fastcall soldierGetSizeSmall(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getSizeSmall(prev);
}

bool __fastcall soldierGetSexM(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getSexM(prev);
}

int __fastcall soldierGetLevel(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getLevel(prev);
}

int __fastcall soldierGetHitPoints(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    const auto& restrictions = game::gameRestrictions();

    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    auto value = thiz->getInteger("getHitPoint", prev->vftable->getHitPoints(prev));
    return std::clamp(value, restrictions.unitHp->min, restrictions.unitHp->max);
}

int* __fastcall soldierGetArmor(const game::IUsSoldier* thisptr, int /*%edx*/, int* armor)
{
    const auto& restrictions = game::gameRestrictions();

    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    auto value = thiz->getInteger("getArmor", *prev->vftable->getArmor(prev, armor));
    *armor = std::clamp(value, restrictions.unitArmor->min, restrictions.unitArmor->max);
    return armor;
}

const game::CMidgardID* __fastcall soldierGetBaseUnitImplId(const game::IUsSoldier* thisptr,
                                                            int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getBaseUnitImplId(prev);
}

const game::LDeathAnimCategory* __fastcall soldierGetDeathAnim(const game::IUsSoldier* thisptr,
                                                               int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getDeathAnim(prev);
}

int* __fastcall soldierGetRegen(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    auto value = thiz->getInteger("getRegen", *prev->vftable->getRegen(prev));
    thiz->regen = std::clamp(value, 0, 100);
    return &thiz->regen;
}

int __fastcall soldierGetXpNext(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    return thiz->getInteger("getXpNext", prev->vftable->getXpNext(prev));
}

int __fastcall soldierGetXpKilled(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    return thiz->getInteger("getXpKilled", prev->vftable->getXpKilled(prev));
}

const game::LImmuneCat* getImmuneCatById(int categoryId, const game::LImmuneCat* default)
{
    using namespace game;

    const auto& immunities = ImmuneCategories::get();

    switch ((ImmuneId)categoryId) {
    case ImmuneId::Notimmune:
        return immunities.notimmune;
    case ImmuneId::Once:
        return immunities.once;
    case ImmuneId::Always:
        return immunities.always;
    default:
        return default;
    }
}

const game::LImmuneCat* __fastcall soldierGetImmuneByAttackClass(
    const game::IUsSoldier* thisptr,
    int /*%edx*/,
    const game::LAttackClass* attackClass)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    auto prevValue = prev->vftable->getImmuneByAttackClass(prev, attackClass);
    auto value = thiz->getIntegerIntParam("getImmuneToAttack", (int)attackClass->id,
                                          (int)prevValue->id);
    return getImmuneCatById(value, prevValue);
}

const game::LImmuneCat* __fastcall soldierGetImmuneByAttackSource(
    const game::IUsSoldier* thisptr,
    int /*%edx*/,
    const game::LAttackSource* attackSource)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    auto prev = thiz->getPrevSoldier();

    auto prevValue = prev->vftable->getImmuneByAttackSource(prev, attackSource);
    auto value = thiz->getIntegerIntParam("getImmuneToSource", (int)attackSource->id,
                                          (int)prevValue->id);
    return getImmuneCatById(value, prevValue);
}

game::IAttack* __fastcall soldierGetAttackById(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    return castSoldierToCustomModifier(thisptr)->getAttack(true);
}

game::IAttack* __fastcall soldierGetSecondAttackById(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    return castSoldierToCustomModifier(thisptr)->getAttack(false);
}

bool __fastcall soldierGetAttackTwice(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getAttackTwice(prev);
}

const game::Bank* __fastcall soldierGetEnrollCost(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getEnrollCost(prev);
}

const game::Bank* __fastcall soldierGetReviveCost(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getReviveCost(prev);
}

const game::Bank* __fastcall soldierGetHealCost(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getHealCost(prev);
}

const game::Bank* __fastcall soldierGetTrainingCost(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getTrainingCost(prev);
}

const game::CMidgardID* __fastcall soldierGetDynUpg1(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getDynUpg1(prev);
}

int __fastcall soldierGetDynUpgLvl(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getDynUpgLvl(prev);
}

const game::CMidgardID* __fastcall soldierGetDynUpg2(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getDynUpg2(prev);
}

bool __fastcall soldierGetWaterOnly(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    auto prev = castSoldierToCustomModifier(thisptr)->getPrevSoldier();
    return prev->vftable->getWaterOnly(prev);
}

void __fastcall modifierDtor(game::CUmModifier* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castModifierToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

game::CUmModifier* __fastcall modifierCopy(const game::CUmModifier* thisptr, int /*%edx*/)
{
    using namespace game;

    auto thiz = castModifierToCustomModifier(thisptr);

    auto copy = (CCustomModifier*)Memory::get().allocate(sizeof(CCustomModifier));
    customModifierCopyCtor(copy, thiz);
    return &copy->umModifier;
}

bool __fastcall modifierCanApplyWithLeadership(const game::CUmModifier* thisptr,
                                               int /*%edx*/,
                                               const int* leadership)
{
    return true;
}

bool __fastcall modifierCanApplyToUnit(const game::CUmModifier* thisptr,
                                       int /*%edx*/,
                                       const game::IUsUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    // TODO: script function + unit and isLeader as args
    bool isLeaderOnly = false;
    auto thiz = castModifierToCustomModifier(thisptr);
    if (isLeaderOnly)
        return fn.castUnitImplToStackLeader(unit) != nullptr;
    else
        return fn.castUnitImplToSoldier(unit) != nullptr;
}

bool __fastcall modifierCanApplyToUnitCategory(const game::CUmModifier* thisptr,
                                               int /*%edx*/,
                                               const game::LUnitCategory* unitCategory)
{
    using namespace game;

    const auto& unitCategories = UnitCategories::get();

    // TODO: script function + unit category as arg
    bool isLeaderOnly = false;
    auto thiz = castModifierToCustomModifier(thisptr);
    if (!isLeaderOnly)
        return true;

    auto id = unitCategory->id;
    return id == unitCategories.noble->id || id == unitCategories.leader->id
           || id == unitCategories.summon->id || id == unitCategories.illusion->id;
}

bool __fastcall modifierIsLower(const game::CUmModifier* thisptr, int /*%edx*/)
{
    return false; // TODO: script function
}

bool __fastcall modifierIsBoost(const game::CUmModifier* thisptr, int /*%edx*/)
{
    return false; // TODO: script function
}

bool __fastcall modifierHasElement(const game::CUmModifier* thisptr,
                                   int /*%edx*/,
                                   game::ModifierElementTypeFlag type)
{
    using namespace game;

    auto thiz = castModifierToCustomModifier(thisptr);

    bool result = false; // TODO: check that corresponding script function exists
    thiz->lastElementQuery = result ? type : ModifierElementTypeFlag::None;
    return result;
}

int __fastcall modifierGetFirstElementValue(const game::CUmModifier* thisptr, int /*%edx*/)
{
    using namespace game;

    auto thiz = castModifierToCustomModifier(thisptr);

    // The values are used to check if this modifier can be applied, to apply spell effects, etc.
    // Custom modifiers do not have defined element values.
    // The goal here is to provide universal static values to pass most of the checks,
    // while having neutral effect if the values are applied directly.
    switch (thiz->lastElementQuery) {
    case ModifierElementTypeFlag::Leadership:
        // Used by GetCombinedLeadership (Akella 0x4a7d6d) to check if Leadership leader upgrade can
        // be offered.
        return 0;
    case ModifierElementTypeFlag::MoveAbility:
        // Used by CanUseItem (Akella 0x5ed7cf) to check if leader already has this movement bonus.
        return emptyCategoryId; // game::GroundId
    case ModifierElementTypeFlag::LeaderAbility:
        // Used by CanUseItem (Akella 0x5ed7cf) to check if leader already has this ability.
        return emptyCategoryId; // game::LeaderAbilityId
    case ModifierElementTypeFlag::QtyDamage:
    case ModifierElementTypeFlag::Power:
    case ModifierElementTypeFlag::Initiative:
        // Used by ApplyPercentModifiers (Akella 0x577515) to exclude editor modifiers when
        // calculating bonus damage/power/ini in GenerateAttackDescription (Akella 0x57652b).
        return 0;
    case ModifierElementTypeFlag::Hp:
        // Used by CEffectModifierApply (Akella 0x439cd1) to apply Gallean's Boon G000UM7545.
        return 0;
    case ModifierElementTypeFlag::Armor:
        // Used by ApplyFortificationArmor (Akella 0x4212a2).
        // Used by ApplyAbsoluteModifiers (Akella 0x57646e) to exclude editor modifiers when
        // calculating bonus armor in GenerateUnitDescription (Akella 0x5757eb).
        return 0;
    case ModifierElementTypeFlag::ImmunityAlways:
    case ModifierElementTypeFlag::ImmunityOnce:
        // Used by CanUseItem (Akella 0x5ed7cf) to check if unit already immune to attack source.
        return emptyCategoryId; // game::AttackSourceId
    case ModifierElementTypeFlag::ImmunityclassAlways:
    case ModifierElementTypeFlag::ImmunityclassOnce:
        // Used by CanUseItem (Akella 0x5ed7cf) to check if unit already immune to attack class.
        return emptyCategoryId; // game::AttackClassId
    case ModifierElementTypeFlag::MoveAllowance:
    case ModifierElementTypeFlag::ScoutingRange:
    case ModifierElementTypeFlag::Regeneration:
    case ModifierElementTypeFlag::AttackDrain:
    default:
        return 0;
    }
}

const char* __fastcall modifierGetDescription(const game::CUmModifier* thisptr, int /*%edx*/)
{
    std::string textIdString; // TODO: script function
    return getGlobalText(textIdString.c_str());
}

void __fastcall modifierUpdateUnitImplId(game::CUmModifier* thisptr, int /*%edx*/)
{
    auto thiz = castModifierToCustomModifier(thisptr);
    thiz->usUnit.id = thiz->getPrev()->id;
}

void __fastcall stackLeaderDtor(game::IUsStackLeader* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castStackLeaderToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

int __fastcall stackLeaderGetMovement(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getMovement(prev);
}

const char* __fastcall stackLeaderGetAbilityName(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getAbilityName(prev);
}

bool __fastcall stackLeaderHasMovementBonus(const game::IUsStackLeader* thisptr,
                                            int /*%edx*/,
                                            const game::LGroundCategory* ground)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->hasMovementBonus(prev, ground);
}

int __fastcall stackLeaderGetScout(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getScout(prev);
}

int __fastcall stackLeaderGetLeadership(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getLeadership(prev);
}

int __fastcall stackLeaderGetNegotiate(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getNegotiate(prev);
}

bool __fastcall stackLeaderHasAbility(const game::IUsStackLeader* thisptr,
                                      int /*%edx*/,
                                      const game::LLeaderAbility* ability)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->hasAbility(prev, ability);
}

bool __fastcall stackLeaderGetFastRetreat(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getFastRetreat(prev);
}

int __fastcall stackLeaderGetLowerCost(const game::IUsStackLeader* thisptr, int /*%edx*/)
{
    // TODO: script function
    auto prev = castStackLeaderToCustomModifier(thisptr)->getPrevStackLeader();
    return prev->vftable->getLowerCost(prev);
}

void __fastcall attackDtor(game::IAttack* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castAttackToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

const char* __fastcall attackGetName(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getName(prev);
}

const char* __fastcall attackGetDescription(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getDescription(prev);
}

game::LAttackClass* __fastcall attackGetAttackClass(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getAttackClass(prev);
}

game::LAttackSource* __fastcall attackGetAttackSource(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getAttackSource(prev);
}

int __fastcall attackGetInitiative(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getInitiative(prev);
}

int* __fastcall attackGetPower(const game::IAttack* thisptr, int /*%edx*/, int* power)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getPower(prev, power);
}

game::LAttackReach* __fastcall attackGetAttackReach(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getAttackReach(prev);
}

int __fastcall attackGetQtyDamage(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getQtyDamage(prev);
}

int __fastcall attackGetQtyHeal(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getQtyHeal(prev);
}

int __fastcall attackGetDrain(const game::IAttack* thisptr, int /*%edx*/, int damage)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getDrain(prev, damage);
}

int __fastcall attackGetLevel(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getLevel(prev);
}

const game::CMidgardID* __fastcall attackGetAltAttackId(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getAltAttackId(prev);
}

bool __fastcall attackGetInfinite(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getInfinite(prev);
}

game::IdVector* __fastcall attackGetWards(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getWards(prev);
}

bool __fastcall attackGetCritHit(const game::IAttack* thisptr, int /*%edx*/)
{
    // TODO: script function, differentiate between primary and secondary
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    return prev->vftable->getCritHit(prev);
}

void __fastcall attackGetData(const game::IAttack* thisptr, int /*%edx*/, game::CAttackData* value)
{
    auto prev = castAttackToCustomModifier(thisptr)->getPrevAttack(thisptr);
    prev->vftable->getData(prev, value);
}

void initUnitRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usUnit;
    replaceRttiInfo(info, CUmUnitApi::vftable().usUnit, false);

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&unitDtor;
    vftable.cast = (IUsUnitVftable::Cast)&unitCast;
    vftable.getCategory = (IUsUnitVftable::GetCategory)&unitGetCategory;
}

void initSoldierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usSoldier;
    replaceRttiInfo(info, CUmUnitApi::vftable().usSoldier, false);

    auto& vftable = info.vftable;
    vftable.destructor = (IUsUnitExtensionVftable::Destructor)&soldierDtor;
    vftable.getName = (IUsSoldierVftable::GetCStr)&soldierGetName;
    vftable.getDescription = (IUsSoldierVftable::GetCStr)&soldierGetDescription;
    vftable.getRaceId = (IUsSoldierVftable::GetId)&soldierGetRaceId;
    vftable.getSubrace = (IUsSoldierVftable::GetSubrace)&soldierGetSubrace;
    vftable.getBranch = (IUsSoldierVftable::GetBranch)&soldierGetBranch;
    vftable.getSizeSmall = (IUsSoldierVftable::GetBool)&soldierGetSizeSmall;
    vftable.getSexM = (IUsSoldierVftable::GetBool)&soldierGetSexM;
    vftable.getLevel = (IUsSoldierVftable::GetInt)&soldierGetLevel;
    vftable.getHitPoints = (IUsSoldierVftable::GetInt)&soldierGetHitPoints;
    vftable.getArmor = (IUsSoldierVftable::GetArmor)&soldierGetArmor;
    vftable.getBaseUnitImplId = (IUsSoldierVftable::GetId)&soldierGetBaseUnitImplId;
    vftable.getDeathAnim = (IUsSoldierVftable::GetDeathAnim)&soldierGetDeathAnim;
    vftable.getRegen = (IUsSoldierVftable::GetRegen)&soldierGetRegen;
    vftable.getXpNext = (IUsSoldierVftable::GetInt)&soldierGetXpNext;
    vftable.getXpKilled = (IUsSoldierVftable::GetInt)&soldierGetXpKilled;
    vftable.getImmuneByAttackClass = (IUsSoldierVftable::
                                          GetImmuneByAttackClass)&soldierGetImmuneByAttackClass;
    vftable.getImmuneByAttackSource = (IUsSoldierVftable::
                                           GetImmuneByAttackSource)&soldierGetImmuneByAttackSource;
    vftable.getAttackById = (IUsSoldierVftable::GetAttackById)&soldierGetAttackById;
    vftable.getSecondAttackById = (IUsSoldierVftable::GetAttackById)&soldierGetSecondAttackById;
    vftable.getAttackTwice = (IUsSoldierVftable::GetBool)&soldierGetAttackTwice;
    vftable.getEnrollCost = (IUsSoldierVftable::GetBank)&soldierGetEnrollCost;
    vftable.getReviveCost = (IUsSoldierVftable::GetBank)&soldierGetReviveCost;
    vftable.getHealCost = (IUsSoldierVftable::GetBank)&soldierGetHealCost;
    vftable.getTrainingCost = (IUsSoldierVftable::GetBank)&soldierGetTrainingCost;
    vftable.getDynUpg1 = (IUsSoldierVftable::GetId)&soldierGetDynUpg1;
    vftable.getDynUpgLvl = (IUsSoldierVftable::GetDynUpgLvl)&soldierGetDynUpgLvl;
    vftable.getDynUpg2 = (IUsSoldierVftable::GetId)&soldierGetDynUpg2;
    vftable.getWaterOnly = (IUsSoldierVftable::GetBool)&soldierGetWaterOnly;
}

void initModifierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.umModifier;
    replaceRttiInfo(info, CUmUnitApi::vftable().umModifier, false);

    auto& vftable = info.vftable;
    vftable.destructor = (CUmModifierVftable::Destructor)&modifierDtor;
    vftable.copy = (CUmModifierVftable::Copy)&modifierCopy;
    vftable.canApplyWithLeadership = (CUmModifierVftable::
                                          CanApplyWithLeadership)&modifierCanApplyWithLeadership;
    vftable.canApplyToUnit = (CUmModifierVftable::CanApplyToUnit)&modifierCanApplyToUnit;
    vftable.canApplyToUnitCategory = (CUmModifierVftable::
                                          CanApplyToUnitCategory)&modifierCanApplyToUnitCategory;
    vftable.isLower = (CUmModifierVftable::GetBool)&modifierIsLower;
    vftable.isBoost = (CUmModifierVftable::GetBool)&modifierIsBoost;
    vftable.hasElement = (CUmModifierVftable::HasElement)&modifierHasElement;
    vftable.getFirstElementValue = (CUmModifierVftable::
                                        GetFirstElementValue)&modifierGetFirstElementValue;
    vftable.getDescription = (CUmModifierVftable::GetDescription)&modifierGetDescription;
    vftable.updateUnitImplId = (CUmModifierVftable::UpdateUnitImplId)&modifierUpdateUnitImplId;
}

void initStackLeaderRttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IUsStackLeader for dynamic casts so we should be fine
    // with base RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.usStackLeader;
    replaceRttiInfo(info, IUsStackLeaderApi::vftable(), false);

    auto& vftable = info.vftable;
    vftable.destructor = (IUsUnitExtensionVftable::Destructor)&stackLeaderDtor;
    vftable.getMovement = (IUsStackLeaderVftable::GetInt)&stackLeaderGetMovement;
    vftable.getAbilityName = (IUsStackLeaderVftable::GetAbilityName)&stackLeaderGetAbilityName;
    vftable
        .hasMovementBonus = (IUsStackLeaderVftable::HasMovementBonus)&stackLeaderHasMovementBonus;
    vftable.getScout = (IUsStackLeaderVftable::GetInt)&stackLeaderGetScout;
    vftable.getLeadership = (IUsStackLeaderVftable::GetInt)&stackLeaderGetLeadership;
    vftable.getNegotiate = (IUsStackLeaderVftable::GetInt)&stackLeaderGetNegotiate;
    vftable.hasAbility = (IUsStackLeaderVftable::HasAbility)&stackLeaderHasAbility;
    vftable.getFastRetreat = (IUsStackLeaderVftable::GetFastRetreat)&stackLeaderGetFastRetreat;
    vftable.getLowerCost = (IUsStackLeaderVftable::GetInt)&stackLeaderGetLowerCost;
}

void initAttackRttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IAttack for dynamic casts so we should be fine
    // with base RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.attack;
    replaceRttiInfo(info, IAttackApi::vftable(), false);

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&attackDtor;
    vftable.getName = (IAttackVftable::GetCStr)&attackGetName;
    vftable.getDescription = (IAttackVftable::GetCStr)&attackGetDescription;
    vftable.getAttackClass = (IAttackVftable::GetAttackClass)&attackGetAttackClass;
    vftable.getAttackSource = (IAttackVftable::GetAttackSource)&attackGetAttackSource;
    vftable.getInitiative = (IAttackVftable::GetInitiative)&attackGetInitiative;
    vftable.getPower = (IAttackVftable::GetPower)&attackGetPower;
    vftable.getAttackReach = (IAttackVftable::GetAttackReach)&attackGetAttackReach;
    vftable.getQtyDamage = (IAttackVftable::GetInt)&attackGetQtyDamage;
    vftable.getQtyHeal = (IAttackVftable::GetInt)&attackGetQtyHeal;
    vftable.getDrain = (IAttackVftable::GetDrain)&attackGetDrain;
    vftable.getLevel = (IAttackVftable::GetInt)&attackGetLevel;
    vftable.getAltAttackId = (IAttackVftable::GetId)&attackGetAltAttackId;
    vftable.getInfinite = (IAttackVftable::GetBool)&attackGetInfinite;
    vftable.getWards = (IAttackVftable::GetWards)&attackGetWards;
    vftable.getCritHit = (IAttackVftable::GetBool)&attackGetCritHit;
    vftable.getData = (IAttackVftable::GetData)&attackGetData;
}

void initAttack2RttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IAttack for dynamic casts so we should be fine
    // with base RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.attack2;
    replaceRttiInfo(info, IAttackApi::vftable(), false);

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&attackDtor;
    vftable.getName = (IAttackVftable::GetCStr)&attackGetName;
    vftable.getDescription = (IAttackVftable::GetCStr)&attackGetDescription;
    vftable.getAttackClass = (IAttackVftable::GetAttackClass)&attackGetAttackClass;
    vftable.getAttackSource = (IAttackVftable::GetAttackSource)&attackGetAttackSource;
    vftable.getInitiative = (IAttackVftable::GetInitiative)&attackGetInitiative;
    vftable.getPower = (IAttackVftable::GetPower)&attackGetPower;
    vftable.getAttackReach = (IAttackVftable::GetAttackReach)&attackGetAttackReach;
    vftable.getQtyDamage = (IAttackVftable::GetInt)&attackGetQtyDamage;
    vftable.getQtyHeal = (IAttackVftable::GetInt)&attackGetQtyHeal;
    vftable.getDrain = (IAttackVftable::GetDrain)&attackGetDrain;
    vftable.getLevel = (IAttackVftable::GetInt)&attackGetLevel;
    vftable.getAltAttackId = (IAttackVftable::GetId)&attackGetAltAttackId;
    vftable.getInfinite = (IAttackVftable::GetBool)&attackGetInfinite;
    vftable.getWards = (IAttackVftable::GetWards)&attackGetWards;
    vftable.getCritHit = (IAttackVftable::GetBool)&attackGetCritHit;
    vftable.getData = (IAttackVftable::GetData)&attackGetData;
}

void initRttiInfo()
{
    static bool initialized = false;
    if (!initialized) {
        initUnitRttiInfo();
        initSoldierRttiInfo();
        initModifierRttiInfo();
        initStackLeaderRttiInfo();
        initAttackRttiInfo();
        initAttack2RttiInfo();
        initialized = true;
    }
}

void initVftable(CCustomModifier* thisptr)
{
    thisptr->usUnit.vftable = &rttiInfo.usUnit.vftable;
    thisptr->usSoldier.vftable = &rttiInfo.usSoldier.vftable;
    thisptr->umModifier.vftable = &rttiInfo.umModifier.vftable;
    thisptr->usStackLeader.vftable = &rttiInfo.usStackLeader.vftable;
    thisptr->attack.vftable = &rttiInfo.attack.vftable;
    thisptr->attack2.vftable = &rttiInfo.attack2.vftable;
}

game::CUmModifier* createCustomModifier(const char* script,
                                        const game::CMidgardID* id,
                                        const game::GlobalData** globalData)
{
    using namespace game;

    auto customModifier = (CCustomModifier*)Memory::get().allocate(sizeof(CCustomModifier));
    customModifierCtor(customModifier, script, id, globalData);
    return &customModifier->umModifier;
}

} // namespace hooks
