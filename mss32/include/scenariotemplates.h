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

#ifndef SCENARIOTEMPLATES_H
#define SCENARIOTEMPLATES_H

#include "maptemplate.h"

namespace hooks {

/** Cached info about random scenario generator template. */
struct ScenarioTemplate
{
    ScenarioTemplate(std::string&& filename, rsg::MapTemplateSettings&& settings)
        : filename{std::move(filename)}
        , settings{std::move(settings)}
    { }

    std::string filename;
    rsg::MapTemplateSettings settings;
};

using ScenarioTemplates = std::vector<ScenarioTemplate>;

/** Tries to load templates for random scenario generator. */
bool loadScenarioTemplates();

/** Frees previously loaded random scenario generator templates data. */
void freeScenarioTemplates();

const ScenarioTemplates& getScenarioTemplates();

} // namespace hooks

#endif // SCENARIOTEMPLATES_H
