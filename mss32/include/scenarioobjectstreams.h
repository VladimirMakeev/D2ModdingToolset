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

#ifndef SCENARIOOBJECTSTREAMS_H
#define SCENARIOOBJECTSTREAMS_H

#include "d2assert.h"
#include "d2vector.h"

namespace game {

struct CStreamRegisterBase;

/** Holds stream registers with their raw names for scenario objects streaming. */
struct ScenarioObjectStreams
{
    Vector<CStreamRegisterBase*> knownStreams;
    /** Maps raw names of CStreamRegisterBase child classes to indices in knownStreams. */
    void* rawNameIndicesMapping;
};

assert_size(ScenarioObjectStreams, 20);

namespace ScenarioObjectStreamsApi {

struct Api
{
    /** Adds specified streamRegister to ScenarioObjectStreams. */
    using AddStreamRegister = bool(__stdcall*)(CStreamRegisterBase* streamRegister,
                                               const char* rawName);
    AddStreamRegister addStreamRegister;
};

Api& get();

} // namespace ScenarioObjectStreamsApi

} // namespace game

#endif // SCENARIOOBJECTSTREAMS_H
