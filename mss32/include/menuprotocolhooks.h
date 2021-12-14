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

#ifndef MENUPROTOCOLHOOKS_H
#define MENUPROTOCOLHOOKS_H

namespace game {
struct CMenuProtocol;
struct CMenuPhase;
struct String;
} // namespace game

namespace hooks {

game::CMenuProtocol* __fastcall menuProtocolCtorHooked(game::CMenuProtocol* thisptr,
                                                       int /*%edx*/,
                                                       game::CMenuPhase* menuPhase);

void __fastcall menuProtocolDisplayCallbackHooked(game::CMenuProtocol* thisptr,
                                                  int /*%edx*/,
                                                  game::String* string,
                                                  bool a3,
                                                  int selectedIndex);

void __fastcall menuProtocolContinueHandlerHooked(game::CMenuProtocol* thisptr, int /*%edx*/);

} // namespace hooks

#endif // MENUPROTOCOLHOOKS_H
