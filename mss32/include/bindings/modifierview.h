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

#ifndef MODIFIERVIEW_H
#define MODIFIERVIEW_H

#include "idview.h"

namespace sol {
class state;
}

namespace game {
struct CUmModifier;
} // namespace game

namespace bindings {

class ModifierView
{
public:
    ModifierView(const game::CUmModifier* modifier);

    static void bind(sol::state& lua);

    IdView getId() const;

private:
    const game::CUmModifier* modifier;
};

} // namespace bindings

#endif // MODIFIERVIEW_H
