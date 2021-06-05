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

namespace editor {

struct CIsoViewData
{
    char unknown[16];
    CDialogInterf* dialogInterf;
    char unknown2[72];
};

static_assert(sizeof(CIsoViewData) == 92,
              "Size of CIsoViewData structure must be exactly 92 bytes");

static_assert(offsetof(CIsoViewData, dialogInterf) == 16,
              "CIsoViewData::dialogInterf offset must be 16 bytes");

/** Represents DLG_ISO_VIEW from ScenEdit.dlg. */
struct CIsoView : public CFullScreenInterf
{
    CIsoViewData* isoViewData;
};

} // namespace editor
} // namespace game

#endif // ISOVIEW_H
