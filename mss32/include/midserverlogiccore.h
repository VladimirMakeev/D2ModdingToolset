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
#include "d2map.h"
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
    Map<std::uint32_t /* netId */, String /* playerName */>* sessionPlayers;
    int startingGold;
    int startingMana;
    /** Determines whether expansion-only (RotE) fields of midgard objects should be serialized.
     * Namely, when this is true, the following additional fields are serialized:
     * - CMidPlayer::alwaysAi
     * - CMidStackTemplate::aiPriority
     * - CMidEffectUnfog::unfog
     * - CMidEffectDiplomacy::alwaysAtWar
     * - CMidEffectAlly::permanentAlliance
     * Also, when the flag is set, the special id 'G25500FFFF' is getting serialized at the start of
     * CRefreshInfo stream.
     */
    bool isExpansionContent;
    char padding2[3];
};

assert_size(CMidServerLogicCoreData, 72);
assert_offset(CMidServerLogicCoreData, list, 24);
assert_offset(CMidServerLogicCoreData, players, 48);

struct CMidServerLogicCore : public IMqNetTraffic
{
    CMidServerLogicCoreData* coreData;
};

} // namespace game

#endif // MIDSERVERLOGICCORE_H
