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

#ifndef STRINGPAIRARRAYPTR_H
#define STRINGPAIRARRAYPTR_H

#include "d2pair.h"
#include "d2vector.h"
#include "smartptr.h"

namespace game {

using StringPairArray = Vector<Pair<char[11], char[257]>>;
using StringPairArrayPtr = SmartPtr<StringPairArray>;

namespace StringPairArrayPtrApi {

struct Api
{
    using Constructor = StringPairArrayPtr*(__thiscall*)(StringPairArrayPtr* thisptr);
    Constructor constructor;

    using SetData = void(__thiscall*)(StringPairArrayPtr* thisptr, StringPairArray* data);
    SetData setData;
};

Api& get();

} // namespace StringPairArrayPtrApi

} // namespace game

#endif // STRINGPAIRARRAYPTR_H
