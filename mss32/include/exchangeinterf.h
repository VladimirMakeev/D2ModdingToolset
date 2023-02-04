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

#ifndef EXHANGEINTERF_H
#define EXHANGEINTERF_H

#include "middatacache.h"
#include "middragdropinterf.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CExchangeInterfData
{
    CMidgardID stackLeftSideId;
    int unknown;
    int unknown2;
    CMidgardID stackRightSideId;
    char unknown3[168];
};

assert_size(CExchangeInterfData, 184);

/**
 * Shows window of exchange between two stacks.
 * Represents DLG_EXCHANGE from Interf.dlg.
 */
struct CExchangeInterf : public CMidDataCache2::INotify
{
    CMidDragDropInterf dragDropInterf;
    CExchangeInterfData* data;
};

assert_size(CExchangeInterf, 36);
assert_offset(CExchangeInterf, data, 32);

namespace CExchangeInterfApi {
struct Api
{
    using Constructor = CExchangeInterf*(__thiscall*)(CExchangeInterf* thisptr,
                                                      void* taskOpenInterf,
                                                      CPhaseGame* phaseGame,
                                                      CMidgardID* stackLeftSide,
                                                      CMidgardID* stackRightSide);
    Constructor constructor;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CExchangeInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CExchangeInterf* exchangeInterf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;
};

Api& get();

struct Vftable
{
    const CMidDataCache2::INotifyVftable* notify;
    const CInterfaceVftable* dragDropInterf;
    const ITaskManagerHolderVftable* taskManagerHolder;
    const IMidDropManagerVftable* dropManager;
};

Vftable& vftable();

} // namespace CExchangeInterfApi

} // namespace game

#endif // EXHANGEINTERF_H
