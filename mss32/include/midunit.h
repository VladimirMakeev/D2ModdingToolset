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

#include "idlist.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct IUsUnit;
struct CScenarioVisitor;

/** Holds unit related data in scenario file and game. */
struct CMidUnit : public IMidScenarioObject
{
    CMidgardID unitId;
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

static_assert(sizeof(CMidUnit) == 80, "Size of CMidUnit structure must be exactly 80 bytes");

static_assert(offsetof(CMidUnit, dynLevel) == 24, "CMidUnit::dynLevel offset must be 24 bytes");
static_assert(offsetof(CMidUnit, origTypeId) == 44, "CMidUnit::origTypeId offset must be 44 bytes");
static_assert(offsetof(CMidUnit, origXp) == 60, "CMidUnit::origXp offset must be 60 bytes");

namespace CMidUnitApi {

struct Api
{
    using AddRemoveModifier = bool(__thiscall*)(CMidUnit* thisptr, const CMidgardID* modifierId);
    AddRemoveModifier addModifier;
    AddRemoveModifier removeModifier;

    using GetModifiers = bool(__thiscall*)(CMidUnit* thisptr, game::IdList* value);
    GetModifiers getModifiers;

    using GetHpMax = int(__thiscall*)(const CMidUnit* thisptr);
    GetHpMax getHpMax;

    using Transform = bool(__thiscall*)(CMidUnit* thisptr,
                                        const CScenarioVisitor* visitor,
                                        const IMidgardObjectMap* objectMap,
                                        const CMidgardID* transformImplId,
                                        bool keepHp);
    Transform transform;

    using Untransform = bool(__thiscall*)(CMidUnit* thisptr, const CScenarioVisitor* visitor);
    Untransform untransform;

    using AddModifiers = bool(__stdcall*)(const game::IdList* value,
                                          CMidUnit* unit,
                                          char* errorBuffer,
                                          bool checkApplicable);
    AddModifiers addModifiers;

    using RemoveModifiers = bool(__stdcall*)(IUsUnit** unitImpl);
    RemoveModifiers removeModifiers;

    using ReplaceImpl = bool(__stdcall*)(IUsUnit** unitImpl, IUsUnit* value);
    ReplaceImpl replaceImpl;
};

Api& get();

} // namespace CMidUnitApi

} // namespace game

#endif // MIDUNIT_H
