/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
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

#ifndef ATTACKCLASSCAT_H
#define ATTACKCLASSCAT_H

#include "categories.h"

namespace game {

struct LAttackClassTable : public CEnumConstantTable<AttackClassId>
{ };

struct LAttackClass : public Category<AttackClassId>
{ };

namespace AttackClassCategories {

struct Categories
{
    LAttackClass* paralyze;
    LAttackClass* petrify;
    LAttackClass* damage;
    LAttackClass* drain;
    LAttackClass* heal;
    LAttackClass* fear;
    LAttackClass* boostDamage;
    LAttackClass* lowerDamage;
    LAttackClass* lowerInitiative;
    LAttackClass* poison;
    LAttackClass* frostbite;
    LAttackClass* revive;
    LAttackClass* drainOverflow;
    LAttackClass* cure;
    LAttackClass* summon;
    LAttackClass* drainLevel;
    LAttackClass* giveAttack;
    LAttackClass* doppelganger;
    LAttackClass* transformSelf;
    LAttackClass* transformOther;
    LAttackClass* blister;
    LAttackClass* bestowWards;
    LAttackClass* shatter;
};

Categories& get();

} // namespace AttackClassCategories

} // namespace game

#endif // ATTACKCLASSCAT_H
