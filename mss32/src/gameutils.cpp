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
#include "dynamiccast.h"
#include "fortification.h"
#include "game.h"
#include "midclient.h"
#include "midclientcore.h"
#include "midgard.h"
#include "midgardmap.h"
#include "midgardmapblock.h"
#include "midgardobjectmap.h"
#include "midgardplan.h"
#include "midgardscenariomap.h"
#include "midplayer.h"
#include "midruin.h"
#include "midscenvariables.h"
#include "midserver.h"
#include "midserverlogic.h"
#include "midstack.h"
#include "scenarioinfo.h"
#include "scenedit.h"
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

const game::CMidUnitGroup* getGroup(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* groupId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, groupId);
    switch (CMidgardIDApi::get().getType(groupId)) {
    case IdType::Stack: {
        auto stack = (const CMidStack*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                   rtti.CMidStackType, 0);
        return stack ? &stack->group : nullptr;
    }
    case IdType::Fortification: {
        auto fortification = static_cast<const CFortification*>(obj);
        return fortification ? &fortification->group : nullptr;
    }
    case IdType::Ruin:
        auto ruin = static_cast<const CMidRuin*>(obj);
        return ruin ? &ruin->group : nullptr;
    }

    return nullptr;
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
    auto stack = getStackByUnitId(objectMap, unitId);
    if (stack) {
        return getPlayer(objectMap, &stack->ownerId);
    }

    auto fort = getFortByUnitId(objectMap, unitId);
    if (fort) {
        return getPlayer(objectMap, &fort->ownerId);
    }

    return nullptr;
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

} // namespace hooks
