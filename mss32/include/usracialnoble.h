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

#ifndef USRACIALNOBLE_H
#define USRACIALNOBLE_H

#include "usglobal.h"
#include "usleaderimpl.h"
#include "usnoble.h"
#include "ussoldierimpl.h"
#include "usstackleaderimpl.h"
#include "usunitimpl.h"
#include <cstddef>

namespace game {

/**
 * Represents noble (thief) unit template.
 * Noble unit in scenario is represented by CMidUnit
 * which accesses noble template data via CMidUnit::unitImpl pointer.
 * Noble units used for spying and various thief operations.
 */
struct TUsRacialNoble
    : public IUsNoble
    , public IUsGlobal
    , public TUsUnitImpl
    , public TUsSoldierImpl
    , public TUsStackLeaderImpl
    , public TUsLeaderImpl
{ };

assert_size(TUsRacialNoble, 52);
assert_offset(TUsRacialNoble, TUsRacialNoble::IUsNoble::vftable, 0);
assert_offset(TUsRacialNoble, TUsRacialNoble::IUsGlobal::vftable, 4);
assert_offset(TUsRacialNoble, TUsRacialNoble::TUsUnitImpl::vftable, 8);
assert_offset(TUsRacialNoble, TUsRacialNoble::TUsSoldierImpl::vftable, 28);
assert_offset(TUsRacialNoble, TUsRacialNoble::TUsStackLeaderImpl::vftable, 36);
assert_offset(TUsRacialNoble, TUsRacialNoble::TUsLeaderImpl::vftable, 44);

} // namespace game

#endif // USRACIALNOBLE_H
