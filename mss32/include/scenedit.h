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

#ifndef SCENEDIT_H
#define SCENEDIT_H

#include <cstddef>

namespace game {

struct CScenEditData;
struct CScenEditDataUnknown;
struct CScenEditDataUnknownData;
struct CMidgardScenarioMap;

struct CScenEdit
{
    CScenEditData* data;
};

static_assert(sizeof(CScenEdit) == 4, "Size of CScenEdit structure must be exactly 4 bytes");

struct CScenEditData
{
    bool initialized;
    char unknown[23];
    CScenEditDataUnknown* unknown2;
    char unknown3[1080];
};

static_assert(sizeof(CScenEditData) == 1108,
              "Size of CScenEditData structure must be exactly 1108 bytes");

static_assert(offsetof(CScenEditData, unknown2) == 24,
              "CScenEditData::unknown2 offset must be 24 bytes");

struct CScenEditDataUnknown
{
    CScenEditDataUnknownData* data;
};

static_assert(sizeof(CScenEditDataUnknown) == 4,
              "Size of CScenEditDataUnknown structure must be exactly 4 bytes");

struct CScenEditDataUnknownData
{
    int unknown;
    int unknown2;
    CMidgardScenarioMap* scenarioMap;
    int unknown3;
    int unknown4;
    int unknown5;
};

static_assert(sizeof(CScenEditDataUnknownData) == 24,
              "Size of CScenEditDataUnknownData structure must be exactly 24 bytes");

namespace CScenEditApi {

struct Api
{
    using Instance = CScenEdit*(__cdecl*)();
    Instance instance;
};

Api& get();

} // namespace CScenEditApi

} // namespace game

#endif // SCENEDIT_H
