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

#ifndef LANDMARK_H
#define LANDMARK_H

#include "landmarkcat.h"
#include "midgardid.h"
#include "midobject.h"
#include "mqpoint.h"
#include "textandid.h"

namespace game {

struct TLandmarkData
{
    CMqPoint size;
    bool mountain;
    char padding[3];
    LLandmarkCategory category;
    TextAndId name;
    TextAndId description;
};

static_assert(sizeof(TLandmarkData) == 40,
              "Size of TLandmarkData structure must be exactly 40 bytes");

/** Holds landmark information read from GLmark.dbf. */
struct TLandmark : public IMidObject
{
    CMidgardID landmarkId;
    TLandmarkData* data;
};

static_assert(sizeof(TLandmark) == 12, "Size of TLandmark structure must be exactly 12 bytes");

} // namespace game

#endif // LANDMARK_H
