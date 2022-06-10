/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef STRATEGICSPELL_H
#define STRATEGICSPELL_H

#include "aispellcat.h"
#include "currency.h"
#include "groundcat.h"
#include "idvector.h"
#include "midobject.h"
#include "spellcat.h"
#include "textandid.h"

namespace game {

struct LAttackSource;

struct TStrategicSpellData
{
    LSpellCategory spellCategory;
    LAiSpellCategory aiSpellCategory;
    TextAndId name;
    TextAndId description;
    TextAndId effectDescription;
    int level;
    Bank castingCost;
    Bank buyCost;
    CMidgardID unitId;
    CMidgardID modifierId;
    int restoreMovePoints;
    int area;
    int damageQty;
    LAttackSource* damageSource;
    int healQty;
    LGroundCategory groundCategory;
    bool changeGround;
    char padding[3];
    IdVector wards;
};

assert_size(TStrategicSpellData, 136);

/** Holds information read from GSpells.dbf. */
struct TStrategicSpell : public IMidObject
{
    TStrategicSpellData* data;
};

assert_size(TStrategicSpell, 12);

} // namespace game

#endif // STRATEGICSPELL_H
