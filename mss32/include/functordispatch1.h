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

#ifndef FUNCTORDISPATCH1_H
#define FUNCTORDISPATCH1_H

namespace game {

template <typename T>
struct CBFunctorDispatch1Vftable;

template <typename T>
struct CBFunctorDispatch1
{
    CBFunctorDispatch1Vftable<T>* vftable;
};

template <typename T>
struct CBFunctorDispatch1Vftable
{
    /** Calls functor-specific callback function, passing it T as a single parameter. */
    using RunCallback = void(__thiscall*)(CBFunctorDispatch1<T>* thistr, T value);
    RunCallback runCallback;
};

static_assert(sizeof(CBFunctorDispatch1Vftable<void*>) == 1 * sizeof(void*),
              "CBFunctorDispatch1 vftable must have exactly 1 method");

template <typename T, typename U>
struct CBFunctorDispatch1wRetVftable;

template <typename T, typename U>
struct CBFunctorDispatch1wRet
{
    CBFunctorDispatch1wRetVftable<T, U>* vftable;
};

template <typename T, typename U>
struct CBFunctorDispatch1wRetVftable
{
    /**
     * Calls functor-specific callback function that returns U, passing it T as a single parameter.
     */
    using RunCallback = U(__thiscall*)(CBFunctorDispatch1wRet<T, U>* thisptr, T value);
    RunCallback runCallback;
};

} // namespace game

#endif // FUNCTORDISPATCH1_H
