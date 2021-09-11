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

#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "log.h"
#include <filesystem>
#include <fmt/format.h>
#include <lua.hpp>
#include <optional>
#include <sol/sol.hpp>

namespace hooks {

template <typename T>
static inline std::optional<T> getFunction(const sol::object object, const char* name)
{
    const sol::type objectType = object.get_type();

    if (objectType != sol::type::function) {
        logError("mssProxyError.log",
                 fmt::format("'{:s}' is not a function, type: {:d}", name, (int)objectType));
        return std::nullopt;
    }

    if (!object.is<T>()) {
        logError("mssProxyError.log", fmt::format("Function '{:s}' has wrong signature", name));
        return std::nullopt;
    }

    return static_cast<T>(object.as<sol::function>());
}

/**
 * Returns script function with specified name to call from c++.
 * @tparam T expected script function signature.
 * @param[in] lua fully initialized and loaded lua state wrapper where to search.
 * @param[in] name function name in lua script.
 */
template <typename T>
static inline std::optional<T> getScriptFunction(const sol::state& lua, const char* name)
{
    return getFunction<T>(lua[name], name);
}

/**
 * Returns function with specified name from lua environment to call from c++.
 * @tparam T expected script function signature.
 * @param[in] env lua environment where to search.
 * @param[in] name function name in lua script.
 */
template <typename T>
static inline std::optional<T> getScriptFunction(const sol::environment& env, const char* name)
{
    return getFunction<T>(env[name], name);
}

/** Returns lua state wrapper with specified script loaded in it and api bound. */
std::optional<sol::state> loadScriptFile(const std::filesystem::path& path,
                                         bool alwaysExists = false,
                                         bool bindApi = false);

/** Returns lua state wrapper with optionally bound api. */
sol::state createLuaState(bool bindApi = false);

} // namespace hooks

#endif // SCRIPTS_H
