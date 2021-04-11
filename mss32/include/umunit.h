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

#ifndef UMUNIT_H
#define UMUNIT_H

#include "smartptr.h"
#include "textandid.h"
#include "ummodifier.h"
#include "ussoldier.h"
#include "usunit.h"

namespace game {

struct CUmUnitData;

struct CUmUnit
{
    IUsUnit usUnit;
    IUsSoldier* usSoldier;
    CUmModifier umModifier;
    CUmUnitData* data;
};

static_assert(sizeof(CUmUnit) == 24, "Size of CUmUnit structure must be exactly 24 bytes");

struct CUmUnitData
{
    ModifierValue hp;
    bool isPercentHp;
    char padding[3];
    ModifierValue regeneration;
    ModifierValue armor;
    SmartPtr<LAttackSource> immunity;
    SmartPtr<LImmuneCat> immuneCat;
    SmartPtr<LAttackClass> immunityC;
    SmartPtr<LImmuneCat> immuneCatC;
    TextAndId desc;
};

static_assert(sizeof(CUmUnitData) == 68, "Size of CUmUnitData structure must be exactly 68 bytes");

} // namespace game

#endif // UMUNIT_H
