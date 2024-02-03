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

#ifndef ISOVIEW_H
#define ISOVIEW_H

#include "fullscreeninterf.h"

namespace game {

struct CDialogInterf;
struct CPalMapInterf;
struct CImage2Text;

namespace editor {

struct CIsoViewData
{
    int unknown;
    CDialogInterf* dlgIsoView;
    CDialogInterf* dlgIso;
    CDialogInterf* dlgIsoPal;
    CDialogInterf* dialogInterf;
    CPalMapInterf* palMap;
    char unknown5[24];
    CImage2Text* image2Text;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    int unknown8;
    int unknown9;
    int unknown10;
    bool showPalMap;
    char padding[3];
};

assert_size(CIsoViewData, 92);
assert_offset(CIsoViewData, dialogInterf, 16);
assert_offset(CIsoViewData, unknown3, 56);

/** Represents DLG_ISO_VIEW from ScenEdit.dlg. */
struct CIsoView : public CFullScreenInterf
{
    CIsoViewData* isoViewData;
};

} // namespace editor
} // namespace game

#endif // ISOVIEW_H
