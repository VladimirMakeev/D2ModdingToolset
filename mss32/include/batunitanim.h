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

#ifndef BATUNITANIM_H
#define BATUNITANIM_H

#include "battlemsgdata.h"
#include "batviewerutils.h"
#include "d2list.h"
#include "d2string.h"
#include "midgardid.h"
#include "mqanimation.h"
#include "mqrect.h"
#include "racecategory.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CMqPresentationManager;
struct IMidgardObjectMap;
struct CBatViewer2DEngine;
struct CBatImagesLoader;

struct CBatUnitAnimData
{
    SmartPtr<CMqPresentationManager> presentationMgr;
    IMidgardObjectMap* objectMap;
    CMidgardID unitId;
    CMidgardID baseUnitId;
    LRaceCategory raceCategory;
    bool retreated;  /**< Set according to BattleStatus::Retreated. */
    bool retreating; /**< Set according to BattleStatus::Retreat. */
    bool hidden;     /**< Set according to BattleStatus::Hidden. */
    bool dead;       /**< Set according to BattleStatus::Dead. */
    char unknown;
    bool bigUnit;
    bool disabled; /**< Set according to BattleStatus::Paralyze or BattleStatus::Petrify. */
    bool unknown2;
    CMidgardID unknownId;
    int unknown3;
    BattleMsgData battleMsgData;
    bool attacker;
    bool unknown11;
    char padding[2];
    int unitPosition;
    CBatViewer2DEngine* batViewer2dEngine;
    CBatImagesLoader* imagesLoader;
    CBatViewerUtils::CAnimCounter* animCounter;
    CMqRect smallUnitImageArea;
    CMqRect bigUnitImageArea1;
    CMqRect bigUnitImageArea2;
    SmartPointer ptr1;
    SmartPointer ptr2;
    SmartPointer ptr3;
    char unknown4[12];
    void* unknown5;
    void* unknown6;
    void* unknown7;
    bool battleAnimations;
    bool battleSpeedInstant;
    char padding2[2];
    String string1;
    List<void*>* list; /**< Points to CBattleViewerInterfData2::list4. */
    String string2;
    char unknown10[12];
};

static_assert(sizeof(CBatUnitAnimData) == 4136,
              "Size of CBatUnitAnimData structure must be exactly 4136 bytes");

static_assert(offsetof(CBatUnitAnimData, objectMap) == 8,
              "CBatUnitAnimData::objectMap offset must be 8 bytes");

static_assert(offsetof(CBatUnitAnimData, bigUnit) == 37,
              "CBatUnitAnimData::bigUnit offset must be 37 bytes");

static_assert(offsetof(CBatUnitAnimData, unknownId) == 40,
              "CBatUnitAnimData::unknownId offset must be 40 bytes");

static_assert(offsetof(CBatUnitAnimData, battleMsgData) == 48,
              "CBatUnitAnimData::battleMsgData offset must be 48 bytes");

static_assert(offsetof(CBatUnitAnimData, smallUnitImageArea) == 3988,
              "CBatUnitAnimData::smallUnitImageArea offset must be 3988 bytes");

static_assert(offsetof(CBatUnitAnimData, ptr1) == 4036,
              "CBatUnitAnimData::ptr1 offset must be 4036 bytes");

static_assert(offsetof(CBatUnitAnimData, unknown5) == 4072,
              "CBatUnitAnimData::unknown5 offset must be 4072 bytes");

static_assert(offsetof(CBatUnitAnimData, string1) == 4088,
              "CBatUnitAnimData::string1 offset must be 4088 bytes");

static_assert(offsetof(CBatUnitAnimData, string2) == 4108,
              "CBatUnitAnimData::string2 offset must be 4108 bytes");

struct CBatUnitAnim : public IMqAnimation
{
    CBatUnitAnimData* data;
};

static_assert(sizeof(CBatUnitAnim) == 8, "Size of CBatUnitAnim structure must be exactly 8 bytes");

namespace BatUnitAnimApi {

struct Api
{
    using Update = void(__thiscall*)(CBatUnitAnim* thisptr,
                                     const BattleMsgData* battleMsgData,
                                     bool a3);
    Update update;
};

Api& get();

} // namespace BatUnitAnimApi

} // namespace game

#endif // BATUNITANIM_H
