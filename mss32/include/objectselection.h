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

#ifndef OBJECTSELECTION_H
#define OBJECTSELECTION_H

#include "midgardid.h"
#include "smartptr.h"

namespace game {

struct IMidgardObjectMap;
struct ICanSelect;
struct IMqImage2;

struct ObjectSelectionData
{
    IMidgardObjectMap* objectMap;
    ICanSelect* canSelect;
    CMidgardID playerId;
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    SmartPtr<IMqImage2> selectNoImage;
    SmartPtr<IMqImage2> selectYesImage;
};

static_assert(sizeof(ObjectSelectionData) == 52,
              "Size of ObjectSelectionData structure must be exactly 52 bytes");

} // namespace game

#endif // OBJECTSELECTION_H
