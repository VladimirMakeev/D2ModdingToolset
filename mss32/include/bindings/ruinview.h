/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef RUINVIEW_H
#define RUINVIEW_H

#include <optional>

namespace sol {
class state;
}

namespace game {
struct CMidRuin;
struct IMidgardObjectMap;
} // namespace game

namespace bindings {

struct IdView;
struct Point;
class PlayerView;
class GroupView;
class ItemView;
class CurrencyView;

class RuinView
{
public:
    RuinView(const game::CMidRuin* ruin, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    IdView getId() const;
    Point getPosition() const;
    std::optional<PlayerView> getLooter() const;
    GroupView getGroup() const;
    std::optional<ItemView> getItem() const;
    CurrencyView getCash() const;

private:
    const game::CMidRuin* ruin;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // RUINVIEW_H
