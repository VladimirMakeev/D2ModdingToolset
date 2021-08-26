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

#ifndef EVENTEFFECTCAT_H
#define EVENTEFFECTCAT_H

#include "categories.h"

namespace game {

struct LEventEffectCategoryTable : public CEnumConstantTable<EventEffectId>
{ };

struct LEventEffectCategory : public Category<EventEffectId>
{ };

namespace EventEffectCategories {

struct Categories
{
    LEventEffectCategory* win;
    LEventEffectCategory* createStack;
    LEventEffectCategory* castSpell;
    LEventEffectCategory* castSpellMap;
    LEventEffectCategory* changeOwner;
    LEventEffectCategory* changeOrder;
    LEventEffectCategory* moveBeside;
    LEventEffectCategory* battle;
    LEventEffectCategory* enableEvent;
    LEventEffectCategory* giveSpell;
    LEventEffectCategory* giveItem;
    LEventEffectCategory* moveStack;
    LEventEffectCategory* ally;
    LEventEffectCategory* diplomacy;
    LEventEffectCategory* unfog;
    LEventEffectCategory* removeMountain;
    LEventEffectCategory* removeLmark;
    LEventEffectCategory* changeObjective;
    LEventEffectCategory* popup;
    LEventEffectCategory* destroyItem;
    LEventEffectCategory* removeStack;
    LEventEffectCategory* changeLandmark;
    LEventEffectCategory* changeTerrain;
    LEventEffectCategory* modifyVariable;
};

Categories& get();

/** Returns address of LEventEffectCategory::vftable used in game. */
const void* vftable();

} // namespace EventEffectCategories

namespace LEventEffectCategoryTableApi {

using Api = CategoryTableApi::Api<LEventEffectCategoryTable, LEventEffectCategory>;

Api& get();

/** Returns address of LEventEffectCategoryTable::vftable used in game. */
const void* vftable();

} // namespace LEventEffectCategoryTableApi

} // namespace game

#endif // EVENTEFFECTCAT_H
