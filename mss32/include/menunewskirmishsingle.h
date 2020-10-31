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

#ifndef MENUNEWSKIRMISHSINGLE_H
#define MENUNEWSKIRMISHSINGLE_H

#include "menunewskirmish.h"

namespace game {

/** Single player skirmish menu. */
struct CMenuNewSkirmishSingle : public CMenuNewSkirmish
{ };

namespace CMenuNewSkirmishSingleApi {

struct Api
{
    /**
     * Initializes CMenuNewSkirmishSingle object, sets vftable.
     * @param[in] thisptr pointer to existing CMenuNewSkirmishSingle object.
     * @param a1 meaning unknown.
     * @returns thisptr.
     */
    using Constructor = CMenuNewSkirmishSingle*(__thiscall*)(CMenuNewSkirmishSingle* thisptr,
                                                             int a1);
    Constructor constructor;
};

Api& get();

struct Vftable
{
    void* unknown[38];
};

Vftable* vftable();

} // namespace CMenuNewSkirmishSingleApi

} // namespace game

#endif // MENUNEWSKIRMISHSINGLE_H
