/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "customaibattle.h"
#include "dbffile.h"
#include "log.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

static CustomAiBattleLogic customAiBattleLogic;

const CustomAiBattleLogic& getCustomAiBattleLogic()
{
    return customAiBattleLogic;
}

void initializeCustomAiBattleLogic()
{
    const std::filesystem::path dbfFilePath{globalsFolder() / "GAI.dbf"};

    utils::DbfFile dbf;
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    static const char actionScriptColumnName[]{"ACTION_S"};

    customAiBattleLogic.customBattleLogicEnabled = dbf.column(actionScriptColumnName) != nullptr;
    if (!customAiBattleLogic.customBattleLogicEnabled) {
        return;
    }

    const auto recordsTotal{dbf.recordsTotal()};
    customAiBattleLogic.attitudeBattleLogic.reserve(recordsTotal);

    for (std::uint32_t i = 0u; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted()) {
            continue;
        }

        int id{-1};
        record.value(id, "CATEGORY");
        const auto attitude{static_cast<game::AiAttitudeId>(id)};

        std::string scriptPath;
        record.value(scriptPath, actionScriptColumnName);
        scriptPath = trimSpaces(scriptPath);

        customAiBattleLogic.attitudeBattleLogic[attitude] = std::move(scriptPath);
    }
}

} // namespace hooks
