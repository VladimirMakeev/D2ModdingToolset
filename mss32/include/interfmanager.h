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

#include "d2list.h"
#include "functordispatch2.h"
#include "presentation.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CInterfManager;
struct CInterface;
struct CMqRect;
struct CTooltipImpl;
struct CCursorImpl;
struct IInterfBorderDisplay;

using GlobalKeyPressDispatch = CBFunctorDispatch2wRet<unsigned short, unsigned short, bool>;
using GlobalKeyPressDispatchPtr = SmartPtr<GlobalKeyPressDispatch>;

struct CInterfManagerVftable
{
    using Destructor = void(__thiscall*)(CInterfManager* thisptr, char flags);
    Destructor destructor;

    /** Returns true if interfaces list is empty. */
    using IsInterfacesEmpty = bool(__thiscall*)(const CInterfManager* thisptr);
    IsInterfacesEmpty isInterfacesEmpty;

    /** Returns topmost interface or nullptr if interfaces list is empty. */
    using GetInterface = CInterface*(__thiscall*)(CInterfManager* thisptr);
    GetInterface getInterface;

    void* unknown;

    /** Shows interface as top element. */
    using ShowInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interface);
    ShowInterface showInterface;

    /**
     * Removes topmost interface from list and returns it.
     * Second element in the list is chosen as the new top.
     */
    GetInterface removeInterface;

    /** Hides specified interface. */
    using HideInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interface);
    HideInterface hideInterface;

    /** Clears the list deleting all interfaces. */
    using ClearInterfaces = void(__thiscall*)(CInterfManager* thisptr);
    ClearInterfaces clearInterfaces;

    using SetBorderDisplay = void(__thiscall*)(CInterfManager* thisptr,
                                               IInterfBorderDisplay* borderDisplay);
    SetBorderDisplay setBorderDisplay;

    void* methods[4];

    using SetGlobalKeyHandler = void(__thiscall*)(CInterfManager* thisptr,
                                                  GlobalKeyPressDispatchPtr* handler);
    SetGlobalKeyHandler setGlobalKeyHandler;

    void* methods2[2];

    /** Returns game window resolution. */
    using GetResolution = const CMqRect*(__thiscall*)(const CInterfManager* thisptr);
    GetResolution getResolution;

    void* methods3[2];

    using GetTooltip = CTooltipImpl*(__thiscall*)(CInterfManager* thisptr);
    GetTooltip getTooltip;

    using GetCursor = CCursorImpl*(__thiscall*)(CInterfManager* thisptr);
    GetCursor getCursor;

    void* methods4[2];
};

static_assert(sizeof(CInterfManagerVftable) == 23 * sizeof(void*),
              "CInterfManager vftable must have exactly 23 methods");

struct CInterfManager
{
    const CInterfManagerVftable* vftable;
};

struct CInterfManagerImplData
{
    char unknown[8];
    SmartPtr<CTooltipImpl> tooltipImpl;
    SmartPtr<CCursorImpl> cursorImpl;
    /** Topmost interface is always first in the list. */
    List<CInterface*> interfaces;
    char unknown2[100];
    /**
     * Called before processing handleKeyboard() of topmost interface.
     * Used for checking special key combinations globally in the game.
     */
    GlobalKeyPressDispatchPtr onKeyPressed;
    char unknown3[28];
    IInterfBorderDisplay* borderDisplay;
    char unknown4[20];
};

static_assert(sizeof(CInterfManagerImplData) == 200,
              "Size of CInterfManagerImplData structure must be exactly 200 bytes");

struct CInterfManagerImpl
    : public CInterfManager
    , public IPresentation
{
    CInterfManagerImplData* data;
};

static_assert(sizeof(CInterfManagerImpl) == 12,
              "Size of CInterfManagerImpl structure must be exactly 12 bytes");

static_assert(offsetof(CInterfManagerImpl, CInterfManagerImpl::CInterfManager::vftable) == 0,
              "Vftable offset for CInterfManager in CInterfManagerImpl structure must be 0 bytes");

static_assert(offsetof(CInterfManagerImpl, CInterfManagerImpl::IPresentation::vftable) == 4,
              "Vftable offset for IPresentation in CInterfManagerImpl structure must be 4 bytes");

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
