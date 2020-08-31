/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#include "linkedlist.h"
#include "midgardobjectmap.h"

namespace game {

struct CMidgardScenarioMap;
struct IMidScenarioObject;
struct CMidgardID;

struct CMidDataCache2 : public IMidgardObjectMap
{
    CMidgardScenarioMap** scenarioMap;
    LinkedList list;
    char unknown;
    char padding[3];
    int* unknown2;
    int unknown3;
};

static_assert(sizeof(CMidDataCache2) == 36,
              "Size of CMidDataCache2 structure must be exactly 36 bytes");

namespace CMidDataCache2Api {

struct Vftable
{
    void* unknown[5];

    using FindScenarioObjectById = IMidScenarioObject*(__thiscall*)(CMidDataCache2* thisptr,
                                                                    const CMidgardID* id);
    FindScenarioObjectById findScenarioObjectById;

    void* unknown2[7];
};

static_assert(sizeof(Vftable) == 13 * sizeof(void*),
              "CMidDataCache2 vftable must have exactly 13 methods");

} // namespace CMidDataCache2Api

} // namespace game

#endif // MIDDATACACHE_H
