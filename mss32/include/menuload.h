/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef MENULOAD_H
#define MENULOAD_H

#include "menubase.h"

namespace game {

/** Base class for all menus that loads previously saved game. */
struct CMenuLoad : public CMenuBase
{ };

namespace CMenuLoadApi {

struct Api
{
    using ButtonCallback = void(__thiscall*)(CMenuLoad* thisptr);
    /** Loads selected game save file. */
    ButtonCallback buttonLoadCallback;

    /** Creates host player client and requests game version from server. */
    using CreateHostPlayer = void(__thiscall*)(CMenuLoad* thisptr);
    CreateHostPlayer createHostPlayer;
};

Api& get();

} // namespace CMenuLoadApi

} // namespace game

#endif // MENULOAD_H
