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

#include "unitutils.h"
#include "attack.h"
#include "attacksourcecat.h"
#include "attacksourcelist.h"
#include "attackutils.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "dynamiccast.h"
#include "fortcategory.h"
#include "fortification.h"
#include "game.h"
#include "gameutils.h"
#include "globaldata.h"
#include "globalvariables.h"
#include "immunecat.h"
#include "leaderabilitycat.h"
#include "log.h"
#include "lordtype.h"
#include "midgardid.h"
#include "midgardmap.h"
#include "midgardmapblock.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midstack.h"
#include "midunit.h"
#include "midvillage.h"
#include "modifierutils.h"
#include "racetype.h"
#include "scenarioinfo.h"
#include "settings.h"
#include "ummodifier.h"
#include "unitgenerator.h"
#include "unitmodifier.h"
#include "usleader.h"
#include "usracialsoldier.h"
#include "ussoldier.h"
#include "ussoldierimpl.h"
#include "usstackleader.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void generateUnitImplByAttackId(const game::CMidgardID* attackId)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    int typeIndex = id.getTypeIndex(attackId);
    int categoryIndex = id.getCategoryIndex(attackId);
    IdCategory category = id.getCategory(attackId);

    CMidgardID unitImplId{};
    id.fromParts(&unitImplId, category, categoryIndex, IdType::UnitGenerated, typeIndex);

    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->generateUnitImpl(unitGenerator, &unitImplId);
}

game::IUsSoldier* castUnitImplToSoldierWithLogging(const game::IUsUnit* unitImpl)
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(unitImpl);
    if (!soldier) {
        hooks::logError("mssProxyError.log", fmt::format("Failed to cast unit impl {:s} to soldier",
                                                         hooks::idToString(&unitImpl->id)));
    }

    return soldier;
}

void getSoldierAttackSourceImmunities(const game::LImmuneCat* immuneCat,
                                      const game::IUsSoldier* soldier,
                                      game::List<game::LAttackSource>* value)
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

bool isUnitSmall(const game::CMidUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    return soldier->vftable->getSizeSmall(soldier);
}

game::CMidgardID getGlobalUnitImplId(const game::CMidgardID* unitImplId)
{
    using namespace game;

    CMidgardID globalImplId{};
    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->getGlobalUnitImplId(unitGenerator, &globalImplId, unitImplId);

    return globalImplId;
}

game::TUsUnitImpl* getGlobalUnitImpl(const game::CMidgardID* unitImplId)
{
    const game::CMidgardID globalImplId{getGlobalUnitImplId(unitImplId)};
    return getUnitImpl(&globalImplId);
}

game::TUsUnitImpl* generateUnitImpl(const game::CMidgardID* unitImplId, int level)
{
    using namespace game;

    CMidgardID generatedId;
    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &generatedId, unitImplId, level);

    if (unitGenerator->vftable->isUnitGenerated(unitGenerator, &generatedId)) {
        unitGenerator->vftable->generateUnitImpl(unitGenerator, &generatedId);
    }

    return getUnitImpl(&generatedId);
}

game::TUsUnitImpl* getUnitImpl(const game::CMidgardID* unitImplId)
{
    using namespace game;

    const auto& global = GlobalDataApi::get();

    auto globalData = *global.getGlobalData();
    auto result = (TUsUnitImpl*)global.findById(globalData->units, unitImplId);
    if (!result) {
        logError("mssProxyError.log",
                 fmt::format("Could not find unit impl {:s}", idToString(unitImplId)));
    }

    return result;
}

game::TUsUnitImpl* getUnitImpl(const game::IUsUnit* unit)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    CUmModifier* modifier = nullptr;
    for (auto curr = unit; curr; curr = modifier->data->prev) {
        modifier = (CUmModifier*)dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
        if (!modifier)
            return (TUsUnitImpl*)curr;
    }

    return nullptr;
}

game::TUsSoldierImpl* getSoldierImpl(const game::IUsUnit* unit)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    return (TUsSoldierImpl*)dynamicCast(getUnitImpl(unit), 0, rtti.IUsUnitType,
                                        rtti.TUsSoldierImplType, 0);
}

game::TUsSoldierImpl* getSoldierImpl(const game::IUsSoldier* soldier)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto unit = (IUsUnit*)dynamicCast(soldier, 0, rtti.IUsSoldierType, rtti.IUsUnitType, 0);
    return getSoldierImpl(unit);
}

game::IAttack* getAttack(const game::IUsUnit* unit, bool primary, bool checkAltAttack)
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(unit);
    if (!soldier)
        return nullptr;

    auto attack = primary ? soldier->vftable->getAttackById(soldier)
                          : soldier->vftable->getSecondAttackById(soldier);

    if (attack && checkAltAttack) {
        auto altAttack = getGlobalAttack(attack->vftable->getAltAttackId(attack));
        if (altAttack)
            return wrapAltAttack(unit, altAttack);
    }

    return attack;
}

game::IAttack* getAltAttack(const game::IUsUnit* unit, bool primary)
{
    auto attack = getAttack(unit, primary, false);
    auto altAttack = getGlobalAttack(attack->vftable->getAltAttackId(attack));
    if (!altAttack) {
        return nullptr;
    }

    return wrapAltAttack(unit, altAttack);
}

int getArmor(const game::CMidgardID* unitId,
             const game::IUsSoldier* soldier,
             const game::BattleMsgData* battleMsgData,
             bool includeShattered,
             bool includeFortification)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    int armor;
    soldier->vftable->getArmor(soldier, &armor);

    if (!includeShattered)
        armor -= battle.getUnitShatteredArmor(battleMsgData, unitId);
    if (!includeFortification)
        armor -= battle.getUnitFortificationArmor(battleMsgData, unitId);

    return armor > 0 ? armor : 0;
}

const game::CDynUpgrade* getDynUpgrade(const game::IUsUnit* unit, int upgradeNumber)
{
    using namespace game;

    if (upgradeNumber != 1 && upgradeNumber != 2) {
        return nullptr;
    }

    auto soldier = hooks::castUnitImplToSoldierWithLogging(unit);
    if (!soldier) {
        return nullptr;
    }

    auto id = upgradeNumber == 1 ? soldier->vftable->getDynUpg1(soldier)
                                 : soldier->vftable->getDynUpg2(soldier);
    if (!id) {
        hooks::logError("mssProxyError.log",
                        fmt::format("Dyn upgrade {:d} id is null, unit impl {:s}", upgradeNumber,
                                    hooks::idToString(&unit->id)));
        return nullptr;
    }

    const auto& globalApi = GlobalDataApi::get();
    auto globalData = *globalApi.getGlobalData();

    auto upgrade = globalApi.findDynUpgradeById(globalData->dynUpgrade, id);
    if (!upgrade) {
        hooks::logError("mssProxyError.log", fmt::format("Could not find dyn upgrade {:d} {:s}",
                                                         upgradeNumber, hooks::idToString(id)));
        return nullptr;
    }

    return upgrade;
}

int getCityProtection(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn{gameFunctions()};
    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto findObject{objectMap->vftable->findScenarioObjectById};

    auto objectId{fn.getStackFortRuinId(unitId, objectMap)};
    const auto objectType{CMidgardIDApi::get().getType(objectId)};

    CMidgardID fortificationId{emptyId};

    if (objectType == IdType::Fortification) {
        fortificationId = *objectId;
    } else if (objectType == IdType::Stack) {
        auto stackObject{findObject(objectMap, objectId)};
        auto stack{(const CMidStack*)dynamicCast(stackObject, 0, rtti.IMidScenarioObjectType,
                                                 rtti.CMidStackType, 0)};

        if (stack && stack->insideId != emptyId) {
            fortificationId = stack->insideId;
        }
    }

    if (fortificationId != emptyId) {
        auto fortification{(CFortification*)findObject(objectMap, &fortificationId)};

        if (fortification) {
            auto vftable{(const CFortificationVftable*)fortification->vftable};

            const auto& list{IdListApi::get()};
            IdList modifiers;
            list.constructor(&modifiers);

            vftable->getProtection(fortification, objectMap, &modifiers);

            const auto protection{applyModifiers(0, modifiers, ModifierElementTypeFlag::Armor)};
            list.destructor(&modifiers);

            return protection;
        }
    }

    // Only fortifications (cities) grant protection
    return 0;
}

static int getLordRegenBonus(const game::CMidPlayer* player)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& globalApi = GlobalDataApi::get();

    const auto globalData = *globalApi.getGlobalData();
    const auto vars = *globalData->globalVariables;

    if (!player || player->capturedById != emptyId) {
        return 0;
    }

    if (fn.isRaceCategoryUnplayable(&player->raceType->data->raceType)) {
        return 0;
    }

    const auto lords = globalData->lords;
    const auto lordType = (const TLordType*)globalApi.findById(lords, &player->lordId);
    if (lordType->data->lordCategory.id == LordCategories::get().warrior->id) {
        return vars->fighterLeaderRegen;
    }

    return 0;
}

static int getFortRegen(int base,
                        const game::IMidgardObjectMap* objectMap,
                        const game::CFortification* fort)
{
    using namespace game;

    auto vftable{static_cast<const CFortificationVftable*>(fort->vftable)};
    auto category{vftable->getCategory(fort)};
    if (category->id == FortCategories::get().village->id) {
        auto village{static_cast<const CMidVillage*>(fort)};
        if (village->riotTurn > 0) {
            return 0; // Units in rioting villages take damage instead of regeneration
        }
    }

    int regen;
    return base + *vftable->getRegen(fort, &regen, objectMap);
}

static int getTerrainRegenBonus(const game::IMidgardObjectMap* objectMap,
                                const game::CMidPlayer* player,
                                const game::CMidStack* stack)
{
    using namespace game;

    auto map = getMidgardMap(objectMap);
    auto block = getMidgardMapBlock(objectMap, &map->id, map->mapSize, stack->position.x,
                                    stack->position.y);

    LTerrainCategory terrain{};
    if (!CMidgardMapBlockApi::get().getTerrain(block, &terrain, &stack->position)) {
        return 0;
    }

    auto raceTerrain = getTerrainCategory(&player->raceType->data->raceType);
    if (terrain.id == raceTerrain->id) {
        return 10; // Strictly hard-coded in game, not even const data
    }

    return 0;
}

// See CEffectRegenUnitsApply, CEffectRegenRuinsApply
int getUnitRegen(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& globalApi = GlobalDataApi::get();

    const auto globalData = *globalApi.getGlobalData();
    const auto vars = *globalData->globalVariables;

    const CMidPlayer* player = nullptr;
    const CFortification* fort = nullptr;
    const CMidRuin* ruin = nullptr;
    const CMidStack* stack = getStackByUnitId(objectMap, unitId);
    if (stack) {
        player = getPlayer(objectMap, &stack->ownerId);
        if (stack->insideId != emptyId) {
            fort = getFort(objectMap, &stack->insideId);
        }
    } else {
        fort = getFortByUnitId(objectMap, unitId);
        if (fort) {
            player = getPlayer(objectMap, &fort->ownerId);
        } else {
            ruin = getRuinByUnitId(objectMap, unitId);
        }
    }

    auto unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    int result = *soldier->vftable->getRegen(soldier);
    result += getLordRegenBonus(player);
    if (fort) {
        // Total regen in fort can be zero in case of riot
        result = getFortRegen(result, objectMap, fort);
    } else if (ruin) {
        // Units in ruins have fixed regen value, no other factors apply
        result = vars->regenRuin;
    } else {
        // Terrain bonus apply only outside
        result += getTerrainRegenBonus(objectMap, player, stack);
    }

    return std::clamp(result, 0, 100);
}

int getUnitHpMax(const game::CMidUnit* unit)
{
    using namespace game;

    if (unit->transformed && unit->keepHp)
        return unit->hpBefMax;

    auto soldier = gameFunctions().castUnitImplToSoldier(unit->unitImpl);
    return soldier->vftable->getHitPoints(soldier);
}

const char* getUnitName(const game::CMidUnit* unit)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();

    auto impl = unit->unitImpl;
    if (castUnitImpl(impl, rtti.IUsStackLeaderType) && !castUnitImpl(impl, rtti.IUsNobleType)
        && !castUnitImpl(impl, rtti.IUsSummonType)) {
        return unit->name.string ? unit->name.string : "";
    }

    auto soldier = (IUsSoldier*)castUnitImpl(impl, rtti.IUsSoldierType);
    return soldier->vftable->getName(soldier);
}

game::IUsUnitExtension* castUnitImpl(const game::IUsUnit* unitImpl,
                                     const game::TypeDescriptor* type)
{
    using namespace game;

    auto& typeInfoRawName = *RttiApi::get().typeInfoRawName;

    return unitImpl ? unitImpl->vftable->cast(unitImpl, typeInfoRawName(type)) : nullptr;
}

int computeUnitEffectiveHpForAi(int hp, int armor)
{
    if (userSettings().fixEffectiveHpFormula) {
        return computeUnitEffectiveHp(hp, armor);
    }

    if (hp < 0) {
        return 0;
    }

    return hp * armor / 100 + hp;
}

int computeUnitEffectiveHp(int hp, int armor)
{
    if (hp <= 0) {
        return 0;
    }

    int factor = 100 - armor;
    if (factor <= 0)
        return std::numeric_limits<int>::max();

    return hp * 100 / factor;
}

int computeShatterDamage(const game::CMidgardID* unitId,
                         const game::IUsSoldier* soldier,
                         const game::BattleMsgData* battleMsgData,
                         const game::IAttack* attack)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    int armor = getArmor(unitId, soldier, battleMsgData, false, false);

    int limit = userSettings().shatteredArmorMax
                - battle.getUnitShatteredArmor(battleMsgData, unitId);

    int result = attack->vftable->getQtyDamage(attack);
    if (result > armor)
        result = armor;
    if (result > limit)
        result = limit;
    if (result > userSettings().shatterDamageMax)
        result = userSettings().shatterDamageMax;

    return result;
}

void updateAttackCountAfterTransformation(game::BattleMsgData* battleMsgData,
                                          const game::CMidUnit* unit,
                                          bool prevAttackTwice)
{
    using namespace game;

    for (auto& turn : battleMsgData->turnsOrder) {
        if (turn.unitId == unit->id) {
            const auto soldier = gameFunctions().castUnitImplToSoldier(unit->unitImpl);
            bool attackTwice = soldier && soldier->vftable->getAttackTwice(soldier);

            if (!prevAttackTwice && attackTwice)
                turn.attackCount++;
            else if (prevAttackTwice && !attackTwice && turn.attackCount > 1)
                turn.attackCount--;

            break;
        }
    }
}

bool isStackLeaderAndAllowedToUseBattleItems(const game::IMidgardObjectMap* objectMap,
                                             const game::CMidgardID* unitId,
                                             const game::BattleMsgData* battleMsgData)
{
    using namespace game;

    const auto& battleApi = BattleMsgDataApi::get();

    auto stack = getStack(objectMap, battleMsgData, unitId);
    if (!stack || stack->leaderId != *unitId)
        return false;

    if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::Transform))
        return userSettings().allowBattleItems.onTransformOther;
    else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::TransformSelf))
        return userSettings().allowBattleItems.onTransformSelf;
    else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::TransformDrainLevel))
        return userSettings().allowBattleItems.onDrainLevel;
    else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::TransformDoppelganger))
        return userSettings().allowBattleItems.onDoppelganger;

    return true;
}

bool hasLeaderAbility(const game::IUsUnit* unitImpl, const game::LLeaderAbility* ability)
{
    using namespace game;

    auto stackLeader{gameFunctions().castUnitImplToStackLeader(unitImpl)};
    if (!stackLeader) {
        return false;
    }

    return stackLeader->vftable->hasAbility(stackLeader, ability);
}

bool hasCriticalHitLeaderAbility(const game::IUsUnit* unitImpl)
{
    using namespace game;

    return hasLeaderAbility(unitImpl, LeaderAbilityCategories::get().criticalHit);
}

bool validateUnit(game::CMidUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    IUsUnit* origImpl = unit->transformed ? hooks::getUnitImpl(&unit->origTypeId) : unit->unitImpl;
    auto soldier = fn.castUnitImplToSoldier(origImpl);

    bool result = false;
    auto validXp = std::clamp(unit->currentXp, 0, soldier->vftable->getXpNext(soldier));
    if (unit->currentXp != validXp) {
        unit->currentXp = validXp;
        result = true;
    }

    auto validHp = std::clamp(unit->currentHp, 0, getUnitHpMax(unit));
    if (unit->currentHp != validHp) {
        unit->currentHp = validHp;
        result = true;
    }

    return result;
}

bool canUnitGainXp(const game::IUsUnit* unitImpl)
{
    using namespace game;

    auto unitCategory = unitImpl->vftable->getCategory(unitImpl);
    return unitCategory->id != UnitId::Noble && unitCategory->id != UnitId::Summon
           && unitCategory->id != UnitId::Illusion && unitCategory->id != UnitId::Guardian;
}

bool isNextUnitImpl(const game::IUsUnit* unitImpl, const game::IUsUnit* currImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (fn.castUnitImplToLeader(currImpl)) {
        auto leader = fn.castUnitImplToLeader(unitImpl);
        if (leader) {
            return *leader->vftable->getPrevUnitImplId(leader) == currImpl->id;
        }
    } else {
        auto racialSoldier = fn.castUnitImplToRacialSoldier(unitImpl);
        if (racialSoldier) {
            return *racialSoldier->vftable->getPrevUnitImplId(racialSoldier) == currImpl->id;
        }
    }

    return false;
}

bool hasNextTierUnitImpl(const game::IUsUnit* unitImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& idApi = CMidgardIDApi::get();
    const auto& globalApi = GlobalDataApi::get();

    const auto globalData = *globalApi.getGlobalData();
    const auto& units = globalData->units->map->data;
    for (auto it = units.bgn; it != units.end; ++it) {
        if (idApi.getType(&it->first) != IdType::UnitGenerated) {
            if (isNextUnitImpl(it->second, unitImpl)) {
                return true;
            }
        }
    }

    return false;
}

bool hasMaxTierUpgradeBuilding(const game::IMidgardObjectMap* objectMap,
                               const game::IUsUnit* unitImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto racialSoldier = fn.castUnitImplToRacialSoldier(unitImpl);
    if (!racialSoldier) {
        return false;
    }

    auto upgradeBuildingId = racialSoldier->vftable->getUpgradeBuildingId(racialSoldier);
    if (*upgradeBuildingId == emptyId) {
        return false;
    }

    auto scenarioInfo = getScenarioInfo(objectMap);
    return getBuildingLevel(upgradeBuildingId) >= scenarioInfo->unitMaxTier;
}

bool isNextTierUnitImpl(const game::IMidgardObjectMap* objectMap,
                        const game::CMidPlayer* player,
                        const game::CMidUnit* unit,
                        const game::TUsUnitImpl* unitImpl,
                        bool* requiresBuilding)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& idApi = CMidgardIDApi::get();

    if (idApi.getType(&unitImpl->id) == IdType::UnitGenerated) {
        return false;
    }

    if (!isNextUnitImpl(unitImpl, unit->unitImpl)) {
        return false;
    }

    auto racialSoldier = fn.castUnitImplToRacialSoldier(unitImpl);
    if (racialSoldier) {
        auto upgradeBuildingId = *racialSoldier->vftable->getUpgradeBuildingId(racialSoldier);
        if (upgradeBuildingId != emptyId) {
            if (!player || !lordHasBuilding(&player->lordId, &upgradeBuildingId)) {
                return false;
            }

            *requiresBuilding = !playerHasBuilding(objectMap, player, &upgradeBuildingId);
            return true;
        }
    }

    *requiresBuilding = false;
    return true;
}

const game::TUsUnitImpl* getUpgradeUnitImpl(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidPlayer* player,
                                            const game::CMidUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& globalApi = GlobalDataApi::get();
    const auto& idApi = CMidgardIDApi::get();

    if (!unit->dynLevel && idApi.getType(&unit->unitImpl->id) != IdType::UnitGenerated) {
        bool requiresBuilding = false;
        const auto globalData = *globalApi.getGlobalData();
        const auto& units = globalData->units->map->data;
        for (auto it = units.bgn; it != units.end; ++it) {
            if (isNextTierUnitImpl(objectMap, player, unit, it->second, &requiresBuilding)) {
                if (!requiresBuilding) {
                    return it->second;
                }
            }
        }

        if (requiresBuilding && !hasMaxTierUpgradeBuilding(objectMap, unit->unitImpl)) {
            return nullptr;
        }
    }

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    return generateUnitImpl(&unit->unitImpl->id, soldier->vftable->getLevel(soldier) + 1);
}

int getGeneratedUnitImplLevelMax()
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    const auto globalData = *globalApi.getGlobalData();
    const auto units = globalData->units;

    return 0xffff / units->baseCount;
}

} // namespace hooks
