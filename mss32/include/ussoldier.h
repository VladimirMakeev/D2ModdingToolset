/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef USSOLDIER_H
#define USSOLDIER_H

#include "d2assert.h"
#include "usunitextension.h"

namespace game {

struct CMidgardID;
struct LSubRaceCategory;
struct LUnitBranch;
struct LDeathAnimCategory;
struct IAttack;
struct LImmuneCat;
struct LAttackClass;
struct LAttackSource;
struct Bank;

struct IUsSoldierVftable;

struct IUsSoldier : public IUsUnitExtensionT<IUsSoldierVftable>
{ };

struct IUsSoldierVftable : public IUsUnitExtensionVftable
{
    using GetCStr = const char*(__thiscall*)(const IUsSoldier* thisptr);
    GetCStr getName;
    GetCStr getDescription;

    using GetId = const CMidgardID*(__thiscall*)(const IUsSoldier* thisptr);
    GetId getRaceId;

    using GetSubrace = const LSubRaceCategory*(__thiscall*)(const IUsSoldier* thisptr);
    GetSubrace getSubrace;

    using GetBranch = const LUnitBranch*(__thiscall*)(const IUsSoldier* thisptr);
    GetBranch getBranch;

    using GetBool = bool(__thiscall*)(const IUsSoldier* thisptr);
    GetBool getSizeSmall;
    GetBool getSexM;

    using GetInt = int(__thiscall*)(const IUsSoldier* thisptr);
    GetInt getLevel;
    GetInt getHitPoints;

    using GetArmor = int*(__thiscall*)(const IUsSoldier* thisptr, int* armor);
    GetArmor getArmor;

    GetId getBaseUnitImplId;

    using GetDeathAnim = const LDeathAnimCategory*(__thiscall*)(const IUsSoldier* thisptr);
    GetDeathAnim getDeathAnim;

    using GetRegen = int*(__thiscall*)(const IUsSoldier* thisptr);
    GetRegen getRegen;

    GetInt getXpNext;
    GetInt getXpKilled;

    /**
     * Returns immune category for specified attack class.
     * @returns found immune category or not immune if not found.
     */
    using GetImmuneByAttackClass = const LImmuneCat*(__thiscall*)(const IUsSoldier* thisptr,
                                                                  const LAttackClass* attackClass);
    GetImmuneByAttackClass getImmuneByAttackClass;

    /**
     * Returns immune category for specified attack source.
     * @returns found immune category or not immune if not found.
     */
    using GetImmuneByAttackSource = const LImmuneCat*(
        __thiscall*)(const IUsSoldier* thisptr, const LAttackSource* attackSource);
    GetImmuneByAttackSource getImmuneByAttackSource;

    using GetAttackById = IAttack*(__thiscall*)(const IUsSoldier* thisptr);
    GetAttackById getAttackById;
    GetAttackById getSecondAttackById;

    GetBool getAttackTwice;

    using GetBank = const Bank*(__thiscall*)(const IUsSoldier* thisptr);
    GetBank getEnrollCost;
    GetBank getReviveCost;
    GetBank getHealCost;
    GetBank getTrainingCost;

    GetId getDynUpg1;

    using GetDynUpgLvl = int(__thiscall*)(const IUsSoldier* thisptr);
    GetDynUpgLvl getDynUpgLvl;

    GetId getDynUpg2;

    GetBool getWaterOnly;
};

assert_vftable_size(IUsSoldierVftable, 29);

} // namespace game

#endif // USSOLDIER_H
