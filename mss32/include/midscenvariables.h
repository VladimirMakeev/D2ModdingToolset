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

#ifndef MIDSCENVARIABLES_H
#define MIDSCENVARIABLES_H

#include "d2vector.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "sortedlist.h"

namespace game {

struct ScenarioVariableData
{
    char name[36];
    int value;
};

static_assert(sizeof(ScenarioVariableData) == 40,
              "Size of ScenarioVariableData structure must be exactly 40 bytes");

struct ScenarioVariable
{
    int variableId;
    ScenarioVariableData data;
};

static_assert(sizeof(ScenarioVariable) == 44,
              "Size of ScenarioVariable structure must be exactly 44 bytes");

/**
 * Holds variables information in scenario file and game.
 * Scenario variables used in events logic.
 */
struct CMidScenVariables : public IMidScenarioObject
{
    CMidgardID variablesId;
    int unknown;
    SortedList<ScenarioVariable> variables;
};

static_assert(sizeof(CMidScenVariables) == 40,
              "Size of CMidScenVariables structure must be exactly 40 bytes");

using ScenarioVariablesListNode = SortedListNode<ScenarioVariable>;

namespace CMidScenVariablesApi {

struct Api
{
    using Advance = void(__stdcall*)(ScenarioVariablesListNode** current,
                                     ScenarioVariablesListNode* end);
    Advance advance;
};

Api& get();

} // namespace CMidScenVariablesApi

} // namespace game

#endif // MIDSCENVARIABLES_H
