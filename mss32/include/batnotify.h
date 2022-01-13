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

#ifndef BATNOTIFY_H
#define BATNOTIFY_H

namespace game {

struct IBatNotifyVftable;
struct BattleMsgData;
struct CMidgardID;

enum class BattleAction : int;

struct IBatNotify
{
    IBatNotifyVftable* vftable;
};

struct IBatNotifyVftable
{
    using Destructor = void(__thiscall*)(IBatNotify* thisptr, char flags);
    Destructor destructor;

    /** Sends CStackBattleActionMsg from client to server. */
    using NotifyBattleAction = void(__thiscall*)(IBatNotify* thisptr,
                                                 const BattleMsgData* battleMsgData,
                                                 BattleAction action,
                                                 const CMidgardID* attackerUnitId,
                                                 const CMidgardID* targetUnitId);
    NotifyBattleAction notifyBattleAction;

    /** Same as calling notifyBattleAction() with action == BattleAction::UseItem. */
    using NotifyUseItem = void(__thiscall*)(IBatNotify* thisptr,
                                            const BattleMsgData* battleMsgData,
                                            const CMidgardID* attackerUnitId,
                                            const CMidgardID* targetUnitId);
    NotifyUseItem notifyUseItem;

    /** Handles CCmdBattleResultMsg messages. */
    using BattleResult = void(__thiscall*)(IBatNotify* thisptr);
    BattleResult battleResult;

    /** Closes battle window and shows stack management window if specified. */
    using CloseBattleWindow = void(__thiscall*)(IBatNotify* thisptr, bool requestPaperdoll);
    CloseBattleWindow closeBattleWindow;

    /** Handles chat window during battle. */
    using OpenChatWindow = void(__thiscall*)(IBatNotify* thisptr);
    OpenChatWindow openChatWindow;
};

} // namespace game

#endif // BATNOTIFY_H
