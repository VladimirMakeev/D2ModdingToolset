/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef VIEWEXPORTEDLEADERINTERF_H
#define VIEWEXPORTEDLEADERINTERF_H

#include "interffullscreen.h"
#include "leaderunitdescriptor.h"

namespace game {

struct CDialogInterf;

struct CViewExportedLeaderInterfData
{
    CDialogInterf* dialog;
    CLeaderUnitDescriptorData descriptorData;
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
};

assert_size(CViewExportedLeaderInterfData, 316);

struct CViewExportedLeaderInterf : public CInterfFullScreen
{
    CViewExportedLeaderInterfData* data;
};

assert_size(CViewExportedLeaderInterf, 12);

} // namespace game

#endif // VIEWEXPORTEDLEADERINTERF_H
