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

#ifndef STRINGARRAY_H
#define STRINGARRAY_H

#include "d2assert.h"
#include "d2vector.h"

namespace game {

struct String;

using StringArray = Vector<String>;

assert_size(StringArray, 16);

namespace StringArrayApi {

struct Api
{
    /**
     * Adds new string element to the end of the array.
     * @param[in] thisptr pointer to array to add to.
     * @param[in] string element to push.
     * @returns unknown.
     */
    using PushBack = void*(__thiscall*)(StringArray* thisptr, const String* string);
    PushBack pushBack;
};

/** Returns StringArray functions according to determined version of the game. */
Api& get();

} // namespace StringArrayApi

} // namespace game

#endif // STRINGARRAY_H
