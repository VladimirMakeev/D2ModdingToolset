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

#ifndef FUNCTORDISPATCH0_H
#define FUNCTORDISPATCH0_H

#include "d2assert.h"

namespace game {

struct CBFunctorDispatch0Vftable;

struct CBFunctorDispatch0
{
    CBFunctorDispatch0Vftable* vftable;
};

struct CBFunctorDispatch0Vftable
{
    using RunCallback = void(__thiscall*)(CBFunctorDispatch0* thisptr);
    RunCallback runCallback;
};

assert_vftable_size(CBFunctorDispatch0Vftable, 1);

template <typename T>
struct CBFunctorDispatch0wRetVftable;

template <typename T>
struct CBFunctorDispatch0wRet
{
    CBFunctorDispatch0wRetVftable<T>* vftable;
};

template <typename T>
struct CBFunctorDispatch0wRetVftable
{
    /** Calls functor-specific callback function that returns T. */
    using RunCallback = T(__thiscall*)(CBFunctorDispatch0wRet<T>* thistr);
    RunCallback runCallback;
};

assert_vftable_size(CBFunctorDispatch0wRetVftable<void*>, 1);

} // namespace game

#endif // FUNCTORDISPATCH0_H
