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

#ifndef EVENTCONDITIONCAT_H
#define EVENTCONDITIONCAT_H

#include "categories.h"

namespace game {

struct LEventCondCategoryTable : public CEnumConstantTable<EventConditionId>
{ };

struct LEventCondCategory : public Category<EventConditionId>
{ };

namespace EventCondCategories {

struct Categories
{
    LEventCondCategory* frequency;
    LEventCondCategory* location;
    LEventCondCategory* enterCity;
    LEventCondCategory* ownCity;
    LEventCondCategory* killStack;
    LEventCondCategory* ownItem;
    LEventCondCategory* leaderOwnItem;
    LEventCondCategory* diplomacy;
    LEventCondCategory* alliance;
    LEventCondCategory* lootRuin;
    LEventCondCategory* transformLand;
    LEventCondCategory* visitSite;
    LEventCondCategory* leaderToZone;
    LEventCondCategory* leaderToCity;
    LEventCondCategory* itemToLocation;
    LEventCondCategory* stackExists;
    LEventCondCategory* varInRange;
};

Categories& get();

/** Returns address of LEventCondCategory::vftable used in game. */
const void* vftable();

} // namespace EventCondCategories

namespace LEventCondCategoryTableApi {

using Api = CategoryTableApi::Api<LEventCondCategoryTable, LEventCondCategory>;

Api& get();

/** Returns address of LEventCondCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LEventCondCategoryTableApi

} // namespace game

#endif // EVENTCONDITIONCAT_H
