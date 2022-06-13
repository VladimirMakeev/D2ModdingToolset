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

assert_size(CEffectInterfData, 8);

/** Base class for all event effects popup windows. */
struct CEffectInterf : public CPopupInterf
{
    CEffectInterfData* data;
};

assert_size(CEffectInterf, 28);

struct CEffectInterfVftable : public CInterfaceVftable
{
    using SetEventEffect = bool(__thiscall*)(CEffectInterf* thisptr, CMidEvEffect* eventEffect);
    SetEventEffect setEventEffect;
};

assert_vftable_size(CEffectInterfVftable, 35);

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
