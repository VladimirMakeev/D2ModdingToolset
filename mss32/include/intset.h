/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef INTSET_H
#define INTSET_H

#include "d2pair.h"
#include "d2set.h"

namespace game {

using IntSet = Set<int>;
using IntSetNode = SetNode<int>;
using IntSetIterator = SetIterator<int>;

namespace IntSetApi {

struct Api
{
    /** Performs targets list initialization. */
    using Constructor = IntSet*(__thiscall*)(IntSet* thisptr);
    Constructor constructor;

    /** Frees memory allocated for list. */
    using Destructor = void(__thiscall*)(IntSet* thisptr);
    Destructor destructor;

    /** Clears list contents. */
    using Clear = void(__thiscall*)(IntSet* thisptr);
    Clear clear;

    /** Inserts new element to the list. */
    using Insert = Pair<IntSetIterator, bool>*(__thiscall*)(IntSet* thisptr,
                                                            Pair<IntSetIterator, bool>* iterator,
                                                            const int* unitPosition);
    Insert insert;

    /** Removes existing element from list. */
    using Erase = void(__thiscall*)(IntSet* thisptr, const int* unitPosition);
    Erase erase;
};

Api& get();

} // namespace IntSetApi

} // namespace game

#endif // INTSET_H
