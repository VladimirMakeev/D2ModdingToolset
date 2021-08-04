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

#ifndef EVENTCONDITIONS_H
#define EVENTCONDITIONS_H

#include "midevcondition.h"
#include "midgardid.h"

namespace game {

/** Frequency (every n days). */
struct CMidCondFrequency : public CMidEvCondition
{
    int frequency; /**< In days [1 : 500]. */
};

static_assert(sizeof(CMidCondFrequency) == 20,
              "Size of CMidCondFrequency structure must be exactly 20 bytes");

/** Entering a predefined zone. */
struct CMidCondLocation : public CMidEvCondition
{
    CMidgardID locationId;
};

static_assert(sizeof(CMidCondLocation) == 20,
              "Size of CMidCondLocation structure must be exactly 20 bytes");

/** Entering a city. */
struct CMidCondEnterCity : public CMidEvCondition
{
    CMidgardID cityId;
};

static_assert(sizeof(CMidCondEnterCity) == 20,
              "Size of CMidCondEnterCity structure must be exactly 20 bytes");

/** Owning a city. */
struct CMidCondOwnCity : public CMidEvCondition
{
    CMidgardID cityId;
};

static_assert(sizeof(CMidCondOwnCity) == 20,
              "Size of CMidCondOwnCity structure must be exactly 20 bytes");

/** Destroying a stack. */
struct CMidCondKillStack : public CMidEvCondition
{
    CMidgardID stackId;
};

static_assert(sizeof(CMidCondKillStack) == 20,
              "Size of CMidCondKillStack structure must be exactly 20 bytes");

/** Owning an item. */
struct CMidCondOwnItem : public CMidEvCondition
{
    CMidgardID itemId;
};

static_assert(sizeof(CMidCondOwnItem) == 20,
              "Size of CMidCondOwnItem structure must be exactly 20 bytes");

/** Specific leader owning an item. */
struct CMidCondLeaderOwnItem : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID itemId;
};

static_assert(sizeof(CMidCondLeaderOwnItem) == 24,
              "Size of CMidCondLeaderOwnItem structure must be exactly 24 bytes");

/** Diplomacy relations. */
struct CMidCondDiplomacy : public CMidEvCondition
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    int diplomacy; /**< Percentage based value [-100 : 100]. */
};

static_assert(sizeof(CMidCondDiplomacy) == 28,
              "Size of CMidCondDiplomacy structure must be exactly 28 bytes");

/** Being in alliance. */
struct CMidCondAlliance : public CMidEvCondition
{
    CMidgardID playerId1;
    CMidgardID playerId2;
};

static_assert(sizeof(CMidCondAlliance) == 24,
              "Size of CMidCondAlliance structure must be exactly 24 bytes");

/** Looting a ruin. */
struct CMidCondLootRuin : public CMidEvCondition
{
    CMidgardID ruinId;
};

static_assert(sizeof(CMidCondLootRuin) == 20,
              "Size of CMidCondLootRuin structure must be exactly 20 bytes");

/** Transforming x% of the land. */
struct CMidCondTerraform : public CMidEvCondition
{
    int percentLand; /**< Percentage based value [-100 : 100]. */
};

static_assert(sizeof(CMidCondTerraform) == 20,
              "Size of CMidCondTerraform structure must be exactly 20 bytes");

/** Visiting a site. */
struct CMidCondVisitSite : public CMidEvCondition
{
    CMidgardID siteId;
};

static_assert(sizeof(CMidCondVisitSite) == 20,
              "Size of CMidCondVisitSite structure must be exactly 20 bytes");

/** Stack to location. */
struct CMidCondLeaderToZone : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID locationId;
};

static_assert(sizeof(CMidCondLeaderToZone) == 24,
              "Size of CMidCondLeaderToZone structure must be exactly 24 bytes");

/** Stack to city. */
struct CMidCondLeaderToCity : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID cityId;
};

static_assert(sizeof(CMidCondLeaderToCity) == 24,
              "Size of CMidCondLeaderToCity structure must be exactly 24 bytes");

/** Item to location. */
struct CMidCondItemToLocation : public CMidEvCondition
{
    CMidgardID itemId;
    CMidgardID locationId;
};

static_assert(sizeof(CMidCondItemToLocation) == 24,
              "Size of CMidCondItemToLocation structure must be exactly 24 bytes");

/** Stack existance. */
struct CMidCondStackExists : public CMidEvCondition
{
    CMidgardID stackId;
    int existanceStatus;
};

static_assert(sizeof(CMidCondStackExists) == 24,
              "Size of CMidCondStackExists structure must be exactly 24 bytes");

enum class CondVarComparison : int
{
    IgnoreSecond, /**< Check if first variable is in range, ignore second. */
    BothInRange,  /**< Both variables must be in their respective ranges. */
    AnyInRange,   /**< First or second variable in their respective ranges. */
};

/** Variable is in range. */
struct CMidCondVarInRange : public CMidEvCondition
{
    struct Variable
    {
        int id; /**< Scenario variable id, see ScenarioVariable::variableId. */
        int min;
        int max;
    };

    Variable variable1;
    Variable variable2;
    CondVarComparison comparison;
};

static_assert(sizeof(CMidCondVarInRange) == 44,
              "Size of CMidCondVarInRange structure must be exactly 44 bytes");

} // namespace game

#endif // EVENTCONDITIONS_H
