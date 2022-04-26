/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "umunithooks.h"
#include "dbtable.h"
#include "game.h"
#include "mempool.h"
#include "ummodifier.h"
#include "umunit.h"
#include "utils.h"

namespace hooks {

game::CUmUnit* __fastcall umUnitCtorHooked(game::CUmUnit* thisptr,
                                           int /*%edx*/,
                                           const game::CMidgardID* modifierId,
                                           game::CDBTable* dbTable,
                                           const game::GlobalData** globalData)
{
    using namespace game;

    const auto& umUnitApi = CUmUnitApi::get();
    const auto& dbApi = CDBTableApi::get();

    thisptr->usUnit.unitId = emptyId;

    CUmModifierApi::get().constructor(&thisptr->umModifier, modifierId, globalData);

    thisptr->data = (CUmUnitDataPatched*)Memory::get().allocate(sizeof(CUmUnitDataPatched));
    if (thisptr->data)
        umUnitApi.dataConstructor(thisptr->data);

    thisptr->usUnit.vftable = CUmUnitApi::vftable().usUnit;
    thisptr->usSoldier.vftable = CUmUnitApi::vftable().usSoldier;
    thisptr->umModifier.vftable = CUmUnitApi::vftable().umModifier;

    if (dbApi.eof(dbTable))
        dbApi.missingValueException(dbApi.getName(dbTable), idToString(modifierId).c_str());

    for (; !dbApi.eof(dbTable); dbApi.next(dbTable))
        umUnitApi.readData(dbTable, thisptr->data, globalData);

    return thisptr;
}

game::CUmUnit* __fastcall umUnitCopyCtorHooked(game::CUmUnit* thisptr,
                                               int /*%edx*/,
                                               const game::CUmUnit* src)
{
    using namespace game;

    thisptr->usUnit.unitId = src->usUnit.unitId;

    CUmModifierApi::get().copyConstructor(&thisptr->umModifier, &src->umModifier);

    thisptr->data = (CUmUnitDataPatched*)Memory::get().allocate(sizeof(CUmUnitDataPatched));
    if (thisptr->data)
        CUmUnitApi::get().dataCopyConstructor(thisptr->data, src->data);

    thisptr->usUnit.vftable = CUmUnitApi::vftable().usUnit;
    thisptr->usSoldier.vftable = CUmUnitApi::vftable().usSoldier;
    thisptr->umModifier.vftable = CUmUnitApi::vftable().umModifier;

    return thisptr;
}

int* __fastcall umUnitGetRegenHooked(const game::IUsSoldier* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto umunit = castSoldierToUmUnit(thisptr);
    auto data = (CUmUnitDataPatched*)umunit->data;

    auto unitImpl = umunit->umModifier.data->prev;
    auto soldier = fn.castUnitImplToSoldier(unitImpl);

    int stacked = *soldier->vftable->getRegen(soldier);
    stacked += data->regeneration.value;
    if (stacked > 100)
        stacked = 100;

    // The value can be potentially accessed by parallel thread, store only final result here
    data->regenerationStacked = stacked;
    return &data->regenerationStacked;
}

} // namespace hooks
