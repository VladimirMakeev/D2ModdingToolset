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
#include "dbtable.h"
#include "mempool.h"
#include "umattack.h"
#include "utils.h"

namespace hooks {

game::CUmAttack* __fastcall umAttackCtorHooked(game::CUmAttack* thisptr,
                                               int /*%edx*/,
                                               const game::CMidgardID* modifierId,
                                               game::CDBTable* dbTable,
                                               const game::GlobalData** globalData)
{
    using namespace game;

    const auto& umAttackApi = CUmAttackApi::get();
    const auto& umAttackVftable = CUmAttackApi::vftable();
    const auto& attackModifiedApi = CAttackModifiedApi::get();
    const auto& dbApi = CDBTableApi::get();

    thisptr->usUnit.id = emptyId;

    CUmModifierApi::get().constructor(&thisptr->umModifier, modifierId, globalData);

    thisptr->data = (CUmAttackDataPatched*)Memory::get().allocate(sizeof(CUmAttackDataPatched));
    umAttackApi.dataConstructor(thisptr->data);

    thisptr->usUnit.vftable = umAttackVftable.usUnit;
    thisptr->usSoldier.vftable = umAttackVftable.usSoldier;
    thisptr->umModifier.vftable = umAttackVftable.umModifier;

    if (dbApi.eof(dbTable))
        dbApi.missingValueException(dbApi.getName(dbTable), idToString(modifierId).c_str());

    for (; !dbApi.eof(dbTable); dbApi.next(dbTable))
        umAttackApi.readData(dbTable, thisptr->data, globalData);

    auto& attackData = thisptr->data->attackModified.data;
    attackData->initiative = thisptr->data->initiative.value;
    attackData->power = thisptr->data->power.value;
    attackData->qtyDamage = thisptr->data->qtyDamage.value;
    attackData->attackDrain = thisptr->data->attackDrain.value;

    attackModifiedApi.copyConstructor(&thisptr->data->altAttackModified,
                                      &thisptr->data->attackModified);

    return thisptr;
}

game::CUmAttack* __fastcall umAttackCopyCtorHooked(game::CUmAttack* thisptr,
                                                   int /*%edx*/,
                                                   const game::CUmAttack* src)
{
    using namespace game;

    const auto& umAttackVftable = CUmAttackApi::vftable();
    const auto& attackModifiedApi = CAttackModifiedApi::get();

    thisptr->usUnit.id = src->usUnit.id;

    CUmModifierApi::get().copyConstructor(&thisptr->umModifier, &src->umModifier);

    thisptr->data = (CUmAttackDataPatched*)Memory::get().allocate(sizeof(CUmAttackDataPatched));
    CUmAttackApi::get().dataCopyConstructor(thisptr->data, src->data);
    attackModifiedApi.copyConstructor(&thisptr->data->altAttackModified,
                                      &src->data->altAttackModified);

    thisptr->usUnit.vftable = umAttackVftable.usUnit;
    thisptr->usSoldier.vftable = umAttackVftable.usSoldier;
    thisptr->umModifier.vftable = umAttackVftable.umModifier;

    return thisptr;
}

} // namespace hooks
