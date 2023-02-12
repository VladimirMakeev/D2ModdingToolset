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

#ifndef ENCLAYOUTCITYHOOKS_H
#define ENCLAYOUTCITYHOOKS_H

namespace game {
struct IMidgardObjectMap;
struct CDialogInterf;
struct CFortification;
struct CEncLayoutCity;
struct IMidObject;
struct CMidUnitGroup;
} // namespace game

namespace hooks {

void __fastcall encLayoutCityUpdateHooked(game::CEncLayoutCity* thisptr,
                                          int /*%edx*/,
                                          const game::IMidgardObjectMap* objectMap,
                                          const game::CFortification* fort,
                                          game::CDialogInterf* dialog);

void __fastcall encLayoutCityOnObjectChangedHooked(game::CEncLayoutCity* thisptr,
                                                   int /*%edx*/,
                                                   const game::IMidObject* obj);

void __stdcall encLayoutCityUpdateGroupUiHooked(const game::IMidgardObjectMap* objectMap,
                                                const game::CMidUnitGroup* group,
                                                game::CDialogInterf* dialog,
                                                const char* txtStackNameFormat,
                                                const char* imgStackNameFormat,
                                                bool isStackGroup);

} // namespace hooks

#endif // ENCLAYOUTCITYHOOKS_H
