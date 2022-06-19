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

#ifndef MENUBASEEDITOR_H
#define MENUBASEEDITOR_H

#include "fullscreeninterf.h"

namespace game::editor {

/** Base class for all menus used in Scenario Editor. */
struct CMenuBase : public CFullScreenInterf
{
    void* menuBaseData;
};

assert_size(CMenuBase, 20);

} // namespace game::editor

#endif // MENUBASEEDITOR_H
