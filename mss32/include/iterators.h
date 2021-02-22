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

#ifndef ITERATORS_H
#define ITERATORS_H

#include "midgardobjectmap.h"

namespace game {

using IteratorPtr = SmartPtr<IMidgardObjectMap::Iterator>;

namespace Iterators {

struct Api
{
    /**
     * Creates iterator for traversing specific types of objects on map.
     * @param[inout] iterator smart pointer that will own iterator after creation.
     * Must be freed after use.
     * @param[in] objectMap map to traverse.
     */
    using CreateIterator = IteratorPtr*(__stdcall*)(IteratorPtr* iterator,
                                                    IMidgardObjectMap* objectMap);

    // Traverse CMidPlayer objects
    CreateIterator createPlayersIterator;
    CreateIterator createPlayersEndIterator;
    // Traverse CFortification objects
    CreateIterator createFortificationsIterator;
    CreateIterator createFortificationsEndIterator;
};

Api& get();

} // namespace Iterators

} // namespace game

#endif // ITERATORS_H
