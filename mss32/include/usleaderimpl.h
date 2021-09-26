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

#ifndef USLEADERIMPL_H
#define USLEADERIMPL_H

#include "currency.h"
#include "leadercategory.h"
#include "midgardid.h"
#include "usleader.h"

namespace game {

struct TUsLeaderImplData
{
    CMidgardID unitId;
    CMidgardID prevId;
    Bank enrollCost;
    LLeaderCategory category;
};

static_assert(sizeof(TUsLeaderImplData) == 32,
              "Size of TUsLeaderImplData structure must be exactly 32 bytes");

/** Implementation of leader interface. */
struct TUsLeaderImpl : public IUsLeader
{
    TUsLeaderImplData* data;
};

static_assert(sizeof(TUsLeaderImpl) == 8,
              "Size of TUsLeaderImpl structure must be exactly 8 bytes");

} // namespace game

#endif // USLEADERIMPL_H
