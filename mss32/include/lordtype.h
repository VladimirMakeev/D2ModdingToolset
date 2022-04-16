/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef LORDTYPE_H
#define LORDTYPE_H

#include "d2list.h"
#include "d2set.h"
#include "lordcat.h"
#include "midgardid.h"
#include "midobject.h"
#include "textandid.h"
#include <cstddef>
#include <cstdint>

namespace game {

using BuildListDataNode = SetNode<CMidgardID>;

static_assert(sizeof(BuildListDataNode) == 20,
              "Size of BuildListDataNode structure must be exactly 20 bytes");

using BuildListData = Set<CMidgardID>;

static_assert(sizeof(BuildListData) == 28,
              "Size of BuildListData structure must be exactly 28 bytes");

/** Name and meaning were assumed. */
using BuildListIterator = SetIterator<CMidgardID>;

static_assert(sizeof(BuildListIterator) == 12,
              "Size of BuildListIterator structure must be exactly 12 bytes");

/** Holds information read from GBuiList.dbf. */
struct TLordTypeBuildList
{
    CMidgardID buildingId;
    BuildListData data;
};

static_assert(sizeof(TLordTypeBuildList) == 32,
              "Size of TLordTypeBuildList structure must be exactly 32 bytes");

struct LeaderUpgData
{
    CMidgardID modifierId;
    int leaderLevel;
    int priority;
    TextAndId name;
    TextAndId description;
    char unknown[8];
};

static_assert(sizeof(LeaderUpgData) == 36,
              "Size of LeaderUpgData structure must be exactly 36 bytes");

static_assert(offsetof(LeaderUpgData, name) == 12, "LeaderUpgData::name offset must be 12 bytes");

static_assert(offsetof(LeaderUpgData, description) == 20,
              "LeaderUpgData::description offset must be 20 bytes");

/** Holds information read from GLeaUpg.dbf. */
struct CLordLeaderUpg
{
    void* vftable;
    CMidgardID lordId;
    List<LeaderUpgData> upgrades;
};

static_assert(sizeof(CLordLeaderUpg) == 24,
              "Size of CLordLeaderUpg structure must be exactly 24 bytes");

struct TLordTypeData
{
    CMidgardID raceId;
    TextAndId name;
    TextAndId description;
    char picture[9];
    char padding[3];
    TLordTypeBuildList* buildList;
    CLordLeaderUpg* leaderUpg;
    void* unknown;
    LLordCategory lordCategory;
};

static_assert(sizeof(TLordTypeData) == 56,
              "Size of TLordTypeData structure must be exactly 56 bytes");

static_assert(offsetof(TLordTypeData, name) == 4, "TLordTypeData::name offset must be 4 bytes");

static_assert(offsetof(TLordTypeData, description) == 12,
              "TLordTypeData::description offset must be 12 bytes");

static_assert(offsetof(TLordTypeData, buildList) == 32,
              "TLordTypeData::buildList offset must be 32 bytes");

/** Holds information read from GLord.dbf. */
struct TLordType : public IMidObject
{
    CMidgardID lordId;
    TLordTypeData* data;
};

static_assert(sizeof(TLordType) == 12, "Size of TLordType structure must be exactly 12 bytes");

} // namespace game

#endif // LORDTYPE_H
