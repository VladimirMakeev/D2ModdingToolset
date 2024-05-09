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

#ifndef TASKOBJADDSITE_H
#define TASKOBJADDSITE_H

#include "sitecategories.h"
#include "taskobj.h"

namespace game {

namespace editor {

struct CObjectInterf;

struct CTaskObjAddSiteData
{
    LSiteCategory siteCategory;
    SmartPtr<CursorHandle> selectUnitCursor;
    SmartPtr<CursorHandle> noDragDropCursor;
};

assert_size(CTaskObjAddSiteData, 28);

/** Creates site objects on scenario map. */
struct CTaskObjAddSite : public CTaskObj
{
    CTaskObjAddSiteData* siteData;
};

assert_size(CTaskObjAddSite, 32);

namespace CTaskObjAddSiteApi {

struct Api
{
    using Constructor = CTaskObjAddSite*(__thiscall*)(CTaskObjAddSite* thisptr,
                                                      CObjectInterf* objInterf,
                                                      LSiteCategory category);
    Constructor constructor;

    CTaskObjVftable::DoAction doAction;
};

Api& get();

} // namespace CTaskObjAddSiteApi

} // namespace editor

} // namespace game

#endif // TASKOBJADDSITE_H
