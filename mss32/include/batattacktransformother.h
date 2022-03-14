/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef BATATTACKTRANSFORMOTHER_H
#define BATATTACKTRANSFORMOTHER_H

#include "batattack.h"
#include "idlist.h"

namespace game {

struct IAttack;

struct CBatAttackTransformOther : public CBatAttackBase
{
    CMidgardID unitId;
    CMidgardID unitOrItemId;
    int attackNumber;
    IAttack* attack;
    IdList smallUnits;
    IdList bigUnits;
};

static_assert(sizeof(CBatAttackTransformOther) == 52,
              "Size of CBatAttackTransformOther structure must be exactly 24 bytes");

namespace CBatAttackTransformOtherApi {

IBatAttackVftable* vftable();

} // namespace CBatAttackTransformOtherApi

} // namespace game

#endif // BATATTACKTRANSFORMOTHER_H
