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

#ifndef MAINVIEW2_H
#define MAINVIEW2_H

#include "fullscreeninterf.h"
#include "middatacache.h"

namespace game {

struct CPhaseGame;
struct CZoomInterface;
struct CDialogInterf;
struct IMqImage2;

struct CMainView2
    : public CFullScreenInterf
    , public CMidDataCache2::INotify
{
    CPhaseGame* phaseGame;
    CInterface* popupDialog;
    CMqRect imgPaletteArea;
    CInterface* dlgIso;
    CZoomInterface* zoomInterface;
    CMqRect imgMainArea;
    CDialogInterf* resourcePopup;
    char unknown14;
    char padding[3];
    int unknown15;
    CDialogInterf* dialogInterf;
    CMqRect imgResourceArea;
    CMqPoint bgndFillSize;
    CMqRect dlgIsoArea;
    void* notify1;
    IMqImage2* isoOutline;
    char unknown29;
    char padding2[3];
    IMqImage2* isoPalFill;
    CMqPoint imgPaletteTopLeftXBottomRightY;
    IMqImage2* isoBlackbar;
    int unknown34;
    int unknown35;
    bool unknown36;
    bool unknown37;
    char padding3[2];
    CMqRect dlgIsoArea2;
    CMqPoint dlgIsoTopLeft;
};

static_assert(sizeof(CMainView2) == 188, "Size of CMainView2 structure must be exactly 188 bytes");

} // namespace game

#endif // MAINVIEW2_H
