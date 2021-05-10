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

#include "commandmsghooks.h"
#include "cmdbattlechooseactionmsg.h"
#include "cmdbattleendmsg.h"
#include "cmdbattleresultmsg.h"
#include "cmdbattlestartmsg.h"
#include "dynamiccast.h"
#include "netmsgutils.h"
#include "originalfunctions.h"

namespace hooks {

void __fastcall cmdBattleStartMsgSerializeHooked(game::CCmdBattleStartMsg* thisptr,
                                                 int /*%edx*/,
                                                 game::CMqStream* stream)
{
    serializeMsgWithBattleMsgData((game::CNetMsg*)thisptr, &thisptr->battleMsgData,
                                  getOriginalFunctions().cmdBattleStartMsgSerialize, stream);
}

void __fastcall cmdBattleChooseActionMsgSerializeHooked(game::CCmdBattleChooseActionMsg* thisptr,
                                                        int /*%edx*/,
                                                        game::CMqStream* stream)
{
    serializeMsgWithBattleMsgData((game::CNetMsg*)thisptr, &thisptr->battleMsgData,
                                  getOriginalFunctions().cmdBattleChooseActionMsgSerialize, stream);
}

void __fastcall cmdBattleResultMsgSerializeHooked(game::CCmdBattleResultMsg* thisptr,
                                                  int /*%edx*/,
                                                  game::CMqStream* stream)
{
    serializeMsgWithBattleMsgData((game::CNetMsg*)thisptr, &thisptr->battleMsgData,
                                  getOriginalFunctions().cmdBattleResultMsgSerialize, stream);
}

void __fastcall cmdBattleEndMsgSerializeHooked(game::CCmdBattleEndMsg* thisptr,
                                               int /*%edx*/,
                                               game::CMqStream* stream)
{
    serializeMsgWithBattleMsgData((game::CNetMsg*)thisptr, &thisptr->battleMsgData,
                                  getOriginalFunctions().cmdBattleEndMsgSerialize, stream);
}

/*
 * Had to patch it here since direct class destructors are not called in some places because they
 * are transformed to inline. CCommandMsg dtor is always called as function by derived classes.
 */
void __fastcall commandMsgDtorHooked(game::CCommandMsg* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto& battle = BattleMsgDataApi::get();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto battleResultMsg = (CCmdBattleResultMsg*)dynamicCast(thisptr, 0, rtti.CNetMsgType,
                                                             rtti.CCommandMsgTempl_10_0_Type, 0);
    if (battleResultMsg != nullptr) {
        battle.destructor(&battleResultMsg->battleMsgData);
        getOriginalFunctions().commandMsgDtor(thisptr);
        return;
    }

    auto battleChooseActionMsg = (CCmdBattleChooseActionMsg*)
        dynamicCast(thisptr, 0, rtti.CNetMsgType, rtti.CCommandMsgTempl_9_0_Type, 0);
    if (battleChooseActionMsg != nullptr) {
        battle.destructor(&battleChooseActionMsg->battleMsgData);
        getOriginalFunctions().commandMsgDtor(thisptr);
        return;
    }

    auto battleStartMsg = (CCmdBattleStartMsg*)dynamicCast(thisptr, 0, rtti.CNetMsgType,
                                                           rtti.CCommandMsgTempl_8_1_Type, 0);
    if (battleStartMsg != nullptr) {
        battle.destructor(&battleStartMsg->battleMsgData);
        getOriginalFunctions().commandMsgDtor(thisptr);
        return;
    }

    auto battleEndMsg = (CCmdBattleEndMsg*)dynamicCast(thisptr, 0, rtti.CNetMsgType,
                                                       rtti.CCommandMsgTempl_11_2_Type, 0);
    if (battleEndMsg != nullptr) {
        battle.destructor(&battleEndMsg->battleMsgData);
        getOriginalFunctions().commandMsgDtor(thisptr);
        return;
    }

    getOriginalFunctions().commandMsgDtor(thisptr);
}

} // namespace hooks
