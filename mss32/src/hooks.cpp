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

#include "hooks.h"
#include "attackimpl.h"
#include "attackreachcat.h"
#include "attackutils.h"
#include "autodialog.h"
#include "batattackbestowwards.h"
#include "batattackdoppelganger.h"
#include "batattackdrain.h"
#include "batattackdrainoverflow.h"
#include "batattackgiveattack.h"
#include "batattackshatter.h"
#include "batattacksummon.h"
#include "batattacktransformself.h"
#include "battleattackinfo.h"
#include "battlemsgdatahooks.h"
#include "bestowwardshooks.h"
#include "buildingbranch.h"
#include "buildingtype.h"
#include "button.h"
#include "capitaldata.h"
#include "capitaldatahooks.h"
#include "cmdbattlechooseactionmsg.h"
#include "cmdbattleendmsg.h"
#include "cmdbattleresultmsg.h"
#include "cmdbattlestartmsg.h"
#include "commandmsghooks.h"
#include "customattackhooks.h"
#include "d2string.h"
#include "dbf/dbffile.h"
#include "dbtable.h"
#include "dialoginterf.h"
#include "difficultylevel.h"
#include "doppelgangerhooks.h"
#include "drainattackhooks.h"
#include "dynamiccast.h"
#include "dynupgrade.h"
#include "editor.h"
#include "encparambase.h"
#include "fortification.h"
#include "functor.h"
#include "globaldata.h"
#include "idlist.h"
#include "interfmanager.h"
#include "interftexthooks.h"
#include "itemtransferhooks.h"
#include "iterators.h"
#include "listbox.h"
#include "log.h"
#include "lordtype.h"
#include "mapgen.h"
#include "mempool.h"
#include "menunewskirmishsingle.h"
#include "menurace.h"
#include "menuracehooks.h"
#include "middatacache.h"
#include "midgardid.h"
#include "midgardmapblockhooks.h"
#include "midgardmsgbox.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midmusic.h"
#include "midplayer.h"
#include "midstack.h"
#include "modifierutils.h"
#include "movepathhooks.h"
#include "musichooks.h"
#include "netmsghooks.h"
#include "originalfunctions.h"
#include "phasegame.h"
#include "playerbuildings.h"
#include "playerincomehooks.h"
#include "racecategory.h"
#include "racehooks.h"
#include "racetype.h"
#include "scenariodata.h"
#include "scenariodataarray.h"
#include "scenarioinfo.h"
#include "settings.h"
#include "smartptr.h"
#include "stackbattleactionmsg.h"
#include "subracecat.h"
#include "subracehooks.h"
#include "summonhooks.h"
#include "terraincat.h"
#include "terrainhooks.h"
#include "terrainnamelisthooks.h"
#include "tileindices.h"
#include "tileindiceshooks.h"
#include "tilevariation.h"
#include "tilevariationhooks.h"
#include "transformselfhooks.h"
#include "unitbranchcat.h"
#include "unitgenerator.h"
#include "unitsforhire.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include "version.h"
#include "visitors.h"
#include <algorithm>
#include <cstring>
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <string>

namespace hooks {

/** Hooks that used only in game. */
static Hooks getGameHooks()
{
    using namespace game;

    auto& fn = gameFunctions();
    auto& battle = BattleMsgDataApi::get();
    auto& orig = getOriginalFunctions();

    // clang-format off
    Hooks hooks{
        // Fix game crash in battles with summoners
        {CMidUnitApi::get().removeModifier, removeModifierHooked, (void**)&orig.removeModifier},
        // Show buildings with custom branch category on the 'other buildings' tab
        {CBuildingBranchApi::get().constructor, buildingBranchCtorHooked},
        // Always place units with melee attack at the front lane in groups controlled by non-neutrals AI
        {fn.chooseUnitLane, chooseUnitLaneHooked},
        // Allow alchemists to buff retreating units
        {CBatAttackGiveAttackApi::get().canPerform, giveAttackCanPerformHooked},
        // Random map generation
        //HookInfo{CMenuNewSkirmishSingleApi::get().constructor, menuNewSkirmishSingleCtorHooked},
        // Support custom battle attack objects
        {fn.createBatAttack, createBatAttackHooked, (void**)&orig.createBatAttack},
        // Support immunity bitmask in BattleMsgData
        {fn.getAttackClassWardFlagPosition, getAttackClassWardFlagPositionHooked, (void**)&orig.getAttackClassWardFlagPosition},
        // Support custom attack animations?
        {fn.attackClassToString, attackClassToStringHooked, (void**)&orig.attackClassToString},
        // Add items transfer buttons to city interface
        {CCityStackInterfApi::get().constructor, cityStackInterfCtorHooked, (void**)&orig.cityStackInterfCtor},
        // Add items transfer buttons to stack exchange interface
        {CExchangeInterfApi::get().constructor, exchangeInterfCtorHooked, (void**)&orig.exchangeInterfCtor},
        // Add items transfer buttons to pickup drop interface
        {CPickUpDropInterfApi::get().constructor, pickupDropInterfCtorHooked, (void**)&orig.pickupDropInterfCtor},
        // Add sell all valuables button to merchant interface
        {CSiteMerchantInterfApi::get().constructor, siteMerchantInterfCtorHooked, (void**)&orig.siteMerchantInterfCtor},
        // Cities can generate daily income depending on scenario variable settings
        {fn.computePlayerDailyIncome, computePlayerDailyIncomeHooked, (void**)&orig.computePlayerDailyIncome},
        // Vampiric attacks can deal critical damage
        {CBatAttackDrainApi::get().onHit, drainAttackOnHitHooked},
        {CBatAttackDrainOverflowApi::get().onHit, drainOverflowAttackOnHitHooked},
        // Support additional music tracks for battle and capital cities
        {CMidMusicApi::get().playBattleTrack, playBattleTrackHooked},
        {CMidMusicApi::get().playCapitalTrack, playCapitalTrackHooked},
        // Fix game crash with pathfinding on 144x144 maps
        {fn.markMapPosition, markMapPositionHooked, (void**)&orig.markMapPosition},
        // Allow user to tweak power computations
        {fn.getAttackPower, getAttackPowerHooked},
        // Fix game crash when AI controlled unit with transform self attack
        // uses alternative attack with 'adjacent' attack range
        {fn.computeUnitEffectiveHp, computeUnitEffectiveHpHooked, (void**)&orig.computeUnitEffectiveHp},
        // Fix bestow wards becoming permanent on warded unit transformation
        {battle.beforeAttack, beforeAttackHooked},
        /**
         * Allows bestow wards to:
         * 1) Grant modifiers even if there are no source wards among them
         * 2) Heal its targets to the ammount specified in QTY_HEAL
         * 3) Heal retreating units
         * 4) Use Revive as a secondary attack
         * 5) Target a unit with a secondary attack even if there are no modifiers that can be
         * applied to this unit
         * 6) Treat modifiers with complete immunity correctly
         */
        {CBatAttackBestowWardsApi::get().canPerform, bestowWardsAttackCanPerformHooked},
        /**
         * Fix bestow wards:
         * 1) Becoming permanent when more than 8 modifiers are applied at once
         * 2) Not resetting attack class wards (when reapplied)
         * 3) Incorrectly resetting attack source ward if its modifier also contains hp, regen or armor element
         */
        {CBatAttackBestowWardsApi::get().onHit, bestowWardsAttackOnHitHooked},
        /**
         * Fix bestow wards with double attack where modifiers granted by first attack are
         * getting removed. The function is used as a backdoor to erase the next attack unit id
         * if it equals current unit id, so modifiers granted by first attack are not removed.
         */
        {battle.setUnknown9Bit1AndClearBoostLowerDamage, setUnknown9Bit1AndClearBoostLowerDamageHooked, (void**)&orig.setUnknown9Bit1AndClearBoostLowerDamage},
        // Allow any attack with QTY_HEAL > 0 to heal units when battle ends (just like ordinary heal does)
        {fn.getUnitHealAttackNumber, getUnitHealAttackNumberHooked},
        // Fix AI not being able to find target for lower damage/ini attack
        {battle.findAttackTarget, findAttackTargetHooked, (void**)&orig.findAttackTarget},
        // Support custom attack sources
        {fn.getUnitAttackSourceImmunities, getUnitAttackSourceImmunitiesHooked},
        {battle.isUnitAttackSourceWardRemoved, isUnitAttackSourceWardRemovedHooked},
        {battle.removeUnitAttackSourceWard, removeUnitAttackSourceWardHooked},
        {battle.addUnitToBattleMsgData, addUnitToBattleMsgDataHooked, (void**)&orig.addUnitToBattleMsgData},
        {fn.getUnitAttackSourceImmunities, getUnitAttackSourceImmunitiesHooked},
        {battle.isUnitAttackSourceWardRemoved, isUnitAttackSourceWardRemovedHooked},
        {battle.removeUnitAttackSourceWard, removeUnitAttackSourceWardHooked},
        // Support new race categories
        {(void*)game::LRaceCategoryTableApi::get().constructor, raceCategoryTableCtorHooked},
        // Validate new races in Grace.dbf, if any
        {(void*)fn.validateRaces, validateRacesHooked},
        // Support new subrace categories
        {(void*)game::LSubRaceCategoryTableApi::get().constructor, subRaceCategoryTableCtorHooked},
        // Support new races in CMenuRace
        {(void*)game::CMenuRaceApi::get().buttonNextCallback, menuRaceBtnNextCallbackHooked},
        {(void*)game::CMenuRaceApi::get().buttonPrevCallback, menuRaceBtnPrevCallbackHooked},
        {(void*)game::CMenuRaceApi::get().updateRaceDescription, updateRaceDescriptionHooked},
        {(void*)game::CMenuRaceApi::get().getRaceCategory, getRaceCategoryHooked},
        {(void*)game::CMenuRaceApi::get().getRaceBgndImageName, getRaceBgndImageNameHooked},
        {(void*)game::RaceCategoryListApi::get().getPlayableRaces, getPlayableRacesHooked},
        {(void*)game::CMenuRaceApi::get().setRacesToSkip, setRacesToSkipHooked},
        {(void*)fn.isRaceCategoryUnplayable, isRaceCategoryUnplayableHooked},
        {(void*)fn.loadLordFaceImages, loadLordFaceImagesHooked, (void**)& orig.loadLordFaceImages},
        // Support new races in Capital.dat
        {(void*)game::CapitalDataApi::get().allocate, allocateCapitalDataHooked},
        {(void*)game::CapitalDataApi::get().read, readCapitalDataHooked},
        // Support new races in capital buldings dialogs
        {(void*)game::CBuildingBranchApi::get().createDialogName, buildingBranchCreateDialogNameHooked},
        // Support custom terrain categories
        {(void*)game::LTerrainCategoryTableApi::get().constructor, terrainCategoryTableCtorHooked},
        {(void*)fn.getTerrainByAbbreviation, getTerrainByAbbreviationHooked, (void**)&orig.getTerrainByAbbreviation},
        // Check CTileVariation records correctness
        {(void*)game::CTileVariationApi::get().checkData, checkTileVariationDataHooked},
        // Map custom races to custom terrains
        {(void*)fn.getTerrainByRace, getTerrainByRaceHooked},
        {(void*)fn.getTerrainByRace2, getTerrainByRaceHooked},
        // Map custom terrains to custom races
        {(void*)fn.getRaceByTerrain, getRaceByTerrainHooked},
        {(void*)fn.getPlayableRaceByTerrain, getPlayableRaceByTerrainHooked},
        // Map tile prefix names to numbers and vice versa
        {(void*)fn.getTilePrefixByName, getTilePrefixByNameHooked, (void**)&orig.getTilePrefixByName},
        {(void*)fn.getTilePrefixName, getTilePrefixNameHooked, (void**)&orig.getTilePrefixName},
        // Support custom terrains in terrain names list
        {(void*)game::TerrainNameListApi::get().getTerrainNameList, getTerrainNameListHooked, (void**)&orig.getTerrainNameList},
        // Support custom terrain tiles
        {(void*)game::TileIndicesApi::get().constructor, tileIndicesCtorHooked},
        {(void*)game::TileIndicesApi::get().getTileDataIndex, getTileDataIndexHooked},
        {(void*)fn.getNumberByTerrainGround, getNumberByTerrainGroundHooked},
        // Support custom tiles in terrain coverage logic
        {(void*)game::CMidgardMapBlockApi::get().countTerrainCoverage, countTerrainCoverageHooked},
    };
    // clang-format on

    if (!unitsForHire().empty()) {
        hooks.emplace_back(HookInfo{fn.addPlayerUnitsToHireList, addPlayerUnitsToHireListHooked});
    }

    if (userSettings().shatteredArmorMax != baseSettings().shatteredArmorMax) {
        // Allow users to customize total armor shatter damage
        hooks.emplace_back(
            HookInfo{CBatAttackShatterApi::get().canPerform, shatterCanPerformHooked});
        hooks.emplace_back(HookInfo{battle.setUnitShatteredArmor, setUnitShatteredArmorHooked});
    }

    if (userSettings().shatterDamageMax != baseSettings().shatterDamageMax) {
        // Allow users to customize maximum armor shatter damage per attack
        hooks.emplace_back(HookInfo{CBatAttackShatterApi::get().onHit, shatterOnHitHooked});
    }

    if (userSettings().showBanners != baseSettings().showBanners) {
        // Allow users to show banners by default
        hooks.emplace_back(HookInfo{fn.toggleShowBannersInit, toggleShowBannersInitHooked});
    }

    if (userSettings().showResources != baseSettings().showResources
        || userSettings().showLandConverted != baseSettings().showLandConverted) {
        // Allow users to show resources panel by default
        hooks.emplace_back(HookInfo{fn.respopupInit, respopupInitHooked});
    }

    if (userSettings().preserveCapitalBuildings != baseSettings().preserveCapitalBuildings) {
        // Allow scenarios with prebuilt buildings in capitals
        hooks.emplace_back(HookInfo{fn.deletePlayerBuildings, deletePlayerBuildingsHooked});
    }

    if (userSettings().carryOverItemsMax != baseSettings().carryOverItemsMax) {
        // Change maximum number of items that player can carry between campaign scenarios
        hooks.emplace_back(HookInfo{CDDCarryOverItemsApi::get().constructor,
                                    carryOverItemsCtorHooked, (void**)&orig.carryOverItemsCtor});
    }

    if (userSettings().criticalHitChance != baseSettings().criticalHitChance) {
        // Allow users to specify critical hit chance
        hooks.emplace_back(
            HookInfo{fn.computeDamage, computeDamageHooked, (void**)&orig.computeDamage});
    }

    if (userSettings().doppelgangerRespectsEnemyImmunity
            != baseSettings().doppelgangerRespectsEnemyImmunity
        || userSettings().doppelgangerRespectsAllyImmunity
               != baseSettings().doppelgangerRespectsAllyImmunity) {
        // Make Doppelganger attack respect target source/class wards and immunities
        hooks.emplace_back(HookInfo{CBatAttackDoppelgangerApi::get().canPerform,
                                    doppelgangerAttackCanPerformHooked});
        hooks.emplace_back(
            HookInfo{CBatAttackDoppelgangerApi::get().isImmune, doppelgangerAttackIsImmuneHooked});
    }

    if (userSettings().leveledDoppelgangerAttack != baseSettings().leveledDoppelgangerAttack) {
        // Allow doppelganger to transform into leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackDoppelgangerApi::get().onHit, doppelgangerAttackOnHitHooked});
    }

    if (userSettings().leveledTransformSelfAttack != baseSettings().leveledTransformSelfAttack) {
        // Allow transform self into leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackTransformSelfApi::get().onHit, transformSelfAttackOnHitHooked});
    }

    if (userSettings().leveledSummonAttack != baseSettings().leveledSummonAttack) {
        // Allow summon leveled units using script logic
        hooks.emplace_back(HookInfo{CBatAttackSummonApi::get().onHit, summonAttackOnHitHooked});
    }

    if (userSettings().missChanceSingleRoll != baseSettings().missChanceSingleRoll) {
        // Compute attack miss chance using single random value, instead of two
        hooks.emplace_back(HookInfo{fn.attackShouldMiss, attackShouldMissHooked});
    }

    if (userSettings().unrestrictedBestowWards != baseSettings().unrestrictedBestowWards) {
        // Support extended modifier count for bestow wards
        // clang-format off
        hooks.emplace_back(HookInfo{battle.constructor, battleMsgDataCtorHooked, (void**)&orig.battleMsgDataCtor});
        hooks.emplace_back(HookInfo{battle.copyConstructor, battleMsgDataCopyCtorHooked});
        hooks.emplace_back(HookInfo{battle.copyConstructor2, battleMsgDataCopyCtorHooked});
        hooks.emplace_back(HookInfo{battle.copyAssignment, battleMsgDataCopyAssignHooked});
        hooks.emplace_back(HookInfo{battle.copy, battleMsgDataCopyHooked});
        hooks.emplace_back(HookInfo{battle.destructor, battleMsgDataDtorHooked});
        hooks.emplace_back(HookInfo{CStackBattleActionMsgApi::vftable()->serialize, stackBattleActionMsgSerializeHooked, (void**)&orig.stackBattleActionMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleStartMsgApi::vftable()->serialize, cmdBattleStartMsgSerializeHooked, (void**)&orig.cmdBattleStartMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleChooseActionMsgApi::vftable()->serialize, cmdBattleChooseActionMsgSerializeHooked, (void**)&orig.cmdBattleChooseActionMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleResultMsgApi::vftable()->serialize, cmdBattleResultMsgSerializeHooked, (void**)&orig.cmdBattleResultMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleEndMsgApi::vftable()->serialize, cmdBattleEndMsgSerializeHooked, (void**)&orig.cmdBattleEndMsgSerialize});
        hooks.emplace_back(HookInfo{CCommandMsgApi::get().destructor, commandMsgDtorHooked, (void**)&orig.commandMsgDtor});
        hooks.emplace_back(HookInfo{CNetMsgApi::get().destructor, netMsgDtorHooked, (void**)&orig.netMsgDtor});
        // clang-format on
    }

    if (userSettings().movementCost.show) {
        // Show movement cost
        hooks.emplace_back(HookInfo{fn.showMovementPath, showMovementPathHooked});
    }

    return hooks;
}

/** Hooks that used only in Scenario Editor. */
static Hooks getScenarioEditorHooks()
{
    using namespace game;

    auto& orig = getOriginalFunctions();

    // clang-format off
    Hooks hooks{
        // Check sites placement the same way as ruins, allowing them to be placed on water
        {editorFunctions.canPlaceSite, editorFunctions.canPlaceRuin},
        // Allow editor to set elves race as caster in 'cast spell on location' event effect
        {editorFunctions.radioButtonIndexToPlayerId, radioButtonIndexToPlayerIdHooked},
        // Fix DLG_R_C_SPELL so it shows actual spell info
        {CEncLayoutSpellApi::get().constructor, encLayoutSpellCtorHooked, (void**)&orig.encLayoutSpellCtor},
        // Allow editor to place more than 200 stacks on a map
        {editorFunctions.countStacksOnMap, countStacksOnMapHooked}
    };
    // clang-format on

    return hooks;
}

Hooks getHooks()
{
    using namespace game;

    Hooks hooks{executableIsGame() ? getGameHooks() : getScenarioEditorHooks()};

    auto& fn = gameFunctions();
    auto& orig = getOriginalFunctions();

    // Register buildings with custom branch category as unit buildings
    hooks.emplace_back(HookInfo{fn.createBuildingType, createBuildingTypeHooked});
    // Support custom building branch category
    hooks.emplace_back(
        HookInfo{LBuildingCategoryTableApi::get().constructor, buildingCategoryTableCtorHooked});
    // Increase maximum allowed game turn
    hooks.emplace_back(HookInfo{fn.isTurnValid, isTurnValidHooked});
    // Support custom attack class category
    hooks.emplace_back(
        HookInfo{LAttackClassTableApi::get().constructor, attackClassTableCtorHooked});
    // Support custom attack class in CAttackImpl constructor
    hooks.emplace_back(HookInfo{CAttackImplApi::get().constructor, attackImplCtorHooked});
    /**
     * Display heal/damage value for any attack with qtyHeal/qtyDamage > 0 regardless of its class.
     * This hook is required for detailedAttackDescription.
     */
    hooks.emplace_back(HookInfo{fn.getAttackQtyDamageOrHeal, getAttackQtyDamageOrHealHooked});
    // Support custom attack sources
    hooks.emplace_back(
        HookInfo{LAttackSourceTableApi::get().constructor, attackSourceTableCtorHooked});
    hooks.emplace_back(
        HookInfo{fn.getSoldierAttackSourceImmunities, getSoldierAttackSourceImmunitiesHooked});
    hooks.emplace_back(HookInfo{fn.getSoldierImmunityPower, getSoldierImmunityPowerHooked,
                                (void**)&orig.getSoldierImmunityPower});
    hooks.emplace_back(HookInfo{fn.getAttackSourceText, getAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.appendAttackSourceText, appendAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.getAttackSourceWardFlagPosition,
                                getAttackSourceWardFlagPositionHooked,
                                (void**)&orig.getAttackSourceWardFlagPosition});

    if (userSettings().debugMode) {
        // Show and log game exceptions information
        hooks.emplace_back(HookInfo{os_exceptionApi::get().throwException, throwExceptionHooked,
                                    (void**)&orig.throwException});
    }

    if (userSettings().shatterDamageUpgradeRatio != baseSettings().shatterDamageUpgradeRatio) {
        // Allow users to customize shatter damage upgrade ratio
        hooks.emplace_back(
            HookInfo{fn.applyDynUpgradeToAttackData, applyDynUpgradeToAttackDataHooked});
    }

    if (userSettings().detailedAttackDescription != baseSettings().detailedAttackDescription) {
        /**
         * Fix missing attack information in unit encyclopedia:
         * 1) Damage of secondary attack if its not either poison, blister or frostbite
         * 2) Power (if applicable), source and reach of alternative attack
         * 3) Value of boost/lower damage if its secondary attack
         * 4) Value of lower initiative
         * 5) Critical hit indication
         * 6) Infinite effect indication
         */
        hooks.emplace_back(HookInfo{fn.generateAttackDescription, generateAttackDescriptionHooked});
    }

    return hooks;
}

Hooks getVftableHooks()
{
    using namespace game;

    Hooks hooks;

    if (CBatAttackBestowWardsApi::vftable())
        // Allow bestow wards to target dead units, so it can be coupled with Revive as a secondary
        // attack
        hooks.emplace_back(
            HookInfo{&CBatAttackBestowWardsApi::vftable()->method15, bestowWardsMethod15Hooked});

    if (userSettings().allowShatterAttackToMiss != baseSettings().allowShatterAttackToMiss) {
        if (CBatAttackShatterApi::vftable())
            // Fix an issue where shatter attack always hits regardless of its power value
            hooks.emplace_back(
                HookInfo{&CBatAttackShatterApi::vftable()->canMiss, shatterCanMissHooked});
    }

    return hooks;
}

void respopupInitHooked(void)
{
    logDebug("mss32Proxy.log", "Resource popup hook start");

    auto& variables = game::gameVariables();

    *variables.minimapMode = userSettings().showLandConverted;
    *variables.respopup = userSettings().showResources;

    logDebug("mss32Proxy.log", "Resource popup hook finished");
}

void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/)
{
    logDebug("mss32Proxy.log", "Show banners hook start");

    char* ptr = (char*)thisptr;
    *ptr = userSettings().showBanners;
    // meaning unknown
    ptr[1] = 0;

    logDebug("mss32Proxy.log", "Show banners hook finished");
    return thisptr;
}

bool __fastcall removeModifierHooked(game::CMidUnit* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* modifierId)
{
    if (!thisptr) {
        return false;
    }

    return getOriginalFunctions().removeModifier(thisptr, modifierId);
}

using ScriptLines = std::vector<std::string>;

ScriptLines::iterator addDialogUiElements(ScriptLines& script,
                                          const std::string& dialogName,
                                          const ScriptLines& newElements)
{
    const std::string dialogStr{std::string{"DIALOG\t"} + dialogName};

    auto line = std::find_if(script.begin(), script.end(), [&dialogStr](const std::string& line) {
        return line.find(dialogStr) != std::string::npos;
    });

    if (line == script.end()) {
        return line;
    }

    // Skip DIALOG and BEGIN lines
    std::advance(line, 2);
    return script.insert(line, newElements.begin(), newElements.end());
}

void addRandomMapGeneratorUi(ScriptLines& script)
{
    const ScriptLines uiElements{
        "\tBUTTON\tBINKW_PROXY_BTN_GEN_MAP,212,122,422,142,,,,,\"\",0",
        "\tTEXT\tBINKW_PROXY_TXT_GEN_MAP,212,122,422,142,\\hC;\\vC;,\"Generate random map\",\"Open map generation menu\""};

    addDialogUiElements(script, "DLG_CHOOSE_SKIRMISH", uiElements);
}

void changeHireDialogUi(ScriptLines& script)
{
    const std::string btnPgUpName{"BINKW_PROXY_BTN_PG_UP"};
    const std::string btnPgDownName{"BINKW_PROXY_BTN_PG_DN"};
    const std::string btnUpName{"BINKW_PROXY_BTN_LIST_UP"};
    const std::string btnDownName{"BINKW_PROXY_BTN_LIST_DN"};

    const std::string buttonImageName{"DLG_UPGRADE_LEADER_ARROW"};
    const std::string btnDownImageName{fmt::format("{:s}_DOWN_", buttonImageName)};
    const std::string btnUpImageName{fmt::format("{:s}_UP_", buttonImageName)};

    const int buttonSize{26};
    const int buttonX{415};
    const int buttonUpY{30};
    const int buttonDownY{480};

    const ScriptLines uiElements{
        // Page up and page down hotkeys
        fmt::format("\tBUTTON\t{0},0,0,10,10,,,,,\"\",0,34", btnPgDownName),
        fmt::format("\tBUTTON\t{0},10,0,20,10,,,,,\"\",0,33", btnPgUpName),
        // Up and down keys, this also enables list scrolling with mouse wheel
        fmt::format("\tBUTTON\t{0},{1},{2},{3},{4},{5}N,{5}H,{5}C,{5}N,\"\",1,38", btnUpName,
                    buttonX, buttonUpY, buttonX + buttonSize, buttonUpY + buttonSize,
                    btnUpImageName),
        fmt::format("\tBUTTON\t{0},{1},{2},{3},{4},{5}N,{5}H,{5}C,{5}N,\"\",1,40", btnDownName,
                    buttonX, buttonDownY, buttonX + buttonSize, buttonDownY + buttonSize,
                    btnDownImageName)};

    auto line = addDialogUiElements(script, "DLG_HIRE_LEADER_2", uiElements);
    auto lboxLine = std::find_if(line, script.end(), [](const std::string& scriptLine) {
        return scriptLine.find("\tLBOX\tLBOX_LEADER") != std::string::npos;
    });

    if (lboxLine != script.end()) {
        *lboxLine = fmt::format(
            "\tLBOX\tLBOX_LEADER,125,43,410,503,285,85,0,7,{0},{1},,,{2},{3},BTN_HIRE_LEADER,,,0,\"\",0",
            btnUpName, btnDownName, btnPgUpName, btnPgDownName);
    }
}

game::AutoDialogData* __fastcall loadScriptFileHooked(game::AutoDialogData* thisptr,
                                                      int /*%edx*/,
                                                      const char* filePath,
                                                      int /*unknown*/)
{
    const auto& stringApi = game::StringApi::get();

    thisptr->initialized = false;
    thisptr->unknown = 0;

    game::StringArray* array = &thisptr->lines;
    std::memset(array, 0, sizeof(game::StringArray));

    stringApi.initFromString(&thisptr->scriptPath, filePath);

    ScriptLines scriptLines;

    {
        std::ifstream stream(filePath);
        if (!stream) {
            logError("mssProxyError.log", "Failed to open AutoDialog script file");
            return thisptr;
        }

        for (std::string line; std::getline(stream, line);) {
            scriptLines.push_back(line);
        }
    }

    // As for now, it is easier to add new ui elements to AutoDialog script
    // and let the game do processing, than reverse-engineer all CDlgPrototype class hierarchy.
    // Use uncommon names to avoid collisions.
    // addRandomMapGeneratorUi(scriptLines);

    if (!unitsForHire().empty()) {
        changeHireDialogUi(scriptLines);
    }

    for (auto& line : scriptLines) {
        if (line.empty()) {
            continue;
        }

        const auto lastCharacter = line.length() - 1;
        if (line[lastCharacter] == '\n') {
            line.erase(lastCharacter);
        }

        game::String str;
        stringApi.initFromString(&str, line.c_str());
        game::StringArrayApi::get().pushBack(array, &str);
        stringApi.free(&str);
    }

    thisptr->initialized = true;
    return thisptr;
}

static int* sub_6804ae(void* thisptr)
{
    return *(int**)(((int*)thisptr)[2]);
}

/**
 * Returns ScenarioDataArrayWrapped, actual meaning and types are unknown.
 * @param[in] unknown parameter is a pointer returned from sub_6804ae().
 */
static game::ScenarioDataArrayWrapped* sub_573134(int* unknown)
{
    return *(game::ScenarioDataArrayWrapped**)(*(unknown + 2) + 20);
}

static size_t getScenariosTotal(const game::ScenarioDataArray& data)
{
    const auto bgn = reinterpret_cast<size_t>(data.bgn);
    const auto end = reinterpret_cast<size_t>(data.end);

    return (end - bgn) / sizeof(game::ScenarioData);
}

static void __fastcall buttonGenerateMapCallback(game::CMenuNewSkirmish* thisptr, int /*%edx*/)
{
    std::string errorMessage;
    if (!showMapGeneratorDialog(errorMessage)) {
        showMessageBox(errorMessage);
    }
}

static void menuNewSkirmishCtor(game::CMenuNewSkirmish* thisptr, int a1, const char* dialogName)
{
    using namespace game;

    const auto& menuBase = CMenuBaseApi::get();
    menuBase.constructor(thisptr, a1);
    thisptr->vftable = (game::CInterfaceVftable*)CMenuNewSkirmishApi::vftable();
    menuBase.createMenu(thisptr, dialogName);

    const auto dialog = menuBase.getDialogInterface(thisptr);
    const auto& menu = CMenuNewSkirmishApi::get();
    const auto& button = CButtonInterfApi::get();
    const auto freeFunctor = FunctorApi::get().createOrFree;
    Functor functor;

    menu.createButtonFunctor(&functor, 0, thisptr, &menu.buttonBackCallback);
    button.assignFunctor(dialog, "BTN_BACK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    menu.createButtonFunctor(&functor, 0, thisptr, &menu.loadScenarioCallback);
    CButtonInterf* loadButton = button.assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    void* callback = buttonGenerateMapCallback;
    menu.createButtonFunctor(&functor, 0, thisptr,
                             (CMenuNewSkirmishApi::Api::ButtonCallback*)&callback);
    button.assignFunctor(dialog, "BINKW_PROXY_BTN_GEN_MAP", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    const auto& listBox = CListBoxInterfApi::get();
    menu.createListBoxDisplayTextFunctor(&functor, 0, thisptr, &menu.displayTextCallback);
    listBox.assignDisplayTextFunctor(dialog, "TLBOX_GAME_SLOT", dialogName, &functor, true);
    freeFunctor(&functor, nullptr);

    menu.createListBoxFunctor(&functor, 0, thisptr, &menu.listBoxCallback);
    listBox.assignFunctor(dialog, "TLBOX_GAME_SLOT", dialogName, &functor);
    freeFunctor(&functor, nullptr);

    int* unknown = sub_6804ae(thisptr);
    ScenarioDataArrayWrapped* scenarios = sub_573134(unknown);

    const size_t scenariosTotal = getScenariosTotal(scenarios->data);
    if (!scenariosTotal) {
        // Nothing to load
        CButtonInterfApi::vftable()->setEnabled(loadButton, false);
        return;
    }

    CListBoxInterf* lBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_GAME_SLOT");
    listBox.initContents(lBox, scenariosTotal);
    menu.updateScenarioUi(unknown, dialog, listBox.selectedIndex(lBox));
}

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /*%edx*/,
    int a1)
{
    menuNewSkirmishCtor(thisptr, a1, "DLG_CHOOSE_SKIRMISH");
    thisptr->vftable = (game::CInterfaceVftable*)game::CMenuNewSkirmishSingleApi::vftable();

    return thisptr;
}

bool __stdcall addPlayerUnitsToHireListHooked(game::CMidDataCache2* dataCache,
                                              const game::CMidgardID* playerId,
                                              const game::CMidgardID* a3,
                                              game::IdList* hireList)
{
    using namespace game;

    const auto& list = IdListApi::get();
    list.setEmpty(hireList);

    const auto& id = CMidgardIDApi::get();
    if (id.getType(a3) != IdType::Fortification) {
        return false;
    }

    auto findScenarioObjectById = dataCache->vftable->findScenarioObjectById;
    auto playerObject = findScenarioObjectById(dataCache, playerId);
    if (!playerObject) {
        logError("mssProxyError.log",
                 fmt::format("Could not find player object with id {:x}", playerId->value));
        return false;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    CMidPlayer* player = (CMidPlayer*)dynamicCast(playerObject, 0, rtti.IMidScenarioObjectType,
                                                  rtti.CMidPlayerType, 0);
    if (!player) {
        logError("mssProxyError.log",
                 fmt::format("Object with id {:x} is not player", playerId->value));
        return false;
    }

    auto buildingsObject = findScenarioObjectById(dataCache, &player->buildingsId);
    if (!buildingsObject) {
        logError("mssProxyError.log", fmt::format("Could not find player buildings with id {:x}",
                                                  player->buildingsId.value));
        return false;
    }

    CPlayerBuildings* buildings = (CPlayerBuildings*)dynamicCast(buildingsObject, 0,
                                                                 rtti.IMidScenarioObjectType,
                                                                 rtti.CPlayerBuildingsType, 0);
    if (!buildings) {
        logError("mssProxyError.log", fmt::format("Object with id {:x} is not player buildings",
                                                  player->buildingsId.value));
        return false;
    }

    const auto& global = GlobalDataApi::get();
    auto races = (*global.getGlobalData())->races;
    TRaceType* race = (TRaceType*)global.findById(races, &player->raceId);

    const auto& addUnitToHireList = gameFunctions().addUnitToHireList;
    const auto& unitBranch = UnitBranchCategories::get();

    addUnitToHireList(race, buildings, unitBranch.fighter, hireList);
    addUnitToHireList(race, buildings, unitBranch.archer, hireList);
    addUnitToHireList(race, buildings, unitBranch.mage, hireList);
    addUnitToHireList(race, buildings, unitBranch.special, hireList);

    gameFunctions().addSideshowUnitToHireList(race, buildings, hireList);

    const int raceIndex = id.getTypeIndex(&player->raceId);
    if (!unitsForHire().empty()) {
        const auto& units = unitsForHire()[raceIndex];
        for (const auto& unit : units) {
            list.add(hireList, &unit);
        }
    }

    return true;
}

static game::LBuildingCategory custom;
static bool customCategoryExists{false};

void __stdcall createBuildingTypeHooked(const game::CDBTable* dbTable,
                                        void* a2,
                                        const game::GlobalData** globalData)
{
    using namespace game;

    auto* buildings = (*globalData)->buildingCategories;
    LBuildingCategory category;
    category.vftable = BuildingCategories::vftable();

    auto& db = CDBTableApi::get();
    db.findBuildingCategory(&category, dbTable, "CATEGORY", buildings);

    auto memAlloc = Memory::get().allocate;
    auto constructor = TBuildingTypeApi::get().constructor;
    TBuildingType* buildingType = nullptr;

    auto& buildingCategories = BuildingCategories::get();
    if (category.id == buildingCategories.unit->id
        || (customCategoryExists && (category.id == custom.id))) {
        // This is TBuildingUnitUpgType constructor
        // without TBuildingTypeData::category validity check
        TBuildingUnitUpgType* unitBuilding = (TBuildingUnitUpgType*)memAlloc(
            sizeof(TBuildingUnitUpgType));

        constructor(unitBuilding, dbTable, globalData);
        unitBuilding->branch.vftable = UnitBranchCategories::vftable();
        unitBuilding->vftable = (IMidObjectVftable*)TBuildingUnitUpgTypeApi::vftable();

        auto* branches = (*globalData)->unitBranches;
        db.findUnitBranchCategory(&unitBuilding->branch, dbTable, "BRANCH", branches);
        db.readUnitLevel(&unitBuilding->level, dbTable, "LEVEL");

        buildingType = unitBuilding;
    } else {
        buildingType = constructor((TBuildingType*)memAlloc(sizeof(TBuildingType)), dbTable,
                                   globalData);
    }

    if (!gameFunctions().addObjectAndCheckDuplicates(a2, buildingType)) {
        db.duplicateRecordException(dbTable, &buildingType->buildingId);
    }
}

game::LBuildingCategoryTable* __fastcall buildingCategoryTableCtorHooked(
    game::LBuildingCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    static const char dbfFileName[] = "LBuild.dbf";

    logDebug("newBuildingType.log", "Hook started");

    {
        utils::DbfFile dbf;
        std::filesystem::path globals{globalsFolderPath};
        if (!dbf.open(globals / dbfFileName)) {
            logError("mssProxyError.log", fmt::format("Could not open {:s}", dbfFileName));
        } else {
            utils::DbfRecord record;
            if (dbf.recordsTotal() > 4 && dbf.record(record, 4)) {
                std::string categoryName;
                if (record.value(categoryName, "TEXT") && trimSpaces(categoryName) == "L_CUSTOM") {
                    customCategoryExists = true;
                    logDebug("newBuildingType.log", "Found custom building category");
                }
            }
        }
    }

    using namespace game;
    auto& table = LBuildingCategoryTableApi::get();
    auto& categories = BuildingCategories::get();

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LBuildingCategoryTableApi::vftable();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(categories.guild, thisptr, "L_GUILD", dbfFileName);
    table.readCategory(categories.heal, thisptr, "L_HEAL", dbfFileName);
    table.readCategory(categories.magic, thisptr, "L_MAGIC", dbfFileName);
    table.readCategory(categories.unit, thisptr, "L_UNIT", dbfFileName);

    if (customCategoryExists) {
        table.readCategory(&custom, thisptr, "L_CUSTOM", dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newBuildingType.log", "Hook finished");
    return thisptr;
}

game::CBuildingBranch* __fastcall buildingBranchCtorHooked(game::CBuildingBranch* thisptr,
                                                           int /*%edx*/,
                                                           game::CPhaseGame* phaseGame,
                                                           int* branchNumber)
{
    using namespace game;

    logDebug("newBuildingType.log", "CBuildingBranchCtor hook started");

    auto memAlloc = Memory::get().allocate;
    CBuildingBranchData* data = (CBuildingBranchData*)memAlloc(sizeof(CBuildingBranchData));

    const auto& buildingBranch = CBuildingBranchApi::get();
    buildingBranch.initData(data);

    thisptr->data = data;
    thisptr->vftable = CBuildingBranchApi::vftable();

    buildingBranch.initBranchList(&thisptr->data->list);
    thisptr->data->branchNumber = *branchNumber;

    auto* dialogName = &thisptr->data->branchDialogName;
    StringApi::get().free(dialogName);
    dialogName->string = nullptr;
    dialogName->length = 0;
    dialogName->lengthAllocated = 0;

    const auto& phase = CPhaseApi::get();
    auto playerId = phase.getCurrentPlayerId(&phaseGame->phase);
    auto objectMap = phase.getObjectMap(&phaseGame->phase);
    auto findScenarioObjectById = objectMap->vftable->findScenarioObjectById;

    auto playerObject = findScenarioObjectById(objectMap, playerId);

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    const CMidPlayer* player = (const CMidPlayer*)dynamicCast(playerObject, 0,
                                                              rtti.IMidScenarioObjectType,
                                                              rtti.CMidPlayerType, 0);

    const LRaceCategory* playerRace = &player->raceType->data->raceType;
    thisptr->data->raceCategory.table = playerRace->table;
    thisptr->data->raceCategory.id = playerRace->id;

    const auto& fn = gameFunctions();
    auto lord = fn.getLordByPlayer(player);
    auto buildList = lord->data->buildList;

    const auto& lordTypeApi = TLordTypeApi::get();
    BuildListIterator iterator;
    lordTypeApi.getIterator(buildList, &iterator);

    const auto globalData = GlobalDataApi::get().getGlobalData();
    auto buildings = (*globalData)->buildings;

    while (true) {
        BuildListIterator endIterator;
        lordTypeApi.getEndIterator(buildList, &endIterator);

        if (iterator.node == endIterator.node && iterator.node2 == endIterator.node2) {
            break;
        }

        const auto findById = GlobalDataApi::get().findById;
        const TBuildingType* buildingType = (const TBuildingType*)findById(buildings,
                                                                           &iterator.node->value);

        const LBuildingCategory* buildingCategory = &buildingType->data->category;
        const auto& buildingCategories = BuildingCategories::get();

        if (buildingCategory->id == buildingCategories.unit->id) {
            const TBuildingUnitUpgType* unitUpg = (const TBuildingUnitUpgType*)
                dynamicCast(buildingType, 0, rtti.TBuildingTypeType, rtti.TBuildingUnitUpgTypeType,
                            0);

            LUnitBranch unitBranch;
            unitBranch.table = unitUpg->branch.table;
            unitBranch.id = unitUpg->branch.id;
            unitBranch.vftable = UnitBranchCategories::vftable();

            const auto& unitBranchCategories = UnitBranchCategories::get();
            const int num = *branchNumber;

            if (unitBranch.id == unitBranchCategories.sideshow->id) {
                buildingBranch.addSideshowUnitBuilding(&thisptr->data->list, unitUpg);
            } else if (unitBranch.id == unitBranchCategories.fighter->id && num == 0
                       || unitBranch.id == unitBranchCategories.mage->id && num == 1
                       || unitBranch.id == unitBranchCategories.archer->id && num == 2
                       || unitBranch.id == unitBranchCategories.special->id && num == 4) {
                buildingBranch.addUnitBuilding(phaseGame, &thisptr->data->list, unitUpg);
            }

        } else if ((buildingCategory->id == buildingCategories.guild->id
                    || buildingCategory->id == buildingCategories.heal->id
                    || buildingCategory->id == buildingCategories.magic->id
                    || (customCategoryExists && (buildingCategory->id == custom.id)))
                   && *branchNumber == 3) {
            buildingBranch.addBuilding(phaseGame, &thisptr->data->list, buildingType);
        }

        lordTypeApi.advanceIterator(&iterator.node, &iterator.node2->unknown);
    }

    logDebug("newBuildingType.log", "Ctor finished");
    return thisptr;
}

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    IAttack* attack = soldier->vftable->getAttackById(soldier);
    auto attackVftable = (const IAttackVftable*)attack->vftable;
    const LAttackReach* reach = attackVftable->getAttackReach(attack);

    // Place units with adjacent attack reach at the front lane, despite of their attack class
    if (reach->id == AttackReachCategories::get().adjacent->id) {
        return 1;
    }

    return 0;
}

bool __stdcall isTurnValidHooked(int turn)
{
    return turn >= 0 && turn <= 9999 || turn == -1;
}

game::CMidgardID* __stdcall radioButtonIndexToPlayerIdHooked(game::CMidgardID* playerId,
                                                             game::IMidgardObjectMap* objectMap,
                                                             int index)
{
    using namespace game;
    const auto& races = RaceCategories::get();
    const auto& fn = editorFunctions;

    const CMidPlayer* player{nullptr};
    switch (index) {
    case 0:
        player = fn.findPlayerByRaceCategory(races.human, objectMap);
        break;
    case 1:
        player = fn.findPlayerByRaceCategory(races.heretic, objectMap);
        break;
    case 2:
        player = fn.findPlayerByRaceCategory(races.undead, objectMap);
        break;
    case 3:
        player = fn.findPlayerByRaceCategory(races.dwarf, objectMap);
        break;
    case 4:
        player = fn.findPlayerByRaceCategory(races.elf, objectMap);
        break;
    }

    *playerId = player ? player->playerId : emptyId;

    return playerId;
}

bool __fastcall giveAttackCanPerformHooked(game::CBatAttackGiveAttack* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetStackId{};
    thisptr->vftable->getTargetStackId(thisptr, &targetStackId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID alliedStackId{};
    fn.getAllyOrEnemyStackId(&alliedStackId, battleMsgData, unitId, true);

    if (targetStackId != alliedStackId) {
        // Do not allow to give additional attacks to enemies
        return false;
    }

    if (*unitId == thisptr->unitId1) {
        // Do not allow to give additional attacks to self
        return false;
    }

    CMidUnit* unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    auto attack = soldier->vftable->getAttackById(soldier);
    const auto attackClass = attack->vftable->getAttackClass(attack);

    const auto& attackCategories = AttackClassCategories::get();

    if (attackClass->id == attackCategories.giveAttack->id) {
        // Do not allow to buff other units with this attack type
        return false;
    }

    auto secondAttack = soldier->vftable->getSecondAttackById(soldier);
    if (!secondAttack) {
        return true;
    }

    const auto secondAttackClass = secondAttack->vftable->getAttackClass(secondAttack);
    // Do not allow to buff other units with this attack type as their second attack
    return secondAttackClass->id != attackCategories.giveAttack->id;
}

bool __fastcall shatterCanPerformHooked(game::CBatAttackShatter* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetStackId{};
    thisptr->vftable->getTargetStackId(thisptr, &targetStackId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID alliedStackId{};
    fn.getAllyOrEnemyStackId(&alliedStackId, battleMsgData, unitId, true);

    if (targetStackId != alliedStackId) {
        // Can't target allies
        return false;
    }

    auto& battle = BattleMsgDataApi::get();
    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::Retreat)) {
        // Can't target retreating units
        return false;
    }

    const int shatteredArmor = battle.getUnitShatteredArmor(battleMsgData, unitId);
    if (shatteredArmor >= userSettings().shatteredArmorMax) {
        return false;
    }

    CMidUnit* unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    int unitArmor{};
    soldier->vftable->getArmor(soldier, &unitArmor);

    const int fortArmor = battle.getUnitFortificationArmor(battleMsgData, unitId);
    const int reducedArmor = unitArmor - shatteredArmor;

    if (reducedArmor > fortArmor) {
        return reducedArmor > 0;
    }

    return false;
}

void __fastcall setUnitShatteredArmorHooked(game::BattleMsgData* thisptr,
                                            int /*%edx*/,
                                            const game::CMidgardID* unitId,
                                            int armor)
{
    using namespace game;

    auto info = BattleMsgDataApi::get().getUnitInfoById(thisptr, unitId);
    if (!info) {
        return;
    }

    info->shatteredArmor = std::clamp(info->shatteredArmor + armor, 0,
                                      userSettings().shatteredArmorMax);
}

void __fastcall shatterOnHitHooked(game::CBatAttackShatter* thisptr,
                                   int /*%edx*/,
                                   game::IMidgardObjectMap* objectMap,
                                   game::BattleMsgData* battleMsgData,
                                   game::CMidgardID* unitId,
                                   game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    auto attackVftable = (const IAttackVftable*)thisptr->attack->vftable;

    const auto damageMax{userSettings().shatterDamageMax};
    int shatterDamage = attackVftable->getQtyDamage(thisptr->attack);
    if (shatterDamage > damageMax) {
        shatterDamage = damageMax;
    }

    setUnitShatteredArmorHooked(battleMsgData, 0, unitId, shatterDamage);

    const auto unit = gameFunctions().findUnitById(objectMap, unitId);

    BattleAttackUnitInfo info{};
    info.unitId = *unitId;
    info.unitImplId = unit->unitImpl->unitId;
    info.attackMissed = false;
    info.damage = 0;

    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);
}

bool __fastcall shatterCanMissHooked(game::CBatAttackShatter* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData,
                                     game::CMidgardID* id)
{
    return true;
}

int __stdcall deletePlayerBuildingsHooked(game::IMidgardObjectMap*, game::CMidPlayer*)
{
    return 0;
}

game::CEncLayoutSpell* __fastcall encLayoutSpellCtorHooked(game::CEncLayoutSpell* thisptr,
                                                           int /*%edx*/,
                                                           game::IMidgardObjectMap* objectMap,
                                                           game::CInterface* interface,
                                                           void* a2,
                                                           game::CMidgardID* spellId,
                                                           game::CEncParamBase* encParam,
                                                           game::CMidgardID* playerId)
{
    using namespace game;

    if (!playerId) {
        IteratorPtr iterator;
        Iterators::get().createPlayersIterator(&iterator, objectMap);

        IteratorPtr endIterator;
        Iterators::get().createPlayersEndIterator(&endIterator, objectMap);

        // Use id of the first found player to show spell info
        if (!iterator.data->vftable->end(iterator.data, endIterator.data)) {
            playerId = iterator.data->vftable->getObjectId(iterator.data);
        }

        auto& freeSmartPtr = SmartPointerApi::get().createOrFree;
        freeSmartPtr((SmartPointer*)&iterator, nullptr);
        freeSmartPtr((SmartPointer*)&endIterator, nullptr);
    }

    // Show spell price and casting cost
    encParam->data->unknown2 = 4;
    return getOriginalFunctions().encLayoutSpellCtor(thisptr, objectMap, interface, a2, spellId,
                                                     encParam, playerId);
}

int __stdcall countStacksOnMapHooked(game::IMidgardObjectMap*)
{
    return 0;
}

game::CDDCarryOverItems* __fastcall carryOverItemsCtorHooked(game::CDDCarryOverItems* thisptr,
                                                             int /*%edx*/,
                                                             game::IMidDropManager* dropManager,
                                                             game::CListBoxInterf* listBox,
                                                             game::CPhaseGame* phaseGame,
                                                             int carryOverItemsMax)
{
    return getOriginalFunctions().carryOverItemsCtor(thisptr, dropManager, listBox, phaseGame,
                                                     userSettings().carryOverItemsMax);
}

void __fastcall markMapPositionHooked(void* thisptr, int /*%edx*/, game::CMqPoint* position)
{
    if (position->x < 0 || position->x >= 144 || position->y < 0 || position->y >= 144)
        return;

    return getOriginalFunctions().markMapPosition(thisptr, position);
}

int __stdcall computeDamageHooked(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::IAttack* attackImpl,
                                  const game::CMidgardID* attackerUnitId,
                                  const game::CMidgardID* targetUnitId,
                                  bool computeCriticalHit,
                                  int* attackDamage,
                                  int* criticalHitDamage)
{
    int damage;
    int critDamage;
    int totalDamage = getOriginalFunctions().computeDamage(objectMap, battleMsgData, attackImpl,
                                                           attackerUnitId, targetUnitId,
                                                           computeCriticalHit, &damage,
                                                           &critDamage);

    if (critDamage != 0) {
        int critChance = userSettings().criticalHitChance;
        bool critMissed = game::gameFunctions().attackShouldMiss(&critChance);
        if (critMissed) {
            totalDamage -= critDamage;
            critDamage = 0;
        }
    }

    if (attackDamage)
        *attackDamage = damage;
    if (criticalHitDamage)
        *criticalHitDamage = critDamage;
    return totalDamage;
}

void __stdcall getAttackPowerHooked(int* power,
                                    const game::IAttack* attack,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* unitId,
                                    const game::BattleMsgData* battleMsgData)
{
    if (!attack) {
        *power = 100;
        return;
    }

    using namespace game;

    const auto attackClass = attack->vftable->getAttackClass(attack);

    if (!attackHasPower(attackClass)) {
        *power = 100;
        return;
    }

    int tmpPower{};
    attack->vftable->getPower(attack, &tmpPower);

    const auto& battle = BattleMsgDataApi::get();
    auto groupId = battle.isUnitAttacker(battleMsgData, unitId) ? &battleMsgData->attackerGroupId
                                                                : &battleMsgData->defenderGroupId;

    const auto& fn = gameFunctions();

    if (!fn.isGroupOwnerPlayerHuman(objectMap, groupId)) {
        const auto& id = CMidgardIDApi::get();

        CMidgardID scenarioInfoId{};
        auto scenarioId = objectMap->vftable->getId(objectMap);

        id.fromParts(&scenarioInfoId, id.getCategory(scenarioId), id.getCategoryIndex(scenarioId),
                     IdType::ScenarioInfo, 0);

        auto scenarioInfo = static_cast<const CScenarioInfo*>(
            objectMap->vftable->findScenarioObjectById(objectMap, &scenarioInfoId));

        const auto& difficulties = DifficultyLevelCategories::get();
        const auto difficultyId = scenarioInfo->gameDifficulty.id;

        const auto& aiAttackPower = userSettings().aiAttackPowerBonus;
        const std::int8_t* bonus = &aiAttackPower.easy;

        if (difficultyId == difficulties.easy->id) {
            bonus = &aiAttackPower.easy;
        } else if (difficultyId == difficulties.average->id) {
            bonus = &aiAttackPower.average;
        } else if (difficultyId == difficulties.hard->id) {
            bonus = &aiAttackPower.hard;
        } else if (difficultyId == difficulties.veryHard->id) {
            bonus = &aiAttackPower.veryHard;
        }

        if (aiAttackPower.absolute) {
            tmpPower += *bonus;
        } else {
            tmpPower += tmpPower * *bonus / 100;
        }

        tmpPower = std::clamp(tmpPower, attackPowerLimits.min, attackPowerLimits.max);
    }

    const auto& attacks = AttackClassCategories::get();
    if (battleMsgData->currentRound > userSettings().disableAllowedRoundMax
        && (attackClass->id == attacks.paralyze->id || attackClass->id == attacks.petrify->id)) {
        tmpPower = 0;
    }

    tmpPower -= battle.getAttackPowerReduction(battleMsgData, unitId);
    *power = std::clamp(tmpPower, attackPowerLimits.min, attackPowerLimits.max);
}

bool __stdcall attackShouldMissHooked(const int* power)
{
    return game::gameFunctions().generateRandomNumber(100) > *power;
}

int __stdcall getUnitHealAttackNumberHooked(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    auto unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    auto attack = soldier->vftable->getAttackById(soldier);
    int qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal > 0)
        return 1;

    auto attack2 = soldier->vftable->getSecondAttackById(soldier);
    if (attack2) {
        qtyHeal = attack2->vftable->getQtyHeal(attack2);
        if (qtyHeal > 0)
            return 2;
    }

    return 0;
}

int __stdcall getAttackQtyDamageOrHealHooked(const game::IAttack* attack, int damageMax)
{
    int qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal)
        return qtyHeal;

    int qtyDamage = attack->vftable->getQtyDamage(attack);
    if (qtyDamage > damageMax)
        qtyDamage = damageMax;

    return qtyDamage;
}

void __stdcall setUnknown9Bit1AndClearBoostLowerDamageHooked(game::BattleMsgData* battleMsgData,
                                                             const game::CMidgardID* unitId,
                                                             game::CMidgardID* nextAttackUnitId)
{
    using namespace game;

    getOriginalFunctions().setUnknown9Bit1AndClearBoostLowerDamage(battleMsgData, unitId,
                                                                   nextAttackUnitId);

    if (nextAttackUnitId->value == unitId->value) {
        nextAttackUnitId->value = emptyId.value;

        const auto& battle = BattleMsgDataApi::get();
        battle.setUnitStatus(battleMsgData, unitId, BattleStatus::Defend, false);
        battle.setAttackPowerReduction(battleMsgData, unitId, 0);
    }
}

void __stdcall beforeAttackHooked(game::BattleMsgData* battleMsgData,
                                  game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    battle.setUnitStatus(battleMsgData, unitId, BattleStatus::Defend, false);

    auto unitInfo = battle.getUnitInfoById(battleMsgData, unitId);

    auto modifiedUnitIds = getModifiedUnitIds(unitInfo);
    for (auto it = modifiedUnitIds.begin(); it != modifiedUnitIds.end(); it++)
        removeModifiers(battleMsgData, objectMap, unitInfo, &(*it));
    resetModifiedUnitsInfo(unitInfo);

    battle.setAttackPowerReduction(battleMsgData, unitId, 0);
}

void __stdcall throwExceptionHooked(const game::os_exception* thisptr, const void* throwInfo)
{
    showErrorMessageBox(fmt::format("Caught exception '{:s}'.\n"
                                    "The {:s} will probably crash now.",
                                    thisptr->message, executableIsGame() ? "game" : "editor"));

    getOriginalFunctions().throwException(thisptr, throwInfo);
}

int __stdcall computeUnitEffectiveHpHooked(const game::IMidgardObjectMap* objectMap,
                                           const game::CMidUnit* unit,
                                           const game::BattleMsgData* battleMsgData)
{
    if (!unit) {
        return 0;
    }

    return getOriginalFunctions().computeUnitEffectiveHp(objectMap, unit, battleMsgData);
}

void __stdcall applyDynUpgradeToAttackDataHooked(const game::CMidgardID* unitImplId,
                                                 game::CUnitGenerator* unitGenerator,
                                                 int unitLevel,
                                                 game::IdType dynUpgradeType,
                                                 const game::CMidgardID* altAttackId,
                                                 game::CAttackData* attackData)
{
    using namespace game;

    CMidgardID leveledUnitImplId{};
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &leveledUnitImplId, unitImplId,
                                               unitLevel);

    CMidgardID globalUnitImplId{};
    unitGenerator->vftable->getGlobalUnitImplId(unitGenerator, &globalUnitImplId,
                                                &leveledUnitImplId);

    CMidgardID leveledAttackId{};
    CMidgardIDApi::get().changeType(&leveledAttackId, &leveledUnitImplId, dynUpgradeType);

    CDynUpgrade* upgrade1 = nullptr;
    CDynUpgrade* upgrade2 = nullptr;
    int upgrade1Count;
    int upgrade2Count;
    gameFunctions().computeUnitDynUpgrade(&globalUnitImplId, unitLevel, &upgrade1, &upgrade2,
                                          &upgrade1Count, &upgrade2Count);

    attackData->attackId = leveledAttackId;
    attackData->altAttack = *altAttackId;

    if (upgrade1)
        attackData->initiative += upgrade1->initiative * upgrade1Count;
    if (upgrade2)
        attackData->initiative += upgrade2->initiative * upgrade2Count;

    if (attackData->power > 0) {
        if (upgrade1)
            attackData->power += upgrade1->power * upgrade1Count;
        if (upgrade2)
            attackData->power += upgrade2->power * upgrade2Count;
    }

    if (attackData->qtyDamage > 0) {
        float factor = 1.0;
        if (attackData->attackClass->id == AttackClassCategories::get().shatter->id)
            factor = (float)userSettings().shatterDamageUpgradeRatio / 100;

        if (upgrade1)
            attackData->qtyDamage += lround(upgrade1->damage * upgrade1Count * factor);
        if (upgrade2)
            attackData->qtyDamage += lround(upgrade2->damage * upgrade2Count * factor);
    }

    if (attackData->qtyHeal > 0) {
        if (upgrade1)
            attackData->qtyHeal += upgrade1->heal * upgrade1Count;
        if (upgrade2)
            attackData->qtyHeal += upgrade2->heal * upgrade2Count;
    }
}

bool __stdcall findAttackTargetHooked(game::IMidgardObjectMap* objectMap,
                                      game::CMidgardID* unitId,
                                      game::IAttack* attack,
                                      game::CMidUnitGroup* targetGroup,
                                      void* a5,
                                      game::BattleMsgData* battleMsgData,
                                      game::CMidgardID* targetUnitId)
{
    using namespace game;

    if (getOriginalFunctions().findAttackTarget(objectMap, unitId, attack, targetGroup, a5,
                                                battleMsgData, targetUnitId)) {
        return true;
    }

    const auto& battle = BattleMsgDataApi::get();

    auto attackClass = attack->vftable->getAttackClass(attack);
    const auto& attackCategories = AttackClassCategories::get();
    if (attackClass->id == attackCategories.lowerDamage->id) {
        return battle.findBoostAttackTarget(objectMap, battleMsgData, targetGroup, a5,
                                            targetUnitId);
    } else if (attackClass->id == attackCategories.lowerInitiative->id) {
        return battle.findFearAttackTarget(objectMap, battleMsgData, targetGroup, a5, targetUnitId);
    }

    return false;
}

bool __stdcall isRaceCategoryUnplayableHooked(const game::LRaceCategory* raceCategory)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    return raceCategory->id == races.neutral->id;
}

void __stdcall loadLordFaceImagesHooked(const char** faceNames,
                                        size_t facesTotal,
                                        game::Vector<game::ImagePair>* faces)
{
    static const char* names[20] = {"FACE_HU_0", "FACE_HU_1", "FACE_HU_2", "FACE_HU_3"};

    if (!faceNames) {
        faceNames = names;
    }

    getOriginalFunctions().loadLordFaceImages(faceNames, facesTotal, faces);
}

char* __fastcall buildingBranchCreateDialogNameHooked(game::CBuildingBranch* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    const auto raceId{thisptr->data->raceCategory.id};

    char raceLetter{'?'};
    if (raceId == races.human->id) {
        raceLetter = 'H';
    } else if (raceId == races.dwarf->id) {
        raceLetter = 'M';
    } else if (raceId == races.heretic->id) {
        raceLetter = 'E';
    } else if (raceId == races.undead->id) {
        raceLetter = 'U';
    } else if (raceId == races.elf->id) {
        raceLetter = 'F';
    } else {
        for (const auto& race : newRaces()) {
            if (raceId == race.category.id) {
                // TODO: get these new race letters from lua
                raceLetter = 'N';
                break;
            }
        }
    }

    static const std::array<const char*, 5> branchNames = {
        {"FIGHTER", "MAGE", "ARCHER", "OTHER", "SPECIAL"}};

    const auto dialogName{
        fmt::format("DLG_{:c}_{:s}", raceLetter, branchNames[thisptr->data->branchNumber])};

    logDebug("capital.log", fmt::format("Race id {:d}, building branch dialog name '{:s}'",
                                        (int)raceId, dialogName));

    StringApi::get().initFromStringN(&thisptr->data->branchDialogName, dialogName.c_str(),
                                     dialogName.length());

    return thisptr->data->branchDialogName.string;
}

} // namespace hooks
