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

#ifndef BATATTACKSHATTER_H
#define BATATTACKSHATTER_H

#include "batattack.h"
#include "midgardid.h"

namespace game {

struct IAttack;

/**
 * Breaks armor.
 * Theurgist unit of Elven Alliance uses this attack.
 */
struct CBatAttackShatter : public CBatAttackBase
{
    CMidgardID unitId1;
    CMidgardID unitId2;
    int attackNumber; /**< 1 if this is a unit's primary attack, 2 for secondary. */
    IAttack* attack;
    int unknown2;
    int unknown3;
    int unknown4;
};

static_assert(sizeof(CBatAttackShatter) == 32,
              "Size of CBatAttackShatter structure must be exactly 32 bytes");

namespace CBatAttackShatterApi {

IBatAttackVftable* vftable();

} // namespace CBatAttackShatterApi

} // namespace game

#endif // BATATTACKSHATTER_H
