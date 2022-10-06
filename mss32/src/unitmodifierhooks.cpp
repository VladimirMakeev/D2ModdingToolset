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
#include "custommodifierfunctions.h"
#include "custommodifiers.h"
#include "d2string.h"
#include "dbtable.h"
#include "globaldata.h"
#include "mempool.h"
#include "modifgroup.h"
#include "unitmodifier.h"
#include <thread>

extern std::thread::id mainThreadId;

namespace hooks {

struct TUnitModifierDataPatched : game::TUnitModifierData
{
    std::string scriptFileName;
    CustomModifierFunctions* main_thread_functions;
    CustomModifierFunctions* worker_thread_functions;
};

const CustomModifierFunctions& getCustomModifierFunctions(const game::TUnitModifier* unitModifier)
{
    auto data = static_cast<TUnitModifierDataPatched*>(unitModifier->data);

    if (std::this_thread::get_id() == mainThreadId) {
        return *data->main_thread_functions;
    } else {
        return *data->worker_thread_functions;
    }
}

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
    const auto& idApi = CMidgardIDApi::get();

    thisptr->vftable = TUnitModifierApi::vftable();
    thisptr->id = emptyId;

    auto data = (TUnitModifierDataPatched*)memAlloc(sizeof(TUnitModifierDataPatched));
    if (data) {
        data->group.id = (ModifierSourceId)emptyCategoryId;
        data->group.table = nullptr;
        data->group.vftable = LModifGroupApi::vftable();
        data->modifier = nullptr;
        new (&data->scriptFileName) std::string();
        data->main_thread_functions = nullptr;
        data->worker_thread_functions = nullptr;
    }
    thisptr->data = data;

    dbApi.readId(&thisptr->id, dbTable, "MODIF_ID");

    dbApi.findModifGroupCategory(&data->group, dbTable, "SOURCE", (*globalData)->modifGroups);
    if (data->group.id == getCustomModifiers().group.id) {
        String script{};
        dbApi.readString(&script, dbTable, "SCRIPT");
        if (script.string)
            data->scriptFileName = script.string;
        stringApi.free(&script);

        String descTxtString{};
        dbApi.readString(&descTxtString, dbTable, "DESC_TXT");
        CMidgardID descTxt;
        idApi.fromString(&descTxt, descTxtString.string);
        stringApi.free(&descTxtString);

        if (descTxt == invalidId)
            idApi.fromString(&descTxt, "x000tg6000"); // "!! Missing modifier name !!"

        bool display;
        dbApi.readBool(&display, dbTable, "DISPLAY");

        data->main_thread_functions = new CustomModifierFunctions(data->scriptFileName);
        data->worker_thread_functions = new CustomModifierFunctions(data->scriptFileName);
        data->modifier = createCustomModifier(thisptr, data->scriptFileName.c_str(), &descTxt,
                                              display, globalData);
    } else {
        dbApi.readModifier(&data->modifier, &thisptr->id, &data->group, globalsFolderPath,
                           codeBaseEnvProxy, globalData);
    }

    return thisptr;
}

void __fastcall unitModifierDtorHooked(game::TUnitModifier* thisptr, int /*%edx*/, char flags)
{
    using namespace game;

    const auto& memFree = Memory::get().freeNonZero;

    auto data = (TUnitModifierDataPatched*)thisptr->data;
    if (data) {
        auto modifier = data->modifier;
        if (modifier)
            modifier->vftable->destructor(modifier, true);

        data->scriptFileName.~basic_string();
        if (data->main_thread_functions) {
            delete data->main_thread_functions;
        }
        if (data->worker_thread_functions) {
            delete data->worker_thread_functions;
        }

        memFree(data);
    }

    if (flags & 1) {
        memFree(thisptr);
    }
}

} // namespace hooks
