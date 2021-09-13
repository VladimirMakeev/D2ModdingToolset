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

#ifndef MIDGARD_H
#define MIDGARD_H

#include "mqanimation.h"
#include "mqnettraffic.h"
#include "smartptr.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct IMqNetService;
struct IMqNetSession;
struct CMidClient;
struct CMidServer;
struct GameSettings;
struct CMenuPhase;
struct CAutoDialog;
struct CUIManager;
struct CInterfManagerImpl;
struct CMqFps;
struct CInterface;

struct CMidgardData
{
    char unknown[4];
    IMqNetService* netService;
    IMqNetSession* netSession;
    void* directPlayLobbySession;
    int unknown6;
    int unknown7;
    int unknown8;
    int unknown9;

    int unknown10;
    int unknown11;
    CMidClient* client;
    CMidServer* server;
    bool multiplayerGame; /**< It can be network or hotseat game. Check hotseat flag to be sure. */
    bool hotseatGame;     /**< Multiplayer game on the same computer. */
    bool host;            /**< True if computer hosts the game, server. */
    bool lobbySessionExists;
    void* midStart;
    CMenuPhase* menuPhase;
    GameSettings** settings;
    void* interfImages;
    CAutoDialog* autoDialog;
    void* menuAnimations;
    void* menuImages;
    SmartPtr<CUIManager> uiManager;
    char unknown12[4];
    std::uint32_t startMessageId;
    char unknown14[24];
    std::uint32_t netMessageId;
    char unknown13[24];
    SmartPtr<CInterfManagerImpl> interfManager;
    char unknown15[32];
    CInterface* waitInterface;
    char unknown16[52];
    CMqFps* fps;
    bool showFullSystemInfo;
    char padding2[3];
};

static_assert(sizeof(CMidgardData) == 252,
              "Size of CMidgardData structure must be exactly 252 bytes");

static_assert(offsetof(CMidgardData, client) == 40, "CMidgardData::client offset must be 40 bytes");

static_assert(offsetof(CMidgardData, host) == 50, "CMidgardData::host offset must be 50 bytes");

static_assert(offsetof(CMidgardData, settings) == 60,
              "CMidgardData::settings offset must be 60 bytes");

static_assert(offsetof(CMidgardData, fps) == 244, "CMidgardData::fps offset must be 244 bytes");

struct CMidgard
    : public IMqNetTraffic
    , public IMqAnimation
{
    CMidgardData* data;
};

static_assert(sizeof(CMidgard) == 12, "Size of CMidgard structure must be exactly 12 bytes");

static_assert(offsetof(CMidgard, CMidgard::IMqNetTraffic::vftable) == 0,
              "Vftable offset for IMqNetTraffic in CMidgard structure must be 0 bytes");

static_assert(offsetof(CMidgard, CMidgard::IMqAnimation::vftable) == 4,
              "Vftable offset for IMqAnimation in CMidgard structure must be 4 bytes");

static_assert(offsetof(CMidgard, data) == 8, "CMidgard::data offset must be 8 bytes");

namespace CMidgardApi {

struct Api
{
    using Instance = CMidgard*(__cdecl*)();
    Instance instance;
};

Api& get();

} // namespace CMidgardApi

} // namespace game

#endif // MIDGARD_H
