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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "interfacechilds.h"
#include "mqrect.h"
#include "smartptr.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct CInterfaceVftable;
struct CInterfManagerImpl;
struct CInterface;
struct IMqRenderer2;
struct CMqPoint;
struct Tooltip;

using TooltipPtr = SmartPtr<Tooltip>;

struct CInterfaceData
{
    SmartPtr<CInterfManagerImpl> interfManager;
    CInterface* parent;
    CMqRect area;
    CInterfaceChilds childs;
    SmartPointer unknown6;
    TooltipPtr tooltip;
};

static_assert(sizeof(CInterfaceData) == 64,
              "Size of CInterfaceData structure must be exactly 64 bytes");

static_assert(offsetof(CInterfaceData, childs) == 28,
              "CInterfaceData::childs offset must be 28 bytes");

/**
 * Base class for all ui elements.
 */
struct CInterface
{
    CInterfaceVftable* vftable;
    CInterfaceData* interfaceData;
};

struct CInterfaceVftable
{
    using Destructor = void(__thiscall*)(CInterface* thisptr, char flags);
    Destructor destructor;

    /** Draws childs of specified root element. */
    using Draw = void(__thiscall*)(CInterface* thisptr,
                                   CInterface* rootElement,
                                   IMqRenderer2* renderer);
    Draw draw;

    void* method2;

    /**
     * Handles mouse buttons events.
     * @param mouseButton Windows mouse button message, 'WM_LBUTTONDOWN' for example.
     */
    using HandleMouse = int(__thiscall*)(CInterface* thisptr,
                                         std::uint32_t mouseButton,
                                         const CMqPoint* mousePosition);
    HandleMouse handleMouse;

    /**
     * Handles keyboard events.
     * @param key Windows virtual key code.
     */
    using HandleKeyboard = int(__thiscall*)(CInterface* thisptr, int key, int a3);
    HandleKeyboard handleKeyboard;

    void* method5;
    void* method6;
    void* method7;
    void* method8;
    void* method9;
    void* method10;

    using Method11 = int(__thiscall*)(CInterface* thisptr);
    Method11 method11;

    using GetArea = CMqRect*(__thiscall*)(CInterface* thisptr);
    GetArea getArea;

    void* method13;
    void* method14;
    void* method15;
    void* method16;
    void* method17;
    void* method18;

    /** Returns parent element. */
    using GetParent = CInterface*(__thiscall*)(CInterface* thisptr);
    GetParent getParent;

    /** Returns number of child elements. */
    using GetChildsCount = int(__thiscall*)(const CInterface* thisptr);
    GetChildsCount getChildsCount;

    /** Returns child element at specified index. */
    using GetChild = CInterface*(__thiscall*)(CInterface* thisptr, const int* index);
    GetChild getChild;

    /** Returns index of child element or -1 if child not found. */
    using GetChildIndex = int*(__thiscall*)(CInterface* thisptr,
                                            int* index,
                                            const CInterface* child);
    GetChildIndex getChildIndex;

    /** Adds new interface element to the end of childs array. */
    using AddChild = void(__thiscall*)(CInterface* thisptr, CInterface* child);
    AddChild addChild;

    using AddChildAndGetIndex = void(__thiscall*)(CInterface* thisptr,
                                                  int* index,
                                                  CInterface* child);

    /** Adds new interface element to the end of childs array and returns its index. */
    AddChildAndGetIndex addChildAndGetIndex;

    /** Set new interface element as first item in childs array. */
    AddChild setFirstChild;

    /**
     * Set new interface element as first item in childs array and returns its index.
     * Calls method 11 and possibly 5 after setting element.
     */
    AddChildAndGetIndex setFirstChildAndGetIndex;

    /**
     * Deletes child element at specified index.
     * Calls method 5 before destroying child element.
     * Does not change childs array size, only sets found element to nullptr
     * after destructor is called.
     */
    using DeleteChildAt = void(__thiscall*)(CInterface* thisptr, const int* index);
    DeleteChildAt deleteChildAt;

    void* method28;
    void* method29;
    void* method30;

    using SetTooltip = void(__thiscall*)(CInterface* thisptr, TooltipPtr* tooltip);
    SetTooltip setTooltip;

    using GetTooltip = TooltipPtr*(__thiscall*)(CInterface* thisptr);
    GetTooltip getTooltip;

    /**
     * Sets specified element as parent of this interface.
     * Parent element will be set if it is not a child of this interface.
     */
    using SetParent = void(__thiscall*)(CInterface* thisptr, CInterface* parent);
    SetParent setParent;
};

static_assert(sizeof(CInterfaceVftable) == 34 * sizeof(void*),
              "CInterface vftable must have exactly 34 methods");

} // namespace game

#endif // INTERFACE_H
