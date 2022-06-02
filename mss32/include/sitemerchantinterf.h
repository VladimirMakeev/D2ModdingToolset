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

static_assert(sizeof(CSiteMerchantInterfData) == 76,
              "Size of CSiteMerchantInterfData structure must be exactly 76 bytes");

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

static_assert(sizeof(CSiteMerchantInterf) == 40,
              "Size of CSiteMerchantInterf structure must be exactly 40 bytes");

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

} // namespace CSiteMerchantInterfApi

} // namespace game

#endif // SITEMERCHANTINTERF_H
