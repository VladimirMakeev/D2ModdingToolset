/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef TASKSPELL_H
#define TASKSPELL_H

#include "canselect.h"
#include "isocbscroll.h"
#include "midgardid.h"
#include "spellcat.h"
#include "tasktilehighlight.h"

namespace game {

struct CPhaseGame;
struct TStrategicSpell;
struct CursorHandle;
struct CInterface;
struct IMqImage2;
struct ObjectSelectionData;

struct CTaskSpellData
{
    CPhaseGame* phaseGame;
    CMidgardID spellId;
    LSpellCategory spellCategory;
    TStrategicSpell* spell;
    ObjectSelectionData* objectSelection;
    SmartPtr<CursorHandle> castInvalidCursor;
    SmartPtr<CursorHandle> castValidCursor;
    SmartPtr<CInterfManagerImpl> interfManager;
    CInterface* messageBox;
    CMqPoint mapPosition;
    int unknown;
    SmartPtr<IMqImage2> spellAreaFogAllowedImage;
    SmartPtr<IMqImage2> spellAreaFogNotAllowedImage;
    SmartPtr<IMqImage2> spellAreaImage;
};

assert_size(CTaskSpellData, 92);

/** Base class for spell cast target selection logic. */
struct CTaskSpell
    : public CTaskTileHighlight
    , public ICanSelect
    , public IIsoCBScroll
{
    CTaskSpellData* data;
};

assert_size(CTaskSpell, 32);

struct CTaskSpellVftable : public ITaskVftable
{
    /**
     * Checks if spell can be cast on specified map position.
     * Returns one of the status codes.
     */
    using CanCast = int(__thiscall*)(const CTaskSpell* thisptr, const CMqPoint* mapPosition);
    CanCast canCast;

    /** Sends CSpellCastMsg to server to perform spellcast. */
    using CastSpell = void(__thiscall*)(const CTaskSpell* thisptr, const CMqPoint* mapPosition);
    CastSpell castSpell;
};

assert_vftable_size(CTaskSpellVftable, 8);

} // namespace game

#endif // TASKSPELL_H
