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

#ifndef MQSTREAM_H
#define MQSTREAM_H

namespace game {

struct CMqStreamVftable
{
    using Destructor = void(__thiscall*)(CMqStream* thisptr, char flags);
    Destructor destructor;

    using Serialize = void(__thiscall*)(CMqStream* thisptr, const void* data, int count);
    Serialize serialize;

    void* methods[3];
};

static_assert(sizeof(CMqStreamVftable) == 5 * sizeof(void*),
              "CMqStream vftable must have exactly 5 methods");

struct CMqStream
{
    const CMqStreamVftable* vftable;
    bool read;
    char padding[3];
};

} // namespace game

#endif // MQSTREAM_H
