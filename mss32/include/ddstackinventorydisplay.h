/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef DDSTACKINVENTORYDISPLAY_H
#define DDSTACKINVENTORYDISPLAY_H

#include "ddfoldedinvdisplay.h"

namespace game {

struct CDDStackInventoryDisplay : public CDDFoldedInvDisplay
{ };

assert_size(CDDStackInventoryDisplay, 20);

namespace CDDStackInventoryDisplayApi {

struct Api
{
    using Constructor = CDDStackInventoryDisplay*(__thiscall*)(CDDStackInventoryDisplay* thisptr,
                                                               CMidDragDropInterf* dragDrop,
                                                               CListBoxInterf* listBox,
                                                               const CMidgardID* objectId,
                                                               IMidgardObjectMap* objectMap);
    Constructor constructor;

    using SetElementCount = void(__thiscall*)(CDDStackInventoryDisplay* thisptr, int elementCount);
    SetElementCount setElementCount;
};

Api& get();

} // namespace CDDStackInventoryDisplayApi

} // namespace game

#endif // DDSTACKINVENTORYDISPLAY_H
