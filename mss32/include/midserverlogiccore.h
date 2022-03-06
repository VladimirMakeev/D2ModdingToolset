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

#ifndef MIDSERVERLOGICCORE_H
#define MIDSERVERLOGICCORE_H

#include "d2list.h"
#include "d2pair.h"
#include "d2set.h"
#include "d2string.h"
#include "d2vector.h"
#include "midgardid.h"
#include "mqnettraffic.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct CMidServer;
struct IMidgardObjectMap;
struct NetMsgEntryData;
struct NetPlayerInfo;

struct CMidServerLogicCoreData
{
    CMidServer* server;
    bool multiplayerGame;
    bool hotseatGame;
    char padding[2];
    int value3; /**< Always set to 3 in constructor. */
    /** Passed to CGameVersionMsg. 40 in RotE, 20 in Guardians of the Light and Galleans Return. */
    int gameVersion;
    NetMsgEntryData** netMsgEntryData;
    IMidgardObjectMap* objectMap;
    List<void*> list;
    int unknown4;
    std::uint32_t playerNetId;
    Vector<NetPlayerInfo>* players;
    int unknown6;
    Set<Pair<std::uint32_t /* netId */, String /* playerName */>>* sessionPlayers;
    int startingGold;
    int startingMana;
    char unknown8[4];
};

static_assert(sizeof(CMidServerLogicCoreData) == 72,
              "Size of CMidServerLogicCoreData structure must be exactly 72 bytes");

static_assert(offsetof(CMidServerLogicCoreData, list) == 24,
              "CMidServerLogicCoreData::list offset must be 24 bytes");

static_assert(offsetof(CMidServerLogicCoreData, players) == 48,
              "CMidServerLogicCoreData::players offset must be 48 bytes");

struct CMidServerLogicCore : public IMqNetTraffic
{
    CMidServerLogicCoreData* coreData;
};

} // namespace game

#endif // MIDSERVERLOGICCORE_H
