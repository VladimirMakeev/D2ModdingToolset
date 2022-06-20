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

#ifndef MIDCAMPAIGN_H
#define MIDCAMPAIGN_H

#include "d2list.h"
#include "midgardid.h"
#include "midobject.h"
#include "racecategory.h"
#include "textandid.h"
#include <cstddef>

namespace game {

/** Holds campaign scenario data read from CScen.dbf. */
struct CMidCampaignScenario
{
    CMidgardID scenarioId;
    char smkBrief[9];
    char smkWin[9];
    char smkLose[9];
    char wavWin[9];
};

assert_size(CMidCampaignScenario, 40);
assert_offset(CMidCampaignScenario, smkWin, 13);
assert_offset(CMidCampaignScenario, smkLose, 22);
assert_offset(CMidCampaignScenario, wavWin, 31);

struct CMidCampaignData
{
    LRaceCategory playableRace;
    char conclusion[9];
    List<CMidCampaignScenario> scenarios;
    TextAndId name;
    TextAndId description;
    bool canImport;
    char padding[3];
};

assert_size(CMidCampaignData, 60);
assert_offset(CMidCampaignData, name, 40);

/** Holds campaign data read from CCamp.dbf. */
struct CMidCampaign : public IMidObject
{
    CMidCampaignData* data;
};

assert_size(CMidCampaign, 12);

} // namespace game

#endif // MIDCAMPAIGN_H
