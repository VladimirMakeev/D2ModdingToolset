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

#ifndef MIDISOGROUNDINDEXER_H
#define MIDISOGROUNDINDEXER_H

#include "isogroundindexer.h"

namespace game {

struct IMidgardObjectMap;

struct CMidIsoGroundIndexerData
{
    IMidgardObjectMap* objectMap;
    /** Accessed in getData as: 48 * ((posY / 192 + 24) % 24) + (posX / 192 + 48) % 48 */
    std::uint8_t groundIndices[1152];
    int mapSeed;
};

static_assert(sizeof(CMidIsoGroundIndexerData) == 1160,
              "Size of CMidIsoGroundIndexerData structure must be exactly 1160 bytes");

struct CMidIsoGroundIndexer : public IIsoGroundIndexer
{
    CMidIsoGroundIndexerData* data;
};

static_assert(sizeof(CMidIsoGroundIndexer) == 8,
              "Size of CMidIsoGroundIndexer structure must be exactly 8 bytes");

} // namespace game

#endif // MIDISOGROUNDINDEXER_H
