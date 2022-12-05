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

#ifndef SITETRAININGCAMPINTERF_H
#define SITETRAININGCAMPINTERF_H

#include "idvector.h"
#include "middatacache.h"
#include "middragdropinterf.h"
#include "resetstackext.h"

namespace game {

struct CDDStackGroup;
struct CTrainingUnit;

struct CSiteTrainingCampInterfData
{
    CMidgardID stackId;
    CDDStackGroup* stackGroup;
    CMidgardID trainingCampId;
    ITask* taskOpenInterf;
    CPhaseGame* phaseGame;
    CTrainingUnit* trainingUnit;
    IdVector trainableUnitIds;
    CMqRect rSlotImageArea;
    int unk9;
};

assert_size(CSiteTrainingCampInterfData, 60);

struct CSiteTrainingCampInterf
    : public CMidDataCache2::INotify
    , public IResetStackExt
    , public CMidDragDropInterf
{
    CSiteTrainingCampInterfData* trainingCampData;
};

assert_size(CSiteTrainingCampInterf, 40);

} // namespace game

#endif // SITETRAININGCAMPINTERF_H
