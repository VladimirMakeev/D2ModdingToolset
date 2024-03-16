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

#ifndef AIATTITUDESTABLE_H
#define AIATTITUDESTABLE_H

#include "aiattitudescat.h"
#include "d2map.h"
#include "smartptr.h"

namespace game {

struct CAiAttitudesTableVftable;
struct CAiAttitudes;

struct CAiAttitudesTableData
{
    Map<LAttitudesCategory, SmartPtr<CAiAttitudes>> attitudes;
};

assert_size(CAiAttitudesTableData, 28);

struct CAiAttitudesTable
{
    CAiAttitudesTableVftable* vftable;
    CAiAttitudesTableData* data;
};

assert_size(CAiAttitudesTable, 8);

struct CAiAttitudesTableVftable
{
    using Destructor = CAiAttitudesTable*(__thiscall*)(CAiAttitudesTable* thisptr, char flags);
    Destructor destructor;
};

assert_vftable_size(CAiAttitudesTableVftable, 1);

namespace CAiAttitudesTableApi {

struct Api
{
    using Find = CAiAttitudes*(__thiscall*)(const CAiAttitudesTable* table,
                                            const LAttitudesCategory* category);
    Find find;
};

Api& get();

} // namespace CAiAttitudesTableApi

} // namespace game

#endif // AIATTITUDESTABLE_H
