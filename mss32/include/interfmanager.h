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

#ifndef INTERFMANAGER_H
#define INTERFMANAGER_H

#include "d2assert.h"
#include "d2list.h"
#include "functordispatch2.h"
#include "mqpresentationmanager.h"
#include "mqrect.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CInterfManager;
struct CInterface;
struct CTooltipImpl;
struct CCursorImpl;
struct IInterfBorderDisplay;

using GlobalKeyPressDispatch = CBFunctorDispatch2wRet<unsigned short, unsigned short, bool>;
using GlobalKeyPressDispatchPtr = SmartPtr<GlobalKeyPressDispatch>;

struct CInterfManagerVftable
{
    using Destructor = void(__thiscall*)(CInterfManager* thisptr, char flags);
    Destructor destructor;

    using GetBool = bool(__thiscall*)(const CInterfManager* thisptr);

    /** Returns true if interfaces list is empty. */
    GetBool isInterfacesEmpty;

    /** Returns topmost interface or nullptr if interfaces list is empty. */
    using GetInterface = CInterface*(__thiscall*)(CInterfManager* thisptr);
    GetInterface getInterface;

    /**
     * Returns next interface in list after specified one or nullptr.
     * Assumption: returns interface that is below specified.
     */
    using GetNextInterface = CInterface*(__thiscall*)(CInterfManager* thisptr, CInterface* interf);
    GetNextInterface getNextInterface;

    /** Shows interface as top element. */
    using ShowInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interf);
    ShowInterface showInterface;

    /**
     * Removes topmost interface from list and returns it.
     * Second element in the list is chosen as the new top.
     */
    GetInterface removeInterface;

    /** Hides specified interface. */
    using HideInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interf);
    HideInterface hideInterface;

    /** Clears the list deleting all interfaces. */
    using ClearInterfaces = void(__thiscall*)(CInterfManager* thisptr);
    ClearInterfaces clearInterfaces;

    using SetBorderDisplay = void(__thiscall*)(CInterfManager* thisptr,
                                               IInterfBorderDisplay* borderDisplay);
    SetBorderDisplay setBorderDisplay;

    using SetRememberedInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interf);
    SetRememberedInterface setRememberedInterface;

    using ClearRememberedInterface = void(__thiscall*)(CInterfManager* thisptr);
    ClearRememberedInterface clearRememberedInterface;

    GetInterface getRememberedInterface;

    using SetMousePosition = void(__thiscall*)(CInterfManager* thisptr, const CMqPoint* position);
    SetMousePosition setMousePosition;

    using SetGlobalKeyHandler = void(__thiscall*)(CInterfManager* thisptr,
                                                  GlobalKeyPressDispatchPtr* handler);
    SetGlobalKeyHandler setGlobalKeyHandler;

    using SetInputAllowed = void(__thiscall*)(CInterfManager* thisptr, bool allowed);
    SetInputAllowed setInputAllowed;

    GetBool isInputAllowed;

    /** Returns game window resolution. */
    using GetResolution = const CMqRect*(__thiscall*)(const CInterfManager* thisptr);
    GetResolution getResolution;

    using SetResolution = void(__thiscall*)(CInterfManager* thisptr);
    SetResolution setResolution;

    using GetPaletteKey = std::uint32_t(__thiscall*)(const CInterfManager* thisptr);
    GetPaletteKey getPaletteKey;

    using GetTooltip = CTooltipImpl*(__thiscall*)(CInterfManager* thisptr);
    GetTooltip getTooltip;

    using GetCursor = CCursorImpl*(__thiscall*)(CInterfManager* thisptr);
    GetCursor getCursor;

    using ConvertPosition = CMqPoint*(__thiscall*)(CInterfManager* thisptr,
                                                   CMqPoint* result,
                                                   CMqPoint* point);

    ConvertPosition method21;
    ConvertPosition method22;
};

assert_vftable_size(CInterfManagerVftable, 23);

struct CInterfManager
{
    const CInterfManagerVftable* vftable;
};

struct CInterfManagerImplData
{
    SmartPtr<CMqPresentationManager> presentationMgr;
    SmartPtr<CTooltipImpl> tooltipImpl;
    SmartPtr<CCursorImpl> cursorImpl;
    /** Topmost interface is always first in the list. */
    List<CInterface*> interfaces;
    UiEvent visibilityEvent;
    UiEvent keypressEvent;
    UiEvent mousePressEvent;
    UiEvent mouseMoveEvent;
    CInterface* rememberedInterface;
    /**
     * Called before processing handleKeyboard() of topmost interface.
     * Used for checking special key combinations globally in the game.
     */
    GlobalKeyPressDispatchPtr onKeyPressed;
    bool inputAllowed;
    char padding[3];
    CMqRect resolution;
    std::uint32_t paletteKey;
    bool unknown5;
    char padding2[3];
    IInterfBorderDisplay* borderDisplay;
    bool unknown3;
    char padding3[3];
    CMqRect area;
};

assert_size(CInterfManagerImplData, 200);

struct CInterfManagerImpl
    : public CInterfManager
    , public CMqPresentationManager::IPresentation
{
    CInterfManagerImplData* data;
};

assert_size(CInterfManagerImpl, 12);
assert_offset(CInterfManagerImpl, CInterfManagerImpl::CInterfManager::vftable, 0);
assert_offset(CInterfManagerImpl, CInterfManagerImpl::IPresentation::vftable, 4);

using InterfManagerImplPtr = SmartPtr<CInterfManagerImpl>;

namespace CInterfManagerImplApi {

struct Api
{
    /** Populates SmartPointer with CInterfManagerImpl object. */
    using Get = InterfManagerImplPtr*(__stdcall*)(InterfManagerImplPtr* managerPtr);
    Get get;
};

Api& get();

} // namespace CInterfManagerImplApi

} // namespace game

#endif // INTERFMANAGER_H
