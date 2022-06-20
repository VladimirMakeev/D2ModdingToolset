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

#ifndef MENUNEWMAP_H
#define MENUNEWMAP_H

#include "menubaseeditor.h"
#include "midgardid.h"
#include "racelist.h"

namespace game {

struct CDialogInterf;
struct String;

namespace editor {

enum class NewMapStatus : int
{
    NotNamed,            /**< The quest has not been named, X001TA0020. */
    AlreadyExists,       /**< This name already exists, X001TA0018. */
    InvalidScenarioInfo, /**< Invalid or missing scenario info, X001TA0000. */
    NoPlayableRaces,     /**< At least one race is required to build a quest, X001TA0019. */
    TooManyRaces,        /**< Too many races selected, X160TA0006. */
    Ok,
};

struct CMenuNewMapData
{
    CDialogInterf* dialogInterf;
    char* scenarioName;
    CMidgardID scenarioFileId;
    int unknown;
    Vector<int> mapSizes;
    Vector<int> unknown2;
};

assert_size(CMenuNewMapData, 48);

/** Represents DLG_NEW_MAP from ScenEdit.dlg. */
struct CMenuNewMap : public CMenuBase
{
    CMenuNewMapData* data;
};

assert_size(CMenuNewMap, 24);

namespace CMenuNewMapApi {

struct Api
{
    /** Converts user input in menu to scenario creation settings. */
    using GetCreationSettings = void(__stdcall*)(CDialogInterf* dialog,
                                                 String* scenarioName,
                                                 int* mapSize,
                                                 RaceCategoryList* races,
                                                 const CMenuNewMapData* data);
    GetCreationSettings getCreationSettings;

    /** Checks map creation settings correctness. */
    using CheckCreationSettings = NewMapStatus(__stdcall*)(const String* scenarioName,
                                                           const int* mapSize,
                                                           const RaceCategoryList* races,
                                                           void* a4);
    CheckCreationSettings checkCreationSettings;
};

Api& get();

} // namespace CMenuNewMapApi

} // namespace editor
} // namespace game

#endif // MENUNEWMAP_H
