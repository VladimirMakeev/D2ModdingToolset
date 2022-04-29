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

#include "unitmodifierhooks.h"
#include "custommodifier.h"
#include "custommodifiers.h"
#include "d2string.h"
#include "dbtable.h"
#include "globaldata.h"
#include "mempool.h"
#include "modifgroup.h"
#include "unitmodifier.h"

namespace hooks {

game::TUnitModifier* __fastcall unitModifierCtorHooked(game::TUnitModifier* thisptr,
                                                       int /*%edx*/,
                                                       const game::CDBTable* dbTable,
                                                       const char* globalsFolderPath,
                                                       void* codeBaseEnvProxy,
                                                       const game::GlobalData** globalData)
{
    using namespace game;

    const auto& memAlloc = Memory::get().allocate;
    const auto& dbApi = CDBTableApi::get();
    const auto& stringApi = StringApi::get();

    thisptr->vftable = TUnitModifierApi::vftable();
    thisptr->id = emptyId;

    auto data = (TUnitModifierData*)memAlloc(sizeof(TUnitModifierData));
    if (data) {
        data->group.id = (ModifierSourceId)-1;
        data->group.table = nullptr;
        data->group.vftable = LModifGroupApi::vftable();
        data->modifier = nullptr;
    }
    thisptr->data = data;

    dbApi.readId(&thisptr->id, dbTable, "MODIF_ID");

    dbApi.findModifGroupCategory(&data->group, dbTable, "SOURCE", (*globalData)->modifGroups);
    if (data->group.id == getCustomModifiers().group.id) {
        String script{};
        dbApi.readString(&script, dbTable, "SCRIPT");

        data->modifier = createCustomModifier(script.string, &thisptr->id, globalData);

        stringApi.free(&script);
    } else {
        dbApi.readModifier(&data->modifier, &thisptr->id, &data->group, globalsFolderPath,
                           codeBaseEnvProxy, globalData);
    }

    return thisptr;
}

} // namespace hooks
