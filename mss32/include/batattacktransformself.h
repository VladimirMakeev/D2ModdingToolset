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

#ifndef BATATTACKTRANSFORMSELF_H
#define BATATTACKTRANSFORMSELF_H

#include "batattack.h"
#include "midgardid.h"

namespace game {

struct IAttack;

/** Represents self-tranform attack. */
struct CBatAttackTransformSelf : public IBatAttack
{
    IBatAttack* altAttack;
    CMidgardID unitId;
    CMidgardID id2;
    int attackNumber;
    IAttack* attack;
};

static_assert(sizeof(CBatAttackTransformSelf) == 24,
              "Size of CBatAttackTransformSelf structure must be exactly 24 bytes");

namespace CBatAttackTransformSelfApi {

struct Api
{
    IBatAttackVftable::OnAttack onHit;
};

Api& get();

} // namespace CBatAttackTransformSelfApi

} // namespace game

#endif // BATATTACKTRANSFORMSELF_H
