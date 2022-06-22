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

#ifndef LEADERUNITDESCRIPTOR_H
#define LEADERUNITDESCRIPTOR_H

#include "attackclasscat.h"
#include "attackreachcat.h"
#include "attacksourcecat.h"
#include "d2list.h"
#include "d2string.h"
#include "encunitdescriptor.h"
#include "leaderabilitycat.h"
#include "midgardid.h"
#include "racecategory.h"

namespace game {

struct CLeaderUnitDescriptorData
{
    String name;
    LRaceCategory raceCategory;
    CMidgardID globalUnitImplId;
    int level;
    int leadership;
    int currentXp;
    int xp;
    int nbBattle;
    int hpMax;
    int armor;
    int attackId;
    LAttackClass attackClass;
    String attackName;
    int attackPower;
    int unknown;
    int attackDamage;
    LAttackSource attackSource;
    int attackInitiative;
    LAttackReach attackReach;
    String attack2Name;
    int attack2Power;
    CMidgardID attack2Id;
    bool attacksTwice;
    char padding[3];
    int movement;
    int scout;
    int unknown2;
    bool unknown3;
    char padding2[3];
    List<LAttackSource> attackSourceImmunities;
    List<LAttackClass> attackClassImmunities;
    List<LAttackSource> attackSourceWards;
    List<LAttackClass> attackClassWards;
    List<LLeaderAbility> leaderAbilities;
    List<CMidgardID> modifiers;
    List<Category<int>> unknown4;
    int unknown5;
};

assert_size(CLeaderUnitDescriptorData, 292);

/** Used by CViewExportedLeaderInterf. */
struct CLeaderUnitDescriptor : public IEncUnitDescriptor
{
    CLeaderUnitDescriptorData data;
};

assert_size(CLeaderUnitDescriptor, 296);

} // namespace game

#endif // LEADERUNITDESCRIPTOR_H
