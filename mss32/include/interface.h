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
struct CursorHandle;

using CursorHandlePtr = SmartPtr<CursorHandle>;
using TooltipPtr = SmartPtr<Tooltip>;

struct CInterfaceData
{
    SmartPtr<CInterfManagerImpl> interfManager;
    CInterface* parent;
    CMqRect area;
    CInterfaceChilds childs;
    CursorHandlePtr cursor;
    TooltipPtr tooltip;
};

assert_size(CInterfaceData, 64);
assert_offset(CInterfaceData, childs, 28);

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

    /** Handles mouse position changes. */
    using OnMouseMove = int(__thiscall*)(CInterface* thisptr, const CMqPoint* mousePosition);
    OnMouseMove onMouseMove;

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

    /** Returns cursor handle of the interface element under the point. */
    using GetCursorAtPoint = CursorHandlePtr*(__thiscall*)(CInterface* thisptr,
                                                           CursorHandlePtr* ptr,
                                                           const CMqPoint* point);
    GetCursorAtPoint getCursorAtPoint;

    /** Returns tooltip of the interface element under the point. */
    using GetTooltipAtPoint = TooltipPtr*(__thiscall*)(CInterface* thisptr,
                                                       TooltipPtr* tooltip,
                                                       const CMqPoint* point);
    GetTooltipAtPoint getTooltipAtPoint;

    /** Called from CInterfManagerImpl::OnMousePress */
    using Method8 = CMqPoint*(__thiscall*)(CInterface* thisptr, CMqPoint* p1, CMqPoint* p2);
    Method8 method8;

    /**
     * Meaning assumed.
     * If returns true then IInterfBorderDisplay should be also drawn.
     * Check CInterfManagerImplDraw for details.
     */
    using DrawBorder = bool(__thiscall*)(const CInterface* thisptr);
    DrawBorder shouldDrawBorder;

    /**
     * Meaning unknown.
     * CInterface returns false,
     * CInterfDialog checks interface below it and call its method, or returns false.
     * CMainView2 returns true, while CFullScreenInterf returns false.
     */
    using Method10 = bool(__thiscall*)(CInterface* thisptr);
    Method10 method10;

    using Method11 = int(__thiscall*)(CInterface* thisptr);
    Method11 method11;

    using GetArea = CMqRect*(__thiscall*)(CInterface* thisptr);
    GetArea getArea;

    using SetArea = bool(__thiscall*)(CInterface* thisptr, const CMqRect* area);
    SetArea setArea;

    /** Returns true if specified point is inside interface element area. */
    using IsPointInside = bool(__thiscall*)(const CInterface* thisptr, const CMqPoint* point);
    IsPointInside isPointInside;

    /** Returns area for CMidPopupInterf, for CInterface returns nullptr. */
    GetArea getArea2;

    /** If returns true, we can call getArea2, result will be non-null.*/
    using Method16 = bool(__thiscall*)(CInterface* thisptr);
    Method16 method16;

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

    /** Sets cursor handle. */
    using SetCursor = CursorHandlePtr*(__thiscall*)(CInterface* thisptr, CursorHandlePtr* cursor);
    SetCursor setCursor;

    /** Returns cursor handle. */
    using GetCursor = CursorHandlePtr*(__thiscall*)(CInterface* thisptr);
    GetCursor getCursor;

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

assert_vftable_size(CInterfaceVftable, 34);

} // namespace game

#endif // INTERFACE_H
