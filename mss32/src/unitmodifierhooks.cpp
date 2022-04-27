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

    auto group = (LModifGroup*)memAlloc(sizeof(LModifGroup));
    if (group) {
        group->id = (ModifierSourceId)-1;
        group->table = nullptr;
        group->vftable = LModifGroupApi::vftable();
        group->modifier = nullptr;
    }
    thisptr->group = group;

    dbApi.readId(&thisptr->id, dbTable, "MODIF_ID");

    String script{};
    if (getCustomModifiers().enabled)
        dbApi.readString(&script, dbTable, "SCRIPT");

    if (script.length) {
        auto customModifier = (CCustomModifier*)memAlloc(sizeof(CCustomModifier));
        customModifierCtor(customModifier, script.string, &thisptr->id, globalData);
        group->modifier = &customModifier->umModifier;
    } else {
        dbApi.findModifGroupCategory(group, dbTable, "SOURCE", (*globalData)->modifGroups);
        dbApi.readModifier(&group->modifier, &thisptr->id, group, globalsFolderPath,
                           codeBaseEnvProxy, globalData);
    }

    stringApi.free(&script);

    return thisptr;
}

} // namespace hooks
