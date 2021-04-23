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

#include "textids.h"
#include "log.h"
#include "utils.h"
#include <fmt/format.h>
#include <sol/sol.hpp>

namespace hooks {

static void readInterfTextIds(TextIds& textids, const sol::state& lua)
{
    const sol::table& table = lua["textids"];
    auto interf = table.get<sol::optional<sol::table>>("interf");
    if (!interf.has_value())
        return;

    textids.interf.sellAllValuables = interf.value().get_or("sellAllValuables", std::string());
    textids.interf.infiniteAttack = interf.value().get_or("infiniteAttack", std::string());
    textids.interf.critHitAttack = interf.value().get_or("critHitAttack", std::string());
}

const TextIds& textIds()
{
    static TextIds value;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;

        const auto path{hooks::gameFolder() / "Scripts" / "textids.lua"};
        if (std::filesystem::exists(path)) {
            sol::state lua;

            auto config = lua.load_file(path.string());
            const auto result = config();

            if (result.valid()) {
                readInterfTextIds(value, lua);
            } else {
                const sol::error err = result;
                hooks::logError("mssProxyError.log",
                                fmt::format("Failed to load textids script '{:s}'.\nReason: '{:s}'",
                                            path.string(), err.what()));
            }
        }
    }

    return value;
}

} // namespace hooks
