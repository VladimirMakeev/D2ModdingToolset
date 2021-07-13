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
    CMidgardID attackId;
    CAttackModifiedData* data;
};

static_assert(sizeof(CAttackModified) == 12,
              "Size of CAttackModified structure must be exactly 12 bytes");

struct CAttackModifiedData
{
    IAttack* underlying;
    int initiative;
    int power;
    int qtyDamage;
    int attackDrain;
};

static_assert(sizeof(CAttackModifiedData) == 20,
              "Size of CAttackModifiedData structure must be exactly 20 bytes");

namespace CAttackModifiedApi {

struct Api
{
    using Wrap = void(__thiscall*)(CAttackModified* thisptr, const IAttack* attack);
    Wrap wrap;
};

Api& get();

const IAttackVftable* vftable();

} // namespace CAttackModifiedApi

} // namespace game

#endif // ATTACKMODIFIED_H
