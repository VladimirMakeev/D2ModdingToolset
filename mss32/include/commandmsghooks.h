/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef COMMANDMSGHOOKS_H
#define COMMANDMSGHOOKS_H

namespace game {
struct CMqStream;
struct CCommandMsg;
struct CCmdBattleStartMsg;
struct CCmdBattleChooseActionMsg;
struct CCmdBattleResultMsg;
struct CCmdBattleEndMsg;
} // namespace game

namespace hooks {

void __fastcall cmdBattleStartMsgSerializeHooked(game::CCmdBattleStartMsg* thisptr,
                                                 int /*%edx*/,
                                                 game::CMqStream* stream);

void __fastcall cmdBattleChooseActionMsgSerializeHooked(game::CCmdBattleChooseActionMsg* thisptr,
                                                        int /*%edx*/,
                                                        game::CMqStream* stream);

void __fastcall cmdBattleResultMsgSerializeHooked(game::CCmdBattleResultMsg* thisptr,
                                                  int /*%edx*/,
                                                  game::CMqStream* stream);

void __fastcall cmdBattleEndMsgSerializeHooked(game::CCmdBattleEndMsg* thisptr,
                                               int /*%edx*/,
                                               game::CMqStream* stream);

void __fastcall commandMsgDtorHooked(game::CCommandMsg* thisptr, int /*%edx*/);

} // namespace hooks

#endif // COMMANDMSGHOOKS_H
