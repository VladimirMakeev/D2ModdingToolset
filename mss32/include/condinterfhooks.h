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

#ifndef CONDINTERFHOOKS_H
#define CONDINTERFHOOKS_H

namespace game {
struct ITask;
struct CMidgardID;
struct LEventCondCategory;

namespace editor {
struct CCondInterf;
}

} // namespace game

namespace hooks {

game::editor::CCondInterf* __stdcall createCondInterfFromCategoryHooked(
    game::ITask* task,
    void* a2,
    const game::CMidgardID* eventId,
    const game::LEventCondCategory* category);

} // namespace hooks

#endif // CONDINTERFHOOKS_H
