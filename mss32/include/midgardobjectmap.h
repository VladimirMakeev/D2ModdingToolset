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

#ifndef MIDGARDOBJECTMAP_H
#define MIDGARDOBJECTMAP_H

namespace game {

struct IMidgardObjectMap;
struct IMidScenarioObject;
struct CMidgardID;

struct IMidgardObjectMapVftable
{
    void* unknown[5];

    using FindScenarioObjectById =
        IMidScenarioObject*(__thiscall*)(const IMidgardObjectMap* thisptr, const CMidgardID* id);
    FindScenarioObjectById findScenarioObjectById;

    void* unknown2[7];
};

static_assert(sizeof(IMidgardObjectMapVftable) == 13 * sizeof(void*),
              "IMidgardObjectMap vftable must have exactly 13 methods");

struct IMidgardObjectMap
{
    const IMidgardObjectMapVftable* vftable;
};

} // namespace game

#endif // MIDGARDOBJECTMAP_H
