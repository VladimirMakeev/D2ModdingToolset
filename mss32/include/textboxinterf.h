/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef TEXTBOXINTERFACE_H
#define TEXTBOXINTERFACE_H

#include "d2string.h"
#include "interface.h"

namespace game {

struct CImage2Text;

struct CTextBoxInterfData
{
    int widgetChildIndex;
    CImage2Text* image2Text;
    CMqPoint pos;
    int unknown;
    String text;
    int unknown2;
    int unknown3;
    String format;
    SmartPointer ptr;
    void* unknown4;
    void* unknown5;
};

static_assert(sizeof(CTextBoxInterfData) == 76,
              "Size of CTextBoxInterfData structure must be exactly 76 bytes");

/**
 * Text ui element.
 * Represents TEXT from Interf.dlg or ScenEdit.dlg files.
 */
struct CTextBoxInterf : public CInterface
{
    CTextBoxInterfData* data;
};

static_assert(sizeof(CTextBoxInterf) == 12,
              "Size of CTextBoxInterf structure must be exactly 12 bytes");

namespace CTextBoxInterfApi {

struct Api
{
    using SetString = void(__thiscall*)(CTextBoxInterf* thisptr, const char* value);
    SetString setString;
};

Api& get();

} // namespace CTextBoxInterfApi

} // namespace game

#endif // TEXTBOXINTERFACE_H
