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

#ifndef MIDUNITHOOKS_H
#define MIDUNITHOOKS_H

#include "idlist.h"

namespace game {
struct CMidgardID;
struct IMidgardObjectMap;
struct CMidUnit;
struct CScenarioVisitor;
} // namespace game

namespace hooks {

bool __fastcall addModifierHooked(game::CMidUnit* thisptr,
                                  int /*%edx*/,
                                  const game::CMidgardID* modifierId);

bool __fastcall removeModifierHooked(game::CMidUnit* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* modifierId);

bool __fastcall transformHooked(game::CMidUnit* thisptr,
                                int /*%edx*/,
                                const game::CScenarioVisitor* visitor,
                                const game::IMidgardObjectMap* objectMap,
                                const game::CMidgardID* transformImplId,
                                bool keepHp);

bool __fastcall upgradeHooked(game::CMidUnit* thisptr,
                              int /*%edx*/,
                              const game::CScenarioVisitor* visitor,
                              const game::CMidgardID* upgradeImplId);

bool __stdcall getModifiersHooked(game::IdList* value, const game::CMidUnit* unit);

bool __stdcall addModifiersHooked(const game::IdList* value,
                                  game::CMidUnit* unit,
                                  char* errorBuffer,
                                  bool checkCanApply);

} // namespace hooks

#endif // MIDUNITHOOKS_H
