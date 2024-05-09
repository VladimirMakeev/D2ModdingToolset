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

#ifndef TASKOBJPROP_H
#define TASKOBJPROP_H

#include "taskobj.h"

namespace game {

struct CInterface;

namespace editor {

/** Shows scenario map objects properties. */
struct CTaskObjProp : public CTaskObj
{
    CInterface* propertiesInterface;
};

assert_size(CTaskObjProp, 32);

struct CTaskObjPropVftable : public CTaskObjVftable
{
    using ClosePropertiesInterface = void(__thiscall*)(CTaskObjProp* thisptr);
    ClosePropertiesInterface closePropertiesInterface;
};

assert_vftable_size(CTaskObjPropVftable, 12);

namespace CTaskObjPropApi {

struct Api
{
    using Constructor = CTaskObjProp*(__thiscall*)(CTaskObjProp* thisptr, CObjectInterf* objInterf);
    Constructor constructor;

    CTaskObjVftable::DoAction doAction;
};

Api& get();

} // namespace CTaskObjPropApi

} // namespace editor

} // namespace game

#endif // TASKOBJPROP_H
