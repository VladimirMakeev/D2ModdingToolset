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

#ifndef IDSET_H
#define IDSET_H

#include "d2set.h"
#include "midgardid.h"

namespace game {

using IdSet = Set<CMidgardID>;
using IdSetIterator = SetIterator<CMidgardID>;

namespace IdSetApi {

struct Api
{
    /**
     * Searches for specified id.
     * @returns iterator pointing to the found node or end node if nothing found.
     */
    using Find = IdSetIterator*(__thiscall*)(const IdSet* thisptr,
                                             IdSetIterator* iterator,
                                             const CMidgardID* id);
    Find find;
};

Api& get();

} // namespace IdSetApi

} // namespace game

#endif // IDSET_H
