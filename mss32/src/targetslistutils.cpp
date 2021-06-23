/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "targetslistutils.h"
#include "midunitgroup.h"

namespace hooks {

game::CMidgardID getTargetUnitId(int unitPosition,
                                 const game::CMidUnitGroup* targetGroup,
                                 const game::CMidUnitGroup* otherGroup)
{
    using namespace game;

    const auto& groupApi = CMidUnitGroupApi::get();

    if (unitPosition >= 0) {
        return *groupApi.getUnitIdByPosition(targetGroup, unitPosition);
    } else {
        return *groupApi.getUnitIdByPosition(otherGroup, -(unitPosition + 1));
    }
}

} // namespace hooks
