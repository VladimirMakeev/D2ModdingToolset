/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "idlistutils.h"

namespace hooks {

bool removeIdFromList(game::IdList& list, const game::CMidgardID* id)
{
    using namespace game;

    const auto& listApi = IdListApi::get();

    IdListIterator it;
    listApi.find(&it, list.begin(), list.end(), id);
    if (it != list.end()) {
        listApi.erase(&list, it);
        return true;
    }

    return false;
}

bool addUniqueIdToList(game::IdList& list, const game::CMidgardID* id)
{
    using namespace game;

    const auto& listApi = IdListApi::get();

    IdListIterator it;
    listApi.find(&it, list.begin(), list.end(), id);
    if (it == list.end()) {
        listApi.pushBack(&list, id);
        return true;
    }

    return false;
}

} // namespace hooks
