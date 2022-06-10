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

#ifndef DYNUPGRADE_H
#define DYNUPGRADE_H

#include "currency.h"
#include "midgardid.h"
#include <cstddef>

namespace game {

/** Holds dynamic upgrade data read from GDynUpgr.dbf. */
struct CDynUpgrade
{
    CMidgardID upgradeId;
    Bank enrollCost;
    int hp;
    int armor;
    int regen;
    Bank reviveCost;
    Bank healCost;
    Bank trainingCost;
    int xpKilled;
    int xpNext;
    int move;
    int negotiate;
    int initiative;
    int power;
    int damage;
    int heal;
};

assert_size(CDynUpgrade, 96);
assert_offset(CDynUpgrade, xpKilled, 64);

} // namespace game

#endif // DYNUPGRADE_H
