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

#ifndef MIDMSGSENDER_H
#define MIDMSGSENDER_H

namespace game {

struct IMidMsgSenderVftable;
struct CMidgardID;
struct CCommandMsg;
struct CBatLogic;
struct BattleMsgData;

struct IMidMsgSender
{
    IMidMsgSenderVftable* vftable;
};

struct IMidMsgSenderVftable
{
    using Destructor = void(__thiscall*)(IMidMsgSender* thisptr, char flags);
    Destructor destructor;

    /** Sends objects changes using CRefreshInfo and CCmdEraseObj messages. */
    using SendObjectsChanges = bool(__thiscall*)(IMidMsgSender* thisptr);
    SendObjectsChanges sendObjectsChanges;

    /** Sets player id to the message and calls 'sendMessage'. */
    using SendPlayerMessage = int(__thiscall*)(IMidMsgSender* thisptr,
                                               CCommandMsg* commandMsg,
                                               const CMidgardID* playerId,
                                               char a4);
    SendPlayerMessage sendPlayerMessage;

    /**
     * Sends command message.
     * CMidServerLogic checks for player info, message queue
     * and command message type before sending.
     */
    using SendMessage = int(__thiscall*)(IMidMsgSender* thisptr, CCommandMsg* commandMsg, char a3);
    SendMessage sendMessage;

    /** Starts battle between two unit groups. */
    using StartBattle = int(__thiscall*)(IMidMsgSender* thisptr,
                                         bool duelMode,
                                         const CMidgardID* attackerGroupId,
                                         const CMidgardID* defenderGroupId);
    StartBattle startBattle;

    /** Ends battle and applies battle results. */
    using EndBattle = char(__thiscall*)(IMidMsgSender* thisptr,
                                        CBatLogic* batLogic,
                                        BattleMsgData* battleMsgData);
    EndBattle endBattle;

    /** Adds a single fast battle action to the AI message queue. */
    using FastBattleAction = bool(__thiscall*)(IMidMsgSender* thisptr,
                                               const BattleMsgData* battleMsgData,
                                               const CMidgardID* unitId);
    FastBattleAction fastBattleAction;

    /**
     * Meaning assumed.
     * Set to 1 before sending CCmdUpdateLeadersMsg in the method of CEffectPlayerTurn class.
     * Reset to 0 in another method of CEffectPlayerTurn.
     */
    using SetUpgradeLeaders = bool(__thiscall*)(IMidMsgSender* thisptr, bool value);
    SetUpgradeLeaders setUpgradeLeaders;

    /** Sets player with specified as scenario winner. In case of empty id all players lose. */
    using WinScenario = bool(__thiscall*)(IMidMsgSender* thisptr, const CMidgardID* winnerPlayerId);
    WinScenario winScenario;

    /**
     * Meaning assumed.
     * Sends CCmdExportLeaderMsg command.
     */
    using ExportLeaders = bool(__thiscall*)(IMidMsgSender* thisptr);
    ExportLeaders exportLeaders;
};

static_assert(sizeof(IMidMsgSenderVftable) == 10 * sizeof(void*),
              "Size of IMidMsgSender vftable must have exactly 10 methods");

} // namespace game

#endif // MIDMSGSENDER_H
