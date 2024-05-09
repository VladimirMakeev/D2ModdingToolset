/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "midtaskopeninterfparamresmarket.h"
#include "interface.h"
#include "interfmanager.h"
#include "mempool.h"
#include "midtaskopeninterfparam.h"
#include "siteresourcemarketinterf.h"
#include <cstring>

namespace hooks {

/** Task that opens resource market interface. */
using CMidTaskOpenInterfResMarket = game::CMidTaskOpenInterfParam<game::CInterface>;

static game::ITaskVftable taskResMarketVftable;
static game::ITaskVftable::Destructor midFreeTaskDtor = nullptr;

static void __fastcall taskResMarketDtor(CMidTaskOpenInterfResMarket* thisptr,
                                         int /*%edx*/,
                                         char flags)
{
    using namespace game;

    if (thisptr->interf) {
        // Hide and delete interface, as other site related tasks do
        auto interfManager{thisptr->interfManager.data};
        interfManager->CInterfManager::vftable->hideInterface(interfManager, thisptr->interf);

        thisptr->interf->vftable->destructor(thisptr->interf, 1);
        thisptr->interf = nullptr;
    }

    // Call base class d-tor
    if (midFreeTaskDtor) {
        midFreeTaskDtor(thisptr, flags);
    }
}

game::ITask* createMidTaskOpenInterfParamResMarket(game::CTaskManager* taskManager,
                                                   game::CPhaseGame* phaseGame,
                                                   const game::CMidgardID& visitorStackId,
                                                   const game::CMidgardID& siteId)
{
    using namespace game;

    const auto& allocateMemory{Memory::get().allocate};

    auto task{(CMidTaskOpenInterfResMarket*)allocateMemory(sizeof(CMidTaskOpenInterfResMarket))};
    CMidFreeTaskApi::get().constructor(task, taskManager);

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;

        midFreeTaskDtor = task->vftable->destructor;
        std::memcpy(&taskResMarketVftable, task->vftable, sizeof(ITaskVftable));

        taskResMarketVftable.destructor = (ITaskVftable::Destructor)taskResMarketDtor;
    }

    // Use our own vftable
    task->vftable = &taskResMarketVftable;
    task->interf = createSiteResourceMarketInterf(task, phaseGame, visitorStackId, siteId);

    auto interfManager{task->interfManager.data};
    interfManager->CInterfManager::vftable->showInterface(interfManager, task->interf);
    return task;
}

} // namespace hooks
