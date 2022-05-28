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

#include "batattackutils.h"
#include "visitors.h"

namespace hooks {

bool canHeal(game::IAttack* attack,
             game::IMidgardObjectMap* objectMap,
             game::BattleMsgData* battleMsgData,
             game::CMidgardID* targetUnitId)
{
    using namespace game;

    const auto qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal <= 0)
        return false;

    return BattleMsgDataApi::get().unitCanBeHealed(objectMap, battleMsgData, targetUnitId);
}

int heal(game::IMidgardObjectMap* objectMap,
         game::BattleMsgData* battleMsgData,
         game::CMidUnit* targetUnit,
         int qtyHeal)
{
    using namespace game;

    int hpBefore = targetUnit->currentHp;

    const auto& visitors = VisitorApi::get();
    visitors.changeUnitHp(&targetUnit->id, qtyHeal, objectMap, 1);

    int hpAfter = targetUnit->currentHp;
    BattleMsgDataApi::get().setUnitHp(battleMsgData, &targetUnit->id, hpAfter);

    return hpAfter - hpBefore;
}

} // namespace hooks
