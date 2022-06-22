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

#ifndef ENCLAYOUTSTACK_H
#define ENCLAYOUTSTACK_H

#include "enclayout.h"

namespace game {

struct CEncLayoutStackData;
struct IMidObject;
struct IMidgardObjectMap;
struct CMidStack;

struct CEncLayoutStack : public IEncLayout
{
    CEncLayoutStackData* data;
};

assert_size(CEncLayoutStack, 16);

struct CEncLayoutStackData
{
    char unknown[328];
};

assert_size(CEncLayoutStackData, 328);

struct CEncLayoutStackVftable : CInterfaceVftable
{
    using Update = void(__thiscall*)(CEncLayoutStack* thisptr, const IMidObject* stackObj);
    Update update;
};

assert_vftable_size(CEncLayoutStackVftable, 35);

namespace CEncLayoutStackApi {

struct Api
{
    using Update = void(__thiscall*)(CEncLayoutStack* thisptr,
                                     const IMidgardObjectMap* objectMap,
                                     const CMidStack* stack,
                                     CDialogInterf* dialog);
    Update update;
};

Api& get();

CEncLayoutStackVftable* vftable();

} // namespace CEncLayoutStackApi

} // namespace game

#endif // ENCLAYOUTSTACK_H
