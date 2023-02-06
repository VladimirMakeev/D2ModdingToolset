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

#ifndef SITEMERCHANTINTERF_H
#define SITEMERCHANTINTERF_H

#include "middatacache.h"
#include "middragdropinterf.h"
#include "resetstackext.h"
#include "smartptr.h"

namespace game {

struct CSiteMerchantInterfData
{
    CMidgardID stackId;
    int unknown;
    int unknown2;
    CMidgardID merchantId;
    char unknown3[60];
};

assert_size(CSiteMerchantInterfData, 76);

/**
 * Shows merchant trade window.
 * Represents DLG_MERCHANT from Interf.dlg.
 */
struct CSiteMerchantInterf : public CMidDataCache2::INotify
{
    IResetStackExt resetStackExt;
    CMidDragDropInterf dragDropInterf;
    CSiteMerchantInterfData* data;
};

assert_size(CSiteMerchantInterf, 40);

namespace CSiteMerchantInterfApi {

struct Api
{
    using Constructor = CSiteMerchantInterf*(__thiscall*)(CSiteMerchantInterf* thisptr,
                                                          void* taskOpenInterf,
                                                          CPhaseGame* phaseGame,
                                                          CMidgardID* stackId,
                                                          CMidgardID* merchantId);
    Constructor constructor;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CSiteMerchantInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CSiteMerchantInterf* merchantInterf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;
};

Api& get();

struct Vftable
{
    const CMidDataCache2::INotifyVftable* notify;
    const IResetStackExtVftable* resetStackExt;
    const CInterfaceVftable* dragDropInterf;
    const ITaskManagerHolderVftable* taskManagerHolder;
    const IMidDropManagerVftable* dropManager;
};

Vftable& vftable();

} // namespace CSiteMerchantInterfApi

} // namespace game

#endif // SITEMERCHANTINTERF_H
