/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef ENCLAYOUTUNITHOOKS_H
#define ENCLAYOUTUNITHOOKS_H

namespace game {
struct CMqRect;
struct CMidgardID;
struct IMidgardObjectMap;
struct CInterface;
struct IEncUnitDescriptor;
struct CEncParamBase;
struct CEncLayoutUnit;
struct CEncLayoutUnitData;
} // namespace game

namespace hooks {

game::CEncLayoutUnit* __fastcall encLayoutUnitCtorHooked(game::CEncLayoutUnit* thisptr,
                                                         int /*%edx*/,
                                                         const game::IMidgardObjectMap* objectMap,
                                                         game::CInterface* parent,
                                                         const game::CMqRect* rect,
                                                         const game::CMidgardID* unitId,
                                                         const game::CEncParamBase* encParam,
                                                         const game::CMidgardID* playerId);

game::CEncLayoutUnit* __fastcall encLayoutUnitCtor2Hooked(game::CEncLayoutUnit* thisptr,
                                                          int /*%edx*/,
                                                          game::IEncUnitDescriptor* descriptor,
                                                          game::CInterface* parent,
                                                          const game::CMqRect* rect,
                                                          const game::CEncParamBase* encParam);

game::CEncLayoutUnitData* __fastcall encLayoutUnitDataCtorHooked(game::CEncLayoutUnitData* thisptr,
                                                                 int /*%edx*/);

void __fastcall encLayoutUnitDataDtorHooked(game::CEncLayoutUnitData* thisptr, int /*%edx*/);

void __fastcall encLayoutUnitInitializeHooked(game::CEncLayoutUnit* thisptr,
                                              const game::CEncParamBase* encParam);

void __fastcall encLayoutUnitUpdateHooked(game::CEncLayoutUnit* thisptr, int /*%edx*/);

} // namespace hooks

#endif // ENCLAYOUTUNITHOOKS_H
