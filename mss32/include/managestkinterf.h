/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef MANAGESTKINTERF_H
#define MANAGESTKINTERF_H

#include "d2string.h"
#include "d2vector.h"
#include "middatacache.h"
#include "middragdropinterf.h"
#include "midgardid.h"
#include "mqrect.h"
#include "resetstackext.h"
#include "smartptr.h"

namespace game {

struct CDDStackGroup;
struct CDDStackInventoryDisplay;
struct CPaperdollChildInterf;
struct CMidDropAction;
struct ITask;
struct CPhaseGame;
struct IMqImage2;

struct CAudioRegionHolder
{
    CPhaseGame* phaseGame;
};

assert_size(CAudioRegionHolder, 4);

struct CManageStkInterfData
{
    CMidgardID stackId;
    CDDStackGroup* ddStackGroup;
    CDDStackInventoryDisplay* ddStackInventoryDisplay;
    CPaperdollChildInterf* paperdollChildInterf;
    CMqRect paperdollArea;
    CMidDropAction* dismissUnitAction;
    CMidDropAction* dynLevelUnitAction;
    ITask* taskOpenInterf;
    CPhaseGame* phaseGame;
    Vector<SmartPtr<IMqImage2>> leaderAbilityIcons;
    Vector<String> leaderAbilityNames;
    CMqRect slot1Area;
    CAudioRegionHolder* audioRegionHolder;
    bool isImportLeader;
    char padding[3];
};

assert_size(CManageStkInterfData, 104);

struct CManageStkInterf
    : public CMidDataCache2::INotify
    , public IResetStackExt
    , public CMidDragDropInterf
{
    CManageStkInterfData* manageStkInterfData;
};

assert_size(CManageStkInterf, 40);

namespace CManageStkInterfApi {

struct Vftable
{
    const CMidDataCache2::INotifyVftable* notify;
    const IResetStackExtVftable* resetStackExt;
    const CInterfaceVftable* interface;
    const ITaskManagerHolderVftable* taskManagerHolder;
    const IMidDropManagerVftable* dropManager;
};

Vftable& vftable();

} // namespace CManageStkInterfApi

} // namespace game

#endif // MANAGESTKINTERF_H
