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

#include "encparambasehooks.h"
#include "battlemsgdata.h"
#include "encparambase.h"

namespace hooks {

void __stdcall encParamBaseAddUnitBattleInfoHooked(game::CEncParamBase* param,
                                                   const game::CMidgardID* unitId,
                                                   const game::BattleMsgData* battleMsgData)
{
    using namespace game;

    const auto& api = CEncParamBaseApi::get();
    const auto& battleApi = BattleMsgDataApi::get();

    auto data = param->data;
    if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::BoostDamageLvl1)) {
        data->statuses |= (int)CEncParamBaseDataStatus::BoostDamageLvl1;
    } else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::BoostDamageLvl2)) {
        data->statuses |= (int)CEncParamBaseDataStatus::BoostDamageLvl2;
    } else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::BoostDamageLvl3)) {
        data->statuses |= (int)CEncParamBaseDataStatus::BoostDamageLvl3;
    } else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::BoostDamageLvl4)) {
        data->statuses |= (int)CEncParamBaseDataStatus::BoostDamageLvl4;
    }

    if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl1)) {
        data->statuses |= (int)CEncParamBaseDataStatus::LowerDamageLvl1;
    } else if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl2)) {
        data->statuses |= (int)CEncParamBaseDataStatus::LowerDamageLvl2;
    }

    if (battleApi.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerInitiative)) {
        data->statuses |= (int)CEncParamBaseDataStatus::LowerInitiative;
    }

    auto unitInfo = battleApi.getUnitInfoById(battleMsgData, unitId);
    if (unitInfo != nullptr) {
        api.setData(param, CEncParamBaseDataKey::AttackSourceImmunityStatuses,
                    unitInfo->attackSourceImmunityStatuses.patched);
        api.setData(param, CEncParamBaseDataKey::AttackClassImmunityStatuses,
                    unitInfo->attackClassImmunityStatuses);
    }
}

} // namespace hooks
