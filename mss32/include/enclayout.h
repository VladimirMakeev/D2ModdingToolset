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

#ifndef ENCLAYOUT_H
#define ENCLAYOUT_H

#include "interface.h"

namespace game {

struct IEncLayoutVftable;
struct CDialogInterf;
struct IMidObject;

struct IEncLayout : public CInterfaceT<IEncLayoutVftable>
{
    CDialogInterf* dialog;
};

assert_size(IEncLayout, 12);

struct IEncLayoutVftable : public CInterfaceVftable
{
    using OnObjectChanged = void(__thiscall*)(IEncLayout* thisptr, const IMidObject* obj);
    OnObjectChanged onObjectChanged;
};

assert_vftable_size(IEncLayoutVftable, 35);

namespace IEncLayoutApi {

struct Api
{
    using Constructor = IEncLayout*(__thiscall*)(IEncLayout* thisptr,
                                                 CInterface* parent,
                                                 const CMqRect* area);
    Constructor constructor;

    using Destructor = void(__thiscall*)(IEncLayout* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace IEncLayoutApi

} // namespace game

#endif // ENCLAYOUT_H
