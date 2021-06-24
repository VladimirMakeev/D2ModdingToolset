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
#include "textandid.h"
#include <cstdint>

namespace game {

struct IAttackVftable;
struct CAttackData;

/** Base class for units attacks. */
struct IAttack
{
    const IAttackVftable* vftable;
};

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

    using GetInitiative = int(__thiscall*)(const IAttack* thisptr);
    GetInitiative getInitiative;

    /** Power is chance to hit / accuracy. */
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

    using GetData = void(__thiscall*)(const IAttack* thisptr, CAttackData* value);
    GetData getData;
};

static_assert(sizeof(IAttackVftable) == 17 * sizeof(void*),
              "IAttack vftable must have exactly 17 methods");

/** Contains values and references to CAttackImplData fields. */
struct CAttackData
{
    CMidgardID attackId;
    int initiative;
    int power;
    int qtyDamage;
    int qtyHeal;
    TextAndId* name;
    TextAndId* description;
    LAttackClass* attackClass;
    LAttackSource* attackSource;
    LAttackReach* attackReach;
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

namespace IAttackApi {

struct Api
{
    using GetId = const CMidgardID*(__thiscall*)(const IAttack* thisptr);
    GetId getId;
};

Api& get();

} // namespace IAttackApi

} // namespace game

#endif // ATTACK_H
