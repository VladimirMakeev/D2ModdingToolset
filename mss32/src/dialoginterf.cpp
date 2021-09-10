/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "dialoginterf.h"
#include "version.h"
#include <array>

namespace game::CDialogInterfApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::FindControl)0x50c206,
        (Api::FindUiElement<CButtonInterf>)0x50baaf,
        (Api::FindUiElement<CListBoxInterf>)0x50bacf,
        (Api::FindUiElement<CTextBoxInterf>)0x50bb0f,
        (Api::FindUiElement<CPictureInterf>)0x50badf,
        (Api::FindUiElement<CRadioButtonInterf>)0x50baef,
        (Api::FindUiElement<CToggleButton>)0x50bb1f,
        (Api::FindUiElement<CSpinButtonInterf>)0x50baff,
        (Api::FindUiElement<CEditBoxInterf>)0x50babf,
        (Api::FindUiElement<CScrollBarInterf>)nullptr,
        (Api::ShowControl)0x5c9d95,
        (Api::HideControl)0x50c291,
    },
    // Russobit
    Api{
        (Api::FindControl)0x50c206,
        (Api::FindUiElement<CButtonInterf>)0x50baaf,
        (Api::FindUiElement<CListBoxInterf>)0x50bacf,
        (Api::FindUiElement<CTextBoxInterf>)0x50bb0f,
        (Api::FindUiElement<CPictureInterf>)0x50badf,
        (Api::FindUiElement<CRadioButtonInterf>)0x50baef,
        (Api::FindUiElement<CToggleButton>)0x50bb1f,
        (Api::FindUiElement<CSpinButtonInterf>)0x50baff,
        (Api::FindUiElement<CEditBoxInterf>)0x50babf,
        (Api::FindUiElement<CScrollBarInterf>)nullptr,
        (Api::ShowControl)0x5c9d95,
        (Api::HideControl)0x50c291,
    },
    // Gog
    Api{
        (Api::FindControl)0x50b70a,
        (Api::FindUiElement<CButtonInterf>)0x50afb3,
        (Api::FindUiElement<CListBoxInterf>)0x50afd3,
        (Api::FindUiElement<CTextBoxInterf>)0x50b013,
        (Api::FindUiElement<CPictureInterf>)0x50afe3,
        (Api::FindUiElement<CRadioButtonInterf>)0x50aff3,
        (Api::FindUiElement<CToggleButton>)0x50b023,
        (Api::FindUiElement<CSpinButtonInterf>)0x50b003,
        (Api::FindUiElement<CEditBoxInterf>)0x50afc3,
        (Api::FindUiElement<CScrollBarInterf>)nullptr,
        (Api::ShowControl)0x5c8d63,
        (Api::HideControl)0x50b795,
    },
    // Scenario Editor
    Api{
        (Api::FindControl)0x4a3d15,
        (Api::FindUiElement<CButtonInterf>)0x4a3501,
        (Api::FindUiElement<CListBoxInterf>)0x4a3521,
        (Api::FindUiElement<CTextBoxInterf>)0x4a3571,
        (Api::FindUiElement<CPictureInterf>)0x4a3531,
        (Api::FindUiElement<CRadioButtonInterf>)0x4a3541,
        (Api::FindUiElement<CToggleButton>)0x4a3581,
        (Api::FindUiElement<CSpinButtonInterf>)0x4a3561,
        (Api::FindUiElement<CEditBoxInterf>)0x4a3511,
        (Api::FindUiElement<CScrollBarInterf>)0x4a3551,
        (Api::ShowControl)0x4d19c7,
        (Api::HideControl)0x4a3da0,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CDialogInterfApi
