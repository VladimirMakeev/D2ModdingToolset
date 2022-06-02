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

#ifndef EFFECTINTERF_H
#define EFFECTINTERF_H

#include "popupinterf.h"
#include "smartptr.h"

namespace game {

struct CMidEvEffect;
struct CMidgardID;
struct LEventEffectCategory;
struct IMidgardObjectMap;
struct String;

namespace editor {

struct CEffectInterfHandler;

struct CEffectInterfData
{
    CEffectInterfHandler* interfHandler;
    int unknown;
};

static_assert(sizeof(CEffectInterfData) == 8,
              "Size of CEffectInterfData structure must be exactly 8 bytes");

/** Base class for all event effects popup windows. */
struct CEffectInterf : public CPopupInterf
{
    CEffectInterfData* data;
};

static_assert(sizeof(CEffectInterf) == 28,
              "Size of CEffectInterf structure must be exactly 28 bytes");

struct CEffectInterfVftable : public CInterfaceVftable
{
    using SetEventEffect = bool(__thiscall*)(CEffectInterf* thisptr, CMidEvEffect* eventEffect);
    SetEventEffect setEventEffect;
};

static_assert(sizeof(CEffectInterfVftable) == 35 * sizeof(void*),
              "CEffectInterf vftable must have exactly 35 methods");

namespace CEffectInterfApi {

struct Api
{
    using Constructor = CEffectInterf*(__thiscall*)(CEffectInterf* thisptr,
                                                    const char* dialogName,
                                                    ITask* task);
    Constructor constructor;

    /** Creates effect interface from specified category. */
    using CreateFromCategory = CEffectInterf*(__stdcall*)(ITask* task,
                                                          void* unknown,
                                                          const CMidgardID* eventId,
                                                          const LEventEffectCategory* category);
    CreateFromCategory createFromCategory;

    using GetObjectMap = IMidgardObjectMap*(__thiscall*)(void* thisptr);
    GetObjectMap getObjectMap;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CEffectInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int a2,
                                                          CEffectInterf* effectInterf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;

    struct ListBoxDisplayCallback
    {
        using Callback = void(__thiscall*)(CEffectInterf* thisptr,
                                           String* string,
                                           bool,
                                           int selectedIndex);
        Callback callback;
        int unknown;
    };

    using CreateListBoxDisplayFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                                  int a2,
                                                                  CEffectInterf* effectInterf,
                                                                  ListBoxDisplayCallback* callback);
    CreateListBoxDisplayFunctor createListBoxDisplayFunctor;
};

Api& get();

} // namespace CEffectInterfApi

} // namespace editor

} // namespace game

#endif // EFFECTINTERF_H
