/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "scenariotemplates.h"
#include "maptemplatereader.h"
#include "utils.h"
#include <sol/sol.hpp>

namespace hooks {

static ScenarioTemplates scenarioTemplates;

bool loadScenarioTemplates()
{
    namespace fs = std::filesystem;

    const auto& folder{templatesFolder()};
    if (!fs::exists(folder)) {
        return true;
    }

    std::set<fs::path> templateFiles;
    // Make sure we read templates always in the same order (alphabetical)
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().extension() == ".lua") {
            templateFiles.insert(entry.path());
        }
    }

    for (const auto& templateFile : templateFiles) {
        try {
            // Create a new lua VM until we use environments.
            // Without them VM gets polluted with previous data and works incorrect
            sol::state lua;
            rsg::bindLuaApi(lua);

            scenarioTemplates.emplace_back(templateFile.string(),
                                           rsg::readTemplateSettings(templateFile, lua));
        } catch (const std::exception&) {
            // Silently ignore lua files that are not templates
        }
    }

    return true;
}

void freeScenarioTemplates()
{
    scenarioTemplates.clear();
}

const ScenarioTemplates& getScenarioTemplates()
{
    return scenarioTemplates;
}

} // namespace hooks
