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

#include "idview.h"
#include <sol/sol.hpp>

namespace bindings {

IdView::IdView(const char* id)
{
    if (id) {
        game::CMidgardIDApi::get().fromString(&this->id, id);
    } else {
        this->id = game::emptyId;
    }
}

IdView::IdView(const std::string& id)
{
    if (!id.empty()) {
        game::CMidgardIDApi::get().fromString(&this->id, id.c_str());
    } else {
        this->id = game::emptyId;
    }
}

IdView::IdView(const game::CMidgardID* id)
    : id(id ? *id : game::emptyId)
{ }

IdView::IdView(const game::CMidgardID& id)
    : id(id)
{ }

void IdView::bind(sol::state& lua)
{
    auto id = lua.new_usertype<IdView>(
        "Id",
        sol::constructors<IdView(const char*), IdView(const std::string&),
                          IdView(const game::CMidgardID*), IdView(const game::CMidgardID&)>());
    id["value"] = sol::property(&IdView::getValue);
    id["type"] = sol::property(&IdView::getType);
    id["typeIndex"] = sol::property(&IdView::getTypeIndex);
    id["emptyId"] = IdView::getEmptyId;
    id["summonId"] = IdView::getSummonId;
}

int IdView::getValue() const
{
    return id.value;
}

int IdView::getType() const
{
    return static_cast<int>(game::CMidgardIDApi::get().getType(&id));
}

int IdView::getTypeIndex() const
{
    // Duplicates impl of CMidgardIDApi::GetTypeIndex for better performance
    return id.value & 0xFFFF;
}

IdView IdView::getEmptyId()
{
    return IdView{game::emptyId};
}

IdView IdView::getSummonId(int position)
{
    using namespace game;

    CMidgardID summonId = emptyId;
    CMidgardIDApi::get().summonUnitIdFromPosition(&summonId, position);

    return IdView{summonId};
}

} // namespace bindings
