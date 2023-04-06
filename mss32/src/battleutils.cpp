/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "battleutils.h"
#include "battlemsgdata.h"
#include "game.h"
#include "midunit.h"
#include "settings.h"
#include "ussoldier.h"

namespace hooks {

int adjustUnitXpReceived(const game::BattleMsgData* battleMsgData,
                         const game::CMidUnit* unit,
                         int value)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();

    auto result = value;
    if (result <= 0) {
        result = 1;
    }

    if (!userSettings().battle.allowRetreatedUnitsToUpgrade
        && battleApi.getUnitStatus(battleMsgData, &unit->id, BattleStatus::Retreated)) {
        auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
        auto xpNext = soldier->vftable->getXpNext(soldier);
        if (result + unit->currentXp >= xpNext) {
            result = xpNext - unit->currentXp - 1;
        }
    }

    return result;
}

} // namespace hooks
