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

#ifndef AISTACKGENERIC_H
#define AISTACKGENERIC_H

#include "aistack.h"
#include "d2assert.h"

namespace game {

struct IMidgardObjectMap;
struct IAiActionFilter;

struct CAiStackGeneric : IAiStack
{
    IMidgardObjectMap* objectMap;
    CMidgardID stackId;
    CMidStack* stack;
    bool waterOnly;
    bool inCityUnderRiot;
    char padding[2];
    CAiAttitudes* attitudes;
    bool unknown24;
    char padding2[3];
    IAiActionFilter* actionFilter;
};

assert_size(CAiStackGeneric, 32);
assert_offset(CAiStackGeneric, unknown24, 24);

} // namespace game

#endif // AISTACKGENERIC_H
