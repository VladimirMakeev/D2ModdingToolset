/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef AIGIVEITEMSACTIONHOOKS_H
#define AIGIVEITEMSACTIONHOOKS_H

namespace game {
struct IMidgardObjectMap;
struct CMidServerLogicData;
struct IAiTacticInternal;
struct CAiGiveItemsAction;
} // namespace game

namespace hooks {

void __fastcall aiGiveItemsActionExecuteHooked(game::CAiGiveItemsAction* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::CMidServerLogicData* serverLogicData,
                                               game::IAiTacticInternal* tacticInternal);

} // namespace hooks

#endif // AIGIVEITEMSACTIONHOOKS_H
