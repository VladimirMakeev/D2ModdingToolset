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

#ifndef MIDSCENARIOOBJECT_H
#define MIDSCENARIOOBJECT_H

#include "midobject.h"

namespace game {

struct IMidScenarioObjectVftable;
struct IMidgardStreamEnv;
struct IMidgardObjectMap;

/** Base class for all objects in scenario. */
struct IMidScenarioObject : public IMidObjectT<IMidScenarioObjectVftable>
{ };

struct IMidScenarioObjectVftable : public IMidObjectVftable
{
    /** Serializes object to/from .sg file, .dbf file or network. */
    using Stream = void(__thiscall*)(IMidScenarioObject* thisptr,
                                     const IMidgardObjectMap* objectMap,
                                     IMidgardStreamEnv* streamEnv);
    Stream stream;

    /**
     * Returns true if object has correct data.
     * For example, called when Scenario Editor checks scenario for inconsistencies.
     */
    using IsValid = bool(__thiscall*)(const IMidScenarioObject* thisptr,
                                      const IMidgardObjectMap* objectMap);
    IsValid isValid;

    /** Meaning and usage cases unknown. */
    using Method3 = bool(__thiscall*)(const IMidScenarioObject* thisptr, int a2);
    Method3 method3;
};

static_assert(sizeof(IMidScenarioObjectVftable) == 4 * sizeof(void*),
              "IMidScenarioObject vftable must have exactly 4 methods");

} // namespace game

#endif // MIDSCENARIOOBJECT_H
