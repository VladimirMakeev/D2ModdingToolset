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

#ifndef MENUBASE_H
#define MENUBASE_H

#include "interffullscreen.h"
#include <cstddef>

namespace game {

struct CDialogInterf;

struct CMenuBaseData
{
    char unknown[48];
};

/** Base class for all menus. */
struct CMenuBase : public CInterfFullScreen
{
    CMenuBaseData* menuBaseData;
};

static_assert(sizeof(CMenuBase) == 12, "Size of CMenuBase structure must be exactly 12 bytes");

static_assert(offsetof(CMenuBase, menuBaseData) == 8,
              "CMenuBase::menuBaseData offset must be 8 bytes");

namespace CMenuBaseApi {

struct Api
{
    /**
     * Initializes CMenuBase object, sets vftable.
     * @param[in] thisptr pointer to existing CMenuBase object.
     * @param a1 meaning unknown.
     * @returns thisptr.
     */
    using Constructor = CMenuBase*(__thiscall*)(CMenuBase* thisptr, int a1);
    Constructor constructor;

    /** Creates ui elements of specified dialog to be used in menu. */
    using CreateMenu = int(__thiscall*)(CMenuBase* thisptr, const char* dialogName);
    CreateMenu createMenu;

    /** Returns CDialogInterf object. */
    using GetDialogInterface = CDialogInterf*(__thiscall*)(CMenuBase* thisptr);
    GetDialogInterface getDialogInterface;
};

Api& get();

} // namespace CMenuBaseApi

} // namespace game

#endif // MENUBASE_H
