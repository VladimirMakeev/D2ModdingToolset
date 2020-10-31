/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef FUNCTOR_H
#define FUNCTOR_H

namespace game {

/**
 * Used as wrapper for ui elements callbacks.
 * Assumption: smart pointer with reference counter as int* instead of 'unknown'
 * and owned data instead of 'functorData'.
 */
struct Functor
{
    void* unknown;
    void* functorData;
};

namespace FunctorApi {

struct Api
{
    /** Creates Functor with specified data, frees memory if functorData is nullptr. */
    using CreateOrFree = void*(__thiscall*)(Functor* thisptr, void* functorData);
    CreateOrFree createOrFree;
};

Api& get();

} // namespace FunctorApi

} // namespace game

#endif // FUNCTOR_H
