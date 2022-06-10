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
#include "smartptr.h"

namespace game {

struct CPhaseGame;
struct CZoomInterface;
struct CDialogInterf;
struct IMqImage2;
struct CToggleButton;

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

assert_size(CMainView2, 188);

namespace CMainView2Api {

struct Api
{
    using ShowIsoDialog = void(__thiscall*)(CMainView2* thisptr);
    ShowIsoDialog showIsoDialog;

    /**
     * Creates and shows dialog on minimap and options buttons area.
     * When dialogName is nullptr, shows DLG_ISO_LAND or DLG_ISO_PAL.
     * @param[in] dialogName name of the dialog to show instead of minimap or land capture.
     */
    using ShowDialog = void(__thiscall*)(CMainView2* thisptr, const char* dialogName);
    ShowDialog showDialog;

    struct ToggleButtonCallback
    {
        using Callback = void(__thiscall*)(CMainView2* thisptr, bool, CToggleButton*);

        Callback callback;
        int unknown;
    };

    using CreateToggleButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                                int a2,
                                                                CMainView2* mainView,
                                                                ToggleButtonCallback* callback);
    CreateToggleButtonFunctor createToggleButtonFunctor;
};

Api& get();

} // namespace CMainView2Api

} // namespace game

#endif // MAINVIEW2_H
