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

#ifndef EVENTEFFECTS_H
#define EVENTEFFECTS_H

#include "d2set.h"
#include "mideveffect.h"
#include "ordercat.h"
#include "stringandid.h"
#include "terraincat.h"

namespace game {

/** Win or lose scenario. */
struct CMidEffectWin : public CMidEvEffect
{
    bool win;
    char padding[3];
    CMidgardID playerId;
};

static_assert(sizeof(CMidEffectWin) == 32,
              "Size of CMidEffectWin structure must be exactly 32 bytes");

/** Create new stack. */
struct CMidEffectCreateStack : public CMidEvEffect
{
    CMidgardID stackTemplateId;
    CMidgardID locationId;
};

static_assert(sizeof(CMidEffectCreateStack) == 32,
              "Size of CMidEffectCreateStack structure must be exactly 32 bytes");

/** Cast spell on triggerer. */
struct CMidEffectCastSpell : public CMidEvEffect
{
    CMidgardID spellId;
    CMidgardID playerId;
};

static_assert(sizeof(CMidEffectCastSpell) == 32,
              "Size of CMidEffectCastSpell structure must be exactly 32 bytes");

/** Cast spell at specific location. */
struct CMidEffectCastSpellMap : public CMidEvEffect
{
    CMidgardID spellId;
    CMidgardID locationId;
    CMidgardID playerId;
};

static_assert(sizeof(CMidEffectCastSpellMap) == 36,
              "Size of CMidEffectCastSpellMap structure must be exactly 36 bytes");

/** Change stack owner. */
struct CMidEffectChangeOwner : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
    CMidgardID playerId;
    bool playAnimation;
    char padding2[3];
};

static_assert(sizeof(CMidEffectChangeOwner) == 40,
              "Size of CMidEffectChangeOwner structure must be exactly 40 bytes");

/** Change stack order. */
struct CMidEffectChangeOrder : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
    LOrderCategory order;
    CMidgardID orderTargetId;
};

static_assert(sizeof(CMidEffectChangeOrder) == 48,
              "Size of CMidEffectChangeOrder structure must be exactly 48 bytes");

/** Move stack next to triggerer. */
struct CMidEffectMoveBeside : public CMidEvEffect
{
    CMidgardID stackId;
};

static_assert(sizeof(CMidEffectMoveBeside) == 28,
              "Size of CMidEffectMoveBeside structure must be exactly 28 bytes");

/** Go into battle. */
struct CMidEffectBattle : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
};

static_assert(sizeof(CMidEffectBattle) == 32,
              "Size of CMidEffectBattle structure must be exactly 32 bytes");

/** Enable/disable another event. */
struct CMidEffectEnableEvent : public CMidEvEffect
{
    CMidgardID targetEventId;
    bool enable;
    char padding[3];
};

static_assert(sizeof(CMidEffectEnableEvent) == 32,
              "Size of CMidEffectEnableEvent structure must be exactly 32 bytes");

/** Give spell. */
struct CMidEffectGiveSpell : public CMidEvEffect
{
    CMidgardID spellId;
};

static_assert(sizeof(CMidEffectGiveSpell) == 28,
              "Size of CMidEffectGiveSpell structure must be exactly 28 bytes");

enum GiveItemTarget : int
{
    Triggerer,
    Capital,
};

/** Give item. */
struct CMidEffectGiveItem : public CMidEvEffect
{
    CMidgardID itemId;
    GiveItemTarget giveTo;
};

static_assert(sizeof(CMidEffectGiveItem) == 32,
              "Size of CMidEffectGiveItem structure must be exactly 32 bytes");

/** Move stack to specific location. */
struct CMidEffectMove : public CMidEvEffect
{
    CMidgardID stackTemplateId;
    CMidgardID locationId;
    bool moveTriggererStack;
    char padding[3];
};

static_assert(sizeof(CMidEffectMove) == 36,
              "Size of CMidEffectMove structure must be exactly 36 bytes");

/** Ally two AI players. */
struct CMidEffectAlly : public CMidEvEffect
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    bool permanentAlliance;
    char padding[3];
};

static_assert(sizeof(CMidEffectAlly) == 36,
              "Size of CMidEffectAlly structure must be exactly 36 bytes");

/** Change player diplomacy meter. */
struct CMidEffectDiplomacy : public CMidEvEffect
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    int diplomacy; /**< Percentage based value [-100 : 100]. */
    bool alwaysAtWar;
    char padding[3];
};

static_assert(sizeof(CMidEffectDiplomacy) == 40,
              "Size of CMidEffectDiplomacy structure must be exactly 40 bytes");

enum class UnfogRadius : int
{
    Radius1x1,
    Radius3x3,
    Radius5x5,
    Radius7x7,
    Radius9x9,
    Radius11x11,
    Radius13x13,
    Radius15x15,
    Radius17x17,
    Radius19x19,
    Radius21x21,
    Radius23x23,
    Radius25x25,
    Radius27x27,
    Radius29x29,
    Radius31x31,
    Radius33x33,
    Radius35x35,
    Radius37x37,
    Radius39x39,
    Radius41x41,
    Radius43x43,
    Radius45x45,
    Radius47x47,
    Radius49x49,
};

/** Unfog or fog an area of the map. */
struct CMidEffectUnfog : public CMidEvEffect
{
    Set<CMidgardID> players;
    CMidgardID locationId;
    bool unfog;
    char padding[3];
    UnfogRadius radius;
};

static_assert(sizeof(CMidEffectUnfog) == 64,
              "Size of CMidEffectUnfog structure must be exactly 64 bytes");

/** Remove mountains around a location. */
struct CMidEffectMountain : public CMidEvEffect
{
    CMidgardID locationId;
};

static_assert(sizeof(CMidEffectMountain) == 28,
              "Size of CMidEffectMountain structure must be exactly 28 bytes");

/** Remove landmark. */
struct CMidEffectRmvLMark : public CMidEvEffect
{
    CMidgardID landmarkId;
    bool playAnimation;
    char padding[3];
};

static_assert(sizeof(CMidEffectRmvLMark) == 32,
              "Size of CMidEffectRmvLMark structure must be exactly 32 bytes");

/** Change landmark. */
struct CMidEffectChangeLandmark : public CMidEvEffect
{
    CMidgardID landmarkId;
    CMidgardID landmarkTypeId;
};

static_assert(sizeof(CMidEffectChangeLandmark) == 32,
              "Size of CMidEffectChangeLandmark structure must be exactly 32 bytes");

/** Change terrain. */
struct CMidEffectChangeTerrain : public CMidEvEffect
{
    CMidgardID locationId;
    LTerrainCategory terrain;
    int areaSize;
};

static_assert(sizeof(CMidEffectChangeTerrain) == 44,
              "Size of CMidEffectChangeTerrain structure must be exactly 44 bytes");

/** Change scenario objective text. */
struct CMidEffectObjective : public CMidEvEffect
{
    StringAndId objective;
};

static_assert(sizeof(CMidEffectObjective) == 36,
              "Size of CMidEffectObjective structure must be exactly 36 bytes");

enum class PopupShowMode : int
{
    ShowTriggerer,
    ShowAll,
    ShowAffectedOnly,
};

/** Display popup message. */
struct CMidEffectPopup : public CMidEvEffect
{
    StringAndId popupText;
    char* music;
    char* sound;
    char* image;
    char* image2;
    bool leftSide;
    char padding[3];
    PopupShowMode showMode;
    bool unknown;
    char padding2[3];
};

static_assert(sizeof(CMidEffectPopup) == 64,
              "Size of CMidEffectPopup structure must be exactly 64 bytes");

/** Destroy item. */
struct CMidEffectDestroyItem : public CMidEvEffect
{
    CMidgardID itemId;
    bool onlyTriggerer;
    char padding[3];
};

static_assert(sizeof(CMidEffectDestroyItem) == 32,
              "Size of CMidEffectDestroyItem structure must be exactly 32 bytes");

/** Remove stack. */
struct CMidEffectRemoveStack : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
};

static_assert(sizeof(CMidEffectRemoveStack) == 32,
              "Size of CMidEffectRemoveStack structure must be exactly 32 bytes");

enum class ModifyVariableOperation : int
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Replace,
};

/** Modify variable. */
struct CMidEffectModifyVariable : public CMidEvEffect
{
    ModifyVariableOperation operation;
    int value;
    int variableId;
};

static_assert(sizeof(CMidEffectModifyVariable) == 36,
              "Size of CMidEffectModifyVariable structure must be exactly 36 bytes");

} // namespace game

#endif // EVENTEFFECTS_H
