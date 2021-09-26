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

#ifndef USRACIALLEADER_H
#define USRACIALLEADER_H

#include "usglobal.h"
#include "usleaderimpl.h"
#include "ussoldierimpl.h"
#include "usstackleaderimpl.h"
#include "usunitimpl.h"
#include <cstddef>

namespace game {

/**
 * Represents leader unit template.
 * Leader unit in scenario is represented by CMidUnit
 * which accesses leader template data via CMidUnit::unitImpl pointer.
 * Leader units can learn abilities, use scrolls, wands, orbs and other items.
 * They are created and deleted along with the stacks.
 */
struct TUsRacialLeader
    : public IUsGlobal
    , public TUsUnitImpl
    , public TUsSoldierImpl
    , public TUsStackLeaderImpl
    , public TUsLeaderImpl
{ };

static_assert(sizeof(TUsRacialLeader) == 48,
              "Size of TUsRacialLeader structure must be exactly 48 bytes");

static_assert(offsetof(TUsRacialLeader, TUsRacialLeader::IUsGlobal::vftable) == 0,
              "Vftable offset for IUsGlobal in TUsRacialLeader structure must be 0 bytes");

static_assert(offsetof(TUsRacialLeader, TUsRacialLeader::TUsUnitImpl::vftable) == 4,
              "Vftable offset for TUsUnitImpl in TUsRacialLeader structure must be 4 bytes");

static_assert(offsetof(TUsRacialLeader, TUsRacialLeader::TUsSoldierImpl::vftable) == 24,
              "Vftable offset for TUsSoldierImpl in TUsRacialLeader structure must be 24 bytes");

static_assert(
    offsetof(TUsRacialLeader, TUsRacialLeader::TUsStackLeaderImpl::vftable) == 32,
    "Vftable offset for TUsStackLeaderImpl in TUsRacialLeader structure must be 32 bytes");

static_assert(offsetof(TUsRacialLeader, TUsRacialLeader::TUsLeaderImpl::vftable) == 40,
              "Vftable offset for TUsLeaderImpl in TUsRacialLeader structure must be 40 bytes");

} // namespace game

#endif // USRACIALLEADER_H
