/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef INTINTMAP_H
#define INTINTMAP_H

#include "d2map.h"
#include "d2pair.h"

namespace game {

using IntIntMap = Map<int, int>;
using IntIntMapNode = MapNode<int, int>;
using IntIntMapIterator = MapIterator<int, int>;

namespace IntIntMapApi {

struct Api
{
    using Insert =
        Pair<IntIntMapIterator, bool>*(__thiscall*)(IntIntMap* thisptr,
                                                    Pair<IntIntMapIterator, bool>* result,
                                                    Pair<int, int>* value);
    Insert insert;

    using Find = IntIntMapIterator*(__thiscall*)(const IntIntMap* thisptr,
                                                 IntIntMapIterator* result,
                                                 const int* value);
    Find find;
};

Api& get();

} // namespace IntIntMapApi

} // namespace game

#endif // INTINTMAP_H
