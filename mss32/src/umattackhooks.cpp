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
#include "attackutils.h"
#include "game.h"
#include "umattack.h"

namespace hooks {

game::IAttack* __fastcall umAttackGetAttackByIdHooked(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& attackModifiedApi = CAttackModifiedApi::get();

    auto umattack = castSoldierToUmAttack(thisptr);

    auto prevUnitImpl = umattack->umModifier.data->prev;
    auto prevSoldier = fn.castUnitImplToSoldier(prevUnitImpl);

    auto prev = prevSoldier->vftable->getAttackById(prevSoldier);

    // Return unwrapped attack - its alt attack should be wrapped instead
    if (*prev->vftable->getAltAttackId(prev) != emptyId)
        return prev;

    auto attackModified = &umattack->data->attackModified;
    attackModifiedApi.wrap(attackModified, prev);
    return attackModified;
}

} // namespace hooks
