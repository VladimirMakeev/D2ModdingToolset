/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef SUBRACETYPE_H
#define SUBRACETYPE_H

#include "midobject.h"
#include "subracecat.h"
#include "textandid.h"

namespace game {

struct TSubRaceTypeVftable;
struct TSubRaceTypeData;

/** Holds subrace information read from GSubRace.dbf. */
struct TSubRaceType : public IMidObjectT<TSubRaceTypeVftable>
{
    TSubRaceTypeData* data;
};

assert_size(TSubRaceType, 12);

struct TRaceTypeVftable : public IMidObjectVftable
{
    /** Assumption: can be Validate method, similar to TRaceType::Validate. */
    void* method1;
};

assert_vftable_size(TRaceTypeVftable, 2);

struct TSubRaceTypeData
{
    LSubRaceCategory category;
    TextAndId name;
};

assert_size(TSubRaceTypeData, 20);

} // namespace game

#endif // SUBRACETYPE_H
