/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "smartptr.h"

namespace game {

struct CUIManager;
struct CUIManagerData;
struct CUIManagerUnknownData;
struct Functor;
struct CMqPoint;
struct CMqUIKernelSimple;

struct CUIManager
{
    const void* vftable;
    CUIManagerData* data;
};

static_assert(sizeof(CUIManager) == 8, "Size of CUIManager structure must be exactly 8 bytes");

struct CUIManagerData
{
    void* uiController;
    CMqUIKernelSimple* uiKernel;
};

static_assert(sizeof(CUIManagerData) == 8,
              "Size of CUIManagerData structure must be exactly 8 bytes");

using UIManagerPtr = SmartPtr<CUIManager>;

struct CUIManagerUnknownData
{
    UIManagerPtr uiManager;
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
};

static_assert(sizeof(CUIManagerUnknownData) == 24,
              "Size of CUIManagerUnknownData structure must be exactly 24 bytes");

namespace CUIManagerApi {

struct Api
{
    using Get = UIManagerPtr*(__stdcall*)(UIManagerPtr* managerPtr);
    Get get;

    using CreateUnknownData = CUIManagerUnknownData*(__thiscall*)(CUIManager* thisptr,
                                                                  CUIManagerUnknownData* value,
                                                                  Functor* functor,
                                                                  int a4);
    CreateUnknownData createUnknownData;

    using UnknownDataCopy = CUIManagerUnknownData*(__thiscall*)(CUIManagerUnknownData* thisptr,
                                                                const CUIManagerUnknownData* src);
    UnknownDataCopy unknownDataCopy;

    using UnknownDataDestructor = void(__thiscall*)(CUIManagerUnknownData* thisptr);
    UnknownDataDestructor unknownDataDtor;

    using GetMousePosition = CMqPoint*(__thiscall*)(const CUIManager* thisptr, CMqPoint* value);
    GetMousePosition getMousePosition;
};

Api& get();

} // namespace CUIManagerApi

} // namespace game

#endif // UIMANAGER_H
