/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef USUNIT_H
#define USUNIT_H

#include "midobject.h"
#include "usunitextension.h"

namespace game {

struct IUsUnitVftable;
struct LUnitCategory;

struct IUsUnit : public IMidObjectT<IUsUnitVftable>
{ };

struct IUsUnitVftable : public IMidObjectVftable
{
    /** Most of the time it works like dynamic_cast calculating offset of extension interface.
     * However, if this is a unit modifier (CUmUnit, CUmAttack or CUmStack), an aggregate object can
     * be returned instead (the method functions like queryInterface), so dynamic_cast is no fit.
     */
    using Cast = IUsUnitExtension*(__thiscall*)(const IUsUnit* thisptr, const char* rawTypeName);
    Cast cast;

    using GetCategory = const LUnitCategory*(__thiscall*)(const IUsUnit* thisptr);
    GetCategory getCategory;
};

} // namespace game

#endif // USUNIT_H
