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

#include "scenvariablesview.h"
#include "log.h"
#include "midscenvariables.h"
#include "scenariovariableview.h"
#include "utils.h"
#include <algorithm>
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace bindings {

ScenVariablesView::ScenVariablesView(const game::CMidScenVariables* scenVariables)
    : scenVariables(scenVariables)
{
    // Game does not have functions for search variables by name,
    // but accessing them using it is convenient for scripts.
    // Cache scenario variables for our own fast search by name.
    // This is safe because variables list is created once
    // and there are no additions or deletions of them during the game.
    hooks::forEachScenarioVariable(scenVariables,
                                   [this](const game::ScenarioVariable* variable, std::uint32_t) {
                                       variables[variable->data.name] = variable;
                                   });
}

void ScenVariablesView::bind(sol::state& lua)
{
    auto vars = lua.new_usertype<ScenVariablesView>("ScenarioVariables");
    vars["getVariable"] = &ScenVariablesView::getScenarioVariable;
}

std::optional<ScenarioVariableView> ScenVariablesView::getScenarioVariable(
    const std::string& name) const
{
    std::string ingameName{name};
    // Game stores variable names in uppercase
    std::transform(ingameName.begin(), ingameName.end(), ingameName.begin(), toupper);

    const auto it = variables.find(ingameName);
    if (it == variables.end()) {
        return std::nullopt;
    }

    return ScenarioVariableView{it->second};
}

} // namespace bindings
