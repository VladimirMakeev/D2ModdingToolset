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

#ifndef FORTIFICATION_H
#define FORTIFICATION_H

#include "aipriority.h"
#include "idlist.h"
#include "mapelement.h"
#include "midinventory.h"
#include "midscenarioobject.h"
#include "midunitgroup.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct LFortCategory;
struct CMidStreamFile;

struct CMidUnitGroupFortification : public CMidUnitGroup
{ };

/** Base class for capitals and villages (cities). */
struct CFortification : public IMidScenarioObject
{
    IMapElement mapElement;
    IAiPriority aiPriority;
    StringAndId name;
    StringAndId description;
    CMidUnitGroupFortification group;
    CMidInventory inventory;
    CMidgardID ownerId;
    CMidgardID subraceId;
    CMidgardID stackId;
};

assert_offset(CFortification, aiPriority, 28);
assert_offset(CFortification, name, 36);
assert_offset(CFortification, description, 48);
assert_offset(CFortification, group, 60);
assert_offset(CFortification, inventory, 112);

struct CFortificationVftable : public IMidScenarioObjectVftable
{
    using GetCategory = const LFortCategory*(__thiscall*)(const CFortification* thisptr);
    GetCategory getCategory;

    using GetCString = const char*(__thiscall*)(CFortification* thisptr);
    GetCString getName;
    GetCString getDescription;

    /** Returns daily hp regeneration value. */
    using GetRegen = int*(__thiscall*)(const CFortification* thisptr,
                                       int* regen,
                                       const IMidgardObjectMap* objectMap);
    GetRegen getRegen;

    using GetInt = int(__thiscall*)(const CFortification* thisptr,
                                    const IMidgardObjectMap* objectMap);
    GetInt getMaxUnitsInGarrison;
    GetInt getTier;

    /** Adds protection modifier ids to the list. */
    using GetProtection = void(__thiscall*)(CFortification* thisptr,
                                            const IMidgardObjectMap* objectMap,
                                            IdList* list);
    GetProtection getProtection;

    using SetCString = bool(__thiscall*)(CFortification* thisptr,
                                         int a2,
                                         int a3,
                                         const char* value);
    SetCString setName;
    SetCString setDescription;

    using StreamVillageData = void(__thiscall*)(CFortification* thisptr,
                                                int a2,
                                                CMidStreamFile** stream);
    StreamVillageData streamVillageData;

    /** Changes fortification owner and subrace. */
    using SetOwner = bool(__thiscall*)(CFortification* thisptr,
                                       const CMidgardID* ownerId,
                                       const CMidgardID* subraceId);
    SetOwner setOwner;
};

assert_vftable_size(CFortificationVftable, 15);

} // namespace game

#endif // FORTIFICATION_H
