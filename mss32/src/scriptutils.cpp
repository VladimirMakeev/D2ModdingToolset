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

#include "scriptutils.h"

namespace hooks {

std::vector<bindings::IdView> IdVectorToIds(const game::IdVector* src)
{
    std::vector<bindings::IdView> value;
    for (const game::CMidgardID* it = src->bgn; it != src->end; it++) {
        value.push_back(it);
    }

    return value;
}

void IdsToIdVector(const std::vector<bindings::IdView>& src, game::IdVector* dst)
{
    using namespace game;

    const auto& idVectorApi = IdVectorApi::get();

    IdVector tmp{};
    idVectorApi.reserve(&tmp, 1);
    for (const auto& id : src) {
        idVectorApi.pushBack(&tmp, &(const CMidgardID&)id);
    }
    idVectorApi.copy(dst, tmp.bgn, tmp.end);
    idVectorApi.destructor(&tmp);
}

} // namespace hooks
