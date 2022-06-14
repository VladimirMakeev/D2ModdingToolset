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

#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

namespace sol {
class state;
}

namespace game {
struct CMidPlayer;
} // namespace game

namespace bindings {

struct IdView;
class CurrencyView;

class PlayerView
{
public:
    PlayerView(const game::CMidPlayer* player);

    static void bind(sol::state& lua);

    IdView getId() const;
    int getRaceCategoryId() const;
    int getLordCategoryId() const;
    CurrencyView getBank() const;
    bool isHuman() const;
    bool isAlwaysAi() const;

private:
    const game::CMidPlayer* player;
};

} // namespace bindings

#endif // PLAYERVIEW_H
