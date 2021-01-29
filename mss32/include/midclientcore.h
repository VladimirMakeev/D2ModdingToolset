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

#ifndef MIDCLIENTCORE_H
#define MIDCLIENTCORE_H

#include "mqnetsystem.h"

namespace game {

struct CMidgard;
struct IMidgardObjectMap;
struct MidClientCoreCommandQueueData;
struct CoreCommandUpdate;
struct CCommandCanIgnore;
struct CMidHotseatManager;

struct CMidClientCoreData
{
    CMidgard* midgard;
    int unknown;
    IMidgardObjectMap* objectMap;
    int unknown3;
    MidClientCoreCommandQueueData* commandQueueData;
    CoreCommandUpdate* coreCommandUpdate;
    CCommandCanIgnore* commandCanIgnore;
    CMidHotseatManager* hotseatManager;
};

static_assert(sizeof(CMidClientCoreData) == 32,
              "Size of CMidClientCoreData structure must be exactly 32 bytes");

struct CMidClientCore : public IMqNetSystem
{
    CMidClientCoreData* data;
};

} // namespace game

#endif // MIDCLIENTCORE_H
