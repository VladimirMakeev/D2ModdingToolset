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

#include "battleviewerinterfhooks.h"
#include "attack.h"
#include "batbigface.h"
#include "batimagesloader.h"
#include "battleviewerinterf.h"
#include "batunitanim.h"
#include "batviewer2dengine.h"
#include "button.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "d2string.h"
#include "dialoginterf.h"
#include "functor.h"
#include "game.h"
#include "mempool.h"
#include "middragdropinterf.h"
#include "midgardobjectmap.h"
#include "miditem.h"
#include "midunitgroup.h"
#include "musicfader.h"
#include "togglebutton.h"
#include "uievent.h"
#include "uimanager.h"
#include "unitinfolist.h"
#include "unitpositionlist.h"
#include "unitslotview.h"
#include "unitutils.h"

namespace hooks {

void markAttackTarget(game::CBattleViewerInterf* viewer,
                      const game::CMidgardID* targetId,
                      bool targetIsAttacker,
                      int targetPosition)
{
    using namespace game;

    const auto& viewerApi = BattleViewerInterfApi::get();
    const auto& rectApi = MqRectApi::get();
    const auto& loaderApi = BatImagesLoaderApi::get();
    const auto& engineApi = BatViewer2DEngineApi::get();

    bool isUnitBig = false;
    bool isUnitRetreating = false;
    if (*targetId != emptyId) {
        isUnitBig = viewerApi.isUnitBig(&viewer->data->unknownUnitData, targetId);
        isUnitRetreating = viewerApi.isUnitRetreating(&viewer->data->unknownUnitData, targetId);
    }

    CMqRect unitRect{};
    bool isUnitOnTheLeft = viewerApi.isUnitOnTheLeft(viewer, targetIsAttacker);
    viewerApi.getUnitRect(&unitRect,
                          isUnitOnTheLeft ? &viewer->data->groupAreas2 : &viewer->data->groupAreas1,
                          targetPosition, isUnitBig, isUnitRetreating);

    IMqImage2* image = loaderApi.loadMarkImage(viewer->data->imagesLoader, isUnitBig);
    if (image == nullptr)
        return;

    CMqPoint imageSize{};
    image->vftable->getSize(image, &imageSize);

    CMqPoint unitCenter{};
    rectApi.getCenter(&unitRect, &unitCenter);

    CMqPoint imagePosition{unitCenter.x - imageSize.x / 2, unitRect.p2.y - imageSize.y / 2};
    engineApi.drawTargetMark(viewer->data->batViewer2dEngine, image, &imagePosition,
                             isUnitOnTheLeft, targetPosition);
}

void markAllAttackTargets(game::CBattleViewerInterf* viewer,
                          const game::CMidUnitGroup* targetGroup,
                          const game::UnitInfo* targetInfo,
                          const game::UnitPositionMap* targetPositions)
{
    using namespace game;

    const auto& groupApi = CMidUnitGroupApi::get();

    for (const auto& position : *targetPositions) {
        if (position.first >= 0) {
            const CMidgardID* targetId = groupApi.getUnitIdByPosition(targetGroup, position.first);
            markAttackTarget(viewer, targetId, targetInfo->unitFlags.parts.attacker,
                             position.first);
        }
    }
}

void markCustomAttackTargets(game::CBattleViewerInterf* viewer,
                             const game::CMidgardID* targetGroupId,
                             const game::CMidgardID* selectedUnitId,
                             const game::UnitInfo* targetInfo,
                             const game::IAttack* attack,
                             bool isItemAttack,
                             const CustomAttackReach& attackReach)
{
    using namespace game;

    const auto& fn = gameFunctions();

    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, &viewer->data->battleMsgData, &viewer->data->unitId,
                             true);

    CMidgardID attackUnitId = isItemAttack ? viewer->data->itemId : viewer->data->unitId;
    auto targets = getTargetsToAttackForCustomAttackReach(viewer->data->objectMap,
                                                          &viewer->data->battleMsgData, attack,
                                                          targetGroupId, selectedUnitId,
                                                          &unitGroupId, &viewer->data->unitId,
                                                          &attackUnitId, attackReach);
    for (const auto& target : targets) {
        CMidgardID targetId = target.getUnitId();
        markAttackTarget(viewer, &targetId, targetInfo->unitFlags.parts.attacker,
                         target.getPosition());
    }
}

bool isUnitSelected(game::CBattleViewerInterf* viewer,
                    const game::CMqPoint* mousePosition,
                    const game::UnitInfo* targetInfo,
                    game::CMidgardID* targetId)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    const auto& battle = BattleMsgDataApi::get();
    const auto& rectApi = MqRectApi::get();
    const auto& viewerApi = BattleViewerInterfApi::get();

    id.validateId(targetId, targetInfo->unitId1);

    if (battle.getUnitStatus(&viewer->data->battleMsgData, targetId, BattleStatus::Dead)
        || battle.getUnitStatus(&viewer->data->battleMsgData, targetId, BattleStatus::Retreated)
        || battle.getUnitStatus(&viewer->data->battleMsgData, targetId, BattleStatus::Hidden)) {
        return false;
    }

    bool isUnitBig = viewerApi.isUnitBig(&viewer->data->unknownUnitData, targetId);
    bool isUnitRetreating = viewerApi.isUnitRetreating(&viewer->data->unknownUnitData, targetId);
    bool isUnitOnTheLeft = viewerApi.isUnitOnTheLeft(viewer, targetInfo->unitFlags.parts.attacker);

    CMqRect unitRect{};
    viewerApi.getUnitRectPlusExtra(&unitRect,
                                   isUnitOnTheLeft ? &viewer->data->groupAreas2
                                                   : &viewer->data->groupAreas1,
                                   targetInfo->unitFlags.parts.indexInGroup, isUnitBig,
                                   isUnitRetreating);

    return rectApi.ptInRect(&unitRect, mousePosition);
}

bool markAttackTargetsForAllOrCustomAttackReach(game::CBattleViewerInterf* viewer,
                                                const game::CMidgardID* targetGroupId,
                                                const game::CMidgardID* selectedUnitId,
                                                const game::UnitInfo* targetInfo,
                                                const game::IAttack* attack,
                                                bool isBattleGoing,
                                                bool isItemAttack,
                                                const game::UnitPositionMap* targetPositions)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& mapApi = UnitPositionMapApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();
    const auto& reaches = AttackReachCategories::get();

    if (!isBattleGoing || attack == nullptr)
        return false;

    void* tmp{};
    CMidUnitGroup* targetGroup = fn.getStackFortRuinGroup(tmp, viewer->data->objectMap,
                                                          targetGroupId);

    int selectedPositionInTargetGroup = groupApi.getUnitPosition(targetGroup, selectedUnitId);
    if (selectedPositionInTargetGroup == -1)
        return false;

    UnitPositionPair positionPair;
    mapApi.findByPosition(&positionPair, targetPositions, targetInfo->unitFlags.parts.indexInGroup);
    if (!positionPair.second)
        return false;

    auto attackReach = attack->vftable->getAttackReach(attack);
    if (attackReach->id == reaches.all->id) {
        markAllAttackTargets(viewer, targetGroup, targetInfo, targetPositions);
        return true;
    } else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (attackReach->id == custom.reach.id) {
                if (!custom.markAttackTargets)
                    break;
                markCustomAttackTargets(viewer, targetGroupId, selectedUnitId, targetInfo, attack,
                                        isItemAttack, custom);
                return true;
            }
        }
    }

    return false;
}

void markAttackTargets(game::CBattleViewerInterf* viewer,
                       const game::CMidgardID* targetGroupId,
                       const game::CMidgardID* selectedUnitId,
                       const game::UnitInfo* targetInfo,
                       bool setBigFace,
                       const game::IAttack* attack,
                       bool isBattleGoing,
                       bool isItemAttack,
                       const game::UnitPositionMap* targetPositions)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& viewerApi = BattleViewerInterfApi::get();
    const auto& engineApi = BatViewer2DEngineApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();

    if (setBigFace) {
        CBatBigFace* bigFace = viewerApi.getBigFace(viewer);
        bigFaceApi.setUnitId(bigFace, selectedUnitId);
    }

    if (viewer->data2->selectedUnitId == *selectedUnitId)
        return;
    engineApi.clearTargetMarks(viewer->data->batViewer2dEngine);
    viewer->data2->selectedUnitId = *selectedUnitId;

    if (!markAttackTargetsForAllOrCustomAttackReach(viewer, targetGroupId, selectedUnitId,
                                                    targetInfo, attack, isBattleGoing, isItemAttack,
                                                    targetPositions)) {
        markAttackTarget(viewer, selectedUnitId, targetInfo->unitFlags.parts.attacker,
                         targetInfo->unitFlags.parts.indexInGroup);
    }
}

bool markAttackTargetsIfUnitSelected(game::CBattleViewerInterf* viewer,
                                     const game::CMqPoint* mousePosition,
                                     bool setBigFace,
                                     const game::IAttack* attack,
                                     bool isBattleGoing,
                                     bool isItemAttack,
                                     const game::CMidgardID* targetGroupId,
                                     const game::UnitPositionMap* targetPositions)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    const auto& listApi = UnitInfoListApi::get();

    UnitInfoList targetInfos{};
    listApi.constructor(&targetInfos);
    battle.getUnitInfos(&viewer->data->battleMsgData, &targetInfos, true);
    listApi.sort(&targetInfos);

    bool result = false;
    UnitInfoListIterator it, end;
    listApi.end(&targetInfos, &end);
    for (listApi.begin(&targetInfos, &it); !listApi.equals(&it, &end); listApi.preinc(&it)) {
        CMidgardID selectedUnitId{};
        result = isUnitSelected(viewer, mousePosition, listApi.dereference(&it), &selectedUnitId);
        if (result) {
            markAttackTargets(viewer, targetGroupId, &selectedUnitId, listApi.dereference(&it),
                              setBigFace, attack, isBattleGoing, isItemAttack, targetPositions);
            break;
        }
    }

    listApi.destructor(&targetInfos);
    return result;
}

bool unmarkAttackTargets(game::CBattleViewerInterf* viewer,
                         const game::CMqPoint* mousePosition,
                         bool setBigFace)
{
    using namespace game;

    const auto& viewerApi = BattleViewerInterfApi::get();
    const auto& engineApi = BatViewer2DEngineApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();

    engineApi.clearTargetMarks(viewer->data->batViewer2dEngine);
    viewer->data2->selectedUnitId = emptyId;

    CMidgardID selectedUnitId{};
    viewerApi.getSelectedUnitId(viewerApi.isFlipped(viewer) ? viewer->data->batUnitGroup1
                                                            : viewer->data->batUnitGroup2,
                                &selectedUnitId, mousePosition);
    if (selectedUnitId == emptyId)
        return false;

    if (setBigFace) {
        CBatBigFace* bigFace = viewerApi.getBigFace(viewer);
        bigFaceApi.setUnitId(bigFace, &selectedUnitId);
    }

    return true;
}

bool __fastcall markAttackTargetsHooked(game::CBattleViewerInterf* thisptr,
                                        int /*%edx*/,
                                        const game::CMqPoint* mousePosition,
                                        bool setBigFace)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battle = BattleMsgDataApi::get();

    if (thisptr->data->unitId == emptyId)
        return false;

    if (thisptr->dropManager.vftable->method1(&thisptr->dropManager) > 0) {
        if (thisptr->data->itemId == emptyId)
            return false;

        int itemIndex = battle.getLeaderEquippedBattleItemIndex(thisptr->data->objectMap,
                                                                &thisptr->data->unitId,
                                                                &thisptr->data->battleMsgData,
                                                                &thisptr->data->itemId);

        auto attack = fn.getAttackById(thisptr->data->objectMap, &thisptr->data->itemId, 1, true);
        const auto& targetData = thisptr->data->targetData.items[itemIndex];

        if (markAttackTargetsIfUnitSelected(thisptr, mousePosition, setBigFace, attack, true, true,
                                            &targetData.groupId, &targetData.unitPositions))
            return true;
    } else {
        auto attack = fn.getAttackById(thisptr->data->objectMap, &thisptr->data->unitId, 1, true);
        const auto& targetData = thisptr->data->targetData.attack;

        if (markAttackTargetsIfUnitSelected(thisptr, mousePosition, setBigFace, attack,
                                            targetData.isBattleGoing, false, &targetData.groupId,
                                            &targetData.unitPositions))
            return true;
    }

    return unmarkAttackTargets(thisptr, mousePosition, setBigFace);
}

void setTimerEvent(game::CBattleViewerInterf* viewer,
                   game::BattleViewerInterfApi::Api::Callback callback)
{
    using namespace game;

    Functor functor;
    BattleViewerInterfApi::Api::ButtonCallback buttonCallback{callback};
    BattleViewerInterfApi::get().createButtonFunctor(&functor, 0, viewer, &buttonCallback);

    const auto& uiManagerApi = CUIManagerApi::get();
    UIManagerPtr uiManager;
    uiManagerApi.get(&uiManager);

    const auto& eventApi = UiEventApi::get();
    UiEvent timerEvent;
    uiManagerApi.createTimerEvent(uiManager.data, &timerEvent, &functor, 5);
    eventApi.copy(&viewer->data2->autobattleTimerEvent, &timerEvent);
    eventApi.destructor(&timerEvent);

    SmartPointerApi::get().createOrFree((SmartPointer*)&uiManager, nullptr);
    FunctorApi::get().createOrFree(&functor, nullptr);
}

void updateForNormalAttack(game::CBattleViewerInterf* viewer)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& id = CMidgardIDApi::get();
    const auto& viewerApi = BattleViewerInterfApi::get();
    const auto& mapApi = UnitPositionMapApi::get();
    const auto& listApi = UnitPositionListApi::get();
    const auto& groupApi = CMidUnitGroupApi::get();
    const auto& attackReaches = AttackReachCategories::get();
    const auto& attackClasses = AttackClassCategories::get();

    CMidgardID otherGroupId = invalidId;
    CMidgardID attackerGroupId{};
    id.validateId(&attackerGroupId, viewer->data->battleMsgData.attackerGroupId);
    if (viewer->data->targetData.attack.groupId == attackerGroupId) {
        id.validateId(&otherGroupId, viewer->data->battleMsgData.defenderGroupId);
    } else {
        otherGroupId = attackerGroupId;
    }

    bool unknown = viewer->data->targetData.attack.unknown == false;
    viewerApi.unknownMethod4(&viewer->data->targetUnitData);

    void* tmp{};
    auto targetGroup = fn.getStackFortRuinGroup(tmp, viewer->data->objectMap,
                                                &viewer->data->targetData.attack.groupId);
    auto otherGroup = fn.getStackFortRuinGroup(tmp, viewer->data->objectMap, &otherGroupId);

    UnitPositionMap targetPositions{};
    mapApi.copyConstructor(&targetPositions, &viewer->data->targetData.attack.unitPositions);

    bool hasNegativePosition = mapApi.hasNegativePosition(&targetPositions);
    groupApi.unknownFunction(targetGroup, otherGroup, &targetPositions);

    UnitPositionList targetPositions2{};
    listApi.constructor(&targetPositions2);
    viewerApi.fillTargetPositions(viewer->data->targetData.attack.unknown, &targetPositions,
                                  &targetPositions2);

    auto attack = fn.getAttackById(viewer->data->objectMap, &viewer->data->unitId, 1, true);
    auto attackClass = attack->vftable->getAttackClass(attack);
    viewer->data2->attackClass = *attackClass;

    bool isSupportAttack = fn.isSupportAttackClass(attackClass);
    auto attackReach = attack->vftable->getAttackReach(attack);
    bool isAllAttackReach = attackReach->id == attackReaches.all->id;
    if (isAllAttackReach) {
        for (auto group : {viewer->data->batUnitGroup1, viewer->data->batUnitGroup3,
                           viewer->data->batUnitGroup2, viewer->data->batUnitGroup4}) {
            viewerApi.highlightGroupFrame(group, &viewer->data->targetData.attack.groupId,
                                          isSupportAttack);
        }
    }

    for (const auto& targetPosition : targetPositions2) {
        int absolutePosition;
        bool unknown2;
        CMidgardID targetUnitId = invalidId;
        if (targetPosition.first >= 0) {
            absolutePosition = targetPosition.first;
            unknown2 = viewer->data->targetData.attack.unknown;
            targetUnitId = *groupApi.getUnitIdByPosition(targetGroup, absolutePosition);
        } else {
            absolutePosition = -(targetPosition.first + 1);
            unknown2 = unknown;
            targetUnitId = *groupApi.getUnitIdByPosition(otherGroup, absolutePosition);
        }

        bool isTargetForSummonOrAttackTargetsBothGroups = hasNegativePosition;
        if (isTargetForSummonOrAttackTargetsBothGroups) {
            auto nonAltAttack = fn.getAttackById(viewer->data->objectMap, &viewer->data->unitId, 1,
                                                 false);
            auto nonAltAttackClass = nonAltAttack->vftable->getAttackClass(nonAltAttack);
            isTargetForSummonOrAttackTargetsBothGroups = nonAltAttackClass->id
                                                         != attackClasses.transformSelf->id;
        }

        bool isTargetForSummon = targetUnitId != viewer->data->unitId
                                 && (targetUnitId == emptyId
                                     || attackClass->id == attackClasses.summon->id);
        if (isTargetForSummon) {
            targetUnitId = emptyId;
            id.summonUnitIdFromPosition(&targetUnitId, absolutePosition);
            isTargetForSummonOrAttackTargetsBothGroups = true;

            if (!targetPosition.second)
                continue;

            viewerApi.unknownMethod3(unknown2 ? viewer->data->batUnitGroup1
                                              : viewer->data->batUnitGroup2,
                                     &viewer->data->targetData.attack.groupId, absolutePosition);
            viewerApi.unknownMethod3(unknown2 ? viewer->data->batUnitGroup3
                                              : viewer->data->batUnitGroup4,
                                     &viewer->data->targetData.attack.groupId, absolutePosition);
        } else if (targetPosition.second) {
            viewerApi.unknownMethod2(unknown2 ? viewer->data->batUnitGroup1
                                              : viewer->data->batUnitGroup2,
                                     &targetUnitId, isSupportAttack);
            viewerApi.unknownMethod2(unknown2 ? viewer->data->batUnitGroup3
                                              : viewer->data->batUnitGroup4,
                                     &targetUnitId, isSupportAttack);
        }

        bool canPerformAttackOnTargetOrAllAttackReach = targetPosition.second
                                                        || (targetPosition.first >= 0
                                                            && isAllAttackReach);
        viewerApi.addTargetUnitData(viewer, targetPosition.first, &targetUnitId, unknown2,
                                    isTargetForSummonOrAttackTargetsBothGroups, isTargetForSummon,
                                    canPerformAttackOnTargetOrAllAttackReach);
    }

    if (viewerApi.isFlipped(viewer) != viewer->data->targetData.attack.unknown)
        viewerApi.setCheckedForRightUnitsToggleButton(viewer,
                                                      viewer->data->targetData.attack.unknown);

    listApi.destructor(&targetPositions2);
    mapApi.destructor(&targetPositions);
}

void getMousePosition(game::CMqPoint* value)
{
    using namespace game;

    const auto& uiManagerApi = CUIManagerApi::get();
    const auto& smartPtrApi = SmartPointerApi::get();

    UIManagerPtr uiManager;
    uiManagerApi.get(&uiManager);

    uiManagerApi.getMousePosition(uiManager.data, value);

    smartPtrApi.createOrFree((SmartPointer*)&uiManager, nullptr);
}

void __fastcall battleViewerInterfUpdateHooked(game::IBatViewer* thisptr,
                                               int /*%edx*/,
                                               const game::BattleMsgData* battleMsgData,
                                               const game::CMidgardID* unitId,
                                               const game::Set<game::BattleAction>* actions,
                                               const game::BatViewerTargetDataSet* targetData)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    const auto& battle = BattleMsgDataApi::get();
    const auto& viewerApi = BattleViewerInterfApi::get();
    const auto& animApi = BatUnitAnimApi::get();
    const auto& dragDropApi = CDragAndDropInterfApi::get();
    const auto& dialogApi = CDialogInterfApi::get();
    const auto& toggleButtonApi = CToggleButtonApi::get();
    const auto& listApi = TargetsListApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();

    auto viewer = castBatViewerToBattleViewerInterf(thisptr);
    viewer->data->unknown6 = false;
    viewer->data->unitId = *unitId;
    battle.copyAssignment(&viewer->data->battleMsgData, battleMsgData);

    CMidgardID attackerGroupId{};
    id.validateId(&attackerGroupId, viewer->data->battleMsgData.attackerGroupId);

    viewerApi.battleViewerTargetDataSetCtor(&viewer->data->targetData, targetData);
    viewerApi.battleViewerTargetDataSetSetAttacker(&viewer->data->targetData, &attackerGroupId);

    auto unitAnimation = *viewerApi.getUnitAnimation(viewer, unitId);
    animApi.update(unitAnimation, &viewer->data->battleMsgData, false);

    viewerApi.updateUnknown(viewer, false);
    viewer->data2->unknown4 = false;

    auto dialog = CDragAndDropInterfApi::get().getDialog(viewer);
    bool unknown2 = viewerApi.getUnknown2(&viewer->data->unknownUnitData, &viewer->data->unitId);
    bool autoBattle = unknown2 ? viewer->data2->unknown6 : viewer->data2->unknown7;
    if (viewer->data2->unknown8 != unknown2) {
        auto button = dialogApi.findToggleButton(dialog, "TOG_AUTOBATTLE");
        toggleButtonApi.setChecked(button, autoBattle);
        viewer->data2->unknown8 = unknown2;
    }

    if (autoBattle) {
        setTimerEvent(viewer, viewerApi.autoBattleCallback);
        return;
    }

    if (viewer->data2->unknown10) {
        setTimerEvent(viewer, viewerApi.disableAutoBattleAndResolveCallback);
        return;
    }

    auto toggleRightUnits = dialogApi.findToggleButton(dialog, "TOG_RIGHTUNITS");
    if (toggleRightUnits) {
        toggleRightUnits->vftable->setEnabled(toggleRightUnits, true);
    }

    viewer->data2->normalAttack = false;
    bool canUseItem = false;
    TargetsListIterator it, end;
    for (listApi.begin((TargetsList*)actions, &it), listApi.end((TargetsList*)actions, &end);
         !listApi.equals(&it, &end); listApi.preinc(&it)) {
        BattleAction action = (BattleAction)*listApi.dereference(&it);
        switch (action) {
        case BattleAction::Attack:
            viewer->data2->normalAttack = true;
            break;
        case BattleAction::Retreat: {
            auto button = dialogApi.findButton(dialog, "BTN_RETREAT");
            button->vftable->setEnabled(button, true);
            break;
        }
        case BattleAction::Wait: {
            auto button = dialogApi.findButton(dialog, "BTN_WAIT");
            button->vftable->setEnabled(button, true);
            break;
        }
        case BattleAction::Defend: {
            auto button = dialogApi.findButton(dialog, "BTN_DEFEND");
            button->vftable->setEnabled(button, true);
            break;
        }
        case BattleAction::UseItem:
            canUseItem = true;
            break;
        }
    }

    viewer->data2->unknown3 = 0;

    auto bigFace2 = viewerApi.getBigFace2(viewer);
    bigFaceApi.setUnknown(bigFace2, false);

    viewerApi.updateBattleItems(viewer, canUseItem);

    for (auto group : {viewer->data->batUnitGroup1, viewer->data->batUnitGroup3,
                       viewer->data->batUnitGroup2, viewer->data->batUnitGroup4}) {
        viewerApi.unknownMethod(group, &viewer->data->unitId);
    }

    if (viewer->data2->normalAttack)
        updateForNormalAttack(viewer);

    viewerApi.unknownMethod6(viewer, false);
    bigFaceApi.setUnitId(bigFace2, &viewer->data->unitId);
    viewerApi.unknownMethod7(viewer);

    CMqPoint mousePosition;
    getMousePosition(&mousePosition);
    mousePosition.x -= viewer->data->dialogInterfArea.p1.x;
    mousePosition.y -= viewer->data->dialogInterfArea.p1.y;

    viewerApi.markAttackTargets(viewer, &mousePosition, false);
    viewerApi.unknownMethod8(viewer, &mousePosition);
}

void __fastcall battleViewerInterfUpdateBattleItemsHooked(game::CBattleViewerInterf* thisptr,
                                                          int /*%edx*/,
                                                          bool canUseItem)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();
    const auto& faderApi = CMusicFaderApi::get();
    const auto& viewerApi = BattleViewerInterfApi::get();

    auto fader = faderApi.get();
    if (faderApi.hasEventId(fader))
        faderApi.callback(fader);

    viewerApi.unknownMethod9(thisptr);

    if (!thisptr->data->targetData.attack.isBattleGoing || thisptr->data->unitId == emptyId)
        return;

    if (!isStackLeaderAndAllowedToUseBattleItems(thisptr->data->objectMap, &thisptr->data->unitId,
                                                 &thisptr->data->battleMsgData))
        return;

    CMidgardID itemIds[2];
    battle.getLeaderEquippedBattleItemIds(thisptr->data->objectMap, &thisptr->data->unitId,
                                          &thisptr->data->battleMsgData, itemIds);

    for (int i = 0; i < 2; ++i) {
        if (faderApi.hasEventId(fader))
            faderApi.callback(fader);

        if (itemIds[i] == emptyId)
            continue;

        viewerApi.unknownMethod10(thisptr, &itemIds[i], i, &thisptr->data2->list);

        if (canUseItem) {
            const auto& targetData = thisptr->data->targetData.items[i];
            bool isUnitOnTheLeft = viewerApi.isUnitOnTheLeft(thisptr, targetData.isAttacker);

            viewerApi.unknownMethod11(thisptr, &itemIds[i], &targetData.groupId,
                                      &targetData.unitPositions,
                                      isUnitOnTheLeft ? &thisptr->data->groupAreas2
                                                      : &thisptr->data->groupAreas1,
                                      false, &thisptr->data2->list2);
        }
    }

    if (canUseItem)
        viewerApi.unknownMethod12(thisptr);
}

void batBigFaceCleanUnitData(game::CBatBigFace* thisptr, const game::UnitInfoList& unitInfos)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();

    for (auto it = thisptr->data->unitData.begin(); it != thisptr->data->unitData.end(); ++it) {
        bool unitFound = false;
        for (auto node = unitInfos.head->next; node != unitInfos.head; node = node->next) {
            CMidgardID unitId;
            idApi.validateId(&unitId, node->data.unitId1);

            if (battleApi.getUnitStatus(&thisptr->data->battleMsgData, &unitId,
                                        BattleStatus::Unsummoned)
                || battleApi.getUnitStatus(&thisptr->data->battleMsgData, &unitId,
                                           BattleStatus::Retreated)
                || battleApi.getUnitStatus(&thisptr->data->battleMsgData, &unitId,
                                           BattleStatus::Hidden)) {
                continue;
            }

            if (unitId == it->first) {
                unitFound = true;
                break;
            }
        }

        if (!unitFound) {
            if (thisptr->data->unitId == it->first)
                thisptr->data->unitId = emptyId;
            bigFaceApi.unitDataMapErase(&thisptr->data->unitData, it);
        }
    }
}

void batBigFaceUpdateUnitImplData(game::CBatBigFace* thisptr, game::CBatBigFaceUnitData* unitData)
{
    using namespace game;

    const auto& bigFaceApi = BatBigFaceApi::get();
    const auto& smartPtrApi = SmartPointerApi::get();
    const auto& imagesLoaderApi = BatImagesLoaderApi::get();

    auto unitImplData = bigFaceApi.unitImplDataMapAccess(&thisptr->data->unitImplData,
                                                         &unitData->unitImplId);

    if (unitImplData->faceImage.data == nullptr) {
        auto faceImage = imagesLoaderApi.loadUnitFaceImage(thisptr->data->imagesLoader,
                                                           &unitData->unitImplId,
                                                           !thisptr->data->flippedBattle, true);
        smartPtrApi.createOrFree((SmartPointer*)&unitImplData->faceImage, faceImage);
    }
}

void batBigFaceUpdateItemData(game::CBatBigFace* thisptr,
                              game::CBatBigFaceUnitData* unitData,
                              const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battleApi = BattleMsgDataApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();
    const auto& smartPtrApi = SmartPointerApi::get();
    const auto& imagesLoaderApi = BatImagesLoaderApi::get();

    unitData->showItems = isStackLeaderAndAllowedToUseBattleItems(thisptr->data->objectMap, unitId,
                                                                  &thisptr->data->battleMsgData);
    if (!unitData->showItems) {
        for (auto& itemId : unitData->itemIds) {
            itemId = emptyId;
        }
        return;
    }

    battleApi.getLeaderEquippedBattleItemIds(thisptr->data->objectMap, unitId,
                                             &thisptr->data->battleMsgData, unitData->itemIds);
    for (const auto& itemId : unitData->itemIds) {
        if (itemId == emptyId)
            continue;

        auto item = static_cast<CMidItem*>(
            thisptr->data->objectMap->vftable->findScenarioObjectById(thisptr->data->objectMap,
                                                                      &itemId));

        auto itemImage = imagesLoaderApi.loadItemImage(thisptr->data->imagesLoader,
                                                       &item->globalItemId,
                                                       thisptr->data->flippedBattle);

        auto itemData = bigFaceApi.itemDataMapAccess(&thisptr->data->itemData, &itemId);
        smartPtrApi.createOrFree((SmartPointer*)&itemData->itemImage, itemImage);
    }
}

void batBigFaceUpdateUnitData(game::CBatBigFace* thisptr, const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& bigFaceApi = BatBigFaceApi::get();
    const auto& image2TextApi = CImage2TextApi::get();
    const auto& stringApi = StringApi::get();
    const auto& smartPtrApi = SmartPointerApi::get();

    auto unitData = bigFaceApi.unitDataMapAccess(&thisptr->data->unitData, unitId);

    if (unitData->textImage.data == nullptr) {
        auto textImage = (CImage2Text*)Memory::get().allocate(sizeof(CImage2Text));
        const auto& textArea = thisptr->data->bigFaceTextArea;
        image2TextApi.constructor(textImage, textArea.p2.x - textArea.p1.x,
                                  textArea.p2.y - textArea.p1.y);
        smartPtrApi.createOrFree((SmartPointer*)&unitData->textImage, textImage);
    }

    String description{};
    battleApi.generateBigFaceDescription(&description, thisptr->data->objectMap, unitId,
                                         &thisptr->data->battleMsgData);

    const auto desc = stringApi.cStr(&description);
    if (strcmp(desc, stringApi.cStr(&unitData->textImage.data->text)) != 0)
        image2TextApi.setText(unitData->textImage.data, desc);
    stringApi.free(&description);

    fn.getBaseUnitImplId(&unitData->unitImplId, thisptr->data->objectMap, unitId, false);

    batBigFaceUpdateUnitImplData(thisptr, unitData);

    batBigFaceUpdateItemData(thisptr, unitData, unitId);
}

void __fastcall batBigFaceUpdateHooked(game::CBatBigFace* thisptr,
                                       int /*%edx*/,
                                       const game::BattleMsgData* battleMsgData)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& unitInfoListApi = UnitInfoListApi::get();

    battleApi.copyAssignment(&thisptr->data->battleMsgData, battleMsgData);

    UnitInfoList unitInfos{};
    unitInfoListApi.constructor(&unitInfos);
    battleApi.getUnitInfos(battleMsgData, &unitInfos, true);

    batBigFaceCleanUnitData(thisptr, unitInfos);

    for (auto node = unitInfos.head->next; node != unitInfos.head; node = node->next) {
        CMidgardID unitId;
        idApi.validateId(&unitId, node->data.unitId1);

        batBigFaceUpdateUnitData(thisptr, &unitId);
    }

    unitInfoListApi.destructor(&unitInfos);
}

} // namespace hooks
