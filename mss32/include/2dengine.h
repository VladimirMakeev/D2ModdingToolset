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

#ifndef C2DENGINE_H
#define C2DENGINE_H

namespace game {

struct C2DEngineMapImpl;

struct C2DEngineData
{
    C2DEngineMapImpl* engineMapImpl;
    char unknown[32];
};

static_assert(sizeof(C2DEngineData) == 36,
              "Size of C2DEngineData structure must be exactly 36 bytes");

struct C2DEngine
{
    void* vftable;
    C2DEngineData* data;
};

static_assert(sizeof(C2DEngine) == 8, "Size of C2DEngine structure must be exactly 8 bytes");

} // namespace game

#endif // C2DENGINE_H
