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

#ifndef DDFOLDEDINVDISPLAY_H
#define DDFOLDEDINVDISPLAY_H

#include "ddfoldedinv.h"
#include "miditemext.h"

namespace game {

struct CMidDragDropInterf;

struct CDDFoldedInvDisplayData
{
    IMidDropManager* dropManager;
    bool unknown;
    char padding[3];
    int unknown2;
    int unknown3;
    CMidDragDropInterf* dragDropInterface;
    int elementCount;
    int unknown6;
    int unknown7;
};

assert_size(CDDFoldedInvDisplayData, 32);

struct CDDFoldedInvDisplay
    : public CDDFoldedInv
    , public IMidItemExt
{
    CDDFoldedInvDisplayData* foldedInvDisplayData;
};

assert_size(CDDFoldedInvDisplay, 20);

} // namespace game

#endif // DDFOLDEDINVDISPLAY_H
