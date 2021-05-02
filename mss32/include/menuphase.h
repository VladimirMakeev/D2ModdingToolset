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

#ifndef MENUPHASE_H
#define MENUPHASE_H

#include "catalogvalidate.h"
#include "difficultylevel.h"
#include "linkedlist.h"
#include "midgardid.h"
#include "mqnetsystem.h"
#include "racecategory.h"
#include "smartptr.h"
#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace game {

struct CMidgard;
struct CInterface;
struct CInterfManagerImpl;
struct ScenarioDataArrayWrapped;
struct IMqImage2;

struct CMenuPhaseData
{
    CMidgard* midgard;
    CInterface* currentMenu;
    SmartPtr<CInterfManagerImpl> interfManager;
    int transitionNumber;
    ScenarioDataArrayWrapped* scenarios;
    SmartPtr<IMqImage2> transitionAnimation;
    int unknown5;
    int unknown6;
    LinkedList<LRaceCategory> races;
    LRaceCategory race;
    LDifficultyLevel difficultyLevel;
    char* scenarioFilePath;
    CMidgardID unknownId;
    CMidgardID campaignId;
    char* scenarioName;
    char* scenarioDescription;
    int unknown7;
    int unknown8;
    HANDLE scenarioFileHandle;
    int unknown10;
};

static_assert(sizeof(CMenuPhaseData) == 116,
              "Size of CMenuPhaseData structure must be exactly 116 bytes");

static_assert(offsetof(CMenuPhaseData, races) == 40,
              "CMenuPhaseData::races offset must be 40 bytes");

static_assert(offsetof(CMenuPhaseData, unknownId) == 84,
              "CMenuPhaseData::unknownId offset must be 84 bytes");

struct CMenuPhase
    : public IMqNetSystem
    , public ICatalogValidate
{
    CMenuPhaseData* data;
};

static_assert(sizeof(CMenuPhase) == 12, "Size of CMenuPhase structure must be exactly 12 bytes");

static_assert(offsetof(CMenuPhase, CMenuPhase::IMqNetSystem::vftable) == 0,
              "Vftable offset for IMqNetSystem in CMenuPhase structure must be 0 bytes");

static_assert(offsetof(CMenuPhase, CMenuPhase::ICatalogValidate::vftable) == 4,
              "Vftable offset for ICatalogValidate in CMenuPhase structure must be 4 bytes");

} // namespace game

#endif // MENUPHASE_H
