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

#ifndef ENCYCLOPEDIAPOPUP_H
#define ENCYCLOPEDIAPOPUP_H

#include "interfdialog.h"
#include "middatacache.h"
#include "smartptr.h"
#include "uievent.h"

namespace game {

struct CEncyclopediaPopupData;
struct CPhase;
struct IEncLayout;
struct CEncParamBase;

struct CEncyclopediaPopup
    : public CInterfDialog
    , public CMidDataCache2::INotify
{
    CEncyclopediaPopupData* data;
};

assert_size(CEncyclopediaPopup, 20);

struct CEncyclopediaPopupData
{
    UiEvent uiEvent;
    CPhase* phase;
    IEncLayout* encLayout;
    bool stickyEncy;
    char padding[3];
};

assert_size(CEncyclopediaPopupData, 36);

namespace CEncyclopediaPopupApi {

struct Api
{
    using Constructor = CEncyclopediaPopup*(__thiscall*)(CEncyclopediaPopup* thisptr,
                                                         CEncParamBase* encParam,
                                                         CPhase* phase);
    Constructor constructor;
};

Api& get();

} // namespace CEncyclopediaPopupApi

namespace editor {

struct CEncyclopediaPopupData;

struct CEncyclopediaPopup : public CInterfDialog
{
    CEncyclopediaPopupData* data;
};

assert_size(CEncyclopediaPopup, 16);

struct CEncyclopediaPopupData
{
    CPhase* phase;
    SmartPointer unknown4;
    UiEvent uiEvent;
    IEncLayout* encLayout;
    bool stickyEncy;
    char padding[3];
};

assert_size(CEncyclopediaPopupData, 44);

struct Api
{
    using Constructor = CEncyclopediaPopup*(__thiscall*)(CEncyclopediaPopup* thisptr,
                                                         CEncParamBase* encParam,
                                                         CPhase* phase,
                                                         int a4);
    Constructor constructor;
};

Api& get();

} // namespace editor

} // namespace game

#endif // ENCYCLOPEDIAPOPUP_H
