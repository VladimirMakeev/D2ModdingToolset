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

#ifndef DDFOLDEDINV_H
#define DDFOLDEDINV_H

#include "middropsource.h"
#include "middroptarget.h"
#include "midgardobjectmap.h"

namespace game {

struct CListBoxInterf;
struct IMidDropManager;
struct CInventoryCounter;
struct CursorHandle;

struct CDDFoldedInvData
{
    CListBoxInterf* listBox;
    IMidDropManager* midDropManager;
    CMidgardID unknownId;
    CInventoryCounter* inventoryCounter;
    CMidgardID objectId;
    char unknown4[4];
    IMidgardObjectMap* objectMap;
    char unknown5[28];
    IMqImage2* dragDropValidImage;
    IMqImage2* dragDropInvalidImage;
    SmartPtr<CursorHandle> defaultCursor;
    SmartPtr<CursorHandle> noDragDropCursor;
    char unknown6[4];
};

assert_size(CDDFoldedInvData, 84);

// TODO: has bigger vftable than IMidDropSource: 27 methods instead of 10
struct CDDFoldedInv
    : public IMidDropSource
    , public IMidDropTarget
{
    CDDFoldedInvData* foldedInvData;
};

assert_size(CDDFoldedInv, 12);

} // namespace game

#endif // DDFOLDEDINV_H
