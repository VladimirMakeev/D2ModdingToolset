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

#ifndef MENURANDOMSCENARIO_H
#define MENURANDOMSCENARIO_H

#include "map.h"
#include "maptemplate.h"
#include "menubase.h"
#include "waitgenerationinterf.h"
#include <array>
#include <thread>
#include <utility>

namespace game {
struct CButtonInterf;
}

namespace hooks {

enum class GenerationStatus : int
{
    NotStarted,    /**< Random scenario generation has not started yet. */
    InProcess,     /**< Generation is in process, generator thread is running. */
    Canceled,      /**< Generation was canceled by player. */
    Done,          /**< Generation successfully done, scenario can be serialized. */
    LimitExceeded, /**< Generation could not succeed in specified number of attempts. */
    Error,         /**< Generation was aborted with an error. */
};

/** Base menu for random scenario generation. */
struct CMenuRandomScenario : public game::CMenuBase
{
    using StartScenario = void (*)(CMenuRandomScenario* menu);

    CMenuRandomScenario(game::CMenuPhase* menuPhase,
                        StartScenario startScenario,
                        const char* dialogName);
    ~CMenuRandomScenario();

    game::UiEvent uiEvent{};
    std::thread generatorThread;
    rsg::MapTemplate scenarioTemplate;
    rsg::MapPtr scenario;

    // Tracks which button shows which race image
    using RaceIndices = std::array<std::pair<game::CButtonInterf*, int /* image index */>, 4>;
    RaceIndices raceIndices;

    WaitGenerationInterf* popup{};
    GenerationStatus generationStatus{GenerationStatus::NotStarted};
    StartScenario startScenario{};
    bool cancelGeneration{false};
};

void prepareToStartRandomScenario(CMenuRandomScenario* menu, bool networkGame = false);

} // namespace hooks

#endif // MENURANDOMSCENARIO_H
