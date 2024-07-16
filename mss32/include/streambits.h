/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef STREAMBITS_H
#define STREAMBITS_H

#include "mqstream.h"

namespace game {

struct CMidgardID;

struct CStreamBits : public CMqStream
{
    void* buffer;
    int bufferSize;
    int unknown4;
    char unknown5;
    char unknown6;
    char padding[2];
};

assert_size(CStreamBits, 24);

namespace CStreamBitsApi {

struct Api
{
    using SerializeId = bool(__stdcall*)(CStreamBits* stream, CMidgardID* id);
    SerializeId serializeId;

    /** Create CStreamBits object in a read mode. */
    using ReadConstructor = CStreamBits*(__thiscall*)(CStreamBits* thisptr,
                                                      int dummy,
                                                      void* buffer,
                                                      std::uint32_t bufferSize,
                                                      bool a5);
    ReadConstructor readConstructor;

    using Destructor = void(__thiscall*)(CStreamBits* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace CStreamBitsApi

} // namespace game

#endif // STREAMBITS_H
