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

#ifndef PICKUPDROPINTERF_H
#define PICKUPDROPINTERF_H

#include "middatacache.h"
#include "middragdropinterf.h"
#include "resetstackext.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CPickUpDropInterfData
{
    CMidgardID stackId;
    int unknown;
    int unknown2;
    CMidgardID bagId;
    char unknown3[56];
};

static_assert(sizeof(CPickUpDropInterfData) == 72,
              "Size of CPickUpDropInterfData structure must be exactly 72 bytes");

/**
 * Shows window of exchange between stack and treasure chest.
 * Represents DLG_PICKUP_DROP from Interf.dlg.
 */
struct CPickUpDropInterf : public CMidDataCache2::INotify
{
    IResetStackExt resetStackExt;
    CMidDragDropInterf dragDropInterf;
    CPickUpDropInterfData* data;
};

static_assert(sizeof(CPickUpDropInterf) == 40,
              "Size of CPickUpDropInterf structure must be exactly 40 bytes");

static_assert(offsetof(CPickUpDropInterf, dragDropInterf) == 8,
              "CPickUpDropInterf::dragDropInterf offset must be 8 bytes");

namespace CPickUpDropInterfApi {

struct Api
{
    using Constructor = CPickUpDropInterf*(__thiscall*)(CPickUpDropInterf* thisptr,
                                                        void* taskOpenInterf,
                                                        CPhaseGame* phaseGame,
                                                        CMidgardID* stackId,
                                                        CMidgardID* bagId);
    Constructor constructor;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CPickUpDropInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CPickUpDropInterf* pickupInterf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;
};

Api& get();

} // namespace CPickUpDropInterfApi

} // namespace game

#endif // PICKUPDROPINTERF_H
