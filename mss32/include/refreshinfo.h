/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef REFRESHINFO_H
#define REFRESHINFO_H

#include "d2assert.h"
#include "netmsg.h"

namespace game {

struct CMidgardID;
struct CMidgardScenarioMap;

struct CRefreshInfo : CNetMsg
{
    CMidgardScenarioMap* scenarioMap;
    bool isExpansionContent;
};

assert_size(CRefreshInfo, 12);

namespace CRefreshInfoApi {

struct Api
{
    using Constructor = CRefreshInfo*(__thiscall*)(CRefreshInfo* thisptr);
    Constructor constructor;

    using Constructor2 = CRefreshInfo*(__thiscall*)(CRefreshInfo* thisptr,
                                                    CMidgardID* scenarioFileId,
                                                    bool isExpansionContent);
    Constructor2 constructor2;

    using Destructor = void(__thiscall*)(CRefreshInfo* thisptr);
    Destructor destructor;

    using AddObject = bool(__thiscall*)(CRefreshInfo* thisptr,
                                        const CMidgardScenarioMap* scenarioMap,
                                        const CMidgardID* objectId);
    AddObject addObject;
};

Api& get();

CNetMsgVftable* vftable();

} // namespace CRefreshInfoApi

} // namespace game

#endif // REFRESHINFO_H
