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

struct CMidDataCache2 : public IMidgardObjectMap
{
    struct INotify
    {
        void* vftable;
    };

    CMidgardScenarioMap** scenarioMap;
    IdList list;
    char unknown;
    char padding[3];
    int* unknown2;
    int unknown3;
};

static_assert(sizeof(CMidDataCache2) == 36,
              "Size of CMidDataCache2 structure must be exactly 36 bytes");

} // namespace game

#endif // MIDDATACACHE_H
