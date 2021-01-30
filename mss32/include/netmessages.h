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

#ifndef NETMESSAGES_H
#define NETMESSAGES_H

namespace game {

struct CPhaseGame;
struct CMidgardID;

namespace NetMessagesApi {

struct Api
{
    /**
     * Assumption: sends network message about item exchange from client to server.
     * Sends CStackExchangeItemMsg.
     * @param[in] thisptr pointer to CPhaseGame object that will send the message.
     * @param[in] fromObjectId id of the object transfer item from.
     * @param[in] toObjectId id of the object that receives item.
     * @param[in] itemId id of the item to exchange.
     * @param a5 meaning unknown, currently set to 1.
     */
    using SendStackExchangeItemMsg = void(__thiscall*)(game::CPhaseGame* thisptr,
                                                       const game::CMidgardID* fromObjectId,
                                                       const game::CMidgardID* toObjectId,
                                                       const game::CMidgardID* itemId,
                                                       char a5);
    SendStackExchangeItemMsg sendStackExchangeItemMsg;
};

Api& get();

} // namespace NetMessagesApi

} // namespace game

#endif // NETMESSAGES_H
