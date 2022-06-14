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

#ifndef FORTVIEW_H
#define FORTVIEW_H

#include <optional>
#include <vector>

namespace sol {
class state;
}

namespace game {
struct CFortification;
struct IMidgardObjectMap;
} // namespace game

namespace bindings {

struct IdView;
class PlayerView;
class GroupView;
class StackView;
class ItemView;

class FortView
{
public:
    FortView(const game::CFortification* fort, const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    IdView getId() const;
    PlayerView getOwner() const;
    GroupView getGroup() const;
    std::optional<StackView> getVisitor() const;
    int getSubrace() const;
    std::vector<ItemView> getInventoryItems() const;

private:
    const game::CFortification* fort;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // FORTVIEW_H
