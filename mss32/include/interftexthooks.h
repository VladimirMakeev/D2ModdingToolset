/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef INTERFTEXTHOOKS_H
#define INTERFTEXTHOOKS_H

#include "idlist.h"

namespace game {
struct String;
struct LAttackSource;
struct IEncUnitDescriptor;
struct CDialogInterf;
} // namespace game

namespace hooks {

void __stdcall generateAttackDescriptionHooked(game::IEncUnitDescriptor* descriptor,
                                               game::CDialogInterf* dialog,
                                               int boostDamageLevel,
                                               int lowerDamageLevel,
                                               int lowerInitiativeLevel,
                                               const game::IdList* editorModifiers,
                                               int damageMax);

game::String* __stdcall getAttackSourceTextHooked(game::String* value,
                                                  const game::LAttackSource* attackSource);

void __stdcall appendAttackSourceTextHooked(const game::LAttackSource* attackSource,
                                            game::String* value,
                                            bool* valueIsNotEmpty);

} // namespace hooks

#endif // INTERFTEXTHOOKS_H
