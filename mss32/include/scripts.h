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

/** Returns lua environment with bound api and specified source loaded and executed. */
sol::environment executeScript(const std::string& source,
                               sol::protected_function_result& result,
                               bool bindScenario = false);

/** Returns lua environment with bound api and specified file loaded and executed. */
std::optional<sol::environment> executeScriptFile(const std::filesystem::path& path,
                                                  bool alwaysExists = false,
                                                  bool bindScenario = false);

/**
 * Returns function with specified name from lua environment to call from c++.
 * @tparam T expected script function signature.
 * @param[in] environment lua environment where to search.
 * @param[in] name function name in lua script.
 * @param[in] alwaysExists true to show error message if the function does not exist.
 */
template <typename T>
static inline std::optional<T> getScriptFunction(const sol::environment& environment,
                                                 const char* name,
                                                 bool alwaysExists = false)
{
    const sol::object object = environment[name];
    const sol::type objectType = object.get_type();

    if (objectType != sol::type::function) {
        if (alwaysExists) {
            showErrorMessageBox(
                fmt::format("'{:s}' is not a function, type: {:d}.", name, (int)objectType));
        }
        return std::nullopt;
    }

    if (!object.is<T>()) {
        showErrorMessageBox(fmt::format("Function '{:s}' has wrong signature.", name));
        return std::nullopt;
    }

    return static_cast<T>(object.as<sol::function>());
}

template <typename T>
static inline void getScriptFunction(const sol::environment& environment,
                                     const char* name,
                                     std::optional<T>* value,
                                     bool alwaysExists = false)
{
    *value = getScriptFunction<T>(environment, name, alwaysExists);
}

/**
 * Returns function with specified name from lua environment to call from c++.
 * @tparam T expected script function signature.
 * @param[in] path filename of script source.
 * @param[in] name function name in lua script.
 * @param[out] environment lua environment where the function executes.
 * @param[in] alwaysExists true to show error message if the function does not exist.
 * @param[in] bindScenario true to bind global 'getScenario' function.
 */
template <typename T>
static inline std::optional<T> getScriptFunction(const std::filesystem::path& path,
                                                 const char* name,
                                                 std::optional<sol::environment>& environment,
                                                 bool alwaysExists = false,
                                                 bool bindScenario = false)
{
    environment = executeScriptFile(path, alwaysExists, bindScenario);
    if (!environment)
        return std::nullopt;

    auto function = getScriptFunction<T>(*environment, name, false);
    if (!function && alwaysExists) {
        showErrorMessageBox(fmt::format("Could not find function '{:s}' in script '{:s}'.\n"
                                        "Make sure function exists and has correct signature.",
                                        name, path.string()));
    }

    return function;
}

/**
 * Returns protected_function that is less performant than unsafe_function, but has error handling.
 * sol::function is defined as unsafe_function for Release build. If an error occurs in such
 * function, it causes Lua stack to not properly unwind after try...catch block, that eventually
 * leads to stack overflow. See https://github.com/ThePhD/sol2/issues/65 for details.
 * @param[in] environment lua environment where to search.
 * @param[in] name function name in lua script.
 * @param[in] alwaysExists true to show error message if the function does not exist.
 */
std::optional<sol::protected_function> getProtectedScriptFunction(
    const sol::environment& environment,
    const char* name,
    bool alwaysExists = false);

} // namespace hooks

#endif // SCRIPTS_H
