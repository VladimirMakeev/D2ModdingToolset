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

#ifndef MIDUNIT_H
#define MIDUNIT_H

#include "d2assert.h"
#include "idlist.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct IUsUnit;
struct CScenarioVisitor;
struct CMidUnitVftable;

/** Holds unit related data in scenario file and game. */
struct CMidUnit : public IMidScenarioObject
{
    int creation;
    StringAndId name;
    bool dynLevel;
    char padding[3];
    IUsUnit* unitImpl;
    int currentHp;
    int currentXp;
    bool transformed;
    char padding2[3];
    CMidgardID origTypeId;
    bool keepHp;
    char padding3[3];
    int hpBefore;
    int hpBefMax;
    int origXp;
    IdList origModifiers;
};

assert_size(CMidUnit, 80);

assert_offset(CMidUnit, dynLevel, 24);
assert_offset(CMidUnit, origTypeId, 44);
assert_offset(CMidUnit, origXp, 60);

struct CMidUnitVftable : IMidScenarioObjectVftable
{
    /**
     * Called by server when new unit is created.
     * Used in CreateUnit, CVisitorChangeStackLeader::Apply and CVisitorAddUnitToGroup::Apply.
     */
    using InitWithSoldierImpl = bool(__thiscall*)(CMidUnit* thisptr,
                                                  const IMidgardObjectMap* objectMap,
                                                  const CMidgardID* unitImplId,
                                                  const int* turn);
    InitWithSoldierImpl initWithSoldierImpl;

    /**
     * Called by server when unit is removed. Calls RemoveModifiers and sets unitImpl to null.
     * Used in CVisitorChangeStackLeader::Apply and CVisitorRmvUnitFromGroup::Apply.
     * Not actually a finalize method as it is not called every time a unit instance is destroyed.
     * Destructor also calls RemoveModifiers on its own.
     */
    using RemoveUnitImpl = bool(__thiscall*)(CMidUnit* thisptr, int a2);
    RemoveUnitImpl removeUnitImpl;
};

static_assert(sizeof(CMidUnitVftable) == 6 * sizeof(void*),
              "CMidUnit vftable must have exactly 6 methods");

namespace CMidUnitApi {

struct Api
{
    using AddRemoveModifier = bool(__thiscall*)(CMidUnit* thisptr, const CMidgardID* modifierId);
    AddRemoveModifier addModifier;
    AddRemoveModifier removeModifier;

    using GetHpMax = int(__thiscall*)(const CMidUnit* thisptr);
    GetHpMax getHpMax;

    using Upgrade = bool(__thiscall*)(CMidUnit* thisptr,
                                      const CScenarioVisitor* visitor,
                                      const CMidgardID* upgradeImplId);
    Upgrade upgrade;

    using Transform = bool(__thiscall*)(CMidUnit* thisptr,
                                        const CScenarioVisitor* visitor,
                                        const IMidgardObjectMap* objectMap,
                                        const CMidgardID* transformImplId,
                                        bool keepHp);
    Transform transform;

    using Untransform = bool(__thiscall*)(CMidUnit* thisptr, const CScenarioVisitor* visitor);
    Untransform untransform;

    using GetModifiers = bool(__stdcall*)(IdList* value, const CMidUnit* unit);
    GetModifiers getModifiers;

    using AddModifiers = bool(__stdcall*)(const IdList* value,
                                          CMidUnit* unit,
                                          char* errorBuffer,
                                          bool checkCanApply);
    AddModifiers addModifiers;

    using RemoveModifiers = bool(__stdcall*)(IUsUnit** unitImpl);
    RemoveModifiers removeModifiers;

    using ReplaceImpl = bool(__stdcall*)(IUsUnit** unitImpl, IUsUnit* value);
    ReplaceImpl replaceImpl;
};

Api& get();

CMidUnitVftable* vftable();

} // namespace CMidUnitApi

} // namespace game

#endif // MIDUNIT_H
