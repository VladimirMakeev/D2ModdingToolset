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

#include "presentation.h"
#include <cstddef>

namespace game {

struct CInterfManager;
struct CInterface;
struct SmartPointer;

struct CInterfManagerVftable
{
    void* unknown[4];

    /** Assumption: Shows interface as top element. */
    using ShowInterface = bool(__thiscall*)(CInterfManager* thisptr, CInterface* interface);
    ShowInterface showInterface;

    void* unknown2[18];
};

static_assert(sizeof(CInterfManagerVftable) == 23 * sizeof(void*),
              "CInterfManagerVftable vftable must have exactly 23 methods");

struct CInterfManager
{
    const CInterfManagerVftable* vftable;
};

/** Must be 200 bytes according to CInterfManagerImpl constructor. */
struct CInterfManagerImplData
{
    char unknown[200];
};

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

namespace CInterfManagerImplApi {

struct Api
{
    /** Populates SmartPointer with CInterfManagerImpl object. */
    using Get = SmartPointer*(__stdcall*)(SmartPointer* managerPtr);
    Get get;
};

Api& get();

} // namespace CInterfManagerImplApi

} // namespace game

#endif // INTERFMANAGER_H
