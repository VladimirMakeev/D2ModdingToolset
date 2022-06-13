/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef BATTLEVIEWERINTERF_H
#define BATTLEVIEWERINTERF_H

#include "attackclasscat.h"
#include "avoidflickerimage.h"
#include "battlemsgdata.h"
#include "batviewer.h"
#include "batviewerutils.h"
#include "d2map.h"
#include "d2string.h"
#include "d2vector.h"
#include "draganddropinterf.h"
#include "midgardid.h"
#include "mqrect.h"
#include "smartptr.h"
#include "uievent.h"
#include "unitpositionlist.h"
#include "unitpositionmap.h"
#include <cstddef>

namespace game {

struct BattleAttackInfo;
struct CBatViewer2DEngine;
struct CBatImagesLoader;
struct CBatBigFace;
struct CBatUnitGroup2;
struct CBatUnitAnim;
struct CMidgardMsgBox;
struct IMidgardObjectMap;
struct CBatEncyclopediaInterf;
struct CBatLog;
struct IBatNotify;

struct CBattleViewerGroupAreas
{
    CMqRect unitImageAreas[6];
    CMqRect unitBgndImageArea;
    String unitBgndString;
    CMqRect slotImageAreas[6];
    CMqRect meterImageAreas[6];
    CMqRect bigFaceImageArea;
    CMqRect bigTextImageArea;
    CMqRect itemImageAreas[2];
    CMqRect itemBgndImageArea;
    String itemBgndString;
};

assert_size(CBattleViewerGroupAreas, 416);

struct CBattleViewerUnknownData
{
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
};

assert_size(CBattleViewerUnknownData, 16);

struct CBattleViewerUnknown
{
    CBattleViewerUnknownData data;
    CBattleViewerUnknownData data2[6];
    CBattleViewerUnknownData data3[6];
    String string;
    Set<void> list; /** < Each node contains 16 bytes of data. */
};

assert_offset(CBattleViewerUnknown, data3, 112);
assert_offset(CBattleViewerUnknown, string, 208);

struct CBattleViewerAttackTargetData
{
    bool isBattleGoing;
    bool unknown;
    char padding[2];
    CMidgardID groupId;
    UnitPositionMap unitPositions;
};

assert_size(CBattleViewerAttackTargetData, 36);

struct CBattleViewerItemTargetData
{
    bool isAttacker;
    bool unknown;
    char padding[2];
    CMidgardID groupId;
    UnitPositionMap unitPositions;
};

assert_size(CBattleViewerItemTargetData, 36);

struct CBattleViewerTargetDataSet
{
    CBattleViewerAttackTargetData attack;
    CBattleViewerItemTargetData items[2];
};

assert_size(CBattleViewerTargetDataSet, 108);

struct CBattleViewerUnknownUnitData
{
    CMidgardID unknownId;
    int unknown;
    bool unknown2;
    bool isUnitBig;
    bool isUnitRetreating;
    char padding;
};

assert_size(CBattleViewerUnknownUnitData, 12);

using CUnknownUnitDataMap = Map<CMidgardID, CBattleViewerUnknownUnitData>;

struct CUnitRectAndId
{
    CMqRect rect;
    CMidgardID id;
};

assert_size(CUnitRectAndId, 20);

struct CBattleViewerTargetUnitData : CUnitRectAndId
{
    int position;
    bool isAttacker; /**< Meaning assumed. */
    bool isRetreating;
    bool isBig;
    bool isTargetForSummonOrAttackTargetsBothGroups;
    bool isTargetForSummon;
    bool canPerformAttackOnTargetOrAllAttackReach;
    char padding[2];
};

assert_size(CBattleViewerTargetUnitData, 32);

struct CBattleViewerInterfData
{
    CAvoidFlickerImage avoidFlickerImage;
    void* sounds;
    BattleMsgData battleMsgData;
    CUnknownUnitDataMap unknownUnitData;
    CMidgardID unitId;
    CBattleViewerTargetDataSet targetData;
    BattleAttackInfo** attackInfo;
    char unknown3[4];
    CMidgardID itemId;
    Vector<CBattleViewerTargetUnitData> targetUnitData;
    CBatViewer2DEngine* batViewer2dEngine;
    CMqRect dialogInterfArea;
    CBatImagesLoader* imagesLoader;
    CBattleViewerGroupAreas groupAreas1;
    CBattleViewerGroupAreas groupAreas2;
    CBatBigFace* bigFace;
    CBatUnitGroup2* batUnitGroup1;
    CBatUnitGroup2* batUnitGroup2;
    char unknown4[8];
    CBatBigFace* bigFace2;
    CBatUnitGroup2* batUnitGroup3;
    CBatUnitGroup2* batUnitGroup4;
    char unknown41[8];
    CBatUnitAnim* unitAnimations[6];
    CBatUnitAnim* unitAnimations2[6];
    CBattleViewerUnknown unknown11;
    char unknown5[4];
    CBatViewerUtils::CAnimCounter animCounter;
    char unknown10;
    bool unknown12;
    char unknown13[2];
    CMqRect unitGroupSpotArea;
    bool unknown7;
    bool unknown8;
    bool unknown9;
    bool flippedBattle;
    bool bothPlayersHuman;
    bool unknown6;
    char unknown61[2];
    CMidgardMsgBox* messageBox;
    IMidgardObjectMap* objectMap;
    bool stickyEncy;
    char padding[3];
};

assert_size(CBattleViewerInterfData, 5384);
assert_offset(CBattleViewerInterfData, batViewer2dEngine, 4136);
assert_offset(CBattleViewerInterfData, imagesLoader, 4156);
assert_offset(CBattleViewerInterfData, bigFace, 4992);
assert_offset(CBattleViewerInterfData, bigFace2, 5012);
assert_offset(CBattleViewerInterfData, bothPlayersHuman, 5368);
assert_offset(CBattleViewerInterfData, objectMap, 5376);

struct CBattleViewerInterfData2
{
    char* dialogName;
    int battleSpeed;
    bool battleAnimations;
    bool battleSpeedInstant;
    char padding[2];
    CMidgardID targetUnitId;
    LAttackClass attackClass;
    bool normalAttack;
    bool unknown4;
    char unknown5;
    char unknown51;
    UiEvent autobattleTimerEvent;
    bool unknown6;
    bool unknown7;
    bool unknown8;
    char unknown9;
    CBatEncyclopediaInterf* encyclopedia;
    char unknown1[48];
    CBatLog* log;
    List<void*> list; /**< Nodes unknown. */
    List<void*> list2;
    List<void*> list3;
    char unknown2[4];
    IBatNotify* battleNotify;
    CMidgardID selectedUnitId;
    List<void*> list4; /**< Each node contains 16 bytes of data. */
    char unknown3;
    bool unknown10;
    char unknown11;
    char unknown12;
};

assert_size(CBattleViewerInterfData2, 196);

/**
 * Renders battle screen and handles user input during battle.
 * Represents DLG_BATTLE_A from Interf.dlg.
 */
struct CBattleViewerInterf : public CDragAndDropInterf
{
    IBatViewer batViewer;
    CBattleViewerInterfData* data;
    CBattleViewerInterfData2* data2;
};

assert_size(CBattleViewerInterf, 36);

static inline CBattleViewerInterf* castBatViewerToBattleViewerInterf(IBatViewer* value)
{
    return reinterpret_cast<CBattleViewerInterf*>((std::uintptr_t)value
                                                  - offsetof(CBattleViewerInterf, batViewer));
}

namespace BattleViewerInterfApi {

struct Api
{
    using MarkAttackTargets = bool(__thiscall*)(CBattleViewerInterf* thisptr,
                                                const CMqPoint* mousePosition,
                                                bool setBigFace);
    MarkAttackTargets markAttackTargets;

    using IsUnitOnTheLeft = bool(__thiscall*)(const CBattleViewerInterf* thisptr, bool isAttacker);
    IsUnitOnTheLeft isUnitOnTheLeft;

    using IsFlipped = bool(__thiscall*)(const CBattleViewerInterf* thisptr);
    IsFlipped isFlipped;

    using GetBigFace = CBatBigFace*(__thiscall*)(const CBattleViewerInterf* thisptr);
    GetBigFace getBigFace;
    GetBigFace getBigFace2;

    using GetUnitRect = CMqRect*(__stdcall*)(CMqRect* value,
                                             CBattleViewerGroupAreas* groupAreas,
                                             int unitPosition,
                                             bool isUnitBig,
                                             bool isUnitRetreating);
    GetUnitRect getUnitRect;
    GetUnitRect getUnitRectPlusExtra;

    using GetBoolById = bool(__thiscall*)(CUnknownUnitDataMap* thisptr, const CMidgardID* unitId);
    GetBoolById isUnitBig;
    GetBoolById isUnitRetreating;
    GetBoolById getUnknown2;

    using GetSelectedUnitId = CMidgardID*(__thiscall*)(const CBatUnitGroup2* thisptr,
                                                       CMidgardID* value,
                                                       const CMqPoint* mousePosition);
    GetSelectedUnitId getSelectedUnitId;

    using HighlightGroupFrame = void(__thiscall*)(CBatUnitGroup2* thisptr,
                                                  const CMidgardID* targetGroupId,
                                                  bool isSupportAttack);
    HighlightGroupFrame highlightGroupFrame;

    using UnknownMethod = void(__thiscall*)(CBatUnitGroup2* thisptr, const CMidgardID* unitId);
    UnknownMethod unknownMethod;

    using UnknownMethod2 = void(__thiscall*)(CBatUnitGroup2* thisptr,
                                             const CMidgardID* targetUnitId,
                                             bool isSupportAttack);
    UnknownMethod2 unknownMethod2;

    using UnknownMethod3 = void(__thiscall*)(CBatUnitGroup2* thisptr,
                                             const CMidgardID* targetGroupId,
                                             int unitPosition);
    UnknownMethod3 unknownMethod3;

    using GetUnitAnimation = CBatUnitAnim**(__thiscall*)(const CBattleViewerInterf* thisptr,
                                                         const CMidgardID* unitId);
    GetUnitAnimation getUnitAnimation;

    using UpdateUnknown = void(__thiscall*)(CBattleViewerInterf* thisptr, bool a2);
    UpdateUnknown updateUnknown;

    using UpdateBattleItems = void(__thiscall*)(CBattleViewerInterf* thisptr, bool canUseItem);
    UpdateBattleItems updateBattleItems;

    using CBattleViewerTargetDataSetConstructor = CBattleViewerTargetDataSet*(
        __thiscall*)(CBattleViewerTargetDataSet* thisptr, const BatViewerTargetDataSet* src);
    CBattleViewerTargetDataSetConstructor battleViewerTargetDataSetCtor;

    using CBattleViewerTargetDataSetSetAttacker =
        void(__thiscall*)(CBattleViewerTargetDataSet* thisptr, const CMidgardID* attackerGroupId);
    CBattleViewerTargetDataSetSetAttacker battleViewerTargetDataSetSetAttacker;

    using Callback = void(__thiscall*)(CBattleViewerInterf* thisptr);
    Callback autoBattleCallback;
    Callback disableAutoBattleAndResolveCallback;

    struct ButtonCallback
    {
        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CBattleViewerInterf* viewer,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;

    using UnknownMethod4 = void(__thiscall*)(Vector<CBattleViewerTargetUnitData>* thisptr);
    UnknownMethod4 unknownMethod4;

    using FillTargetPositions = void(__stdcall*)(bool a1,
                                                 UnitPositionMap* positions,
                                                 UnitPositionList* value);
    FillTargetPositions fillTargetPositions;

    using AddTargetUnitData = void(__thiscall*)(CBattleViewerInterf* thisptr,
                                                int targetPosition,
                                                const CMidgardID* targetUnitId,
                                                bool isAttacker, /**< Meaning assumed. */
                                                bool isTargetForSummonOrAttackTargetsBothGroups,
                                                bool isTargetForSummon,
                                                bool canPerformAttackOnTargetOrAllAttackReach);
    AddTargetUnitData addTargetUnitData;

    using SetCheckedForRightUnitsToggleButton = void(__stdcall*)(CBattleViewerInterf* thisptr,
                                                                 bool checked);
    SetCheckedForRightUnitsToggleButton setCheckedForRightUnitsToggleButton;

    using UnknownMethod6 = void(__thiscall*)(CBattleViewerInterf* thisptr, bool a2);
    UnknownMethod6 unknownMethod6;

    using UnknownMethod7 = void(__thiscall*)(CBattleViewerInterf* thisptr);
    UnknownMethod7 unknownMethod7;

    using UpdateCursor = bool(__thiscall*)(CBattleViewerInterf* thisptr,
                                           const CMqPoint* mousePosition);
    UpdateCursor updateCursor;

    using UnknownMethod9 = void(__thiscall*)(CBattleViewerInterf* thisptr);
    UnknownMethod9 unknownMethod9;

    using UnknownMethod10 = void(__thiscall*)(CBattleViewerInterf* thisptr,
                                              const CMidgardID* battleItemId,
                                              int battleItemIndex,
                                              const List<void*>* a4);
    UnknownMethod10 unknownMethod10;

    using UnknownMethod11 = void(__thiscall*)(CBattleViewerInterf* thisptr,
                                              const CMidgardID* id,
                                              const CMidgardID* targetGroupId,
                                              const UnitPositionMap* targetPositions,
                                              const CBattleViewerGroupAreas* groupAreas,
                                              bool a6,
                                              const List<void*>* a7);
    UnknownMethod11 unknownMethod11;

    using UnknownMethod12 = void(__thiscall*)(CBattleViewerInterf* thisptr);
    UnknownMethod12 unknownMethod12;
};

Api& get();

const IBatViewerVftable* vftable();

} // namespace BattleViewerInterfApi

} // namespace game

#endif // BATTLEVIEWERINTERF_H
