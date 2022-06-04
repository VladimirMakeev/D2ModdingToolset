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

#ifndef ATTACKMODIFIED_H
#define ATTACKMODIFIED_H

#include "attack.h"

namespace game {

struct CAttackModifiedData;

struct CAttackModified : public IAttack
{
    CAttackModifiedData* data;
};

assert_size(CAttackModified, 12);

struct CAttackModifiedData
{
    IAttack* prev;
    int initiative;
    int power;
    int qtyDamage;
    int attackDrain;
};

assert_size(CAttackModifiedData, 20);

namespace CAttackModifiedApi {

struct Api
{
    using Constructor = CAttackModified*(__thiscall*)(CAttackModified* thisptr);
    Constructor constructor;

    using CopyConstructor = CAttackModified*(__thiscall*)(CAttackModified* thisptr,
                                                          const CAttackModified* src);
    CopyConstructor copyConstructor;

    using Wrap = void(__thiscall*)(CAttackModified* thisptr, const IAttack* attack);
    Wrap wrap;
};

Api& get();

const IAttackVftable* vftable();

} // namespace CAttackModifiedApi

} // namespace game

#endif // ATTACKMODIFIED_H
