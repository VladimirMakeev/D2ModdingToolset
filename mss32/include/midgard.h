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

#ifndef MIDGARD_H
#define MIDGARD_H

#include "mqanimation.h"
#include "mqnettraffic.h"
#include <cstddef>

namespace game {

struct IMqNetService;
struct IMqNetSession;
struct CMidClient;
struct CMidServer;
struct GameSettings;

struct CMidgardData
{
    char unknown[4];
    IMqNetService** netService;
    IMqNetSession** netSession;
    char unknown2[28];
    CMidClient** client;
    CMidServer** server;
    char unknown3[12];
    GameSettings** settings;
    char unknown4[188];
};

static_assert(sizeof(CMidgardData) == 252,
              "Size of CMidgardData structure must be exactly 252 bytes");

static_assert(offsetof(CMidgardData, client) == 40, "CMidgardData::client offset must be 40 bytes");

static_assert(offsetof(CMidgardData, settings) == 60,
              "CMidgardData::settings offset must be 60 bytes");

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

} // namespace game

#endif // MIDGARD_H
