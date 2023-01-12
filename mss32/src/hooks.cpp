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
#include "batattackdrainlevel.h"
#include "batattackdrainoverflow.h"
#include "batattackgiveattack.h"
#include "batattackshatter.h"
#include "batattacksummon.h"
#include "batattacktransformother.h"
#include "batattacktransformself.h"
#include "batattackuntransformeffect.h"
#include "batbigface.h"
#include "battleattackinfo.h"
#include "battlemsgdatahooks.h"
#include "battleviewerinterf.h"
#include "battleviewerinterfhooks.h"
#include "bestowwardshooks.h"
#include "buildingbranch.h"
#include "buildingtype.h"
#include "button.h"
#include "cmdbattlechooseactionmsg.h"
#include "cmdbattleendmsg.h"
#include "cmdbattleresultmsg.h"
#include "cmdbattlestartmsg.h"
#include "commandmsghooks.h"
#include "condinterf.h"
#include "condinterfhooks.h"
#include "customattackhooks.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "d2string.h"
#include "dbfaccess.h"
#include "dbtable.h"
#include "dialoginterf.h"
#include "difficultylevel.h"
#include "displayhandlershooks.h"
#include "doppelgangerhooks.h"
#include "drainattackhooks.h"
#include "drainlevelhooks.h"
#include "dynamiccast.h"
#include "dynupgrade.h"
#include "editor.h"
#include "effectinterfhooks.h"
#include "effectresulthooks.h"
#include "enclayoutcityhooks.h"
#include "enclayoutruinhooks.h"
#include "enclayoutstack.h"
#include "enclayoutstackhooks.h"
#include "enclayoutunithooks.h"
#include "encparambase.h"
#include "eventconditioncathooks.h"
#include "eventeffectcathooks.h"
#include "fortcategory.h"
#include "fortification.h"
#include "gameutils.h"
#include "globaldata.h"
#include "idlist.h"
#include "interfmanager.h"
#include "interftexthooks.h"
#include "isoenginegroundhooks.h"
#include "itemtransferhooks.h"
#include "leaderabilitycat.h"
#include "listbox.h"
#include "log.h"
#include "lordtype.h"
#include "mainview2.h"
#include "mainview2hooks.h"
#include "mapgen.h"
#include "mempool.h"
#include "menuloadskirmishmultihooks.h"
#include "menunewskirmishhooks.h"
#include "menunewskirmishsingle.h"
#include "menuphasehooks.h"
#include "menuprotocolhooks.h"
#include "middatacache.h"
#include "midevconditionhooks.h"
#include "mideveffecthooks.h"
#include "mideventhooks.h"
#include "midgardid.h"
#include "midgardmsgbox.h"
#include "midgardscenariomap.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midmusic.h"
#include "midplayer.h"
#include "midscenvariables.h"
#include "midserverlogic.h"
#include "midserverlogichooks.h"
#include "midstack.h"
#include "midunitdescriptor.h"
#include "midunitdescriptorhooks.h"
#include "midunithooks.h"
#include "midvillage.h"
#include "modifgroup.h"
#include "modifgrouphooks.h"
#include "modifierutils.h"
#include "movepathhooks.h"
#include "musichooks.h"
#include "netmsghooks.h"
#include "netplayerinfo.h"
#include "originalfunctions.h"
#include "phasegame.h"
#include "playerbuildings.h"
#include "playerincomehooks.h"
#include "racecategory.h"
#include "racetype.h"
#include "restrictions.h"
#include "scenariodata.h"
#include "scenariodataarray.h"
#include "scenarioinfo.h"
#include "settings.h"
#include "smartptr.h"
#include "stackbattleactionmsg.h"
#include "summonhooks.h"
#include "testconditionhooks.h"
#include "transformotherhooks.h"
#include "transformselfhooks.h"
#include "umattack.h"
#include "umattackhooks.h"
#include "umunit.h"
#include "umunithooks.h"
#include "unitbranchcat.h"
#include "unitgenerator.h"
#include "unitmodifier.h"
#include "unitmodifierhooks.h"
#include "unitsforhire.h"
#include "unitutils.h"
#include "untransformeffecthooks.h"
#include "usracialsoldier.h"
#include "ussoldier.h"
#include "usstackleader.h"
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
        {CMidUnitApi::get().removeModifier, removeModifierHooked},
        // Fix unit transformation to include hp mods into current hp recalculation
        {CMidUnitApi::get().transform, transformHooked},
        // Show buildings with custom branch category on the 'other buildings' tab
        {CBuildingBranchApi::get().constructor, buildingBranchCtorHooked},
        // Allow alchemists to buff retreating units
        {CBatAttackGiveAttackApi::vftable()->canPerform, giveAttackCanPerformHooked},
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
        {CBatAttackDrainApi::vftable()->onHit, drainAttackOnHitHooked},
        {CBatAttackDrainOverflowApi::vftable()->onHit, drainOverflowAttackOnHitHooked},
        // Support additional music tracks for battle and capital cities
        {CMidMusicApi::get().playBattleTrack, playBattleTrackHooked},
        {CMidMusicApi::get().playCapitalTrack, playCapitalTrackHooked},
        // Fix game crash with pathfinding on 144x144 maps
        {fn.markMapPosition, markMapPositionHooked, (void**)&orig.markMapPosition},
        // Allow user to tweak power computations
        {fn.getAttackPower, getAttackPowerHooked},
        // Fix game crash when AI controlled unit with transform self attack
        // uses alternative attack with 'adjacent' attack range
        // Fix incorrect calculation of effective HP used by AI for target prioritization
        {fn.computeUnitEffectiveHpForAi, computeUnitEffectiveHpForAiHooked},
        // Allow transform-self attack to not consume a unit turn for transformation
        // Fixes modifiers becoming permanent after modified unit is transformed
        // Support custom attack damage ratios
        {battle.beforeBattleTurn, beforeBattleTurnHooked},
        // Fix free transform-self to properly reset if the same unit has consequent turns in consequent battles
        {battle.beforeBattleRound, beforeBattleRoundHooked, (void**)&orig.beforeBattleRound},
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
        {CBatAttackBestowWardsApi::vftable()->canPerform, bestowWardsAttackCanPerformHooked},
        /**
         * Fix bestow wards:
         * 1) Becoming permanent when more than 8 modifiers are applied at once
         * 2) Not resetting attack class wards (when reapplied)
         * 3) Incorrectly resetting attack source ward if its modifier also contains hp, regen or armor element
         */
        // Fixes modifiers getting lost after modified unit is untransformed
        {CBatAttackBestowWardsApi::vftable()->onHit, bestowWardsAttackOnHitHooked},
        // Fix bestow wards with double attack where modifiers granted by first attack are removed
        {battle.afterBattleTurn, afterBattleTurnHooked},
        // Allow any attack with QTY_HEAL > 0 to heal units when battle ends (just like ordinary heal does)
        {fn.getUnitHealAttackNumber, getUnitHealAttackNumberHooked},
        // Fix AI not being able to find target for lower damage/ini attack
        // Fix incorrect AI prioritization of shatter attack targets
        {battle.findAttackTarget, findAttackTargetHooked, (void**)&orig.findAttackTarget},
        // Support custom attack sources
        {fn.getUnitAttackSourceImmunities, getUnitAttackSourceImmunitiesHooked},
        {battle.isUnitAttackSourceWardRemoved, isUnitAttackSourceWardRemovedHooked},
        {battle.removeUnitAttackSourceWard, removeUnitAttackSourceWardHooked},
        {battle.addUnitToBattleMsgData, addUnitToBattleMsgDataHooked, (void**)&orig.addUnitToBattleMsgData},
        {fn.getUnitAttackSourceImmunities, getUnitAttackSourceImmunitiesHooked},
        {battle.isUnitAttackSourceWardRemoved, isUnitAttackSourceWardRemovedHooked},
        {battle.removeUnitAttackSourceWard, removeUnitAttackSourceWardHooked},
        // Support custom attack reaches
        {battle.fillTargetsList, fillTargetsListHooked},
        {battle.fillEmptyTargetsList, fillEmptyTargetsListHooked},
        {battle.getTargetsToAttack, getTargetsToAttackHooked},
        {battle.findDoppelgangerAttackTarget, findDoppelgangerAttackTargetHooked},
        {battle.findDamageAttackTargetWithNonAllReach, findDamageAttackTargetWithNonAllReachHooked},
        {BattleViewerInterfApi::get().markAttackTargets, markAttackTargetsHooked},
        {fn.isGroupSuitableForAiNobleMisfit, isGroupSuitableForAiNobleMisfitHooked},
        {fn.isUnitSuitableForAiNobleDuel, isUnitSuitableForAiNobleDuelHooked},
        {fn.isAttackBetterThanItemUsage, isAttackBetterThanItemUsageHooked},
        {fn.getSummonUnitImplIdByAttack, getSummonUnitImplIdByAttackHooked},
        {fn.isSmallMeleeFighter, isSmallMeleeFighterHooked},
        {fn.cAiHireUnitEval, cAiHireUnitEvalHooked},
        {fn.getMeleeUnitToHireAiRating, getMeleeUnitToHireAiRatingHooked},
        {fn.computeTargetUnitAiPriority, computeTargetUnitAiPriorityHooked},
        // Allow users to specify critical hit chance
        // Support custom attack damage ratios
        {fn.computeDamage, computeDamageHooked},
        // Fix occasional crash with incorrect removal of summoned unit info
        // Fix persistent crash with summons when unrestrictedBestowWards is enabled
        {battle.removeUnitInfo, removeUnitInfoHooked},
        // Fix bug where transform-self attack is unable to target self if alt attack is targeting allies
        {CBatAttackTransformSelfApi::vftable()->fillTargetsList, transformSelfAttackFillTargetsListHooked},
        // Allow transform self into leveled units using script logic
        // Allow transform-self attack to not consume a unit turn for transformation
        // Fix bug where transform-self attack is unable to target self if alt attack is targeting allies
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackTransformSelfApi::vftable()->onHit, transformSelfAttackOnHitHooked},
        // Allow transform other into leveled units using script logic
        // Fix bug where transform-other attack selects melee vs ranged transform based on attacker position rather than target position
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackTransformOtherApi::vftable()->onHit, transformOtherAttackOnHitHooked},
        // Allow to drain different number of levels using script logic
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackDrainLevelApi::vftable()->onHit, drainLevelAttackOnHitHooked},
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackUntransformEffectApi::vftable()->onHit, untransformEffectAttackOnHitHooked},
        // Fix inability to target self for transformation in case of transform-self + summon attack
        // Remove persistent marking of all target units in case of transform-self attack
        {BattleViewerInterfApi::vftable()->update, battleViewerInterfUpdateHooked},
        // Fix missing modifiers of alternative attacks
        {fn.getUnitAttacks, getUnitAttacksHooked},
        // Support custom event conditions
        {ITestConditionApi::get().create, createTestConditionHooked, (void**)&orig.createTestCondition},
        // Support custom event effects
        //{IEffectResultApi::get().create, createEffectResultHooked, (void**)&orig.createEffectResult},
        // Support additional as well as high tier units in hire list
        {fn.addPlayerUnitsToHireList, addPlayerUnitsToHireListHooked},
        {fn.shouldAddUnitToHire, shouldAddUnitToHireHooked},
        {fn.enableUnitInHireListUi, enableUnitInHireListUiHooked},
        // Allow scenarios with prebuilt buildings in capitals
        // Start with prebuilt temple in capital for warrior lord depending on user setting
        {fn.buildLordSpecificBuildings, buildLordSpecificBuildingsHooked},
        // Allows transformed leaders (doppelganger, drain-level, transform-self/other attacks) to use battle items (potions, orbs and talismans)
        {BattleViewerInterfApi::get().updateBattleItems, battleViewerInterfUpdateBattleItemsHooked},
        {BatBigFaceApi::get().update, batBigFaceUpdateHooked},
        {battle.updateBattleActions, updateBattleActionsHooked},
        // Support race-specific village graphics
        {GameImagesApi::get().getCityPreviewLargeImageNames, getCityPreviewLargeImageNamesHooked, (void**)&orig.getCityPreviewLargeImageNames},
        {GameImagesApi::get().getCityIconImageNames, getCityIconImageNamesHooked, (void**)&orig.getCityIconImageNames},
        // Support grid toggle button
        {CMainView2Api::get().showIsoDialog, mainView2ShowIsoDialogHooked},
        // Reference ground rendering implementation
        // TODO: fix occasional magenta 'triangles' showing up after closing capital window
        //{CGroundTextureApi::vftable()->draw, groundTextureDrawHooked},
        //{CGroundTextureApi::isoEngineVftable()->render, isoEngineGroundRenderHooked},
        // Support native modifiers
        {CMidUnitApi::get().upgrade, upgradeHooked},
        // Fix doppelganger attack using alternative attack when attacker is transformed (by doppelganger, drain-level, transform-self/other attacks)
        {battle.cannotUseDoppelgangerAttack, cannotUseDoppelgangerAttackHooked},
        // Support custom modifiers
        {fn.loadScenarioMap, loadScenarioMapHooked, (void**)&orig.loadScenarioMap},
        // Fix incomplete scenario loading when its object size exceed network message buffer size of 512 KB
        {CMidServerLogicApi::get().sendRefreshInfo, midServerLogicSendRefreshInfoHooked},
    };
    // clang-format on

    if (userSettings().shatteredArmorMax != baseSettings().shatteredArmorMax) {
        // Allow users to customize total armor shatter damage
        hooks.emplace_back(
            HookInfo{CBatAttackShatterApi::vftable()->canPerform, shatterCanPerformHooked});
        hooks.emplace_back(HookInfo{battle.setUnitShatteredArmor, setUnitShatteredArmorHooked});
    }

    if (userSettings().shatterDamageMax != baseSettings().shatterDamageMax) {
        // Allow users to customize maximum armor shatter damage per attack
        hooks.emplace_back(HookInfo{CBatAttackShatterApi::vftable()->onHit, shatterOnHitHooked});
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

    if (userSettings().carryOverItemsMax != baseSettings().carryOverItemsMax) {
        // Change maximum number of items that player can carry between campaign scenarios
        hooks.emplace_back(HookInfo{CDDCarryOverItemsApi::get().constructor,
                                    carryOverItemsCtorHooked, (void**)&orig.carryOverItemsCtor});
    }

    if (userSettings().doppelgangerRespectsEnemyImmunity
            != baseSettings().doppelgangerRespectsEnemyImmunity
        || userSettings().doppelgangerRespectsAllyImmunity
               != baseSettings().doppelgangerRespectsAllyImmunity) {
        // Make Doppelganger attack respect target source/class wards and immunities
        hooks.emplace_back(HookInfo{CBatAttackDoppelgangerApi::vftable()->canPerform,
                                    doppelgangerAttackCanPerformHooked});
        hooks.emplace_back(HookInfo{CBatAttackDoppelgangerApi::vftable()->isImmune,
                                    doppelgangerAttackIsImmuneHooked});
    }

    if (userSettings().leveledDoppelgangerAttack != baseSettings().leveledDoppelgangerAttack) {
        // Allow doppelganger to transform into leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackDoppelgangerApi::vftable()->onHit, doppelgangerAttackOnHitHooked});
    }

    if (userSettings().leveledSummonAttack != baseSettings().leveledSummonAttack) {
        // Allow summon leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackSummonApi::vftable()->onHit, summonAttackOnHitHooked});
    }

    if (userSettings().missChanceSingleRoll != baseSettings().missChanceSingleRoll) {
        // Compute attack miss chance using single random value, instead of two
        hooks.emplace_back(HookInfo{fn.attackShouldMiss, attackShouldMissHooked});
    }

    if (userSettings().unrestrictedBestowWards != baseSettings().unrestrictedBestowWards) {
        // Increases total ward limit for bestow-wards attack from 8 to 48
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

    if (isLobbySupported()) {
        // clang-format off
        // Support new menu windows
        hooks.emplace_back(HookInfo{CMenuPhaseApi::get().setTransition, menuPhaseSetTransitionHooked, (void**)&orig.menuPhaseSetTransition});
        // Support custom lobby server
        hooks.emplace_back(HookInfo{CMenuProtocolApi::get().createMenu, menuProtocolCreateMenuHooked});
        hooks.emplace_back(HookInfo{CMenuProtocolApi::get().continueHandler, menuProtocolContinueHandlerHooked, (void**)&orig.menuProtocolContinueHandler});
        hooks.emplace_back(HookInfo{CMenuProtocolApi::get().displayCallback, menuProtocolDisplayCallbackHooked, (void**)&orig.menuProtocolDisplayCallback});
        hooks.emplace_back(HookInfo{CMenuNewSkirmishApi::get().loadScenarioCallback, menuNewSkirmishLoadScenarioCallbackHooked, (void**)&orig.menuNewSkirmishLoadScenario});
        hooks.emplace_back(HookInfo{CMenuNewSkirmishMultiApi::get().createServer, menuNewSkirmishMultiCreateServerHooked, (void**)&orig.menuNewSkirmishMultiCreateServer});
        hooks.emplace_back(HookInfo{CMenuLoadApi::get().buttonLoadCallback, menuLoadSkirmishMultiLoadScenarioHooked, (void**)&orig.menuLoadSkirmishMultiLoadScenario});
        hooks.emplace_back(HookInfo{CMenuLoadApi::get().createHostPlayer, menuLoadSkirmishMultiCreateHostPlayerHooked, (void**)&orig.menuLoadSkirmishMultiCreateHostPlayer});
        // clang-format on
    }

    if (userSettings().debugMode) {
        // clang-format off
        // Log changed objects ids being send by server to serverLogicSendObjects.log
        if (userSettings().debug.sendObjectsChangesTreshold > 0) {
            hooks.emplace_back(HookInfo{CMidServerLogicApi::vftable().midMsgSender->sendObjectsChanges, midServerLogicSendObjectsChangesHooked, (void**)&orig.midServerLogicSendObjectsChanges});
        }
        // clang-format on
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
        {editorFunctions.countStacksOnMap, countStacksOnMapHooked},
        // Support custom event conditions
        {CMidEvConditionApi::get().getInfoString, eventConditionGetInfoStringHooked, (void**)&orig.eventConditionGetInfoString},
        {CMidEvConditionApi::get().getDescription, eventConditionGetDescriptionHooked, (void**)&orig.eventConditionGetDescription},
        {CMidEvConditionApi::get().getBrief, eventConditionGetBriefHooked, (void**)&orig.eventConditionGetBrief},
        {editor::CCondInterfApi::get().createFromCategory, createCondInterfFromCategoryHooked, (void**)&orig.createCondInterfFromCategory},
        {CMidEventApi::get().checkValid, checkEventValidHooked, (void**)&orig.checkEventValid},
        // Support custom event effects
        //{CMidEvEffectApi::get().getInfoString, eventEffectGetInfoStringHooked, (void**)&orig.eventEffectGetInfoString},
        //{CMidEvEffectApi::get().getDescription, eventEffectGetDescriptionHooked, (void**)&orig.eventEffectGetDescription},
        //{CMidEvEffectApi::get().getBrief, eventEffectGetBriefHooked, (void**)&orig.eventEffectGetBrief},
        //{editor::CEffectInterfApi::get().createFromCategory, createEffectInterfFromCategoryHooked, (void**)&orig.createEffectInterfFromCategory},
        {CMidgardScenarioMapApi::get().checkObjects, checkMapObjectsHooked},
        // Support custom modifiers
        {CMidgardScenarioMapApi::get().stream, scenarioMapStreamHooked, (void**)&orig.scenarioMapStream},
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
    // Support custom attack damage ratios
    // Support per-attack crit settings
    hooks.emplace_back(HookInfo{CAttackImplApi::get().constructor, attackImplCtorHooked});
    hooks.emplace_back(HookInfo{CAttackImplApi::get().constructor2, attackImplCtor2Hooked,
                                (void**)&orig.attackImplCtor2});
    hooks.emplace_back(HookInfo{CAttackImplApi::vftable()->getData, attackImplGetDataHooked,
                                (void**)&orig.attackImplGetData});
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
    hooks.emplace_back(HookInfo{fn.getSoldierImmunityAiRating, getSoldierImmunityAiRatingHooked,
                                (void**)&orig.getSoldierImmunityAiRating});
    hooks.emplace_back(HookInfo{fn.getAttackSourceText, getAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.appendAttackSourceText, appendAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.getAttackSourceWardFlagPosition,
                                getAttackSourceWardFlagPositionHooked,
                                (void**)&orig.getAttackSourceWardFlagPosition});
    // Support custom attack reaches
    hooks.emplace_back(
        HookInfo{LAttackReachTableApi::get().constructor, attackReachTableCtorHooked});
    hooks.emplace_back(HookInfo{fn.getAttackClassAiRating, getAttackClassAiRatingHooked});
    hooks.emplace_back(HookInfo{fn.getAttackReachAiRating, getAttackReachAiRatingHooked});
    hooks.emplace_back(HookInfo{CMidStackApi::vftable()->initialize, midStackInitializeHooked});
    // Always place melee units at the front lane in groups controlled by non-neutrals AI
    // Support custom attack reaches
    hooks.emplace_back(HookInfo{fn.chooseUnitLane, chooseUnitLaneHooked});
    // Fix missing modifiers of alternative attacks
    hooks.emplace_back(HookInfo{CUmAttackApi::get().constructor, umAttackCtorHooked});
    hooks.emplace_back(HookInfo{CUmAttackApi::get().copyConstructor, umAttackCopyCtorHooked});
    // Fix incorrect representation of alternative attack modifiers in unit encyclopedia
    hooks.emplace_back(
        HookInfo{CMidUnitDescriptorApi::get().getAttack, midUnitDescriptorGetAttackHooked});
    hooks.emplace_back(HookInfo{CMidUnitDescriptorApi::vftable()->getAttackInitiative,
                                midUnitDescriptorGetAttackInitiativeHooked});
    // Fix crash in encyclopedia showing info
    // about dyn leveled (stopped) doppleganger that transformed to stack leader
    hooks.emplace_back(HookInfo{CMidUnitDescriptorApi ::vftable()->isUnitLeader,
                                midUnitDescriptorIsUnitLeaderHooked});

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

    if (userSettings().unitEncyclopedia.detailedAttackDescription
        != baseSettings().unitEncyclopedia.detailedAttackDescription) {
        // Additional display of some stats bonuses, drain, critical hit, custom attack ratios, etc.
        hooks.emplace_back(HookInfo{fn.generateAttackDescription, generateAttackDescriptionHooked});
    }

    // Support custom event conditions
    hooks.emplace_back(
        HookInfo{LEventCondCategoryTableApi::get().constructor, eventCondCategoryTableCtorHooked});

    hooks.emplace_back(HookInfo{CMidEvConditionApi::get().createFromCategory,
                                createEventConditionFromCategoryHooked,
                                (void**)&orig.createEventConditionFromCategory});

    // Support custom event effects
    // hooks.emplace_back(HookInfo{LEventEffectCategoryTableApi::get().constructor,
    //                            eventEffectCategoryTableCtorHooked});

    // hooks.emplace_back(HookInfo{CMidEvEffectApi::get().createFromCategory,
    //                            createEventEffectFromCategoryHooked,
    //                            (void**)&orig.createEventEffectFromCategory});

    // Allow every leader use additional animations on strategic map
    hooks.emplace_back(
        HookInfo{fn.isUnitUseAdditionalAnimation, isUnitUseAdditionalAnimationHooked});

    // Support race-specific village graphics
    hooks.emplace_back(
        HookInfo{DisplayHandlersApi::get().villageHandler, displayHandlerVillageHooked});

    if (userSettings().modifiers.cumulativeUnitRegeneration) {
        // Allow unit regeneration modifiers to stack
        hooks.emplace_back(HookInfo{CUmUnitApi::get().constructor, umUnitCtorHooked});
        hooks.emplace_back(HookInfo{CUmUnitApi::get().copyConstructor, umUnitCopyCtorHooked});
        hooks.emplace_back(
            HookInfo{CUmUnitApi::vftable().usSoldier->getRegen, umUnitGetRegenHooked});
    }

    // Support custom modifiers
    hooks.emplace_back(HookInfo{LModifGroupTableApi::get().constructor, modifGroupTableCtorHooked});
    hooks.emplace_back(HookInfo{TUnitModifierApi::get().constructor, unitModifierCtorHooked});
    hooks.emplace_back(HookInfo{TUnitModifierApi::vftable()->destructor, unitModifierDtorHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().addModifier, addModifierHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::vftable()->stream, midUnitStreamHooked});

    // Support custom modifiers display for unit encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().constructor, encLayoutUnitCtorHooked});
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().constructor2, encLayoutUnitCtor2Hooked});
    hooks.emplace_back(
        HookInfo{CEncLayoutUnitApi::get().dataConstructor, encLayoutUnitDataCtorHooked});
    hooks.emplace_back(
        HookInfo{CEncLayoutUnitApi::get().dataDestructor, encLayoutUnitDataDtorHooked});
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().initialize, encLayoutUnitInitializeHooked,
                                (void**)&orig.encLayoutUnitInitialize});

    // Show effective HP in unit encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().update, encLayoutUnitUpdateHooked,
                                (void**)&orig.encLayoutUnitUpdate});

    // Show total xp-killed in stack/city/ruin encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutStackApi::get().update, encLayoutStackUpdateHooked,
                                (void**)&orig.encLayoutStackUpdate});
    hooks.emplace_back(HookInfo{CEncLayoutCityApi::get().update, encLayoutCityUpdateHooked,
                                (void**)&orig.encLayoutCityUpdate});
    hooks.emplace_back(HookInfo{CEncLayoutRuinApi::get().update, encLayoutRuinUpdateHooked,
                                (void**)&orig.encLayoutRuinUpdate});

    // Support native modifiers
    hooks.emplace_back(HookInfo{CMidUnitApi::get().getModifiers, getModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().addModifiers, addModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().removeModifiers, removeModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::vftable()->initWithSoldierImpl,
                                initWithSoldierImplHooked, (void**)&orig.initWithSoldierImpl});

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

game::DialogScriptData* __fastcall loadScriptFileHooked(game::DialogScriptData* thisptr,
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

static void menuNewSkirmishCtor(game::CMenuNewSkirmish* thisptr,
                                game::CMenuPhase* menuPhase,
                                const char* dialogName)
{
    using namespace game;

    const auto& menuBase = CMenuBaseApi::get();
    menuBase.constructor(thisptr, menuPhase);
    thisptr->vftable = (game::CInterfaceVftable*)CMenuNewSkirmishApi::vftable();
    menuBase.createMenu(thisptr, dialogName);

    const auto dialog = menuBase.getDialogInterface(thisptr);
    const auto& menu = CMenuNewSkirmishApi::get();
    const auto& button = CButtonInterfApi::get();
    const auto freeFunctor = SmartPointerApi::get().createOrFreeNoDtor;
    SmartPointer functor;

    menuBase.createButtonFunctor(&functor, 0, thisptr, &menuBase.buttonBackCallback);
    button.assignFunctor(dialog, "BTN_BACK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    menuBase.createButtonFunctor(&functor, 0, thisptr, &menu.loadScenarioCallback);
    CButtonInterf* loadButton = button.assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    void* callback = buttonGenerateMapCallback;
    menuBase.createButtonFunctor(&functor, 0, thisptr,
                                 (CMenuBaseApi::Api::ButtonCallback*)&callback);
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
        loadButton->vftable->setEnabled(loadButton, false);
        return;
    }

    CListBoxInterf* lBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_GAME_SLOT");
    listBox.setElementsTotal(lBox, scenariosTotal);
    menu.updateScenarioUi(unknown, dialog, listBox.selectedIndex(lBox));
}

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /*%edx*/,
    game::CMenuPhase* menuPhase)
{
    menuNewSkirmishCtor(thisptr, menuPhase, "DLG_CHOOSE_SKIRMISH");
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
    list.clear(hireList);

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

    auto playerBuildings = (CPlayerBuildings*)dynamicCast(buildingsObject, 0,
                                                          rtti.IMidScenarioObjectType,
                                                          rtti.CPlayerBuildingsType, 0);
    if (!playerBuildings) {
        logError("mssProxyError.log", fmt::format("Object with id {:x} is not player buildings",
                                                  player->buildingsId.value));
        return false;
    }

    const auto& global = GlobalDataApi::get();
    const auto globalData = *global.getGlobalData();
    auto races = globalData->races;
    TRaceType* race = (TRaceType*)global.findById(races, &player->raceId);

    const auto& fn = gameFunctions();

    const auto& addUnitToHireList = fn.addUnitToHireList;
    const auto& unitBranch = UnitBranchCategories::get();

    addUnitToHireList(race, playerBuildings, unitBranch.fighter, hireList);
    addUnitToHireList(race, playerBuildings, unitBranch.archer, hireList);
    addUnitToHireList(race, playerBuildings, unitBranch.mage, hireList);
    addUnitToHireList(race, playerBuildings, unitBranch.special, hireList);

    fn.addSideshowUnitToHireList(race, playerBuildings, hireList);

    if (!unitsForHire().empty()) {
        const int raceIndex = id.getTypeIndex(&player->raceId);

        const auto& units = unitsForHire()[raceIndex];
        for (const auto& unit : units) {
            list.pushBack(hireList, &unit);
        }
    }

    const auto& buildList = playerBuildings->buildings;
    if (buildList.length == 0) {
        // Player has no buildings in capital, skip
        return true;
    }

    auto variables{getScenarioVariables(dataCache)};
    if (!variables || !variables->variables.length) {
        return true;
    }

    int hireTierMax{0};
    for (const auto& variable : variables->variables) {
        static const char varName[]{"UNIT_HIRE_TIER_MAX"};
        if (!strncmp(variable.second.name, varName, sizeof(varName))) {
            hireTierMax = variable.second.value;
            break;
        }
    }

    if (hireTierMax <= 1) {
        // No variable defined or high tier hire is disabled, skip.
        return true;
    }

    const auto units = *globalData->units;
    for (auto current = units->data.bgn, end = units->data.end; current != end; ++current) {
        const auto unitImpl = current->second;
        auto soldier = fn.castUnitImplToSoldier(unitImpl);
        if (!soldier) {
            continue;
        }

        if (race->id != *soldier->vftable->getRaceId(soldier)) {
            continue;
        }

        auto racialSoldier = fn.castUnitImplToRacialSoldier(unitImpl);
        if (!racialSoldier) {
            continue;
        }

        auto upgradeBuildingId = racialSoldier->vftable->getUpgradeBuildingId(racialSoldier);
        if (!upgradeBuildingId) {
            continue;
        }

        bool hasBulding{false};
        for (auto node = buildList.head->next; node != buildList.head; node = node->next) {
            if (node->data == *upgradeBuildingId) {
                hasBulding = true;
                break;
            }
        }

        if (!hasBulding) {
            // No building for this unit was built in capital, skip
            continue;
        }

        auto buildingType = (const TBuildingType*)global.findById(globalData->buildings,
                                                                  upgradeBuildingId);
        if (!buildingType) {
            continue;
        }

        auto upgBuilding = (const TBuildingUnitUpgType*)
            dynamicCast(buildingType, 0, rtti.TBuildingTypeType, rtti.TBuildingUnitUpgTypeType, 0);
        if (!upgBuilding) {
            continue;
        }

        if (upgBuilding->level <= hireTierMax) {
            list.pushBack(hireList, &current->first);
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
        db.duplicateRecordException(dbTable, &buildingType->id);
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

    const auto dbfFilePath{std::filesystem::path(globalsFolderPath) / dbfFileName};
    customCategoryExists = utils::dbValueExists(dbfFilePath, "TEXT", "L_CUSTOM");
    if (customCategoryExists)
        logDebug("newBuildingType.log", "Found custom building category");

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

    buildingBranch.initBranchMap(&thisptr->data->map);
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

    const auto globalData = GlobalDataApi::get().getGlobalData();
    auto buildings = (*globalData)->buildings;

    for (const auto& id : buildList->data) {
        const auto findById = GlobalDataApi::get().findById;
        const TBuildingType* buildingType = (const TBuildingType*)findById(buildings, &id);

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
                buildingBranch.addSideshowUnitBuilding(&thisptr->data->map, unitUpg);
            } else if (unitBranch.id == unitBranchCategories.fighter->id && num == 0
                       || unitBranch.id == unitBranchCategories.mage->id && num == 1
                       || unitBranch.id == unitBranchCategories.archer->id && num == 2
                       || unitBranch.id == unitBranchCategories.special->id && num == 4) {
                buildingBranch.addUnitBuilding(phaseGame, &thisptr->data->map, unitUpg);
            }

        } else if ((buildingCategory->id == buildingCategories.guild->id
                    || buildingCategory->id == buildingCategories.heal->id
                    || buildingCategory->id == buildingCategories.magic->id
                    || (customCategoryExists && (buildingCategory->id == custom.id)))
                   && *branchNumber == 3) {
            buildingBranch.addBuilding(phaseGame, &thisptr->data->map, buildingType);
        }
    }

    logDebug("newBuildingType.log", "Ctor finished");
    return thisptr;
}

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    // Place units with adjacent attack reach at the front lane, despite of their attack class
    IAttack* attack = soldier->vftable->getAttackById(soldier);
    return isMeleeAttack(attack) ? 1 : 0;
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

    *playerId = player ? player->id : emptyId;

    return playerId;
}

bool __fastcall giveAttackCanPerformHooked(game::CBatAttackGiveAttack* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetGroupId{};
    thisptr->vftable->getTargetGroupId(thisptr, &targetGroupId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    if (targetGroupId != unitGroupId) {
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

    CMidgardID targetGroupId{};
    thisptr->vftable->getTargetGroupId(thisptr, &targetGroupId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    if (targetGroupId != unitGroupId) {
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
    info.unitImplId = unit->unitImpl->id;
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

bool __stdcall buildLordSpecificBuildingsHooked(game::IMidgardObjectMap* objectMap,
                                                const game::NetPlayerInfo* playerInfo,
                                                int)
{
    using namespace game;

    auto playerObj = objectMap->vftable->findScenarioObjectById(objectMap, &playerInfo->playerId);

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    auto player = (const CMidPlayer*)dynamicCast(playerObj, 0, rtti.IMidScenarioObjectType,
                                                 rtti.CMidPlayerType, 0);

    auto& fn = gameFunctions();
    if (!userSettings().preserveCapitalBuildings) {
        fn.deletePlayerBuildings(objectMap, player);
    }

    auto lordType = fn.getLordByPlayer(player);
    const auto lordCategoryId{lordType->data->lordCategory.id};

    const auto& lordCategories = LordCategories::get();
    const auto& buildingCategories = BuildingCategories::get();

    if (lordCategoryId == lordCategories.diplomat->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.guild);
    }

    if (lordCategoryId == lordCategories.mage->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.magic);
    }

    if (userSettings().buildTempleForWarriorLord && lordCategoryId == lordCategories.warrior->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.heal);
    }

    return true;
}

game::CEncLayoutSpell* __fastcall encLayoutSpellCtorHooked(game::CEncLayoutSpell* thisptr,
                                                           int /*%edx*/,
                                                           game::IMidgardObjectMap* objectMap,
                                                           game::CInterface* interface,
                                                           void* a2,
                                                           game::CMidgardID* spellId,
                                                           game::CEncParamBase* encParam,
                                                           const game::CMidgardID* playerId)
{
    using namespace game;

    if (!playerId) {
        forEachScenarioObject(objectMap, IdType::Player,
                              [&playerId](const IMidScenarioObject* obj) {
                                  // Use id of the first found player to show spell info
                                  if (!playerId) {
                                      playerId = &obj->id;
                                  }
                              });
    }

    // Show spell price and casting cost
    encParam->data->statuses = 4;
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
                                  const game::IAttack* attack,
                                  const game::CMidgardID* attackerUnitId,
                                  const game::CMidgardID* targetUnitId,
                                  bool computeCriticalHit,
                                  int* attackDamage,
                                  int* criticalHitDamage)
{
    using namespace game;

    int armor;
    const auto& fn = gameFunctions();
    fn.computeArmor(&armor, objectMap, battleMsgData, targetUnitId);

    bool isEasyDifficulty = false;
    auto player = getPlayer(objectMap, battleMsgData, attackerUnitId);
    if (player && player->isHuman) {
        const auto& difficulties = DifficultyLevelCategories::get();
        isEasyDifficulty = getScenarioInfo(objectMap)->gameDifficulty.id == difficulties.easy->id;
    }

    int damageMax = fn.computeDamageMax(objectMap, attackerUnitId);
    int damageWithBuffs = fn.computeDamageWithBuffs(attack, damageMax, battleMsgData,
                                                    attackerUnitId, true, isEasyDifficulty);
    int damage = damageWithBuffs * (100 - armor) / 100;

    int critDamage = 0;
    if (computeCriticalHit) {
        bool critHit = attack->vftable->getCritHit(attack);
        if (!critHit) {
            auto unit = fn.findUnitById(objectMap, attackerUnitId);
            critHit = hasCriticalHitLeaderAbility(unit->unitImpl);
        }

        if (critHit) {
            auto customData = getCustomAttackData(attack);
            int critPower = customData.critPower;
            if (!fn.attackShouldMiss(&critPower)) {
                critDamage = damageWithBuffs * customData.critDamage / 100;
            }
        }
    }

    if (getCustomAttacks().damageRatiosEnabled) {
        auto& damageRatios = getCustomDamageRatios(attack);
        auto ratio = damageRatios.find(*targetUnitId);
        if (ratio != damageRatios.end()) {
            damage = applyAttackDamageRatio(damage, ratio->second);
            critDamage = applyAttackDamageRatio(critDamage, ratio->second);
        }
    }

    if (attackDamage)
        *attackDamage = damage;
    if (criticalHitDamage)
        *criticalHitDamage = critDamage;
    return damage + critDamage;
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

    const auto& restrictions = game::gameRestrictions();

    const auto attackClass = attack->vftable->getAttackClass(attack);

    if (!attackHasPower(attackClass->id)) {
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
        const auto& difficulties = DifficultyLevelCategories::get();
        const auto difficultyId = getScenarioInfo(objectMap)->gameDifficulty.id;

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

        tmpPower = std::clamp(tmpPower, restrictions.attackPower->min,
                              restrictions.attackPower->max);
    }

    const auto& attacks = AttackClassCategories::get();
    if (battleMsgData->currentRound > userSettings().disableAllowedRoundMax
        && (attackClass->id == attacks.paralyze->id || attackClass->id == attacks.petrify->id)) {
        tmpPower = 0;
    }

    tmpPower -= battle.getAttackPowerReduction(battleMsgData, unitId);
    *power = std::clamp(tmpPower, restrictions.attackPower->min, restrictions.attackPower->max);
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

static game::CMidgardID currUnitId = game::emptyId;
void __stdcall afterBattleTurnHooked(game::BattleMsgData* battleMsgData,
                                     const game::CMidgardID* unitId,
                                     const game::CMidgardID* nextUnitId)
{
    using namespace game;

    if (*unitId != *nextUnitId) {
        battleMsgData->unknown9 |= 2;
        BattleMsgDataApi::get().removeFiniteBoostLowerDamage(battleMsgData, unitId);
    }

    currUnitId = *unitId;
}

void __stdcall beforeBattleTurnHooked(game::BattleMsgData* battleMsgData,
                                      game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    battle.setUnitStatus(battleMsgData, unitId, BattleStatus::Defend, false);

    // Fix bestow wards with double attack where modifiers granted by first attack are removed
    if (*unitId != currUnitId) {
        auto unitInfo = battle.getUnitInfoById(battleMsgData, unitId);
        auto modifiedUnitIds = getModifiedUnitIds(unitInfo);
        for (auto it = modifiedUnitIds.begin(); it != modifiedUnitIds.end(); it++)
            removeModifiers(battleMsgData, objectMap, unitInfo, &(*it));
        resetModifiedUnitsInfo(unitInfo);
    }

    battle.setAttackPowerReduction(battleMsgData, unitId, 0);

    getCustomAttacks().targets.clear();
    getCustomAttacks().damageRatios.clear();

    auto& freeTransformSelf = getCustomAttacks().freeTransformSelf;
    if (freeTransformSelf.unitId != *unitId) {
        freeTransformSelf.unitId = *unitId;
        freeTransformSelf.turnCount = 0;
        freeTransformSelf.used = false;
    } else if (freeTransformSelf.used) {
        // Fix free transform-self to disable Wait/Defend/Retreat
        auto unitInfo = battle.getUnitInfoById(battleMsgData, unitId);
        if (unitInfo)
            unitInfo->unitFlags.parts.attackedOnceOfTwice = 1;
    }
    freeTransformSelf.turnCount++;
}

void __stdcall throwExceptionHooked(const game::os_exception* thisptr, const void* throwInfo)
{
    if (thisptr && thisptr->message) {
        showErrorMessageBox(fmt::format("Caught exception '{:s}'.\n"
                                        "The {:s} will probably crash now.",
                                        thisptr->message, executableIsGame() ? "game" : "editor"));
    }

    getOriginalFunctions().throwException(thisptr, throwInfo);
}

int __stdcall computeUnitEffectiveHpForAiHooked(const game::IMidgardObjectMap* objectMap,
                                                const game::CMidUnit* unit,
                                                const game::BattleMsgData* battleMsgData)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!unit)
        return 0;

    int armor;
    fn.computeArmor(&armor, objectMap, battleMsgData, &unit->id);

    return computeUnitEffectiveHpForAi(unit->currentHp, armor);
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
        float ratio = 1.0;
        if (attackData->attackClass->id == AttackClassCategories::get().shatter->id)
            ratio = (float)userSettings().shatterDamageUpgradeRatio / 100;

        if (upgrade1)
            attackData->qtyDamage += lround(upgrade1->damage * upgrade1Count * ratio);
        if (upgrade2)
            attackData->qtyDamage += lround(upgrade2->damage * upgrade2Count * ratio);
    }

    if (attackData->qtyHeal > 0) {
        if (upgrade1)
            attackData->qtyHeal += upgrade1->heal * upgrade1Count;
        if (upgrade2)
            attackData->qtyHeal += upgrade2->heal * upgrade2Count;
    }
}

void __stdcall getUnitAttacksHooked(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* unitId,
                                    game::AttackTypePairVector* value,
                                    bool checkAltAttack)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& vectorApi = AttackTypePairVectorApi::get();

    auto unit = fn.findUnitById(objectMap, unitId);

    auto attack = getAttack(unit->unitImpl, true, checkAltAttack);
    AttackTypePair pair{attack, AttackType::Primary};
    vectorApi.pushBack(value, &pair);

    auto attack2 = getAttack(unit->unitImpl, false, checkAltAttack);
    if (attack2) {
        AttackTypePair pair{attack2, AttackType::Secondary};
        vectorApi.pushBack(value, &pair);
    }

    auto item1Attack = fn.getItemAttack(objectMap, unitId, 1);
    if (item1Attack) {
        AttackTypePair pair{item1Attack, AttackType::Item};
        vectorApi.pushBack(value, &pair);
    }

    auto item2Attack = fn.getItemAttack(objectMap, unitId, 2);
    if (item2Attack) {
        AttackTypePair pair{item2Attack, AttackType::Item};
        vectorApi.pushBack(value, &pair);
    }
}

bool __stdcall isUnitUseAdditionalAnimationHooked(const game::CMidgardID*)
{
    return true;
}

bool __stdcall shouldAddUnitToHireHooked(const game::CMidPlayer* player,
                                         game::CPhaseGame* phaseGame,
                                         const game::CMidgardID* unitImplId)
{
    return true;
}

bool __stdcall enableUnitInHireListUiHooked(const game::CMidPlayer* player,
                                            game::CPhaseGame* phaseGame,
                                            const game::CMidgardID* unitImplId)
{
    using namespace game;

    auto objectMap = CPhaseApi::get().getObjectMap(&phaseGame->phase);

    auto buildingsObject = objectMap->vftable->findScenarioObjectById(objectMap,
                                                                      &player->buildingsId);
    if (!buildingsObject) {
        return false;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto playerBuildings = (CPlayerBuildings*)dynamicCast(buildingsObject, 0,
                                                          rtti.IMidScenarioObjectType,
                                                          rtti.CPlayerBuildingsType, 0);
    if (!playerBuildings) {
        return false;
    }

    const auto& global = GlobalDataApi::get();
    const auto globalData = *global.getGlobalData();

    auto unitImpl = (const TUsUnitImpl*)global.findById(globalData->units, unitImplId);
    if (!unitImpl) {
        return false;
    }

    auto racialSoldier = gameFunctions().castUnitImplToRacialSoldier(unitImpl);
    if (!racialSoldier) {
        return false;
    }

    auto soldier = gameFunctions().castUnitImplToSoldier(unitImpl);
    if (!soldier) {
        return false;
    }

    auto enrollBuildingId = racialSoldier->vftable->getEnrollBuildingId(racialSoldier);

    // Starting units are always allowed
    if (soldier->vftable->getLevel(soldier) == 1 && *enrollBuildingId == emptyId) {
        return true;
    }

    // If unit has enroll bulding requirement, check it
    if (*enrollBuildingId != emptyId) {
        const auto& buildList = playerBuildings->buildings;
        for (auto node = buildList.head->next; node != buildList.head; node = node->next) {
            if (node->data == *enrollBuildingId) {
                return true;
            }
        }
        return false;
    }

    // High tier units have only upgrade building requirement:
    // upgrade building is required for units from previous tier to promote
    auto upgradeBuildingId = racialSoldier->vftable->getUpgradeBuildingId(racialSoldier);
    if (*upgradeBuildingId != emptyId) {
        const auto& buildList = playerBuildings->buildings;
        for (auto node = buildList.head->next; node != buildList.head; node = node->next) {
            if (node->data == *upgradeBuildingId) {
                return true;
            }
        }
    }

    return false;
}

void __stdcall getCityPreviewLargeImageNamesHooked(game::List<game::String>* imageNames,
                                                   const void* cityFF,
                                                   const game::LRaceCategory* race,
                                                   int cityTier)
{
    using namespace game;

    const auto& races{RaceCategories::get()};
    const auto raceId{race->id};

    if (cityTier == 0 || raceId == races.neutral->id) {
        getOriginalFunctions().getCityPreviewLargeImageNames(imageNames, cityFF, race, cityTier);
        return;
    }

    char raceSuffix{'?'};
    if (raceId == races.human->id) {
        raceSuffix = 'H';
    } else if (raceId == races.undead->id) {
        raceSuffix = 'U';
    } else if (raceId == races.heretic->id) {
        raceSuffix = 'E';
    } else if (raceId == races.dwarf->id) {
        raceSuffix = 'D';
    } else if (raceId == races.elf->id) {
        raceSuffix = 'F';
    }

    const auto name{fmt::format("ALN{:c}{:d}", raceSuffix, cityTier)};
    const auto length{name.length()};

    GameImagesApi::get().getImageNames(imageNames, cityFF, name.c_str(), length, length);

    if (!imageNames->length) {
        // Fallback to default city image names
        getOriginalFunctions().getCityPreviewLargeImageNames(imageNames, cityFF, race, cityTier);
    }
}

void __stdcall getCityIconImageNamesHooked(game::List<game::String>* imageNames,
                                           const void* iconsFF,
                                           const game::CMidgardID* fortificationId,
                                           const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, fortificationId)};
    if (!obj) {
        return;
    }

    auto fortification{static_cast<const CFortification*>(obj)};
    auto vftable{static_cast<const CFortificationVftable*>(fortification->vftable)};

    auto category{vftable->getCategory(fortification)};

    if (category->id != FortCategories::get().village->id) {
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
        return;
    }

    auto village{static_cast<const CMidVillage*>(fortification)};

    const auto& races{RaceCategories::get()};
    const auto neutralRace{races.neutral};
    const LRaceCategory* ownerRace{neutralRace};

    if (village->ownerId != emptyId) {
        auto player{getPlayer(objectMap, &village->ownerId)};
        if (player) {
            ownerRace = &player->raceType->data->raceType;
        }
    }

    const auto ownerRaceId{ownerRace->id};
    if (ownerRaceId == neutralRace->id) {
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
        return;
    }

    const char* raceSuffix{"HU"};
    if (ownerRaceId == races.human->id) {
        raceSuffix = "HU";
    } else if (ownerRaceId == races.heretic->id) {
        raceSuffix = "HE";
    } else if (ownerRaceId == races.undead->id) {
        raceSuffix = "UN";
    } else if (ownerRaceId == races.dwarf->id) {
        raceSuffix = "DW";
    } else if (ownerRaceId == races.elf->id) {
        raceSuffix = "EL";
    }

    const char tierLetter{'0' + static_cast<char>(village->tierLevel)};
    const auto name{fmt::format("CITY{:s}{:c}", raceSuffix, tierLetter)};
    const auto length{name.length()};

    GameImagesApi::get().getImageNames(imageNames, iconsFF, name.c_str(), length, length);

    if (!imageNames->length) {
        // Fallback to default city icon image names
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
    }
}

bool __fastcall checkMapObjectsHooked(game::CMidgardScenarioMap* scenarioMap, int /*%edx*/)
{
    using namespace game;

    std::memset(scenarioMap->freeIdTypeIndices, 0, sizeof(scenarioMap->freeIdTypeIndices));

    const auto& api{CMidgardScenarioMapApi::get()};

    ScenarioMapDataIterator current{};
    api.begin(scenarioMap, &current);

    ScenarioMapDataIterator end{};
    api.end(scenarioMap, &end);

    const auto& idApi{CMidgardIDApi::get()};

    while (current.foundRecord != end.foundRecord) {
        const auto* objectId{&current.foundRecord->objectId};

        const auto type{static_cast<int>(idApi.getType(objectId))};
        const auto typeIndex{idApi.getTypeIndex(objectId)};

        if (scenarioMap->freeIdTypeIndices[type] <= typeIndex) {
            scenarioMap->freeIdTypeIndices[type] = typeIndex + 1;
        }

        const auto* object{current.foundRecord->object.data};
        if (!object->vftable->isValid(object, scenarioMap)) {
            logError("mssProxyError.log",
                     fmt::format("Scenario object {:s} is invalid", idToString(objectId)));
            return false;
        }

        api.advance(&current);
    }

    return true;
}

void validateUnits(game::CMidgardScenarioMap* scenarioMap)
{
    using namespace game;

    const auto& fn = gameFunctions();

    forEachScenarioObject(scenarioMap, IdType::Unit, [&fn](const IMidScenarioObject* obj) {
        auto unit = (CMidUnit*)obj;

        IUsUnit* origImpl = unit->transformed ? hooks::getUnitImpl(&unit->origTypeId)
                                              : unit->unitImpl;
        auto soldier = fn.castUnitImplToSoldier(origImpl);

        unit->currentXp = std::clamp(unit->currentXp, 0, soldier->vftable->getXpNext(soldier));
        unit->currentHp = std::clamp(unit->currentHp, 0, getUnitHpMax(unit));
    });
}

int __stdcall loadScenarioMapHooked(int a1,
                                    game::CMidStreamEnvFile* streamEnv,
                                    game::CMidgardScenarioMap* scenarioMap)
{
    int result = getOriginalFunctions().loadScenarioMap(a1, streamEnv, scenarioMap);

    validateUnits(scenarioMap);

    return result;
}

bool __fastcall scenarioMapStreamHooked(game::CMidgardScenarioMap* scenarioMap,
                                        int /*%edx*/,
                                        game::IMidgardStreamEnv* streamEnv)
{
    bool result = getOriginalFunctions().scenarioMapStream(scenarioMap, streamEnv);
    if (result) {
        validateUnits(scenarioMap);
    }

    return result;
}

} // namespace hooks
