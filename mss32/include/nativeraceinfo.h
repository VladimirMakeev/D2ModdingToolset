/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef NATIVERACEINFO_H
#define NATIVERACEINFO_H

#include "raceinfo.h"
#include "rsgid.h"

namespace hooks {

class NativeRaceInfo final : public rsg::RaceInfo
{
public:
    NativeRaceInfo(const rsg::CMidgardID& raceId,
                   const rsg::CMidgardID& guardianId,
                   const rsg::CMidgardID& nobleId,
                   rsg::RaceType raceType,
                   rsg::LeaderNames&& leaderNames,
                   std::vector<rsg::CMidgardID>&& leaderIds)
        : leaderNames{std::move(leaderNames)}
        , leaderIds{std::move(leaderIds)}
        , raceId{raceId}
        , guardianId{guardianId}
        , nobleId{nobleId}
        , raceType{raceType}
    { }

    ~NativeRaceInfo() override = default;

    const rsg::CMidgardID& getRaceId() const override
    {
        return raceId;
    }

    rsg::RaceType getRaceType() const override
    {
        return raceType;
    }

    const rsg::CMidgardID& getGuardianUnitId() const override
    {
        return guardianId;
    }

    const rsg::CMidgardID& getNobleLeaderId() const override
    {
        return nobleId;
    }

    const std::vector<rsg::CMidgardID>& getLeaderIds() const override
    {
        return leaderIds;
    }

    const rsg::LeaderNames& getLeaderNames() const override
    {
        return leaderNames;
    }

private:
    rsg::LeaderNames leaderNames;
    std::vector<rsg::CMidgardID> leaderIds;
    rsg::CMidgardID raceId;
    rsg::CMidgardID guardianId;
    rsg::CMidgardID nobleId;
    rsg::RaceType raceType;
};

} // namespace hooks

#endif // NATIVERACEINFO_H
