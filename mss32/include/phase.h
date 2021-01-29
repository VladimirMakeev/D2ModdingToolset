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

#ifndef PHASE_H
#define PHASE_H

namespace game {

struct CMidClient;
struct IMidgardObjectMap;

struct CPhaseData
{
    CMidClient* midClient;
    int unknown;
};

static_assert(sizeof(CPhaseData) == 8, "Size of CPhaseData structure must be exactly 8 bytes");

struct CPhase
{
    void* vftable;
    CPhaseData* data;
};

static_assert(sizeof(CPhase) == 8, "Size of CPhase structure must be exactly 8 bytes");

namespace CPhaseApi {

struct Api
{
    using GetObjectMap = IMidgardObjectMap*(__thiscall*)(CPhase* thisptr);
    GetObjectMap getObjectMap;
};

Api& get();

} // namespace CPhaseApi

} // namespace game

#endif // PHASE_H
