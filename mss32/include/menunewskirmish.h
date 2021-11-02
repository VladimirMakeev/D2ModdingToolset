/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef MENUNEWSKIRMISH_H
#define MENUNEWSKIRMISH_H

#include "menubase.h"

namespace game {

struct String;
struct Functor;

/** Represents base class for different skirmish menus. */
struct CMenuNewSkirmish : public CMenuBase
{ };

namespace CMenuNewSkirmishApi {

struct Api
{
    /**
     * Initializes CMenuNewSkirmish object, creates menu based on dialogName.
     * Creates menu ui elements and assigns functors and hotkeys to them.
     * @param[in] thisptr pointer to existing CMenuNewSkirmish object.
     * @param a1 meaning unknown.
     * @param[in] dialogName name of dialog to create menu and ui elements from.
     * @returns thisptr.
     */
    using Constructor = CMenuNewSkirmish*(__thiscall*)(CMenuNewSkirmish* thisptr,
                                                       int a1,
                                                       const char* dialogName);
    Constructor constructor;

    CMenuBaseApi::Api::ButtonCallback loadScenarioCallback;

    /** Callback function signature used for CMenuNewSkirmish list box. */
    using ListBoxCallback = void(__thiscall*)(CMenuNewSkirmish* thisptr, int selectedIndex);
    ListBoxCallback listBoxCallback;

    /**
     * Creates functor for CMenuNewSkirmish list box.
     * @param[in] functor pointer to existing functor to initialize.
     * @param a2 meaning unknown.
     * @param[in] menu
     * @param[in] pointer to list box callback.
     * @returns initialized functor.
     */
    using CreateListBoxFunctor = Functor*(__stdcall*)(Functor* functor,
                                                      int a2,
                                                      CMenuNewSkirmish* menu,
                                                      const ListBoxCallback* callback);
    CreateListBoxFunctor createListBoxFunctor;

    /**
     * Callback function signature used for list box display text.
     * Used to show actual string values of list box display text ui element.
     * @param[in] thisptr pointer to menu.
     * @param[inout] string string to show in list box display text.
     * @param a3 meaning unknown.
     * @param selectedIndex index of currently selected element in list box.
     */
    using ListBoxDisplayTextCallback = void(__thiscall*)(CMenuNewSkirmish* thisptr,
                                                         String* string,
                                                         bool a3,
                                                         int selectedIndex);
    ListBoxDisplayTextCallback displayTextCallback;

    /**
     * Creates functor for CMenuNewSkirmish list box display text.
     * @param[in] functor pointer to existing functor to initialize.
     * @param a2 meaning unknown.
     * @param[in] menu
     * @param[in] pointer to list box display text callback.
     * @returns initialized functor.
     */
    using CreateListBoxDisplayTextFunctor =
        Functor*(__stdcall*)(Functor* functor,
                             int a2,
                             CMenuNewSkirmish* menu,
                             const ListBoxDisplayTextCallback* callback);
    CreateListBoxDisplayTextFunctor createListBoxDisplayTextFunctor;

    /**
     * Updates scenario related ui elements in menu.
     * @param unknown pointer returned from sub_6804ae().
     * @param dialog
     * @param selectedIndex index of currently selected list box item to update ui according to.
     */
    using UpdateScenarioUi = void(__stdcall*)(void* unknown,
                                              CDialogInterf* dialog,
                                              int selectedIndex);
    UpdateScenarioUi updateScenarioUi;
};

Api& get();

struct Vftable
{
    void* unknown[38];
};

Vftable* vftable();

} // namespace CMenuNewSkirmishApi

} // namespace game

#endif // MENUNEWSKIRMISH_H
