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

#ifndef ISOGROUNDINDEXER_H
#define ISOGROUNDINDEXER_H

#include <cstdint>

namespace game {

struct IIsoGroundIndexerVftable;
struct CMqPoint;

struct IIsoGroundIndexer
{
    IIsoGroundIndexerVftable* vftable;
};

struct IIsoGroundIndexerVftable
{
    using Destructor = void(__thiscall*)(IIsoGroundIndexer* thisptr, char flags);
    Destructor destructor;

    using UpdateData = void(__thiscall*)(IIsoGroundIndexer* thisptr);
    UpdateData updateData;

    using GetData = std::uint8_t(__thiscall*)(const IIsoGroundIndexer* thisptr,
                                              const CMqPoint* position);
    GetData getData;
};

} // namespace game

#endif // ISOGROUNDINDEXER_H
