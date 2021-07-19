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

#ifndef FUNCTORDISPATCH2_H
#define FUNCTORDISPATCH2_H

namespace game {

template <typename T, typename U>
struct CBFunctorDispatch2Vftable;

template <typename T, typename U>
struct CBFunctorDispatch2
{
    CBFunctorDispatch2Vftable<T, U>* vftable;
};

template <typename T, typename U>
struct CBFunctorDispatch2Vftable
{
    /** Calls functor-specific callback function, passing it T and U as a parameters. */
    using RunCallback = void(__thiscall*)(CBFunctorDispatch2<T, U>* thisptr, T value, U value2);
    RunCallback runCallback;
};

static_assert(sizeof(CBFunctorDispatch2Vftable<void*, void*>) == 1 * sizeof(void*),
              "CBFunctorDispatch2 vftable must have exactly 1 method");

template <typename T, typename U, typename Ret>
struct CBFunctorDispatch2wRetVftable;

template <typename T, typename U, typename Ret>
struct CBFunctorDispatch2wRet
{
    CBFunctorDispatch2wRetVftable<T, U, Ret>* vftable;
};

template <typename T, typename U, typename Ret>
struct CBFunctorDispatch2wRetVftable
{
    /** Calls functor-specific callback function, passing it T and U as a parameters. */
    using RunCallback = Ret(__thiscall*)(CBFunctorDispatch2wRet<T, U, Ret>* thisptr,
                                         T value,
                                         U value2);
    RunCallback runCallback;
};

} // namespace game

#endif // FUNCTORDISPATCH2_H
