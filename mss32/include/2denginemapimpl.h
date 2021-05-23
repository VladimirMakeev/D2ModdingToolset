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

#ifndef C2DENGINEMAPIMPL_H
#define C2DENGINEMAPIMPL_H

#include "2denginemap.h"

namespace game {

struct C2DEngineMapImplData
{
    char unknown[120];
};

static_assert(sizeof(C2DEngineMapImplData) == 120,
              "Size of C2DEngineMapImplData structure must be exactly 120 bytes");

struct C2DEngineMapImpl : public C2DEngineMap
{
    C2DEngineMapImplData* data;
};

static_assert(sizeof(C2DEngineMapImpl) == 8,
              "Size of C2DEngineMapImpl structure must be exactly 8 bytes");

} // namespace game

#endif // C2DENGINEMAPIMPL_H
