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

#ifndef STRINGINTLIST_H
#define STRINGINTLIST_H

#include "d2list.h"
#include "d2pair.h"
#include "d2string.h"

namespace game {

using StringIntList = List<Pair<String, int>>;

namespace StringIntListApi {

struct Api
{
    using Constructor = StringIntList*(__thiscall*)(StringIntList* thisptr);
    Constructor constructor;

    using Destructor = void(__thiscall*)(StringIntList* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace StringIntListApi

} // namespace game

#endif // STRINGINTLIST_H
