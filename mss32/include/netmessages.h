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

#include "categoryids.h"
#include "linkedlist.h"
#include "netmsg.h"

namespace game {

struct CPhaseGame;
struct CMidgardID;

/** Send by client from game menu to check game version of a server. */
struct CMenusReqVersionMsg : public CNetMsg
{ };

static_assert(sizeof(CMenusReqVersionMsg) == 4,
              "Size of CMenusReqVersionMsg structure must be exactly 4 bytes");

/** Send by server to clients in response to CMenusReqVersionMsg. */
struct CGameVersionMsg : public CNetMsg
{
    int gameVersion; /**< Initialized with CMidServerLogicCoreData::gameVersion value. */
};

static_assert(sizeof(CGameVersionMsg) == 8,
              "Size of CGameVersionMsg structure must be exactly 8 bytes");

/** Send by client from game menu to get scenario information from server. */
struct CMenusReqInfoMsg : public CNetMsg
{ };

static_assert(sizeof(CMenusReqInfoMsg) == 4,
              "Size of CMenusReqInfoMsg structure must be exactly 4 bytes");

/** Send by server to clients in response to CMenusReqInfoMsg. */
struct CMenusAnsInfoMsg : public CNetMsg
{
    char* scenarioName;
    char* scenarioDescription;
    DifficultyLevelId difficultyLevel;
    int suggestedLevel;
    LinkedList<RaceId> raceIds;
    int startingGold;
    int startingMana;
    int gameTurn;
    int maxPlayers;
    bool unknown5;
    char padding[3];
};

static_assert(sizeof(CMenusAnsInfoMsg) == 56,
              "Size of CMenusAnsInfoMsg structure must be exactly 56 bytes");

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
    using SendStackExchangeItemMsg = void(__thiscall*)(CPhaseGame* thisptr,
                                                       const CMidgardID* fromObjectId,
                                                       const CMidgardID* toObjectId,
                                                       const CMidgardID* itemId,
                                                       char a5);
    SendStackExchangeItemMsg sendStackExchangeItemMsg;

    /**
     * Sends network message about item being sold to merchant.
     * Sends CSiteSellItemMsg.
     * @param[in] thisptr pointer to CPhaseGame object that will send the message.
     * @param[in] siteId id of the merchant object.
     * @param[in] stackId id of the stack that sells item.
     * @param[in] itemId id of the item to sell.
     */
    using SendSiteSellItemMsg = void(__thiscall*)(CPhaseGame* thisptr,
                                                  const CMidgardID* siteId,
                                                  const CMidgardID* stackId,
                                                  const CMidgardID* itemId);
    SendSiteSellItemMsg sendSiteSellItemMsg;
};

Api& get();

CNetMsgVftable* getMenusReqVersionVftable();
CNetMsgVftable* getMenusReqInfoVftable();

} // namespace NetMessagesApi

} // namespace game

#endif // NETMESSAGES_H
