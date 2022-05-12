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

#ifndef GAME_H
#define GAME_H

#include "attacktypepairvector.h"
#include "globaldata.h"
#include "idlist.h"
#include "mqpoint.h"
#include "smartptr.h"

namespace game {

struct TRaceType;
struct CPlayerBuildings;
struct LUnitBranch;
struct CMidDataCache2;
struct CMidgardID;
struct CDBTable;
struct GlobalData;
struct IMidObject;
struct IUsSoldier;
struct IUsNoble;
struct IUsStackLeader;
struct IMidgardObjectMap;
struct TLordType;
struct CMidPlayer;
struct BattleMsgData;
struct CMidUnit;
struct IUsUnit;
struct LAttackSource;
struct LAttackClass;
struct LImmuneCat;
struct IBatAttack;
struct IAttack;
struct CMidUnitGroup;
struct Bank;
struct CMqPoint;
struct CUnitGenerator;
struct CAttackData;
struct CDynUpgrade;
struct CMidStack;
struct CMidgardPlan;
struct IMqImage2;
struct IEncUnitDescriptor;
struct CDialogInterf;
struct String;
struct LRaceCategory;
struct LTerrainCategory;
struct LGroundCategory;
struct IItem;
struct CPhaseGame;
struct IUsRacialSoldier;
struct NetPlayerInfo;
struct LBuildingCategory;
struct IUsLeader;

enum class ModifierElementTypeFlag : int;

/** Sets initial values for 'show resources' and 'minimap mode' toggle buttons. */
using RespopupInitFunc = void (*)(void);

/** Sets initial value for 'show banners' toggle button. */
using ToggleShowBannersInitFunc = void*(__thiscall*)(void* thisptr);

/**
 * Adds tier-1 unit from specified unit branch to player's hire list.
 * @param[in] race player race.
 * @param[in] buildings player buildings.
 * @param[in] branch determines unit to add.
 * @param[inout] hireList list to store results.
 * @returns unknown.
 */
using AddUnitToHireList = void*(__stdcall*)(TRaceType* race,
                                            CPlayerBuildings* buildings,
                                            const LUnitBranch* branch,
                                            IdList* hireList);

/**
 * Adds unit from sideshow branch to player's hire list.
 * @param[in] race player race.
 * @param[in] buildings player buildings.
 * @param[inout] hireList list to store results.
 * @returns unknown.
 */
using AddSideshowUnitToHireList = void*(__stdcall*)(TRaceType* race,
                                                    CPlayerBuildings* buildings,
                                                    IdList* hireList);

/**
 * Adds player's tier-1 units to hire list.
 * @param[in] dataCache pointer to data cache object.
 * @param[in] playerId id of player to process.
 * @param[in] a3 meaning unknown.
 * @param[inout] hireList list to store results.
 * @returns true if units were added to the list.
 */
using AddPlayerUnitsToHireList = bool(__stdcall*)(CMidDataCache2* dataCache,
                                                  const CMidgardID* playerId,
                                                  const CMidgardID* a3,
                                                  IdList* hireList);

/**
 * Callback function used to process each record in GBuild.dbf.
 * Creates TBuildingType or TBuildingUnitUpgType objects from record data and stores them in a2.
 */
using CreateBuildingType = void(__stdcall*)(const CDBTable* dbTable,
                                            void* a2,
                                            const GlobalData** globalData);

/**
 * Adds object to dictionary-like structure if it's not a duplicate.
 * @returns false in case of duplicating object.
 */
using AddObjectAndCheckDuplicates = bool(__thiscall*)(void* thisptr, const IMidObject* object);

/**
 * Decides where to place unit in group.
 * @returns 0 for back lane, 1 for forward lane.
 */
using ChooseUnitLane = int(__stdcall*)(const IUsSoldier* soldier);

/** Returns lord type for specified player. */
using GetLordByPlayer = const TLordType*(__stdcall*)(const CMidPlayer* player);

/** Returns true if current turn value is in allowed range [0 : 999]. */
using IsTurnValid = bool(__stdcall*)(int turn);

/** Returns stack id of allies or enemies for specified unit id. */
using GetAllyOrEnemyGroupId = CMidgardID*(__stdcall*)(CMidgardID* value,
                                                      const BattleMsgData* battleMsgData,
                                                      const CMidgardID* unitId,
                                                      bool allies);

/** Returns pointer to unit found by specified id. */
using FindUnitById = CMidUnit*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                           const CMidgardID* unitId);

using CastUnitImplToSoldier = IUsSoldier*(__stdcall*)(const IUsUnit* unitImpl);
using CastUnitImplToNoble = IUsNoble*(__stdcall*)(const IUsUnit* unitImpl);
using CastUnitImplToStackLeader = IUsStackLeader*(__stdcall*)(const IUsUnit* unitImpl);
using CastUnitImplToLeader = IUsLeader*(__stdcall*)(const IUsUnit* unitImpl);
using CastUnitImplToRacialSoldier = const IUsRacialSoldier*(__stdcall*)(const IUsUnit* unitImpl);

using CreateBatAttack = IBatAttack*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                const BattleMsgData* battleMsgData,
                                                const CMidgardID* unitId,
                                                const CMidgardID* id2,
                                                int attackNumber,
                                                const LAttackClass* attackClass,
                                                bool a7);

using GetAttackById = IAttack*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                           const CMidgardID* id,
                                           int attackNumber,
                                           bool checkAltAttack);

using IsUnitImmuneToAttack = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                              const BattleMsgData* battleMsgData,
                                              const CMidgardID* unitId,
                                              const IAttack* attack,
                                              bool checkAlwaysImmuneOnly);

/** Returns bit flag position (0-31) for the specified attack class. */
using GetAttackClassWardFlagPosition = std::uint32_t(__stdcall*)(const LAttackClass* attackClass);
using AttackClassToString = const char*(__stdcall*)(const LAttackClass* attackClass);

/** Returns bit flag position (0-7) for the specified attack source. */
using GetAttackSourceWardFlagPosition =
    std::uint32_t(__stdcall*)(const LAttackSource* attackSource);

/**
 * Returns pointer to units group of stack, fortification or ruin
 * Actual object type found determined by specified id IdType.
 */
using GetStackFortRuinGroup = CMidUnitGroup*(__thiscall*)(void* thisptr,
                                                          const IMidgardObjectMap* objectMap,
                                                          const CMidgardID* objectId);

/** Deletes all building in capital of specified player. */
using DeletePlayerBuildings = int(__stdcall*)(IMidgardObjectMap* objectMap,
                                              const CMidPlayer* player);

/** Builds lord-specific buildings in capital of player described by NetPlayerInfo. */
using BuildLordSpecificBuildings = bool(__stdcall*)(IMidgardObjectMap* objectMap,
                                                    const NetPlayerInfo* playerInfo,
                                                    int);

/** Adds building of specified category to capital of specified player. */
using AddCapitalBuilding = bool(__stdcall*)(IMidgardObjectMap* objectMap,
                                            const CMidPlayer* player,
                                            const LBuildingCategory* building);

using GetInterfaceText = const char*(__stdcall*)(const CMidgardID* textId);

using ComputePlayerDailyIncome = Bank*(__stdcall*)(Bank* income,
                                                   IMidgardObjectMap* objectMap,
                                                   const CMidgardID* playerId);

/**
 * Computes full attack damage with respect to armor, buffs, debuffs and critical hit.
 * @param[in] objectMap map to search for units.
 * @param[in] battleMsgData battle information to check for buffs and debuffs.
 * @param[in] attackImpl attack for which damage is computed.
 * @param[in] attackerUnitId id of unit performing the attack.
 * @param[in] targetUnitId id of unit being attacked.
 * @param computeCriticalHit if set to true, critical hit ability is checked and critical damage is
 * computed.
 * @param[inout] attackDamage used to store damage without critical hit. Must be non-null if
 * computeCriticalHit is true.
 * @param[inout] criticalHitDamage used to store critical hit damage. Must be non-null if
 * computeCriticalHit is true.
 * @returns computed damage.
 */
using ComputeDamage = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                      const BattleMsgData* battleMsgData,
                                      const IAttack* attack,
                                      const CMidgardID* attackerUnitId,
                                      const CMidgardID* targetUnitId,
                                      bool computeCriticalHit,
                                      int* attackDamage,
                                      int* criticalHitDamage);

using ComputeDamageMax = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                         const CMidgardID* unitId);

using ComputeDamageWithBuffs = int(__stdcall*)(const IAttack* attack,
                                               int damageMax,
                                               const BattleMsgData* battleMsgData,
                                               const CMidgardID* unitId,
                                               bool addRandomDamage,
                                               bool easyDifficulty);

/**
 * Computes unit armor with respect to buffs / debuffs, defend / fortification bonus and shattered
 * armor.
 * @param[out] armor the resulting value.
 * @param[in] objectMap map to search for units.
 * @param[in] battleMsgData battle information to check for buffs and debuffs.
 * @param[in] unitId id of a unit to compute armor for.
 * @returns pointer to the resulting value.
 */
using ComputeArmor = int*(__stdcall*)(int* armor,
                                      const IMidgardObjectMap* objectMap,
                                      const BattleMsgData* battleMsgData,
                                      const CMidgardID* unitId);

/**
 * Used for pathfinding.
 * Causes memory corruption (that leads to crash) on 144x144 maps if the position is out of map
 * bounds.
 */
using MarkMapPosition = void(__thiscall*)(void* thisptr, CMqPoint* position);

/** Returns unit level by impl id. If unit does not exist yet, computes its level. */
using GetUnitLevelByImplId = int(__stdcall*)(const CMidgardID* unitImplId);

/**
 * Returns attack power value (chance to hit / accuracy) adjusted according to game difficulty and
 * batte round.
 */
using GetAttackPower = void(__stdcall*)(int* power,
                                        const IAttack* attack,
                                        const IMidgardObjectMap* objectMap,
                                        const CMidgardID* unitId,
                                        const BattleMsgData* battleMsgData);

/** Returns true if group with specified id belongs to human controlled player. */
using IsGroupOwnerPlayerHuman = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                 const CMidgardID* groupId);

/**
 * Determines whether attack should miss or not depending on its power (chance to hit / accuracy).
 * Compares attack power with arithmetic mean of two random numbers.
 * @returns true if arithmetic mean is greater than power value.
 */
using AttackShouldMiss = bool(__stdcall*)(const int* power);

/** Generates random number in range [0 : maxValue) using special ingame generator. */
using GenerateRandomNumber = int(__stdcall*)(unsigned int maxValue);

/** Generates random number in range [0 : maxValue) using standard CRT rand(). */
using GenerateRandomNumberStd = int(__stdcall*)(unsigned int maxValue);

using GetUnitPositionInGroup = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                               const CMidgardID* groupId,
                                               const CMidgardID* unitId);

/** Returns summon unit impl id for specified attack id according to size and position in group. */
using GetSummonUnitImplIdByAttack = CMidgardID*(__stdcall*)(CMidgardID* summonImplId,
                                                            const CMidgardID* attackId,
                                                            int position,
                                                            bool smallUnit);

/**
 * Returns summon unit impl id for specified summon attack.
 * @param[inout] summonImplId where to store resulting unit impl id.
 * @param[in] objectMap used for object search.
 * @param[in] attackId summon attack id to choose one summon from list.
 * @param[in] groupId id of group where summoned unit supposed to be.
 * @param[in] targetUnitId special summon id to get position in group.
 * @param canSummonBig determines wheter big unit can be summoned or not.
 * @returns pointer to summonImplId.
 */
using GetSummonUnitImplId = CMidgardID*(__stdcall*)(CMidgardID* summonImplId,
                                                    IMidgardObjectMap* objectMap,
                                                    const CMidgardID* attackId,
                                                    const CMidgardID* groupId,
                                                    const CMidgardID* targetUnitId,
                                                    bool canSummonBig);
;

/**
 * Returns number of attacks allowed in battle for specific id ???
 * 1 or 0 for item, 1 or 2 for unit attacks and so on.
 */
using GetAttackImplMagic = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                           const CMidgardID* unitId,
                                           int a3);

/**
 * If unit has primary or secondary attack from Heal class category, returns 1 for primary and 2 for
 * secondary. In case of different attack class returns 0.
 */
using GetUnitHealAttackNumber = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                const CMidgardID* unitId);

/**
 * Returns attack damage or heal depending on its class, allows to specify max damage to return.
 * Used for encyclopedia text generation, by CMidUnitDescriptor and elsewhere.
 */
using GetAttackQtyDamageOrHeal = int(__stdcall*)(const IAttack* attack, int damageMax);

/**
 * Computes unit hp + (hp * armor).
 * Used for battle AI.
 */
using ComputeUnitEffectiveHp = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                               const CMidUnit* unit,
                                               const BattleMsgData* battleMsgData);

/**
 * Calculates upgrade count for the specified unit level and applies upgrade values from
 * GDynUpgr.dbf to the initial attack data.
 */
using ApplyDynUpgradeToAttackData = void(__stdcall*)(const CMidgardID* unitImplId,
                                                     CUnitGenerator* unitGenerator,
                                                     int unitLevel,
                                                     IdType dynUpgradeType,
                                                     const CMidgardID* altAttackId,
                                                     CAttackData* attackData);

using ComputeUnitDynUpgrade = void(__stdcall*)(const CMidgardID* unitImplId,
                                               int unitLevel,
                                               CDynUpgrade** upgrade1,
                                               CDynUpgrade** upgrade2,
                                               int* upgrade1Count,
                                               int* upgrade2Count);

using ShowMovementPath = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                          const CMidgardID* stackId,
                                          List<CMqPoint>* path,
                                          const CMqPoint* lastReachablePoint,
                                          const CMqPoint* pathEnd,
                                          bool a6);

using GetMidgardPlan = CMidgardPlan*(__stdcall*)(const IMidgardObjectMap* objectMap);

using GetBlockingPathNearbyStackId =
    const CMidgardID*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                  const CMidgardPlan* plan,
                                  const CMidStack* stack,
                                  const CMqPoint* lastReachablePoint,
                                  const CMqPoint* pathEnd,
                                  int a6);

/**
 * Searches fortification or ruin at specified position and returns its entrance point.
 * @param[in] objectMap map to search objects.
 * @param[in] plan used to search fortification and ruin objects by position.
 * @param[in] stack used to check fort subrace. Entrance point returned only for forts with
 * different subraces.
 * @param[in] position position to check.
 * @param[inout] entrance pointer to store the result.
 * @returns true if entrance point found.
 */
using GetFortOrRuinEntrance = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                               const CMidgardPlan* plan,
                                               const CMidStack* stack,
                                               const CMqPoint* position,
                                               CMqPoint* entrance);

/**
 * Returns true if stack can move to specified position on a map.
 * @param[in] objectMap map to search objects.
 * @paran[in] position tile position to check.
 * @param[in] stack stack to check.
 * @param[in] plan pointer to plan object to check obstacles.
 */
using StackCanMoveToPosition = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                const CMqPoint* position,
                                                const CMidStack* stack,
                                                const CMidgardPlan* plan);

using IsWaterTileSurroundedByWater = bool(__stdcall*)(const CMqPoint* position,
                                                      const IMidgardObjectMap* objectMap);

using GetStackPositionById = CMqPoint(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                  const CMidgardID* stackId);

/** Applies percentage modifiers of the specified type to the value. */
using ApplyPercentModifiers = int(__stdcall*)(int value,
                                              const IdList* unitModifiers,
                                              ModifierElementTypeFlag type);

/** Creates a formatted description of attack for encyclopedia. */
using GenerateAttackDescription = void(__stdcall*)(IEncUnitDescriptor* descriptor,
                                                   CDialogInterf* dialog,
                                                   int boostDamageLevel,
                                                   int lowerDamageLevel,
                                                   int lowerInitiativeLevel,
                                                   const IdList* editorModifiers,
                                                   int damageMax);

using CreateMenuAnimation = SmartPtr<IMqImage2>*(__stdcall*)(SmartPtr<IMqImage2>* animation,
                                                             const char* animationName);

/** Gets attack source interface text. */
using GetAttackSourceText = String*(__stdcall*)(String* value, const LAttackSource* attackSource);

/** Appends attack source interface text. */
using AppendAttackSourceText = void(__stdcall*)(const LAttackSource* attackSource,
                                                String* value,
                                                bool* valueIsNotEmpty);

using IsRaceCategoryUnplayable = bool(__stdcall*)(const LRaceCategory* raceCategory);

/**
 * Validates TRaceType objects.
 * Checks valirity of unit, guardian, leader and buildings ids.
 * Checks that all race categories were correctly read.
 */
using ValidateRaces = void(__thiscall*)(RacesMap** thisptr, GlobalData** globalData);

/** Checks if race with specified category was loaded from Grace.dbf. */
using CheckRaceExist = void(__stdcall*)(RacesMap** races,
                                        const LRaceCategory* category,
                                        const char* dbfFileName);

/** Gets a list of sources to which the unit has the specified immunity. */
using GetUnitAttackSourceImmunities = void(__stdcall*)(const LImmuneCat* immuneCat,
                                                       const CMidUnit* unit,
                                                       List<LAttackSource>* value);

/** Gets a list of sources to which the soldier has the specified immunity. */
using GetSoldierAttackSourceImmunities = void(__stdcall*)(const IUsSoldier* soldier,
                                                          bool alwaysImmune,
                                                          List<LAttackSource>* value);

/** Gets immunity rating to be used by AI for overall soldier rating calculation. */
using GetSoldierImmunityAiRating = double(__stdcall*)(const IUsSoldier* soldier);

/** Gets attack class rating to be used by AI for overall soldier rating calculation. */
using GetAttackClassAiRating = double(__stdcall*)(const IUsSoldier* soldier, bool a2);

/** Gets attack reach rating to be used by AI for overall soldier rating calculation. */
using GetAttackReachAiRating = double(__stdcall*)(const IUsSoldier* soldier, int targetCount);

/** Gets distance between unit positions in battle. Used for adjacent attack reach calculations. */
using GetUnitPositionDistance = int(__stdcall*)(int unitPosition,
                                                int targetPosition,
                                                bool isTargetAlly);

using GetUnitRaceCategory = LRaceCategory*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                       const CMidgardID* unitId);

using IsGroupSuitableForAiNobleMisfit = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                         const CMidUnitGroup* group);

using IsUnitSuitableForAiNobleDuel = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                      const CMidgardID* unitId);

using IsAttackEffectiveAgainstGroup = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                       const IAttack* attack,
                                                       const CMidUnitGroup* group);

using IsAttackBetterThanItemUsage = bool(__stdcall*)(const IItem* item,
                                                     const IUsSoldier* soldier,
                                                     const CMidgardID* unitImplId);

using ComputeAttackDamageCheckAltAttack = int(__stdcall*)(const IUsSoldier* soldier,
                                                          const CMidgardID* unitImplId,
                                                          const BattleMsgData* battleMsgData,
                                                          const CMidgardID* unitId);

using FillAttackTransformIdList = void(__thiscall*)(const void* thisptr,
                                                    IdList* value,
                                                    const CMidgardID* attackId,
                                                    bool sizeSmall);

using IsSmallMeleeFighter = bool(__stdcall*)(const IUsSoldier* soldier);

using CAiHireUnitEval = bool(__stdcall*)(const IMidgardObjectMap* objectMap,
                                         const CMidUnitGroup* group);

using GetMeleeUnitToHireAiRating = double(__stdcall*)(const CMidgardID* unitImplId,
                                                      bool a2,
                                                      bool a3);

using ComputeTargetUnitAiPriority = int(__stdcall*)(const IMidgardObjectMap* objectMap,
                                                    const CMidgardID* unitId,
                                                    const BattleMsgData* battleMsgData,
                                                    int attackerDamage);

/**
 * Returns terrain category by its abbreviation string.
 * Used in processing GTileDbi.dbf records.
 */
using GetTerrainByAbbreviation = LTerrainCategory*(__stdcall*)(LTerrainCategory* category,
                                                               const char* abbreviation);

/** Returns terrain category that corresponds to specified race category. */
using GetTerrainByRace = const LTerrainCategory*(__stdcall*)(const LRaceCategory* race);

/** Returns race category that corresponds to specified terrain category. */
using GetRaceByTerrain = const LRaceCategory*(__stdcall*)(const LTerrainCategory* terrain);

/** Returns tile prefix number by tile prefix name. */
using GetTilePrefixByName = int(__stdcall*)(const char* tileNamePrefix);

/** Returns tile prefix name by tile prefix number. */
using GetTilePrefixName = const char*(__stdcall*)(int tilePrefixNumber);

/** Assumption: returns color index for minimap. */
using GetNumberByTerrainGround = int(__stdcall*)(const LTerrainCategory* terrain,
                                                 const LGroundCategory* ground);

/** Throws MqDbException with specified message. */
using ThrowGenericException = void(__stdcall*)(const char* message);

/** Returns true if game should ignore events for specified player. */
using IgnorePlayerEvents = bool(__stdcall*)(const CMidgardID* playerId,
                                            const IMidgardObjectMap* objectMap);

/** Returns race's scenario preview image. */
using GetRacePreviewImage = IMqImage2*(__stdcall*)(const LRaceCategory* race);

using IsPlayerRaceUnplayable = bool(__stdcall*)(const CMidgardID* playerId,
                                                const IMidgardObjectMap* objectMap);

using IsSupportAttackClass = bool(__stdcall*)(const LAttackClass* value);

/** Returns a vector of attack + attack-number pairs by unit. */
using GetUnitAttacks = void(__stdcall*)(const IMidgardObjectMap* objectMap,
                                        const CMidgardID* unitId,
                                        AttackTypePairVector* value,
                                        bool checkAltAttack);

using GetItemAttack = IAttack*(__stdcall*)(const IMidgardObjectMap* objectMap,
                                           const CMidgardID* unitId,
                                           int itemNumber);

using IsUnitUseAdditionalAnimation = bool(__stdcall*)(const CMidgardID* unitId);

using CheckUnitForHire = bool(__stdcall*)(const CMidPlayer* player,
                                          CPhaseGame* phaseGame,
                                          const CMidgardID* unitImplId);

using GetBaseUnitImplId = CMidgardID*(__stdcall*)(CMidgardID* value,
                                                  const IMidgardObjectMap* objectMap,
                                                  const CMidgardID* unitId,
                                                  bool original);

using GetUnitImplDamageMax = int(__stdcall*)(CMidgardID* unitImplId);

using ReadGlobalAttacks = AttackMap**(__thiscall*)(AttackMap** thisptr,
                                                   const char* globalsFolderPath,
                                                   void* codeBaseEnvProxy,
                                                   const GlobalData** globalData);

using GetAltAttackIdCheckClass = const CMidgardID*(__stdcall*)(const IAttack* attack);

/** Game and editor functions that can be hooked. */
struct Functions
{
    RespopupInitFunc respopupInit;
    ToggleShowBannersInitFunc toggleShowBannersInit;
    AddUnitToHireList addUnitToHireList;
    AddSideshowUnitToHireList addSideshowUnitToHireList;
    AddPlayerUnitsToHireList addPlayerUnitsToHireList;
    CreateBuildingType createBuildingType;
    AddObjectAndCheckDuplicates addObjectAndCheckDuplicates;
    ChooseUnitLane chooseUnitLane;
    GetLordByPlayer getLordByPlayer;
    IsTurnValid isTurnValid;
    GetAllyOrEnemyGroupId getAllyOrEnemyGroupId;
    FindUnitById findUnitById;
    CastUnitImplToSoldier castUnitImplToSoldier;
    CastUnitImplToStackLeader castUnitImplToStackLeader;
    CreateBatAttack createBatAttack;
    GetAttackById getAttackById;
    IsUnitImmuneToAttack isUnitImmuneToAttack;
    GetAttackClassWardFlagPosition getAttackClassWardFlagPosition;
    AttackClassToString attackClassToString;
    GetAttackSourceWardFlagPosition getAttackSourceWardFlagPosition;
    GetStackFortRuinGroup getStackFortRuinGroup;
    DeletePlayerBuildings deletePlayerBuildings;
    GetInterfaceText getInterfaceText;
    ComputePlayerDailyIncome computePlayerDailyIncome;
    ComputeDamage computeDamage;
    ComputeDamageMax computeDamageMax;
    ComputeDamageWithBuffs computeDamageWithBuffs;
    ComputeArmor computeArmor;
    MarkMapPosition markMapPosition;
    GetUnitLevelByImplId getUnitLevelByImplId;
    GetAttackPower getAttackPower;
    IsGroupOwnerPlayerHuman isGroupOwnerPlayerHuman;
    AttackShouldMiss attackShouldMiss;
    GenerateRandomNumber generateRandomNumber;
    GenerateRandomNumberStd generateRandomNumberStd;
    GetUnitPositionInGroup getUnitPositionInGroup;
    GetSummonUnitImplIdByAttack getSummonUnitImplIdByAttack;
    GetSummonUnitImplId getSummonUnitImplId;
    GetAttackImplMagic getAttackImplMagic;
    GetUnitHealAttackNumber getUnitHealAttackNumber;
    GetAttackQtyDamageOrHeal getAttackQtyDamageOrHeal;
    ComputeUnitEffectiveHp computeUnitEffectiveHp;
    ApplyDynUpgradeToAttackData applyDynUpgradeToAttackData;
    ComputeUnitDynUpgrade computeUnitDynUpgrade;
    ShowMovementPath showMovementPath;
    GetMidgardPlan getMidgardPlan;
    CastUnitImplToNoble castUnitImplToNoble;
    GetBlockingPathNearbyStackId getBlockingPathNearbyStackId;
    GetFortOrRuinEntrance getFortOrRuinEntrance;
    StackCanMoveToPosition stackCanMoveToPosition;
    IsWaterTileSurroundedByWater isWaterTileSurroundedByWater;
    GetStackPositionById getStackPositionById;
    ApplyPercentModifiers applyPercentModifiers;
    GenerateAttackDescription generateAttackDescription;
    CreateMenuAnimation createMenuAnimation;
    GetAttackSourceText getAttackSourceText;
    AppendAttackSourceText appendAttackSourceText;
    IsRaceCategoryUnplayable isRaceCategoryUnplayable;
    ValidateRaces validateRaces;
    CheckRaceExist checkRaceExist;
    GetUnitAttackSourceImmunities getUnitAttackSourceImmunities;
    GetSoldierAttackSourceImmunities getSoldierAttackSourceImmunities;
    GetTerrainByAbbreviation getTerrainByAbbreviation;
    GetTerrainByRace getTerrainByRace;
    GetTerrainByRace getTerrainByRace2;
    GetRaceByTerrain getRaceByTerrain;
    GetRaceByTerrain getPlayableRaceByTerrain;
    GetTilePrefixByName getTilePrefixByName;
    GetTilePrefixName getTilePrefixName;
    GetNumberByTerrainGround getNumberByTerrainGround;
    ThrowGenericException throwGenericException;
    IgnorePlayerEvents ignorePlayerEvents;
    GetRacePreviewImage getRacePreviewImage;
    GetSoldierImmunityAiRating getSoldierImmunityAiRating;
    GetAttackClassAiRating getAttackClassAiRating;
    GetAttackReachAiRating getAttackReachAiRating;
    GetUnitPositionDistance getUnitPositionDistance;
    GetUnitRaceCategory getUnitRaceCategory;
    IsGroupSuitableForAiNobleMisfit isGroupSuitableForAiNobleMisfit;
    IsUnitSuitableForAiNobleDuel isUnitSuitableForAiNobleDuel;
    IsAttackEffectiveAgainstGroup isAttackEffectiveAgainstGroup;
    IsAttackBetterThanItemUsage isAttackBetterThanItemUsage;
    ComputeAttackDamageCheckAltAttack computeAttackDamageCheckAltAttack;
    FillAttackTransformIdList fillAttackTransformIdList;
    IsSmallMeleeFighter isSmallMeleeFighter;
    CAiHireUnitEval cAiHireUnitEval;
    GetMeleeUnitToHireAiRating getMeleeUnitToHireAiRating;
    ComputeTargetUnitAiPriority computeTargetUnitAiPriority;
    IsPlayerRaceUnplayable isPlayerRaceUnplayable;
    IsSupportAttackClass isSupportAttackClass;
    GetUnitAttacks getUnitAttacks;
    GetItemAttack getItemAttack;
    IsUnitUseAdditionalAnimation isUnitUseAdditionalAnimation;
    CheckUnitForHire shouldAddUnitToHire;
    CheckUnitForHire enableUnitInHireListUi;
    CastUnitImplToRacialSoldier castUnitImplToRacialSoldier;
    BuildLordSpecificBuildings buildLordSpecificBuildings;
    AddCapitalBuilding addCapitalBuilding;
    CastUnitImplToLeader castUnitImplToLeader;
    GetBaseUnitImplId getBaseUnitImplId;
    GetUnitImplDamageMax getUnitImplDamageMax;
    ReadGlobalAttacks readGlobalAttacks;
    GetAltAttackIdCheckClass getAltAttackIdCheckClass;
};

/** Global variables used in game. */
struct Variables
{
    int* minimapMode; /**< Value of 0 shows minimap, 1 shows percentage of land converted. */
    unsigned char* respopup;
};

/** Returns functions according to determined version of the game. */
Functions& gameFunctions();

/** Returns variables according to determined version of the game. */
Variables& gameVariables();

} // namespace game

#endif // GAME_H
