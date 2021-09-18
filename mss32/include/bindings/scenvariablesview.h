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

#ifndef SCENVARIABLESVIEW_H
#define SCENVARIABLESVIEW_H

#include <optional>
#include <string>
#include <unordered_map>

namespace sol {
class state;
}

namespace game {
struct CMidScenVariables;
struct ScenarioVariable;
} // namespace game

namespace bindings {

class ScenarioVariableView;

class ScenVariablesView
{
public:
    ScenVariablesView(const game::CMidScenVariables* scenVariables);

    static void bind(sol::state& lua);

    std::optional<ScenarioVariableView> getScenarioVariable(const std::string& name) const;

private:
    std::unordered_map<std::string, const game::ScenarioVariable*> variables;
    const game::CMidScenVariables* scenVariables;
};

} // namespace bindings

#endif // SCENVARIABLESVIEW_H
