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

#ifndef ATTACK_H
#define ATTACK_H

#include "attackclasscat.h"
#include "attackreachcat.h"
#include "attacksourcecat.h"
#include "idvector.h"
#include "midgardid.h"
#include "midobject.h"

namespace game {

/** Base class for units attacks. */
struct IAttack : public IMidObject
{ };

struct IAttackVftable
{
    void* destructor;

    using GetCStr = const char*(__thiscall*)(const IAttack* thisptr);
    GetCStr getName;
    GetCStr getDescription;

    using GetAttackClass = LAttackClass*(__thiscall*)(const IAttack* thisptr);
    GetAttackClass getAttackClass;

    using GetAttackSource = LAttackSource*(__thiscall*)(const IAttack* thisptr);
    GetAttackSource getAttackSource;

    using GetInitiative = int*(__thiscall*)(const IAttack* thisptr);
    GetInitiative getInitiative;

    using GetPower = int*(__thiscall*)(const IAttack* thisptr, int* power);
    GetPower getPower;

    using GetAttackReach = LAttackReach*(__thiscall*)(const IAttack* thisptr);
    GetAttackReach getAttackReach;

    using GetInt = int(__thiscall*)(const IAttack* thisptr);
    GetInt getQtyDamage;
    GetInt getQtyHeal;

    /** Meaning assumed. */
    using GetDrain = int(__thiscall*)(const IAttack* thisptr, int damage);
    GetDrain getDrain;

    GetInt getLevel;
    GetInt getAltAttack;

    using GetBool = bool(__thiscall*)(const IAttack* thisptr);
    GetBool getInfinite;

    using GetWards = IdVector*(__thiscall*)(const IAttack* thisptr);
    GetWards getWards;

    GetBool getCritHit;

    using CopyTo = bool(__thiscall*)(const IAttack* thisptr, void* dest);
    CopyTo copyTo;
};

static_assert(sizeof(IAttackVftable) == 17 * sizeof(void*),
              "IAttack vftable must have exactly 17 methods");

} // namespace game

#endif // ATTACK_H
