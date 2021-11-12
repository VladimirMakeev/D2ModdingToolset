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

#ifndef MIDSERVERBUILDER_H
#define MIDSERVERBUILDER_H

#include "midgardid.h"

namespace game {

struct IMidServerBuilderVftable;
struct CMidServerLogic;
struct CMidServer;

struct IMidServerBuilder
{
    IMidServerBuilderVftable* vftable;
};

struct IMidServerBuilderVftable
{
    using Destructor = void(__thiscall*)(IMidServerBuilder* thisptr, char flags);
    Destructor destructor;

    using CreateServerLogic = CMidServerLogic*(__thiscall*)(IMidServerBuilder* thisptr,
                                                            CMidServer* server);
    CreateServerLogic createServerLogic;

    using LoadScenario = void(__thiscall*)(IMidServerBuilder* thisptr,
                                           CMidServer* midServer,
                                           CMidServerLogic* serverLogic);
    LoadScenario loadScenario;

    using IsMultiplayerGame = bool(__thiscall*)(const IMidServerBuilder* thisptr);
    IsMultiplayerGame isMultiplayerGame;

    /**
     * Assumption: returns number of players in loaded scenario.
     * @returns CMidServerBuilderFull::unknown.
     */
    using GetPlayerCount = int(__thiscall*)(IMidServerBuilder* thisptr);
    GetPlayerCount getPlayerCount;
};

static_assert(sizeof(IMidServerBuilderVftable) == 5 * sizeof(void*),
              "Size of IMidServerBuilder vftable must have exactly 5 methods");

struct CMidServerBuilderFull : public IMidServerBuilder
{
    int unknown;
    char unknown2;
    bool useRandom;
    bool multiplayerGame;
    bool hotseatGame;
    int value3; /**< Always set to 3 in constructor. */
    int gameVersion;
    CMidgardID id;
    CMidgardID campaignId;
    char scenarioFilePath[261];
};

static_assert(sizeof(CMidServerBuilderFull) == 292,
              "Size of CMidServerBuilderFull structure must be exactly 292 bytes");

} // namespace game

#endif // MIDSERVERBUILDER_H
