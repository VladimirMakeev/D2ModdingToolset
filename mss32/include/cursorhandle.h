/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef CURSORHANDLE_H
#define CURSORHANDLE_H

#include "linkedlist.h"
#include "mqrect.h"
#include "smartptr.h"

namespace game {

struct IMqImage2;

struct CursorHandleNodeData
{
    SmartPtr<IMqImage2> image;
    CMqPoint imageHalfSize;
    CMqRect area;
    int unknown;
};

static_assert(sizeof(CursorHandleNodeData) == 36,
              "Size of CursorHandleNodeData structure must be exactly 36 bytes");

struct CursorHandleData
{
    LinkedList<CursorHandleNodeData> list;
    CMqRect area;
};

static_assert(sizeof(CursorHandleData) == 32,
              "Size of CursorHandleData structure must be exactly 32 bytes");

struct CursorHandle
{
    CursorHandleData* data;
};

static_assert(sizeof(CursorHandle) == 4, "Size of CursorHandle structure must be exactly 4 bytes");

/** Cursor types supported in game and their image names. */
enum class CursorType : int
{
    Default,         /**< DEFAULT */
    Wait,            /**< WAIT */
    Alliance,        /**< ALLIANCE */
    Battle,          /**< BATTLE */
    Negotiate,       /**< NEGO */
    EnterCity,       /**< ENTERCITY */
    Move,            /**< MOVE */
    SelectUnit,      /**< SELECTUNIT */
    SelectCity,      /**< SELECTCITY */
    ViewCity,        /**< VIEWCITY */
    ManageStack,     /**< MANAGESTK */
    ScrollValid,     /**< SCROLLVAL */
    ScrollInvalid,   /**< SCROLLINV */
    SpellValid,      /**< SPELLVAL */
    SpellInvalid,    /**< SPELLINV */
    TakBag,          /**< TAKEBAG */
    Exchange,        /**< EXCHANGE */
    Visit,           /**< VISIT */
    DropBag,         /**< DROPBAG */
    DropRodValid,    /**< DROPRODVAL */
    DropRodInvalid,  /**< DROPRODINV */
    BreakRod,        /**< BREAKROD */
    NoDragDrop,      /**< NODRAGDROP */
    DismissValid,    /**< DISMISSVAL */
    DismissInvalid,  /**< DISMISSINV */
    HealValid,       /**< HEALVAL */
    HealInvalid,     /**< HEALINV */
    ReviveValid,     /**< REVIVEVAL */
    ReviveInvalid,   /**< REVIVEINV */
    DynLevelValid,   /**< DYNLEVELVAL */
    DynLevelInvalid, /**< DYNLEVELINV */
};

namespace CursorHandleApi {

struct Api
{
    /** Creates CursorHandle from specified cursorType. */
    using Create = CursorHandle*(__stdcall*)(const CursorType* cursorType);
    Create create;
};

Api& get();

} // namespace CursorHandleApi

} // namespace game

#endif // CURSORHANDLE_H
