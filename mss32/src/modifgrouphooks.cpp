/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "modifgrouphooks.h"
#include "custommodifiers.h"
#include "dbfaccess.h"
#include "log.h"
#include "modifgroup.h"

namespace hooks {

static const char dbfFileName[] = "LModifS.dbf";

game::LModifGroupTable* __fastcall modifGroupTableCtorHooked(game::LModifGroupTable* thisptr,
                                                             int /*%edx*/,
                                                             const char* globalsFolderPath,
                                                             void* codeBaseEnvProxy)
{
    using namespace game;
    logDebug("mss32Proxy.log", "LModifGroupTable c-tor hook started");

    const auto& tableApi = LModifGroupTableApi::get();
    auto& groups = LModifGroupApi::categories();

    const auto dbfFilePath{std::filesystem::path(globalsFolderPath) / dbfFileName};
    bool customGroupExists = utils::dbValueExists(dbfFilePath, "TEXT", customModifGroupName);
    if (customGroupExists)
        logDebug("mss32Proxy.log", "Found custom modifier category");

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LModifGroupTableApi::vftable();

    tableApi.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    tableApi.readCategory(groups.attack, thisptr, "L_ATTACK", dbfFileName);
    tableApi.readCategory(groups.unit, thisptr, "L_UNIT", dbfFileName);
    tableApi.readCategory(groups.stack, thisptr, "L_STACK", dbfFileName);
    if (customGroupExists)
        tableApi.readCategory(&getCustomModifiers().group, thisptr, customModifGroupName,
                              dbfFileName);
    tableApi.initDone(thisptr);

    logDebug("mss32Proxy.log", "LModifGroupTable c-tor hook finished");
    return thisptr;
}

} // namespace hooks
