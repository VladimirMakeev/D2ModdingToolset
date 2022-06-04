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

assert_size(CMidEffectWin, 32);

/** Create new stack. */
struct CMidEffectCreateStack : public CMidEvEffect
{
    CMidgardID stackTemplateId;
    CMidgardID locationId;
};

assert_size(CMidEffectCreateStack, 32);

/** Cast spell on triggerer. */
struct CMidEffectCastSpell : public CMidEvEffect
{
    CMidgardID spellId;
    CMidgardID playerId;
};

assert_size(CMidEffectCastSpell, 32);

/** Cast spell at specific location. */
struct CMidEffectCastSpellMap : public CMidEvEffect
{
    CMidgardID spellId;
    CMidgardID locationId;
    CMidgardID playerId;
};

assert_size(CMidEffectCastSpellMap, 36);

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

assert_size(CMidEffectChangeOwner, 40);

/** Change stack order. */
struct CMidEffectChangeOrder : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
    LOrderCategory order;
    CMidgardID orderTargetId;
};

assert_size(CMidEffectChangeOrder, 48);

/** Move stack next to triggerer. */
struct CMidEffectMoveBeside : public CMidEvEffect
{
    CMidgardID stackId;
};

assert_size(CMidEffectMoveBeside, 28);

/** Go into battle. */
struct CMidEffectBattle : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
};

assert_size(CMidEffectBattle, 32);

/** Enable/disable another event. */
struct CMidEffectEnableEvent : public CMidEvEffect
{
    CMidgardID targetEventId;
    bool enable;
    char padding[3];
};

assert_size(CMidEffectEnableEvent, 32);

/** Give spell. */
struct CMidEffectGiveSpell : public CMidEvEffect
{
    CMidgardID spellId;
};

assert_size(CMidEffectGiveSpell, 28);

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

assert_size(CMidEffectGiveItem, 32);

/** Move stack to specific location. */
struct CMidEffectMove : public CMidEvEffect
{
    CMidgardID stackTemplateId;
    CMidgardID locationId;
    bool moveTriggererStack;
    char padding[3];
};

assert_size(CMidEffectMove, 36);

/** Ally two AI players. */
struct CMidEffectAlly : public CMidEvEffect
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    bool permanentAlliance;
    char padding[3];
};

assert_size(CMidEffectAlly, 36);

/** Change player diplomacy meter. */
struct CMidEffectDiplomacy : public CMidEvEffect
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    int diplomacy; /**< Percentage based value [-100 : 100]. */
    bool alwaysAtWar;
    char padding[3];
};

assert_size(CMidEffectDiplomacy, 40);

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

assert_size(CMidEffectUnfog, 64);

/** Remove mountains around a location. */
struct CMidEffectMountain : public CMidEvEffect
{
    CMidgardID locationId;
};

assert_size(CMidEffectMountain, 28);

/** Remove landmark. */
struct CMidEffectRmvLMark : public CMidEvEffect
{
    CMidgardID landmarkId;
    bool playAnimation;
    char padding[3];
};

assert_size(CMidEffectRmvLMark, 32);

/** Change landmark. */
struct CMidEffectChangeLandmark : public CMidEvEffect
{
    CMidgardID landmarkId;
    CMidgardID landmarkTypeId;
};

assert_size(CMidEffectChangeLandmark, 32);

/** Change terrain. */
struct CMidEffectChangeTerrain : public CMidEvEffect
{
    CMidgardID locationId;
    LTerrainCategory terrain;
    int areaSize;
};

assert_size(CMidEffectChangeTerrain, 44);

/** Change scenario objective text. */
struct CMidEffectObjective : public CMidEvEffect
{
    StringAndId objective;
};

assert_size(CMidEffectObjective, 36);

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

assert_size(CMidEffectPopup, 64);

/** Destroy item. */
struct CMidEffectDestroyItem : public CMidEvEffect
{
    CMidgardID itemId;
    bool onlyTriggerer;
    char padding[3];
};

assert_size(CMidEffectDestroyItem, 32);

/** Remove stack. */
struct CMidEffectRemoveStack : public CMidEvEffect
{
    CMidgardID stackId;
    bool firstOnly;
    char padding[3];
};

assert_size(CMidEffectRemoveStack, 32);

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

assert_size(CMidEffectModifyVariable, 36);

} // namespace game

#endif // EVENTEFFECTS_H
