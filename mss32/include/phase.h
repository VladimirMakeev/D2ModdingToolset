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

#include "d2assert.h"

namespace game {

struct CMidClient;
struct IMidgardObjectMap;
struct CMidgardID;
struct CInterface;

struct CPhaseData
{
    CMidClient* midClient;
    CInterface* encyclopediaPopup;
};

assert_size(CPhaseData, 8);

struct CPhase
{
    void* vftable;
    CPhaseData* data;
};

assert_size(CPhase, 8);

namespace CPhaseApi {

struct Api
{
    using GetObjectMap = IMidgardObjectMap*(__thiscall*)(CPhase* thisptr);
    GetObjectMap getObjectMap;

    using GetCurrentPlayerId = const CMidgardID*(__thiscall*)(CPhase* thisptr);
    GetCurrentPlayerId getCurrentPlayerId;
};

Api& get();

} // namespace CPhaseApi

} // namespace game

#endif // PHASE_H
