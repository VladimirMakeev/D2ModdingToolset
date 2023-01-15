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

#ifndef ENCPARAMBASE_H
#define ENCPARAMBASE_H

#include "d2map.h"
#include "encparam.h"
#include "smartptr.h"

namespace game {

struct CMidgardID;
struct BattleMsgData;

typedef int CEncParamBaseDataStatuses;

// clang-format off
enum class CEncParamBaseDataStatus : int
{
    Unknown1        = 0b0000000000000001,
    Unknown2        = 0b0000000000000010,
    Unknown3        = 0b0000000000000100,

    // Correspond to BattleMsgData::BattleStatus
    BoostDamageLvl1 = 0b0000000000001000,
    BoostDamageLvl2 = 0b0000000000010000,
    BoostDamageLvl3 = 0b0000000000100000,
    BoostDamageLvl4 = 0b0000000001000000,
    LowerDamageLvl1 = 0b0000000010000000,
    LowerDamageLvl2 = 0b0000000100000000,
    LowerInitiative = 0b0000001000000000,

    Unknown4        = 0b0000010000000000,
    Unknown5        = 0b0000100000000000,
};
// clang-format on

enum class CEncParamBaseDataKey : int
{
    // Used by CBattleViewerInterf
    ShatteredArmor = 1,
    FortificationArmor = 2,
    // Used by CDDMageInventory, CDDMageInvDisplay, CDDMerchantInventory, CDDMerchantInvDisplay
    LowerCost = ShatteredArmor,
    // Used by encParamBaseAddUnitBattleInfoHooked
    // Correspond to BattleMsgData::UnitInfo immunity statuses
    AttackSourceImmunityStatuses = 3,
    AttackClassImmunityStatuses = 4,
};

struct CEncParamBaseData
{
    int type;
    CEncParamBaseDataStatuses statuses;
    SmartPointer functor;
    Map<CEncParamBaseDataKey, int> values;
};

assert_size(CEncParamBaseData, 44);

/** Base class for ingame encyclopedia parameters. */
struct CEncParamBase : public IEncParam
{
    CEncParamBaseData* data;
};

assert_size(CEncParamBase, 8);

namespace CEncParamBaseApi {

struct Api
{
    using AddUnitBattleInfo = void(__stdcall*)(CEncParamBase* param,
                                               const CMidgardID* unitId,
                                               const BattleMsgData* battleMsgData);
    AddUnitBattleInfo addUnitBattleInfo;

    using GetData = int(__thiscall*)(const CEncParamBase* thisptr,
                                     CEncParamBaseDataKey key,
                                     int def);
    GetData getData;

    using SetData = void(__thiscall*)(CEncParamBase* thisptr, CEncParamBaseDataKey key, int value);
    SetData setData;
};

Api& get();

} // namespace CEncParamBaseApi

} // namespace game

#endif // ENCPARAMBASE_H
