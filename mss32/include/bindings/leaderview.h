/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef LEADERVIEW_H
#define LEADERVIEW_H

#include "unitview.h"

namespace bindings {

class LeaderView : public UnitView
{
public:
    LeaderView(const game::CMidUnit* unit);

    static void bind(sol::state& lua);

    /** Returns leader category id. */
    int getCategory() const;
    /** Returns leader maximum movement points. */
    int getMovement() const;
    /** Returns leader scouting range. */
    int getScout() const;
    /** Returns current leadership value. */
    int getLeadership() const;
    /** Returns true if leader has LLeaderAbility with specified id. */
    bool hasAbility(int abilityId) const;
    /** Returns true if leader has movement bonus on LGroundCategory with specified id. */
    bool hasMoveBonus(int groundId) const;
};

} // namespace bindings

#endif // LEADERVIEW_H
