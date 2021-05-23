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

#include "netmsghooks.h"
#include "dynamiccast.h"
#include "mqstream.h"
#include "netmsgutils.h"
#include "originalfunctions.h"
#include "stackbattleactionmsg.h"

namespace hooks {

void __fastcall stackBattleActionMsgSerializeHooked(game::CStackBattleActionMsg* thisptr,
                                                    int /*%edx*/,
                                                    game::CMqStream* stream)
{
    serializeMsgWithBattleMsgData(thisptr, &thisptr->battleMsgData,
                                  getOriginalFunctions().stackBattleActionMsgSerialize, stream);
}

void __fastcall netMsgDtorHooked(game::CNetMsg* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    CStackBattleActionMsg* stackBattleActionMsg = (CStackBattleActionMsg*)
        dynamicCast(thisptr, 0, rtti.CNetMsgType, rtti.CStackBattleActionMsgType, 0);
    if (stackBattleActionMsg != nullptr) {
        BattleMsgDataApi::get().destructor(&stackBattleActionMsg->battleMsgData);
    }

    getOriginalFunctions().netMsgDtor(thisptr);
}

} // namespace hooks
