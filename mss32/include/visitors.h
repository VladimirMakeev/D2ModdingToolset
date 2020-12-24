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

#ifndef VISITORS_H
#define VISITORS_H

namespace game {

struct CMidgardID;
struct IMidgardObjectMap;
struct CScenarioVisitorVftable;

/**
 * Base for all visitor classes.
 * Visitors are used to change scenario objects or rules.
 */
struct CScenarioVisitor
{
    const CScenarioVisitorVftable* vftable;
    IMidgardObjectMap* objectMap;
};

struct CScenarioVisitorVftable
{
    void* destructor;

    /** Returns true if visitor can be applied. */
    using CanApply = bool(__thiscall*)(CScenarioVisitor* thisptr);
    CanApply canApply;

    /**
     * Applies visitor.
     * @returns true if visitor was applied successfully.
     */
    using Apply = bool(__thiscall*)(CScenarioVisitor* thisptr);
    Apply apply;
};

namespace VisitorApi {

struct Api
{
    /**
     * Changes unit hit points by specified amount.
     * Handles all necessary logic with min/max values as well as stack leader alive statuses.
     * @param[in] unitId id of unit whose hit points to change.
     * @param amount hit points change amount. Positive value heals unit, negative damages.
     * @param[in] objectMap interface used for unit search.
     * @param apply specifies whether changes should be applied.
     * @returns true if hp was changed when apply was selected. If apply set to 0, returns whether
     * visitor can be applied.
     */
    using ChangeUnitHp = bool(__stdcall*)(const CMidgardID* unitId,
                                          int amount,
                                          IMidgardObjectMap* objectMap,
                                          int apply);
    ChangeUnitHp changeUnitHp;
};

Api& get();

} // namespace VisitorApi
} // namespace game

#endif // VISITORS_H
