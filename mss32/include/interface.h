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

template <typename T>
struct CInterfaceT
{
    T* vftable;
    CInterfaceData* interfaceData;
};

/** Base class for all ui elements. */
struct CInterface : public CInterfaceT<CInterfaceVftable>
{ };

struct CInterfaceVftable
{
    using Destructor = void(__thiscall*)(CInterface* thisptr, char flags);
    Destructor destructor;

    /** Draws childs of specified root element. */
    using Draw = void(__thiscall*)(CInterface* thisptr,
                                   CInterface* rootElement,
                                   IMqRenderer2* renderer);
    Draw draw;

    using Method2 = int(__thiscall*)(CInterface* thisptr, int a2);
    Method2 method2;

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

    /** Meaning assumed. */
    using OnVisibilityChanged = void(__thiscall*)(CInterface* thisptr, int a2, int a3);
    OnVisibilityChanged onVisibilityChanged;

    /** Calls method30 to get the pointer. */
    using Method6 = SmartPointer*(__thiscall*)(CInterface* thisptr,
                                               SmartPointer* ptr,
                                               const CMqPoint* point);
    Method6 method6;

    /** Returns tooltip of the interface element under the point. */
    using GetTooltipAtPoint = TooltipPtr*(__thiscall*)(CInterface* thisptr,
                                                       TooltipPtr* tooltip,
                                                       const CMqPoint* point);
    GetTooltipAtPoint getTooltipAtPoint;

    using Method8 = void*(__thiscall*)(CInterface* thisptr, void* a2, void* a3);
    Method8 method8;

    void* method9;
    void* method10;

    using Method11 = int(__thiscall*)(CInterface* thisptr);
    Method11 method11;

    using GetArea = CMqRect*(__thiscall*)(CInterface* thisptr);
    GetArea getArea;

    using SetArea = bool(__thiscall*)(CInterface* thisptr, const CMqRect* area);
    SetArea setArea;

    /** Returns true if specified point is inside interface element area. */
    using IsPointInside = bool(__thiscall*)(const CInterface* thisptr, const CMqPoint* point);
    IsPointInside isPointInside;

    void* method15;
    void* method16;

    /** Assumption: resizes interface element and its childs. */
    using Method17 = bool(__thiscall*)(CInterface* thisptr, const CMqPoint* size);
    Method17 method17;

    /**
     * Assumption: centers interface element inside specified area.
     * Centers inside parent element if input area is nullptr.
     */
    using Method18 = bool(__thiscall*)(CInterface* thisptr, const CMqRect* area);
    Method18 method18;

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

    /**
     * Returns interface element under specified point.
     * Check if point is inside interface area or its childs.
     * @param deepCheck if true checks whole hierarchy, otherwise checks direct childs only.
     * @returns nullptr if nothing found.
     */
    using GetInterfaceAtPoint = CInterface*(__thiscall*)(const CInterface* thisptr,
                                                         const CMqPoint* point,
                                                         bool deepCheck);
    GetInterfaceAtPoint getInterfaceAtPoint;

    /** Sets unknown6. */
    using Method29 = SmartPointer*(__thiscall*)(CInterface* thisptr, SmartPointer* a2);
    Method29 method29;

    /** Returns unknown6. */
    using Method30 = SmartPointer*(__thiscall*)(CInterface* thisptr);
    Method30 method30;

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
