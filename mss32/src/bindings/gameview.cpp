/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "gameview.h"
#include "settings.h"
#include <sol/sol.hpp>

namespace bindings {

void GameView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<GameView>("GameView");
    view["unitMaxDamage"] = sol::property(&GameView::getUnitMaxDamage);
    view["unitMinDamage"] = sol::property(&GameView::getUnitMinDamage);
    view["unitMaxArmor"] = sol::property(&GameView::getUnitMaxArmor);
    view["leaderAdditionalDamage"] = sol::property(&GameView::getLeaderAdditionalDamage);
}

int GameView::getUnitMaxDamage() const
{
    return hooks::userSettings().unitMaxDamage;
}

int GameView::getUnitMinDamage() const
{
    return 1;
}

int GameView::getLeaderAdditionalDamage() const
{
    return 100;
}

int GameView::getUnitMaxArmor() const
{
    return hooks::userSettings().unitMaxArmor;
}

} // namespace bindings
