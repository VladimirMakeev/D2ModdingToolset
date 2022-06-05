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

#include "custommodifiers.h"
#include "dbffile.h"
#include "log.h"
#include "unitutils.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void fillNativeModifiers(CustomModifiers::NativeMap& value)
{
    using namespace game;

    const auto& idApi = CMidgardIDApi::get();

    utils::DbfFile dbf;
    const auto dbfFilePath{globalsFolder() / "GUmodif.dbf"};
    if (!dbf.open(dbfFilePath))
        return;

    const auto recordsTotal{dbf.recordsTotal()};
    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!dbf.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbfFilePath.filename().string()));
            return;
        }

        if (record.isDeleted())
            continue;

        std::string tmp;
        record.value(tmp, "UNIT_ID");
        tmp = trimSpaces(tmp);

        CMidgardID unitId;
        if (*idApi.fromString(&unitId, tmp.c_str()) == invalidId) {
            logError("mssProxyError.log",
                     fmt::format("Could not read unit id '{:s}' from {:s}", tmp.c_str(),
                                 dbfFilePath.filename().string()));
            continue;
        }

        auto& modifiers = value[unitId.value];
        for (int j = 1;; ++j) {
            if (!record.value(tmp, fmt::format("MODIF_{:d}", j)))
                break;

            tmp = trimSpaces(tmp);
            if (!tmp.size())
                break;

            CMidgardID modifierId;
            if (*idApi.fromString(&modifierId, tmp.c_str()) == invalidId) {
                logError("mssProxyError.log",
                         fmt::format("Could not read modifier id '{:s}' from {:s}", tmp.c_str(),
                                     dbfFilePath.filename().string()));
                break;
            }

            modifiers.push_back(modifierId);
        }
    }
}

void initializeCustomModifiers()
{
    using namespace game;

    auto& value = getCustomModifiers();

    value.group.id = (ModifierSourceId)emptyCategoryId;
    value.group.table = nullptr;
    value.group.vftable = LModifGroupApi::vftable();

    fillNativeModifiers(value.native);
}

CustomModifiers& getCustomModifiers()
{
    static CustomModifiers value{};

    return value;
}

NativeModifiers getNativeModifiers(const game::CMidgardID& unitImplId)
{
    using namespace game;

    NativeModifiers result;

    const auto& native = getCustomModifiers().native;
    auto it = native.find(emptyId.value);
    if (it != native.end())
        result = it->second;

    it = native.find(getGlobalUnitImplId(&unitImplId).value);
    if (it != native.end())
        result.insert(result.end(), it->second.begin(), it->second.end());

    return result;
}

} // namespace hooks
