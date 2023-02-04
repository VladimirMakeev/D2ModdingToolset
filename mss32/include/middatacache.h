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

#ifndef MIDDATACACHE_H
#define MIDDATACACHE_H

#include "idlist.h"
#include "midgardobjectmap.h"

namespace game {

struct CMidgardScenarioMap;
struct IMidScenarioObject;

struct CMidDataCache2 : public IMidgardObjectMap
{
    struct INotifyVftable;

    struct INotify
    {
        INotifyVftable* vftable;
    };

    struct INotifyVftable
    {
        using Notify = void(__thiscall*)(INotify* thisptr, IMidScenarioObject* obj);
        Notify notify;
    };

    assert_vftable_size(INotifyVftable, 1);

    CMidgardScenarioMap** scenarioMap;
    IdList list;
    char unknown;
    char padding[3];
    int* unknown2;
    int unknown3;
};

assert_size(CMidDataCache2, 36);

} // namespace game

#endif // MIDDATACACHE_H
