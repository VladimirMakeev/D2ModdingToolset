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

assert_size(CMidCondFrequency, 20);

/** Entering a predefined zone. */
struct CMidCondLocation : public CMidEvCondition
{
    CMidgardID locationId;
};

assert_size(CMidCondLocation, 20);

/** Entering a city. */
struct CMidCondEnterCity : public CMidEvCondition
{
    CMidgardID cityId;
};

assert_size(CMidCondEnterCity, 20);

/** Owning a city. */
struct CMidCondOwnCity : public CMidEvCondition
{
    CMidgardID cityId;
};

assert_size(CMidCondOwnCity, 20);

/** Destroying a stack. */
struct CMidCondKillStack : public CMidEvCondition
{
    CMidgardID stackId;
};

assert_size(CMidCondKillStack, 20);

/** Owning an item. */
struct CMidCondOwnItem : public CMidEvCondition
{
    CMidgardID itemId;
};

assert_size(CMidCondOwnItem, 20);

/** Specific leader owning an item. */
struct CMidCondLeaderOwnItem : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID itemId;
};

assert_size(CMidCondLeaderOwnItem, 24);

/** Diplomacy relations. */
struct CMidCondDiplomacy : public CMidEvCondition
{
    CMidgardID playerId1;
    CMidgardID playerId2;
    int diplomacy; /**< Percentage based value [-100 : 100]. */
};

assert_size(CMidCondDiplomacy, 28);

/** Being in alliance. */
struct CMidCondAlliance : public CMidEvCondition
{
    CMidgardID playerId1;
    CMidgardID playerId2;
};

assert_size(CMidCondAlliance, 24);

/** Looting a ruin. */
struct CMidCondLootRuin : public CMidEvCondition
{
    CMidgardID ruinId;
};

assert_size(CMidCondLootRuin, 20);

/** Transforming x% of the land. */
struct CMidCondTerraform : public CMidEvCondition
{
    int percentLand; /**< Percentage based value [-100 : 100]. */
};

assert_size(CMidCondTerraform, 20);

/** Visiting a site. */
struct CMidCondVisitSite : public CMidEvCondition
{
    CMidgardID siteId;
};

assert_size(CMidCondVisitSite, 20);

/** Stack to location. */
struct CMidCondLeaderToZone : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID locationId;
};

assert_size(CMidCondLeaderToZone, 24);

/** Stack to city. */
struct CMidCondLeaderToCity : public CMidEvCondition
{
    CMidgardID stackId;
    CMidgardID cityId;
};

assert_size(CMidCondLeaderToCity, 24);

/** Item to location. */
struct CMidCondItemToLocation : public CMidEvCondition
{
    CMidgardID itemId;
    CMidgardID locationId;
};

assert_size(CMidCondItemToLocation, 24);

/** Stack existance. */
struct CMidCondStackExists : public CMidEvCondition
{
    CMidgardID stackId;
    int existanceStatus;
};

assert_size(CMidCondStackExists, 24);

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

assert_size(CMidCondVarInRange, 44);

} // namespace game

#endif // EVENTCONDITIONS_H
