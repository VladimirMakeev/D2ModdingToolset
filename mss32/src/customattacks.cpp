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

#include "customattacks.h"
#include "dbffile.h"
#include "log.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void initializeCustomAttacks()
{
    utils::DbfFile dbf;
    const std::filesystem::path dbfFilePath{globalsFolder() / "Gattacks.dbf"};
    if (!dbf.open(dbfFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbfFilePath.filename().string()));
        return;
    }

    getCustomAttacks().damageRatiosEnabled = dbf.column(damageRatioColumnName)
                                             && dbf.column(damageRatioPerTargetColumnName)
                                             && dbf.column(damageSplitColumnName);

    getCustomAttacks().critSettingsEnabled = dbf.column(critDamageColumnName)
                                             && dbf.column(critPowerColumnName);
}

CustomAttacks& getCustomAttacks()
{
    static CustomAttacks value{};

    return value;
}

} // namespace hooks
