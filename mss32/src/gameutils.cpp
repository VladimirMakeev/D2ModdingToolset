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

#include "gameutils.h"
#include "battlemsgdata.h"
#include "buildingtype.h"
#include "dynamiccast.h"
#include "fortification.h"
#include "game.h"
#include "globalvariables.h"
#include "idset.h"
#include "leaderabilitycat.h"
#include "lordtype.h"
#include "midclient.h"
#include "midclientcore.h"
#include "middiplomacy.h"
#include "midgard.h"
#include "midgardmap.h"
#include "midgardmapblock.h"
#include "midgardmapfog.h"
#include "midgardobjectmap.h"
#include "midgardplan.h"
#include "midgardscenariomap.h"
#include "midplayer.h"
#include "midrod.h"
#include "midruin.h"
#include "midscenvariables.h"
#include "midserver.h"
#include "midserverlogic.h"
#include "midstack.h"
#include "midunit.h"
#include "playerbuildings.h"
#include "scenarioinfo.h"
#include "scenedit.h"
#include "unitutils.h"
#include "ussoldier.h"
#include "version.h"
#include <thread>

extern std::thread::id mainThreadId;

namespace hooks {

static game::CMidgardID createIdWithType(const game::IMidgardObjectMap* objectMap,
                                         game::IdType idType)
{
    using namespace game;

    const auto& idApi{CMidgardIDApi::get()};
    auto scenarioId{objectMap->vftable->getId(objectMap)};

    CMidgardID id{};
    idApi.fromParts(&id, idApi.getCategory(scenarioId), idApi.getCategoryIndex(scenarioId), idType,
                    0);

    return id;
}

bool isGreaterPickRandomIfEqual(int first, int second)
{
    using namespace game;

    const auto& fn = gameFunctions();

    return first > second || (first == second && fn.generateRandomNumber(2) == 1);
}

const game::IMidgardObjectMap* getObjectMap()
{
    using namespace game;

    if (gameVersion() == GameVersion::ScenarioEditor) {
        auto editorData = CScenEditApi::get().instance()->data;
        if (!editorData->initialized)
            return nullptr;

        auto unknown2 = editorData->unknown2;
        if (!unknown2 || !unknown2->data)
            return nullptr;

        return unknown2->data->scenarioMap;
    }

    auto midgard = CMidgardApi::get().instance();
    if (std::this_thread::get_id() == mainThreadId) {
        auto client = midgard->data->client;
        if (!client)
            return nullptr;

        return CMidClientCoreApi::get().getObjectMap(&client->core);
    } else {
        auto server = midgard->data->server;
        if (!server)
            return nullptr;

        return CMidServerLogicApi::get().getObjectMap(server->data->serverLogic);
    }
}

game::CMidUnitGroup* getGroup(game::IMidgardObjectMap* objectMap,
                              const game::CMidgardID* groupId,
                              bool forChange)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto obj = forChange ? objectMap->vftable->findScenarioObjectByIdForChange(objectMap, groupId)
                         : objectMap->vftable->findScenarioObjectById(objectMap, groupId);
    switch (CMidgardIDApi::get().getType(groupId)) {
    case IdType::Stack: {
        auto stack = (CMidStack*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                             rtti.CMidStackType, 0);
        return stack ? &stack->group : nullptr;
    }
    case IdType::Fortification: {
        auto fortification = static_cast<CFortification*>(obj);
        return fortification ? &fortification->group : nullptr;
    }
    case IdType::Ruin:
        auto ruin = static_cast<CMidRuin*>(obj);
        return ruin ? &ruin->group : nullptr;
    }

    return nullptr;
}

const game::CMidUnitGroup* getGroup(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* groupId)
{
    return getGroup(const_cast<game::IMidgardObjectMap*>(objectMap), groupId, false);
}

const game::CMidUnitGroup* getAllyOrEnemyGroup(const game::IMidgardObjectMap* objectMap,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::CMidgardID* unitId,
                                               bool ally)
{
    using namespace game;

    const auto& fn = gameFunctions();

    CMidgardID groupId{};
    fn.getAllyOrEnemyGroupId(&groupId, battleMsgData, unitId, ally);

    void* tmp{};
    return fn.getStackFortRuinGroup(tmp, objectMap, &groupId);
}

const game::CScenarioInfo* getScenarioInfo(const game::IMidgardObjectMap* objectMap)
{
    const auto id{createIdWithType(objectMap, game::IdType::ScenarioInfo)};

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &id);
    if (!obj) {
        return nullptr;
    }

    return static_cast<const game::CScenarioInfo*>(obj);
}

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId)
{
    using namespace game;

    auto playerObj = objectMap->vftable->findScenarioObjectById(objectMap, playerId);
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    return (const CMidPlayer*)dynamicCast(playerObj, 0, rtti.IMidScenarioObjectType,
                                          rtti.CMidPlayerType, 0);
}

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    CMidgardID playerId = battle.isUnitAttacker(battleMsgData, unitId)
                              ? battleMsgData->attackerPlayerId
                              : battleMsgData->defenderPlayerId;

    return getPlayer(objectMap, &playerId);
}

const game::CMidPlayer* getPlayerByUnitId(const game::IMidgardObjectMap* objectMap,
                                          const game::CMidgardID* unitId)
{
    using namespace game;

    auto playerId = getPlayerIdByUnitId(objectMap, unitId);
    return playerId != emptyId ? getPlayer(objectMap, &playerId) : nullptr;
}

const game::CMidgardID getPlayerIdByUnitId(const game::IMidgardObjectMap* objectMap,
                                           const game::CMidgardID* unitId)
{
    using namespace game;

    auto stack = getStackByUnitId(objectMap, unitId);
    if (stack) {
        return stack->ownerId;
    }

    auto fort = getFortByUnitId(objectMap, unitId);
    if (fort) {
        return fort->ownerId;
    }

    return emptyId;
}

const game::CMidScenVariables* getScenarioVariables(const game::IMidgardObjectMap* objectMap)
{
    const auto id{createIdWithType(objectMap, game::IdType::ScenarioVariable)};

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &id)};
    if (!obj) {
        return nullptr;
    }

    return static_cast<const game::CMidScenVariables*>(obj);
}

const game::CMidgardPlan* getMidgardPlan(const game::IMidgardObjectMap* objectMap)
{
    const auto id{createIdWithType(objectMap, game::IdType::Plan)};

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &id)};
    if (!obj) {
        return nullptr;
    }

    return static_cast<const game::CMidgardPlan*>(obj);
}

const game::CMidgardMap* getMidgardMap(const game::IMidgardObjectMap* objectMap)
{
    const auto id{createIdWithType(objectMap, game::IdType::Map)};

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &id)};
    if (!obj) {
        return nullptr;
    }

    return static_cast<const game::CMidgardMap*>(obj);
}

const game::CMidgardMapBlock* getMidgardMapBlock(const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidgardID* mapId,
                                                 int mapSize,
                                                 int x,
                                                 int y)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();

    if (x < 0 || x >= mapSize || y < 0 || y >= mapSize) {
        // Outside of map
        return nullptr;
    }

    CMidgardID blockId{};
    const std::uint32_t blockX = x / 8 * 8;
    const std::uint32_t blockY = y / 4 * 4;
    id.fromParts(&blockId, IdCategory::Scenario, id.getCategoryIndex(mapId), IdType::MapBlock,
                 blockX | (blockY << 8));

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &blockId);
    return static_cast<const CMidgardMapBlock*>(obj);
}

const game::LTerrainCategory* getTerrainCategory(const game::LRaceCategory* raceCategory)
{
    using namespace game;

    const auto& terrains = TerrainCategories::get();
    switch (raceCategory->id) {
    case RaceId::Human:
        return terrains.human;
    case RaceId::Heretic:
        return terrains.heretic;
    case RaceId::Undead:
        return terrains.undead;
    case RaceId::Dwarf:
        return terrains.dwarf;
    case RaceId::Elf:
        return terrains.elf;
    default:
        return terrains.neutral;
    }
}

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* stackId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, stackId);
    if (!obj) {
        return nullptr;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    return (CMidStack*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType, rtti.CMidStackType, 0);
}

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap,
                          const game::BattleMsgData* battleMsgData,
                          const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    CMidgardID groupId = battle.isUnitAttacker(battleMsgData, unitId)
                             ? battleMsgData->attackerGroupId
                             : battleMsgData->defenderGroupId;

    return getStack(objectMap, &groupId);
}

const game::CMidStack* getStackByUnitId(const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* unitId)
{
    using namespace game;

    auto stackId = gameFunctions().getStackIdByUnitId(objectMap, unitId);

    return stackId ? getStack(objectMap, stackId) : nullptr;
}

game::CFortification* getFort(const game::IMidgardObjectMap* objectMap,
                              const game::CMidgardID* fortId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, fortId);
    return static_cast<CFortification*>(obj);
}

game::CFortification* getFort(const game::IMidgardObjectMap* objectMap,
                              const game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    CMidgardID groupId = battle.isUnitAttacker(battleMsgData, unitId)
                             ? battleMsgData->attackerGroupId
                             : battleMsgData->defenderGroupId;

    return getFort(objectMap, &groupId);
}

const game::CFortification* getFortByUnitId(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidgardID* unitId)
{
    using namespace game;

    auto fortId = gameFunctions().getFortIdByUnitId(objectMap, unitId);

    return fortId ? getFort(objectMap, fortId) : nullptr;
}

game::CMidRuin* getRuin(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* ruinId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, ruinId);
    return static_cast<CMidRuin*>(obj);
}

game::CMidRuin* getRuin(const game::IMidgardObjectMap* objectMap,
                        const game::BattleMsgData* battleMsgData,
                        const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    CMidgardID groupId = battle.isUnitAttacker(battleMsgData, unitId)
                             ? battleMsgData->attackerGroupId
                             : battleMsgData->defenderGroupId;

    return getRuin(objectMap, &groupId);
}

const game::CMidRuin* getRuinByUnitId(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId)
{
    using namespace game;

    auto ruinId = gameFunctions().getRuinIdByUnitId(objectMap, unitId);

    return ruinId ? getRuin(objectMap, ruinId) : nullptr;
}

game::CMidRod* getRod(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* rodId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, rodId);
    return static_cast<CMidRod*>(obj);
}

int getGroupXpKilled(const game::IMidgardObjectMap* objectMap, const game::CMidUnitGroup* group)
{
    using namespace game;

    const auto& fn = gameFunctions();

    int result = 0;
    for (const game::CMidgardID* it = group->units.bgn; it != group->units.end; ++it) {
        const auto unit = fn.findUnitById(objectMap, it);
        if (unit->currentHp <= 0) {
            continue;
        }

        const auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
        result += soldier->vftable->getXpKilled(soldier);
    }

    return result;
}

game::CMidInventory* getInventory(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* groupId)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto groupObj = objectMap->vftable->findScenarioObjectById(objectMap, groupId);
    if (idApi.getType(groupId) == IdType::Stack) {
        auto stack = static_cast<CMidStack*>(
            dynamicCast(groupObj, 0, rtti.IMidScenarioObjectType, rtti.CMidStackType, 0));
        return &stack->inventory;
    } else if (idApi.getType(groupId) == IdType::Fortification) {
        auto fort = static_cast<CFortification*>(
            dynamicCast(groupObj, 0, rtti.IMidScenarioObjectType, rtti.CFortificationType, 0));
        return &fort->inventory;
    }

    return nullptr;
}

bool canGroupGainXp(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* groupId)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();

    if (idApi.getType(groupId) == IdType::Stack) {
        auto stack = getStack(objectMap, groupId);
        auto leader = static_cast<const CMidUnit*>(
            objectMap->vftable->findScenarioObjectById(objectMap, &stack->leaderId));
        if (!canUnitGainXp(leader->unitImpl)) {
            return false;
        }
    }

    return true;
}

int getWeaponMasterBonusXpPercent(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* groupId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& idApi = CMidgardIDApi::get();

    if (idApi.getType(groupId) == IdType::Stack) {
        auto stack = getStack(objectMap, groupId);
        auto leader = fn.findUnitById(objectMap, &stack->leaderId);
        if (hasLeaderAbility(leader->unitImpl, LeaderAbilityCategories::get().weaponMaster)) {
            const auto vars = *(*GlobalDataApi::get().getGlobalData())->globalVariables;
            return vars->weaponMaster;
        }
    }

    return 0;
}

int getEasyDifficultyBonusXpPercent(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId)
{
    using namespace game;

    const auto& difficulties = DifficultyLevelCategories::get();

    auto player = getPlayer(objectMap, playerId);
    if (player && player->isHuman) {
        auto scenarioInfo = getScenarioInfo(objectMap);
        if (scenarioInfo->gameDifficulty.id == difficulties.easy->id) {
            return 20;
        }
    }

    return 0;
}

int getAiBonusXpPercent(const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    const auto& difficulties = DifficultyLevelCategories::get();

    auto scenarioInfo = getScenarioInfo(objectMap);
    if (scenarioInfo->suggestedLevel >= 10) {
        auto difficultyId = scenarioInfo->gameDifficulty.id;
        if (difficultyId == difficulties.average->id) {
            return 25;
        } else if (difficultyId == difficulties.hard->id) {
            return 50;
        } else if (difficultyId == difficulties.veryHard->id) {
            return 100;
        }
    }

    return 0;
}

const game::TBuildingType* getBuilding(const game::CMidgardID* buildingId)
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    const auto globalData = *globalApi.getGlobalData();
    return (const TBuildingType*)globalApi.findById(globalData->buildings, buildingId);
}

int getBuildingLevel(const game::CMidgardID* buildingId)
{
    auto building = getBuilding(buildingId);
    return building ? getBuildingLevel(building) : 0;
}

int getBuildingLevel(const game::TBuildingType* building)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto unitBuilding = (const TBuildingUnitUpgType*)dynamicCast(building, 0,
                                                                 rtti.TBuildingTypeType,
                                                                 rtti.TBuildingUnitUpgTypeType, 0);
    return unitBuilding ? unitBuilding->level : 0;
}

const game::CPlayerBuildings* getPlayerBuildings(const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidPlayer* player)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, &player->buildingsId);
    return (const CPlayerBuildings*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                rtti.CPlayerBuildingsType, 0);
}

bool playerHasBuilding(const game::IMidgardObjectMap* objectMap,
                       const game::CMidPlayer* player,
                       const game::CMidgardID* buildingId)
{
    auto playerBuildings = getPlayerBuildings(objectMap, player);
    if (!playerBuildings) {
        return false;
    }

    const auto& buildings = playerBuildings->buildings;
    for (auto node = buildings.head->next; node != buildings.head; node = node->next) {
        if (node->data == *buildingId) {
            return true;
        }
    }

    return false;
}

bool playerHasSiblingUnitBuilding(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidPlayer* player,
                                  const game::TBuildingType* building)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto unitBuilding = (const TBuildingUnitUpgType*)dynamicCast(building, 0,
                                                                 rtti.TBuildingTypeType,
                                                                 rtti.TBuildingUnitUpgTypeType, 0);
    if (!unitBuilding) {
        return false;
    }

    auto playerBuildings = getPlayerBuildings(objectMap, player);
    if (!playerBuildings) {
        return false;
    }

    const auto& buildings = playerBuildings->buildings;
    for (auto node = buildings.head->next; node != buildings.head; node = node->next) {
        auto otherBuilding = getBuilding(&node->data);
        if (otherBuilding->data->category.id == building->data->category.id) {
            auto otherUnitBuilding = (const TBuildingUnitUpgType*)
                dynamicCast(otherBuilding, 0, rtti.TBuildingTypeType, rtti.TBuildingUnitUpgTypeType,
                            0);
            if (otherUnitBuilding && otherUnitBuilding->level == unitBuilding->level
                && otherUnitBuilding->branch.id == unitBuilding->branch.id) {
                return true;
            }
        }
    }

    return false;
}

bool lordHasBuilding(const game::CMidgardID* lordId, const game::CMidgardID* buildingId)
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();
    const auto& idSetApi = IdSetApi::get();

    const GlobalData* globalData = *globalApi.getGlobalData();
    auto lord = static_cast<const TLordType*>(globalApi.findById(globalData->lords, lordId));
    if (!lord) {
        return false;
    }

    IdSetIterator it;
    auto buildings = lord->data->buildList->data;
    return *idSetApi.find(&buildings, &it, buildingId) != buildings.end();
}

const game::CMidDiplomacy* getDiplomacy(const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    const auto id{createIdWithType(objectMap, game::IdType::Diplomacy)};

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &id)};
    if (!obj) {
        return nullptr;
    }

    return static_cast<const game::CMidDiplomacy*>(obj);
}

const game::CMidgardMapFog* getFog(const game::IMidgardObjectMap* objectMap,
                                   const game::CMidPlayer* player)
{
    using namespace game;

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &player->fogId)};
    if (!obj) {
        return nullptr;
    }

    return static_cast<const CMidgardMapFog*>(obj);
}

} // namespace hooks
