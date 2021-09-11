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

#ifndef MIDEVEFFECTHOOKS_H
#define MIDEVEFFECTHOOKS_H

namespace game {
struct CMidgardID;
struct CMidEvEffect;
struct LEventEffectCategory;
struct String;
struct IMidgardObjectMap;
} // namespace game

namespace hooks {

game::CMidEvEffect* __stdcall createEventEffectFromCategoryHooked(
    const game::CMidgardID* eventId,
    const game::LEventEffectCategory* category);

void __stdcall eventEffectGetInfoStringHooked(game::String* info,
                                              const game::IMidgardObjectMap* objectMap,
                                              const game::CMidEvEffect* eventEffect);

void __stdcall eventEffectGetDescriptionHooked(game::String* description,
                                               const game::LEventEffectCategory* category);

void __stdcall eventEffectGetBriefHooked(game::String* brief,
                                         const game::LEventEffectCategory* category);

} // namespace hooks

#endif // MIDEVEFFECTHOOKS_H
