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

#include "listutils.h"

namespace hooks {

bool removeIdFromList(game::IdList& list, const game::CMidgardID* id)
{
    using namespace game;

    const auto& listApi = IdListApi::get();

    IdListIterator it, begin, end;
    listApi.begin(&list, &begin);
    listApi.end(&list, &end);
    listApi.find(&it, begin, end, id);
    if (!listApi.equals(&it, &end)) {
        listApi.erase(&list, it);
        return true;
    }

    return false;
}

bool addUniqueIdToList(game::IdList& list, const game::CMidgardID* id)
{
    using namespace game;

    const auto& listApi = IdListApi::get();

    IdListIterator it, begin, end;
    listApi.begin(&list, &begin);
    listApi.end(&list, &end);
    listApi.find(&it, begin, end, id);
    if (listApi.equals(&it, &end)) {
        listApi.push_back(&list, id);
        return true;
    }

    return false;
}

} // namespace hooks
