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

#ifndef SCENARIOVARIABLEVIEW_H
#define SCENARIOVARIABLEVIEW_H

#include <string>

namespace sol {
class state;
}

namespace game {
struct ScenarioVariable;
} // namespace game

namespace bindings {

class ScenarioVariableView
{
public:
    ScenarioVariableView(const game::ScenarioVariable* variable);

    static void bind(sol::state& lua);

    std::string getName() const;
    int getValue() const;

private:
    const game::ScenarioVariable* variable;
};

} // namespace bindings

#endif // SCENARIOVARIABLEVIEW_H
