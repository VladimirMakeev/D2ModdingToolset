/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "midserverlogic.h"
#include "version.h"
#include <array>

namespace game::CMidServerLogicApi {

// clang-format off
static std::array<Api, 3> functions = {{
    // Akella
    Api{
        (Api::GetObjectMap)0x4298aa,
        (Api::SendRefreshInfo)0x42972f,
        (Api::StackExchangeItem)0x41f5dc,
        (Api::ApplyEventEffectsAndCheckMidEventTriggerers)0x41f986,
        (Api::StackMove)0x41e983,
        (Api::FilterAndProcessEventsNoPlayer)0x442d07,
        (Api::CheckAndExecuteEvent)0x4420f8,
        (Api::ExecuteEventEffects)0x44267c,
        (Api::FilterAndProcessEvents)0x441d45,
        (Api::CheckEventConditions)0x44213a,
        (Api::Constructor)0x41fea6,
        (Api::GetPlayerInfo)0x429866,
        (Api::IsCurrentPlayer)0x41e77f,
    },
    // Russobit
    Api{
        (Api::GetObjectMap)0x4298aa,
        (Api::SendRefreshInfo)0x42972f,
        (Api::StackExchangeItem)0x41f5dc,
        (Api::ApplyEventEffectsAndCheckMidEventTriggerers)0x41f986,
        (Api::StackMove)0x41e983,
        (Api::FilterAndProcessEventsNoPlayer)0x442d07,
        (Api::CheckAndExecuteEvent)0x4420f8,
        (Api::ExecuteEventEffects)0x44267c,
        (Api::FilterAndProcessEvents)0x441d45,
        (Api::CheckEventConditions)0x44213a,
        (Api::Constructor)0x41fea6,
        (Api::GetPlayerInfo)0x429866,
        (Api::IsCurrentPlayer)0x41e77f,
    },
    // Gog
    Api{
        (Api::GetObjectMap)0x5a77e8,
        (Api::SendRefreshInfo)0x42915a,
        (Api::StackExchangeItem)0x41f0c4,
        (Api::ApplyEventEffectsAndCheckMidEventTriggerers)0x41f46e,
        (Api::StackMove)0x41e46b,
        (Api::FilterAndProcessEventsNoPlayer)0x44296c,
        (Api::CheckAndExecuteEvent)0x441d5d,
        (Api::ExecuteEventEffects)0x4422e1,
        (Api::FilterAndProcessEvents)0x4419aa,
        (Api::CheckEventConditions)0x441d9f,
        (Api::Constructor)0x41f98e,
        (Api::GetPlayerInfo)0x429291,
        (Api::IsCurrentPlayer)0x41e267,
    },
}};

static std::array<Vftable, 3> vftables = {{
    // Akella
    Vftable{
        (void*)0x6d00ec,
        (IMidMsgSenderVftable*)0x6d00bc,
    },
    // Russobit
    Vftable{
        (void*)0x6d00ec,
        (IMidMsgSenderVftable*)0x6d00bc,
    },
    // Gog
    Vftable{
        (void*)0x6ce08c,
        (IMidMsgSenderVftable*)0x6ce05c,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

Vftable& vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CMidServerLogicApi
