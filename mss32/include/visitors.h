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
     * Uses CVisitorChangeUnitHP.
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

    /**
     * Sets maximum allowed units count for a specific group.
     * Uses CVisitorForceUnitMax.
     * Changes CMidUnitGroup::maxUnitsAllowed value.
     * @param[in] groupId id of group where unit will be summoned.
     * @param maxUnitsAllowed maximum allowed units in group. Value of -1 disables the restriction.
     * @param[in] objectMap interface used for group search.
     * @param apply specifies whether visitor should be applied.
     */
    using ForceUnitMax = bool(__stdcall*)(const CMidgardID* groupId,
                                          int maxUnitsAllowed,
                                          IMidgardObjectMap* objectMap,
                                          int apply);
    ForceUnitMax forceUnitMax;

    /**
     * Adds unit to the group.
     * Uses CVisitorAddUnitToGroup.
     * @param[in] unitId id of the exising unit to add.
     * @param[in] groupId id of the group in stack, fortification or ruin to add unit to.
     * @param position unit position in group.
     * @param[in] creationTurn number of game turn when unit was created.
     * @param a5 meaning unknown, summon attack logic sets it to 1.
     * @param objectMap interface used for group search.
     * @param apply specifies whether unit addition should be applied.
     * @returns true if unit was added when apply set to 1. If apply set to 0, returns whether
     * visitor can be applied.
     */
    using AddUnitToGroup = bool(__stdcall*)(const CMidgardID* unitId,
                                            const CMidgardID* groupId,
                                            int position,
                                            int* creationTurn,
                                            char a5,
                                            IMidgardObjectMap* objectMap,
                                            int apply);
    AddUnitToGroup addUnitToGroup;

    /**
     * Transfers item from one object with inventory (stack, bag or city) to another.
     * Uses CVisitorExchangeItem.
     * @param[in] fromObjectId id of the object with inventory to transfer from.
     * @param[in] toObjectId id of the object with inventory to receive item.
     * @param[in] itemId id of the item to transfer.
     * @param objectMap interface used for objects search.
     * @param apply specifies whether item transfer should be applied.
     * @returns true if item was transfered when apply set to 1. If apply set to 0, returns whether
     * visitor can be applied.
     */
    using ExchangeItem = bool(__stdcall*)(const CMidgardID* fromObjectId,
                                          const CMidgardID* toObjectId,
                                          const CMidgardID* itemId,
                                          IMidgardObjectMap* objectMap,
                                          int apply);
    ExchangeItem exchangeItem;

    /**
     * Transforms a unit to another.
     * Uses CVisitorTransformUnit.
     * @param[in] unitId id of the unit to be transformed.
     * @param[in] toUnitImplId id of the unit impl to transform to.
     * @param[in] keepHp specifies whether the transformed unit retains its original hp.
     * @param objectMap interface used for objects search.
     * @param apply specifies whether unit transformation should be applied.
     * @returns true if unit was transformed when apply set to 1. If apply set to 0, returns whether
     * visitor can be applied.
     */
    using TransformUnit = bool(__stdcall*)(CMidgardID* unitId,
                                           CMidgardID* toUnitImplId,
                                           bool keepHp,
                                           IMidgardObjectMap* objectMap,
                                           int apply);
    TransformUnit transformUnit;

    /**
     * Untransforms a unit.
     * Uses CVisitorUndoTransformUnit.
     * @param[in] unitId id of the unit to be untransformed.
     * @param objectMap interface used for objects search.
     * @param apply specifies whether unit transformation should be applied.
     * @returns true if unit was untransformed when apply set to 1. If apply set to 0, returns
     * whether visitor can be applied.
     */
    using UndoTransformUnit = bool(__stdcall*)(CMidgardID* unitId,
                                               IMidgardObjectMap* objectMap,
                                               int apply);
    UndoTransformUnit undoTransformUnit;

    /**
     * Extracts unit from group.
     * Uses CVisitorExtractUnitFromGroup.
     * @param[in] unitId id of unit to extract.
     * @param[in] groupId id of group to extract from.
     * @param[in] objectMap interface used for objects search.
     * @param apply specifies whether unit extraction should be applied.
     * @returns true if unit was extracted when apply set to 1. If apply set to 0, returns whether
     * visitor can be applied.
     */
    using ExtractUnitFromGroup = bool(__stdcall*)(const CMidgardID* unitId,
                                                  const CMidgardID* groupId,
                                                  IMidgardObjectMap* objectMap,
                                                  int apply);
    ExtractUnitFromGroup extractUnitFromGroup;
};

Api& get();

} // namespace VisitorApi
} // namespace game

#endif // VISITORS_H
