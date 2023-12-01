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

#ifndef GAMEUTILS_H
#define GAMEUTILS_H

namespace game {
struct CMidgardID;
struct CMidUnitGroup;
struct IMidgardObjectMap;
struct BattleMsgData;
struct CScenarioInfo;
struct CMidPlayer;
struct CMidScenVariables;
struct CMidgardPlan;
struct CMidgardMap;
struct CMidgardMapBlock;
struct CMidStack;
struct CMidRuin;
struct CFortification;
struct LRaceCategory;
struct LTerrainCategory;
struct CMidRod;
struct CMidInventory;
struct CMidDiplomacy;
struct CMidgardMapFog;
struct TBuildingType;
struct CPlayerBuildings;
} // namespace game

namespace hooks {

bool isGreaterPickRandomIfEqual(int first, int second);

/**
 * Picks object map that corresponds to the calling thread (client vs server).
 * Object map can be unavailable while in a loading state.
 * The game has such map available, while editor keeps it unavailable while loading.
 * The only known case so far, is that game/editor calls IUsSoldier::getHitPoint
 * while it loads units from scenario stream thus while the map is just being loaded.
 * This triggers CCustomModifier "getHitPoint" script that can try to access the map.
 */
const game::IMidgardObjectMap* getObjectMap();

game::CMidUnitGroup* getGroup(game::IMidgardObjectMap* objectMap,
                              const game::CMidgardID* groupId,
                              bool forChange);

/** Analogue of GetStackFortRuinGroup that is absent in Scenario Editor. */
const game::CMidUnitGroup* getGroup(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* groupId);

const game::CMidUnitGroup* getAllyOrEnemyGroup(const game::IMidgardObjectMap* objectMap,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::CMidgardID* unitId,
                                               bool ally);

const game::CScenarioInfo* getScenarioInfo(const game::IMidgardObjectMap* objectMap);

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId);

const game::CMidPlayer* getPlayer(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::CMidgardID* unitId);

const game::CMidPlayer* getPlayerByUnitId(const game::IMidgardObjectMap* objectMap,
                                          const game::CMidgardID* unitId);

const game::CMidgardID getPlayerIdByUnitId(const game::IMidgardObjectMap* objectMap,
                                           const game::CMidgardID* unitId);

const game::CMidScenVariables* getScenarioVariables(const game::IMidgardObjectMap* objectMap);

const game::CMidgardPlan* getMidgardPlan(const game::IMidgardObjectMap* objectMap);

const game::CMidgardMap* getMidgardMap(const game::IMidgardObjectMap* objectMap);

const game::CMidgardMapBlock* getMidgardMapBlock(const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidgardID* mapId,
                                                 int mapSize,
                                                 int x,
                                                 int y);

const game::LTerrainCategory* getTerrainCategory(const game::LRaceCategory* raceCategory);

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap,
                          const game::CMidgardID* stackId);

game::CMidStack* getStack(const game::IMidgardObjectMap* objectMap,
                          const game::BattleMsgData* battleMsgData,
                          const game::CMidgardID* unitId);

const game::CMidStack* getStackByUnitId(const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* unitId);

game::CFortification* getFort(const game::IMidgardObjectMap* objectMap,
                              const game::CMidgardID* fortId);

game::CFortification* getFort(const game::IMidgardObjectMap* objectMap,
                              const game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId);

const game::CFortification* getFortByUnitId(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidgardID* unitId);

game::CMidRuin* getRuin(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* ruinId);

game::CMidRuin* getRuin(const game::IMidgardObjectMap* objectMap,
                        const game::BattleMsgData* battleMsgData,
                        const game::CMidgardID* unitId);

const game::CMidRuin* getRuinByUnitId(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId);

game::CMidRod* getRod(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* rodId);

int getGroupXpKilled(const game::IMidgardObjectMap* objectMap, const game::CMidUnitGroup* group);

game::CMidInventory* getInventory(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* groupId);

bool canGroupGainXp(const game::IMidgardObjectMap* objectMap, const game::CMidgardID* groupId);

int getWeaponMasterBonusXpPercent(const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* groupId);

int getEasyDifficultyBonusXpPercent(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId);

int getAiBonusXpPercent(const game::IMidgardObjectMap* objectMap);

const game::TBuildingType* getBuilding(const game::CMidgardID* buildingId);

int getBuildingLevel(const game::CMidgardID* buildingId);

int getBuildingLevel(const game::TBuildingType* building);

const game::CPlayerBuildings* getPlayerBuildings(const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidPlayer* player);

bool playerHasBuilding(const game::IMidgardObjectMap* objectMap,
                       const game::CMidPlayer* player,
                       const game::CMidgardID* buildingId);

bool lordHasBuilding(const game::CMidgardID* lordId, const game::CMidgardID* buildingId);

const game::CMidDiplomacy* getDiplomacy(const game::IMidgardObjectMap* objectMap);

const game::CMidgardMapFog* getFog(const game::IMidgardObjectMap* objectMap,
                                   const game::CMidPlayer* player);

} // namespace hooks

#endif // GAMEUTILS_H
