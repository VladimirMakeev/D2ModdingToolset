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

#ifndef CONDINTERF_H
#define CONDINTERF_H

#include "popupinterf.h"

namespace game {

struct CMidEvCondition;
struct CMidgardID;
struct LEventCondCategory;
struct Functor;
struct IMidgardObjectMap;

namespace editor {

struct CCondInterfHandler;

struct CCondInterfData
{
    CCondInterfHandler* interfHandler;
};

/** Base class for all event conditions popup windows. */
struct CCondInterf : public CPopupInterf
{
    CCondInterfData* condData;
};

static_assert(sizeof(CCondInterf) == 28, "Size of CCondInterf structure must be exactly 28 bytes");

struct CCondInterfVftable : public CInterfaceVftable
{
    using SetEventCondition = bool(__thiscall*)(CCondInterf* thisptr,
                                                CMidEvCondition* eventCondition);
    SetEventCondition setEventCondition;
};

static_assert(sizeof(CCondInterfVftable) == 35 * sizeof(void*),
              "CCondInterf vftable must have exactly 35 methods");

namespace CCondInterfApi {

struct Api
{
    using Constructor = CCondInterf*(__thiscall*)(CCondInterf* thisptr,
                                                  const char* dialogName,
                                                  ITask* task);
    Constructor constructor;

    /** Creates condtion interface from specified category. */
    using CreateFromCategory = CCondInterf*(__stdcall*)(ITask* task,
                                                        void* a2,
                                                        const CMidgardID* eventId,
                                                        const LEventCondCategory* category);
    CreateFromCategory createFromCategory;

    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CCondInterf* thisptr);

        Callback callback;
        int unknown;
    };

    // This is a hack. There is no functor that operates on CCondInterf itself, only derived types.
    // Instead of implementing custom functor for our needs we use button functor
    // from CCondFrequencyInterf. Its safe because all functors for derived types
    // share the same implementation and callbacks signatures.
    // We could reuse functor of any other derived type.
    using CreateButtonFunctor = Functor*(__stdcall*)(Functor* functor,
                                                     int a2,
                                                     CCondInterf* condInterf,
                                                     ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;

    struct RadioButtonCallback
    {
        using Callback = void(__thiscall*)(CCondInterf* thisptr, int selectedButton);

        Callback callback;
        int unknown;
    };

    // Same hack as with button. Here we reusing logic from CCondStackExistsInterf.
    using CreateRadioButtonFunctor = Functor*(__stdcall*)(Functor* functor,
                                                          int a2,
                                                          CCondInterf* condInterf,
                                                          RadioButtonCallback* callback);
    CreateRadioButtonFunctor createRadioButtonFunctor;

    using GetObjectMap = IMidgardObjectMap*(__thiscall*)(void* thisptr);
    GetObjectMap getObjectMap;
};

Api& get();

} // namespace CCondInterfApi

} // namespace editor

} // namespace game

#endif // CONDINTERF_H
