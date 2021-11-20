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

#ifndef MQNETPLAYER_H
#define MQNETPLAYER_H

namespace game {

struct String;
struct IMqNetSession;
struct IMqNetSystem;
struct NetMessageHeader;
struct IMqNetPlayerVftable;

template <typename T = IMqNetPlayerVftable>
struct IMqNetPlayerT
{
    const T* vftable;
};

/**
 * Interface for network players.
 * Player can be client or a server.
 * Single player and hotseat game logic also uses this interface.
 */
struct IMqNetPlayer : public IMqNetPlayerT<>
{ };

struct IMqNetPlayerVftable
{
    using Destructor = void(__thiscall*)(IMqNetPlayer* thisptr, char flags);
    Destructor destructor;

    /** Returns player name. */
    using GetName = String*(__thiscall*)(IMqNetPlayer* thisptr, String* string);
    GetName getName;

    /** Returns player network id. */
    using GetNetId = int(__thiscall*)(IMqNetPlayer* thisptr);
    GetNetId getNetId;

    /** Returns the session the player is currently in. */
    using GetSession = IMqNetSession*(__thiscall*)(IMqNetPlayer* thisptr);
    GetSession getSession;

    /**
     * Returns number of network messages in queue for player.
     * CNetDPlayPlayer calls IDirectPlay4::GetMessageCount here.
     */
    using GetMessageCount = int(__thiscall*)(IMqNetPlayer* thisptr);
    GetMessageCount getMessageCount;

    /**
     * Sends network message to player with specified id.
     * CNetDPlayPlayer calls IDirectPlay4::SendEx here.
     * @returns true if the message was sent successfully.
     */
    using SendNetMessage = bool(__thiscall*)(IMqNetPlayer* thisptr,
                                             int idTo,
                                             const NetMessageHeader* message);
    SendNetMessage sendMessage;

    /**
     * Receives network messages addressed to this player.
     * CNetDPlayPlayer calls IDirectPlay4::Receive with DPRECEIVE_TOPLAYER | DPRECEIVE_ALL here.
     * @param[inout] idFrom pointer to store network id of messages sender.
     * @param[inout] buffer buffer to receive messages. Must be at least 512Kb.
     */
    using ReceiveMessage = int(__thiscall*)(IMqNetPlayer* thisptr,
                                            int* idFrom,
                                            NetMessageHeader* buffer);
    ReceiveMessage receiveMessage;

    /** Sets netSystem proxy for this player. */
    using SetNetSystem = void(__thiscall*)(IMqNetPlayer* thisptr, IMqNetSystem* netSystem);
    SetNetSystem setNetSystem;

    using Method8 = int(__thiscall*)(IMqNetPlayer* thisptr, int a2);
    Method8 method8;
};

static_assert(sizeof(IMqNetPlayerVftable) == 9 * sizeof(void*),
              "IMqNetPlayer vftable must have exactly 9 methods");

} // namespace game

#endif // MQNETPLAYER_H
