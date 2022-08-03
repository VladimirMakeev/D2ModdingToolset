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

#ifndef UNITVIEW_H
#define UNITVIEW_H

#include "unitviewbase.h"

namespace sol {
class state;
}

namespace game {
struct CMidUnit;
} // namespace game

namespace bindings {

class UnitViewDummy;

class UnitView : public UnitViewBase
{
public:
    UnitView(const game::CMidUnit* unit);
    UnitView(const game::CMidUnit* unit, const game::IUsUnit* unitImpl);

    static void bind(sol::state& lua);

    std::optional<UnitViewDummy> getOriginal() const;
    std::vector<ModifierView> getOriginalModifiers() const;

protected:
    game::CMidgardID getIdInternal() const;
    const game::IUsUnit* getImplInternal() const;
    int getXpInternal() const;
    int getHpInternal() const;
    int getHpMaxInternal() const;

    const game::CMidUnit* unit;
    const game::IUsUnit* unitImpl;
};

} // namespace bindings

#endif // UNITVIEW_H
