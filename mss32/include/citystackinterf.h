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

static_assert(sizeof(CCityStackInterfData) == 136,
              "Size of CCityStackInterfData structure must be exactly 136 bytes");

static_assert(offsetof(CCityStackInterfData, reinfGroup) == 20,
              "CCityStackInterfData::reinfGroup offset must be 20 bytes");

static_assert(offsetof(CCityStackInterfData, fortInventoryDisplay) == 28,
              "CCityStackInterfData::fortInventoryDisplay offset must be 28 bytes");

static_assert(offsetof(CCityStackInterfData, dismissUnit) == 56,
              "CCityStackInterfData::dismissUnit offset must be 56 bytes");

static_assert(offsetof(CCityStackInterfData, fortificationId) == 92,
              "CCityStackInterfData::fortificationId offset must be 92 bytes");

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

static_assert(sizeof(CCityStackInterf) == 44,
              "Size of CCityStackInterf structure must be exactly 44 bytes");

static_assert(offsetof(CCityStackInterf, dragDropInterf) == 12,
              "CCityStackInterf::dragDropInterf offset must be 12 bytes");

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

} // namespace CCityStackInterfApi

} // namespace game

#endif // CITYSTACKINTERF_H
