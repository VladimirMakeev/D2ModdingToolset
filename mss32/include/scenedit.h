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

#include "d2assert.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CMidgardScenarioMap;
struct GameSettings;
struct GameImageDataWrapper;
struct CAutoDialog;
struct CInterfManagerImpl;
struct CMqFps;

struct CScenEditDataUnknownData
{
    int unknown;
    int unknown2;
    CMidgardScenarioMap* scenarioMap;
    int unknown3;
    int unknown4;
    int unknown5;
};

assert_size(CScenEditDataUnknownData, 24);

struct CScenEditDataUnknown
{
    CScenEditDataUnknownData* data;
};

assert_size(CScenEditDataUnknown, 4);

struct CScenEditData
{
    bool initialized;
    char padding[3];
    GameSettings** gameSettings;
    GameImageDataWrapper* interfImages;
    CAutoDialog* autoDialog;
    char unknown[8];
    CScenEditDataUnknown* unknown2;
    int unknown3;
    void* cityNames;
    void* ruinNames;
    void* merchantNames;
    void* trainerNames;
    void* mercCampNames;
    void* mageNames;
    void* rndUnits;
    void* rndItems;
    SmartPtr<CInterfManagerImpl> interfManager;
    char unknown4[1028];
    CMqFps* fps;
    bool showFullSystemInfo;
    char padding2[3];
};

assert_size(CScenEditData, 1108);
assert_offset(CScenEditData, unknown2, 24);
assert_offset(CScenEditData, interfManager, 64);

struct CScenEdit
{
    CScenEditData* data;
};

assert_size(CScenEdit, 4);

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
