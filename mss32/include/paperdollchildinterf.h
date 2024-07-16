/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef PAPERDOLLCHILDINTERF_H
#define PAPERDOLLCHILDINTERF_H

#include "interface.h"
#include "midgardid.h"

namespace game {

struct CDialogInterf;
struct CMidDragDropInterf;
struct CDDEquipmentGroup;
struct CPhaseGame;

struct CPaperdollChildInterf : public CInterface
{
    CDialogInterf* paperdollDialog;
    CMidDragDropInterf* dragDropInterface;
    Vector<CMqRect> leaderItemAreas;
    CDDEquipmentGroup* equipmentGroup;
    CMidgardID stackId;
    CPhaseGame* phaseGame;
};

assert_size(CPaperdollChildInterf, 44);

namespace CPaperdollChildInterfApi {

struct Api
{
    using Constructor = CPaperdollChildInterf*(__thiscall*)(CPaperdollChildInterf* thisptr,
                                                            CMidDragDropInterf* dragDrop,
                                                            CPhaseGame* phaseGame,
                                                            const CMidgardID* stackId,
                                                            CInterface* parentInterface,
                                                            const CMqRect* paperdollArea);
    Constructor constructor;
};

Api& get();

} // namespace CPaperdollChildInterfApi

} // namespace game

#endif // PAPERDOLLCHILDINTERF_H
