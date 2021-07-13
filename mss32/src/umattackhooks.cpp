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

#include "umattackhooks.h"
#include "game.h"
#include "umattack.h"

namespace hooks {

game::IAttack* __fastcall umAttackGetAttackByIdHooked(const game::CUmAttack_IUsSoldier* thisptr,
                                                      int /*%edx*/)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& attackModifiedApi = CAttackModifiedApi::get();

    auto unitImpl = thisptr->umModifier.data->underlying;
    auto soldier = fn.castUnitImplToSoldier(unitImpl);

    auto attack = soldier->vftable->getAttackById(soldier);
    auto altAttackId = attack->vftable->getAltAttackId(attack);
    if (*altAttackId != emptyId)
        return attack;

    auto attackModified = &thisptr->data->attackModified;
    attackModifiedApi.wrap(attackModified, attack);
    return attackModified;
}

} // namespace hooks
