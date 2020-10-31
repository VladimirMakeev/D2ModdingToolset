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

#ifndef MIDSERVER_H
#define MIDSERVER_H

#include "mqnetsystem.h"
#include "mqthread.h"
#include <cstddef>

namespace game {

struct CMidServerBuilderFull;

struct CMidServerData
{
    char unknown[24];
    CMidServerBuilderFull** serverBuilder;
    char unknown2[4];
};

static_assert(sizeof(CMidServerData) == 32,
              "Size of CMidServerData structure must be exactly 32 bytes");

struct CMidServer
    : public CMqThread
    , public IMqNetSystem
{
    CMidServerData* data;
};

static_assert(sizeof(CMidServer) == 16, "Size of CMidServer structure must be exactly 16 bytes");

static_assert(offsetof(CMidServer, CMidServer::CMqThread::vftable) == 0,
              "Vftable offset for CMqThread in CMidServer structure must be 0 bytes");

static_assert(offsetof(CMidServer, CMidServer::IMqNetSystem::vftable) == 8,
              "Vftable offset for IMqNetSystem in CMidServer structure must be 8 bytes");

static_assert(offsetof(CMidServer, data) == 12, "CMidServer::data offset must be 12 bytes");

} // namespace game

#endif // MIDSERVER_H
