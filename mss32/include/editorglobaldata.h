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

#ifndef EDITORGLOBALDATA_H
#define EDITORGLOBALDATA_H

#include <cstddef>

namespace game {

struct EditorGlobalDataUnknown;
struct EditorGlobalDataUnknownData;
struct CMidgardScenarioMap;

struct EditorGlobalData
{
    bool initialized;
    char unknown[23];
    EditorGlobalDataUnknown* unknown2;
    char unknown3[1080];
};

static_assert(sizeof(EditorGlobalData) == 1108,
              "Size of EditorGlobalData structure must be exactly 1108 bytes");

static_assert(offsetof(EditorGlobalData, unknown2) == 24,
              "EditorGlobalData::unknown2 offset must be 24 bytes");

struct EditorGlobalDataUnknown
{
    EditorGlobalDataUnknownData* data;
};

static_assert(sizeof(EditorGlobalDataUnknown) == 4,
              "Size of EditorGlobalDataUnknown structure must be exactly 4 bytes");

struct EditorGlobalDataUnknownData
{
    int unknown;
    int unknown2;
    CMidgardScenarioMap* scenarioMap;
    int unknown3;
    int unknown4;
    int unknown5;
};

static_assert(sizeof(EditorGlobalDataUnknownData) == 24,
              "Size of EditorGlobalDataUnknownData structure must be exactly 1108 bytes");

namespace EditorGlobalDataApi {

struct Api
{
    using GetGlobalData = EditorGlobalData**(__cdecl*)();
    GetGlobalData getGlobalData;
};

Api& get();

} // namespace EditorGlobalDataApi

} // namespace game

#endif // EDITORGLOBALDATA_H
