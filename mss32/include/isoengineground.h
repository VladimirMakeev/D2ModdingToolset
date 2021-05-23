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

#ifndef ISOENGINEGROUND_H
#define ISOENGINEGROUND_H

namespace game {

struct CIsoEngineGroundData
{
    char unknown[5448];
};

static_assert(sizeof(CIsoEngineGroundData) == 5448,
              "Size of CIsoEngineGroundData structure must be exactly 5448 bytes");

struct CIsoEngineGround
{
    void* vftable;
    CIsoEngineGroundData* data;
};

static_assert(sizeof(CIsoEngineGround) == 8,
              "Size of CIsoEngineGround structure must be exactly 8 bytes");

} // namespace game

#endif // ISOENGINEGROUND_H
