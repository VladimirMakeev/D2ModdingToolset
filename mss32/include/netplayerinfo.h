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

#ifndef NETPLAYERINFO_H
#define NETPLAYERINFO_H

#include "lordcat.h"
#include "midgardid.h"
#include "racecategory.h"
#include "smartptr.h"
#include <cstdint>

namespace game {

/** Holds information about network or local game player. */
struct NetPlayerInfo
{
    std::uint32_t playerNetId;
    CMidgardID playerId;
    /** Assumption: true if player controlled by human, not AI. */
    bool controlledByHuman;
    char padding[3];
    int face;
    LRaceCategory raceCategory;
    LLordCategory lordCategory;
    char name[65];
    char unknown5[3];
    SmartPtr<void> importLeaderStore;
    bool leaderImportDisabled;
    char unknown6[7];
};

assert_size(NetPlayerInfo, 124);

} // namespace game

#endif // NETPLAYERINFO_H
