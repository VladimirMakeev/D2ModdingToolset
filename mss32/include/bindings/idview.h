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

#ifndef IDVIEW_H
#define IDVIEW_H

#include "midgardid.h"
#include "utils.h"
#include <ostream>

namespace sol {
class state;
}

namespace bindings {

struct IdView
{
    IdView(const char* id);
    IdView(const std::string& id);
    IdView(const game::CMidgardID* id);
    IdView(const game::CMidgardID& id);

    bool operator==(const IdView& other) const
    {
        return id == other.id;
    }

    bool operator!=(const IdView& other) const
    {
        return id != other.id;
    }

    operator const game::CMidgardID&() const
    {
        return id;
    }

    static void bind(sol::state& lua);

    static IdView getEmptyId();

    game::CMidgardID id;
};

/** Overloaded operator allows calling 'tostring()' with IdView in lua scripts. */
static inline std::ostream& operator<<(std::ostream& os, const IdView& id)
{
    os << hooks::idToString(&id.id);
    return os;
}

} // namespace bindings

#endif // IDVIEW_H
