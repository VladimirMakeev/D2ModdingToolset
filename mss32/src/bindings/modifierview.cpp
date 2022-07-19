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

#include "modifierview.h"
#include "ummodifier.h"
#include <sol/sol.hpp>

namespace bindings {

ModifierView::ModifierView(const game::CUmModifier* modifier)
    : modifier{modifier}
{ }

void ModifierView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<ModifierView>("ModifierView");
    view["id"] = sol::property(&getId);
}

IdView ModifierView::getId() const
{
    return modifier->data->modifierId;
}

} // namespace bindings
