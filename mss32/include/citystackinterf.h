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

#ifndef CITYSTACKINTERF_H
#define CITYSTACKINTERF_H

#include "midcommandqueue2.h"
#include "middatacache.h"
#include "middragdropinterf.h"
#include "resetstackext.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CDDReinfGroup;
struct CDDFortInventoryDisplay;
struct CDismissUnit;
struct CDynLevelUnit;
struct CHealUnit;
struct CReviveUnit;

struct CCityStackInterfData
{
    int unknown;
    int unknown2;
    int unknown3;
    CMidgardID unknownId;
    int unknown4;
    CDDReinfGroup* reinfGroup;
    CMidgardID unknownId2;
    CDDFortInventoryDisplay* fortInventoryDisplay;
    char unknown5[24];
    CDismissUnit* dismissUnit;
    CDynLevelUnit* dynLevelUnit;
    CHealUnit* healUnit;
    CReviveUnit* reviveUnit;
    char unknown6[20];
    CMidgardID fortificationId;
    char unknown7[40];
};

assert_size(CCityStackInterfData, 136);
assert_offset(CCityStackInterfData, reinfGroup, 20);
assert_offset(CCityStackInterfData, fortInventoryDisplay, 28);
assert_offset(CCityStackInterfData, dismissUnit, 56);
assert_offset(CCityStackInterfData, fortificationId, 92);

/**
 * Shows stack, garrison and unit hire window of cities and capitals.
 * Represents DLG_CITY_STACK from Interf.dlg.
 */
struct CCityStackInterf : public CMidDataCache2::INotify
{
    CMidCommandQueue2::INotifyCQ commandQueueNotify;
    IResetStackExt* resetStackExt;
    CMidDragDropInterf dragDropInterf;
    CCityStackInterfData* data;
};

assert_size(CCityStackInterf, 44);
assert_offset(CCityStackInterf, dragDropInterf, 12);

namespace CCityStackInterfApi {

struct Api
{
    using Constructor = CCityStackInterf*(__thiscall*)(CCityStackInterf* thisptr,
                                                       void* taskOpenInterf,
                                                       CPhaseGame* phaseGame,
                                                       CMidgardID* cityId);
    Constructor constructor;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CCityStackInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CCityStackInterf* cityStackInterf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;
};

Api& get();

struct Vftable
{
    const CMidDataCache2::INotifyVftable* notify;
    const CMidCommandQueue2::INotifyCQVftable* commandQueueNotify;
    const IResetStackExtVftable* resetStackExt;
    const CInterfaceVftable* dragDropInterf;
    const ITaskManagerHolderVftable* taskManagerHolder;
    const IMidDropManagerVftable* dropManager;
};

Vftable& vftable();

} // namespace CCityStackInterfApi

} // namespace game

#endif // CITYSTACKINTERF_H
