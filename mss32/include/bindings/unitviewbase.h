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

#ifndef UNITVIEWBASE_H
#define UNITVIEWBASE_H

#include "idview.h"
#include <optional>
#include <vector>

namespace sol {
class state;
}

namespace game {
struct IUsUnit;
} // namespace game

namespace bindings {

class UnitImplView;
class ModifierView;

class UnitViewBase
{
public:
    /** Returns unit current implementation. */
    std::optional<UnitImplView> getImpl() const;
    /** Returns unit base implementation. */
    std::optional<UnitImplView> getBaseImpl() const;
    /** Returns unit leveled implementation (without modifiers). */
    std::optional<UnitImplView> getLeveledImpl() const;

    std::vector<ModifierView> getModifiers() const;

    IdView getId() const;
    /** Returns unit's current experience points. */
    int getXp() const;
    int getHp() const;
    int getHpMax() const;

    /** Returns leader category id. */
    int getLeaderCategory() const;
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

protected:
    virtual game::CMidgardID getIdInternal() const = 0;
    virtual const game::IUsUnit* getImplInternal() const = 0;
    virtual int getXpInternal() const = 0;
    virtual int getHpInternal() const = 0;
    virtual int getHpMaxInternal() const = 0;
};

} // namespace bindings

#endif // UNITVIEWBASE_H
