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

#include "eventeffectcathooks.h"
#include "dbf/dbffile.h"
#include "log.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

CustomEventEffects& customEventEffects()
{
    static CustomEventEffects customEffects{};

    return customEffects;
}

static void readCustomEffect(const utils::DbfRecord& record, CustomEventEffect& effect)
{
    const auto& idApi = game::CMidgardIDApi::get();

    std::string info;
    record.value(info, "INFO");
    idApi.fromString(&effect.infoText, info.c_str());

    std::string brief;
    record.value(brief, "BRIEF");
    idApi.fromString(&effect.brief, brief.c_str());

    std::string descr;
    record.value(descr, "DESCR");
    idApi.fromString(&effect.description, descr.c_str());
}

static bool readCustomEffects(const std::filesystem::path& dbfFilePath)
{
    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return false;
    }

    bool customEffects{false};

    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return false;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string categoryName;
        record.value(categoryName, "TEXT");
        categoryName = trimSpaces(categoryName);
    }

    return customEffects;
}

game::LEventEffectCategoryTable* __fastcall eventEffectCategoryTableCtorHooked(
    game::LEventEffectCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    static const char dbfFileName[] = "LEvEffct.dbf";
    const bool customEffectsExist{false};

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LEventEffectCategoryTableApi::vftable();

    const auto& table = LEventEffectCategoryTableApi::get();
    const auto& effects = EventEffectCategories::get();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);

    table.readCategory(effects.win, thisptr, "L_WIN", dbfFileName);
    table.readCategory(effects.createStack, thisptr, "L_CREATE_STACK", dbfFileName);
    table.readCategory(effects.castSpell, thisptr, "L_CAST_SPELL", dbfFileName);
    table.readCategory(effects.castSpellMap, thisptr, "L_CAST_SPELL_MAP", dbfFileName);
    table.readCategory(effects.changeOwner, thisptr, "L_CHANGE_OWNER", dbfFileName);
    table.readCategory(effects.changeOrder, thisptr, "L_CHANGE_ORDER", dbfFileName);
    table.readCategory(effects.moveBeside, thisptr, "L_MOVE_BESIDE", dbfFileName);
    table.readCategory(effects.battle, thisptr, "L_BATTLE", dbfFileName);
    table.readCategory(effects.enableEvent, thisptr, "L_ENABLE_EVENT", dbfFileName);
    table.readCategory(effects.giveSpell, thisptr, "L_GIVE_SPELL", dbfFileName);
    table.readCategory(effects.giveItem, thisptr, "L_GIVE_ITEM", dbfFileName);
    table.readCategory(effects.moveStack, thisptr, "L_MOVE_STACK", dbfFileName);
    table.readCategory(effects.ally, thisptr, "L_ALLY", dbfFileName);
    table.readCategory(effects.diplomacy, thisptr, "L_DIPLOMACY", dbfFileName);
    table.readCategory(effects.unfog, thisptr, "L_UNFOG", dbfFileName);
    table.readCategory(effects.removeMountain, thisptr, "L_REMOVE_MOUNTAIN", dbfFileName);
    table.readCategory(effects.removeLmark, thisptr, "L_REMOVE_LMARK", dbfFileName);
    table.readCategory(effects.changeObjective, thisptr, "L_CHANGE_OBJECTIVE", dbfFileName);
    table.readCategory(effects.popup, thisptr, "L_POPUP", dbfFileName);
    table.readCategory(effects.destroyItem, thisptr, "L_DESTROY_ITEM", dbfFileName);
    table.readCategory(effects.removeStack, thisptr, "L_REMOVE_STACK", dbfFileName);
    table.readCategory(effects.changeLandmark, thisptr, "L_CHANGE_LANDMARK", dbfFileName);
    table.readCategory(effects.changeTerrain, thisptr, "L_CHANGE_TERRAIN", dbfFileName);
    table.readCategory(effects.modifyVariable, thisptr, "L_MODIFY_VARIABLE", dbfFileName);

    if (customEffectsExist) {
    }

    table.initDone(thisptr);
    return thisptr;
}

} // namespace hooks
