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
    char padding[3];
    IdVector wards;
    bool critHit;
    union
    {
        struct
        {
            std::uint8_t damageRatio;
            bool damageRatioPerTarget;
        };
        char padding2[3];
    };
};

static_assert(sizeof(CAttackImplData) == 100,
              "Size of CAttackImplData structure must be exactly 100 bytes");

static_assert(offsetof(CAttackImplData, attackSource) == 44,
              "CAttackImplData::attackSource offset must be 44 bytes");

static_assert(offsetof(CAttackImplData, attackReach) == 56,
              "CAttackImplData::attackReach offset must be 56 bytes");

static_assert(offsetof(CAttackImplData, critHit) == 96,
              "CAttackImplData::critHit offset must be 96 bytes");

/** Holds attack data read from GAttacks.dbf. */
struct CAttackImpl : public IAttack
{
    CMidgardID attackId;
    CAttackImplData* data;
};

static_assert(sizeof(CAttackImpl) == 12, "Size of CAttackImpl structure must be exactly 12 bytes");

namespace CAttackImplApi {

struct Api
{
    using Constructor = CAttackImpl*(__thiscall*)(CAttackImpl* thisptr,
                                                  const CDBTable* dbTable,
                                                  const GlobalData** globalData);
    Constructor constructor;

    using InitData = CAttackImplData*(__thiscall*)(CAttackImplData* thisptr);
    InitData initData;
};

Api& get();

/** Returns address of CAttackImpl::vftable used in game. */
const IAttackVftable* vftable();

} // namespace CAttackImplApi

} // namespace game

#endif // ATTACKIMPL_H
