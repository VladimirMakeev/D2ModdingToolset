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

#include "uimanager.h"
#include "version.h"
#include <array>

namespace game::CUIManagerApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::Get)0x561afc,
        (Api::CreateTimerEventFunctor)0x643370,
        (Api::CreateTimerEvent)0x561f57,
        (Api::CreateUpdateEventFunctor)0x4ef49d,
        (Api::CreateUiEvent)0x561d0d,
        (Api::CreateUiEvent)0x561d58,
        (Api::CreateUiEvent)0x561d92,
        (Api::CreateUiEvent)0x561dcc,
        (Api::CreateUiEvent)0x561e06,
        (Api::CreateUiEvent)0x561e40,
        (Api::CreateMessageEventFunctor)0x53f60b,
        (Api::CreateMessageEvent)0x561ccb,
        (Api::GetMousePosition)0x561c4f,
        (Api::RegisterMessage)0x561c1c,
    },
    // Russobit
    Api{
        (Api::Get)0x561afc,
        (Api::CreateTimerEventFunctor)0x643370,
        (Api::CreateTimerEvent)0x561f57,
        (Api::CreateUpdateEventFunctor)0x4ef49d,
        (Api::CreateUiEvent)0x561d0d,
        (Api::CreateUiEvent)0x561d58,
        (Api::CreateUiEvent)0x561d92,
        (Api::CreateUiEvent)0x561dcc,
        (Api::CreateUiEvent)0x561e06,
        (Api::CreateUiEvent)0x561e40,
        (Api::CreateMessageEventFunctor)0x53f60b,
        (Api::CreateMessageEvent)0x561ccb,
        (Api::GetMousePosition)0x561c4f,
        (Api::RegisterMessage)0x561c1c,
    },
    // Gog
    Api{
        (Api::Get)0x561299,
        (Api::CreateTimerEventFunctor)0x641bd0,
        (Api::CreateTimerEvent)0x5616f4,
        (Api::CreateUpdateEventFunctor)0x4ee943,
        (Api::CreateUiEvent)0x5614aa,
        (Api::CreateUiEvent)0x5614f5,
        (Api::CreateUiEvent)0x56152f,
        (Api::CreateUiEvent)0x561569,
        (Api::CreateUiEvent)0x5615a3,
        (Api::CreateUiEvent)0x5615dd,
        (Api::CreateMessageEventFunctor)0x53ecc0,
        (Api::CreateMessageEvent)0x561468,
        (Api::GetMousePosition)0x5613ec,
        (Api::RegisterMessage)0x5613b9,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CUIManagerApi
