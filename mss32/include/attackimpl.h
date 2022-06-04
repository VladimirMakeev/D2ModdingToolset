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

#ifndef ATTACKIMPL_H
#define ATTACKIMPL_H

#include "attack.h"
#include "textandid.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct GlobalData;
struct CDBTable;

struct CAttackImplData
{
    int initiative;
    int power;
    int qtyDamage;
    int qtyHeal;
    TextAndId name;
    TextAndId description;
    LAttackClass attackClass;
    LAttackSource attackSource;
    LAttackReach attackReach;
    int level;
    CMidgardID altAttack;
    bool infinite;
    union
    {
        struct
        {
            std::uint8_t damageRatio;
            bool damageRatioPerTarget;
            bool damageSplit;
        };
        char padding[3];
    };
    IdVector wards;
    bool critHit;
    union
    {
        struct
        {
            std::uint8_t critDamage;
            std::uint8_t critPower;
        };
        char padding2[3];
    };
};

assert_size(CAttackImplData, 100);
assert_offset(CAttackImplData, attackSource, 44);
assert_offset(CAttackImplData, attackReach, 56);
assert_offset(CAttackImplData, critHit, 96);

/** Holds attack data read from GAttacks.dbf. */
struct CAttackImpl : public IAttack
{
    CAttackImplData* data;
};

assert_size(CAttackImpl, 12);

namespace CAttackImplApi {

struct Api
{
    using Constructor = CAttackImpl*(__thiscall*)(CAttackImpl* thisptr,
                                                  const CDBTable* dbTable,
                                                  const GlobalData** globalData);
    Constructor constructor;

    using Constructor2 = CAttackImpl*(__thiscall*)(CAttackImpl* thisptr, const CAttackData* data);
    Constructor2 constructor2;

    using InitData = CAttackImplData*(__thiscall*)(CAttackImplData* thisptr);
    InitData initData;
};

Api& get();

/** Returns address of CAttackImpl::vftable used in game. */
const IAttackVftable* vftable();

} // namespace CAttackImplApi

} // namespace game

#endif // ATTACKIMPL_H
