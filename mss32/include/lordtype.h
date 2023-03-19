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

/** Holds information read from GBuiList.dbf. */
struct TLordTypeBuildList
{
    CMidgardID lordId;
    Set<CMidgardID> data;
};

assert_size(TLordTypeBuildList, 32);

struct LeaderUpgData
{
    CMidgardID modifierId;
    int leaderLevel;
    int priority;
    TextAndId name;
    TextAndId description;
    char unknown[8];
};

assert_size(LeaderUpgData, 36);
assert_offset(LeaderUpgData, name, 12);
assert_offset(LeaderUpgData, description, 20);

/** Holds information read from GLeaUpg.dbf. */
struct CLordLeaderUpg
{
    void* vftable;
    CMidgardID lordId;
    List<LeaderUpgData> upgrades;
};

assert_size(CLordLeaderUpg, 24);

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

assert_size(TLordTypeData, 56);
assert_offset(TLordTypeData, name, 4);
assert_offset(TLordTypeData, description, 12);
assert_offset(TLordTypeData, buildList, 32);

/** Holds information read from GLord.dbf. */
struct TLordType : public IMidObject
{
    TLordTypeData* data;
};

assert_size(TLordType, 12);

} // namespace game

#endif // LORDTYPE_H
