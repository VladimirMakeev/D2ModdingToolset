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

#ifndef TASKCHANGECOLOR_H
#define TASKCHANGECOLOR_H

#include "taskmapchange.h"

namespace game {

struct LTerrainCategory;

namespace editor {

struct CMapInterf;

enum class SelectedTerrain : int
{
    Heretic,
    Dwarf,
    Human,
    Undead,
    Elf,
};

struct CTaskChangeColorData
{
    SelectedTerrain selectedTerrain;
};

assert_size(CTaskChangeColorData, 4);

/** Changes map terrain. */
struct CTaskChangeColor : public CTaskMapChange
{
    CTaskChangeColorData* data;
};

assert_size(CTaskChangeColor, 28);

namespace CTaskChangeColorApi {

struct Api
{
    using Constructor = CTaskChangeColor*(__thiscall*)(CTaskChangeColor* thisptr,
                                                       CTaskManager* taskManager,
                                                       CMapInterf* mapInterf,
                                                       SelectedTerrain selectedTerrain,
                                                       BrushSize selectedBrush);
    Constructor constructor;

    /** Returns terrain category depending on terrain selection. */
    using GetTerrain = LTerrainCategory*(__stdcall*)(LTerrainCategory* terrain,
                                                     SelectedTerrain selectedTerrain);
    GetTerrain getTerrain;
};

Api& get();

} // namespace CTaskChangeColorApi

} // namespace editor
} // namespace game

#endif // TASKCHANGECOLOR_H
