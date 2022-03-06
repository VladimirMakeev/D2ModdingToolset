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

static_assert(sizeof(CMidCampaignScenario) == 40,
              "Size of CMidCampaignScenario structure must be exactly 40 bytes");

static_assert(offsetof(CMidCampaignScenario, smkWin) == 13,
              "CMidCampaignScenario::smkWin offset must be 13 bytes");

static_assert(offsetof(CMidCampaignScenario, smkLose) == 22,
              "CMidCampaignScenario::smkLose offset must be 22 bytes");

static_assert(offsetof(CMidCampaignScenario, wavWin) == 31,
              "CMidCampaignScenario::wavWin offset must be 31 bytes");

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

static_assert(sizeof(CMidCampaignData) == 60,
              "Size of CMidCampaignData structure must be exactly 60 bytes");

static_assert(offsetof(CMidCampaignData, name) == 40,
              "CMidCampaignData::name offset must be 40 bytes");

/** Holds campaign data read from CCamp.dbf. */
struct CMidCampaign : public IMidObject
{
    CMidgardID campaignId;
    CMidCampaignData* data;
};

static_assert(sizeof(CMidCampaign) == 12,
              "Size of CMidCampaign structure must be exactly 12 bytes");

} // namespace game

#endif // MIDCAMPAIGN_H
