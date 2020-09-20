/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef MIDSERVERBUILDER_H
#define MIDSERVERBUILDER_H

#include "midgardid.h"

namespace game {

struct IMidServerBuilder
{
    void* vftable;
};

struct CMidServerBuilderFull : public IMidServerBuilder
{
    int unknown;
    char unknown2;
    char unknown3;
    char unknown4;
    char unknown5;
    int unknown6;
    int unknown7;
    CMidgardID id;
    int unknown8;
    char string[261];
};

static_assert(sizeof(CMidServerBuilderFull) == 292,
              "Size of CMidServerBuilderFull structure must be exactly 292 bytes");

} // namespace game

#endif // MIDSERVERBUILDER_H
