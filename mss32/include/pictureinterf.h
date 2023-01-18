/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef PICTUREINTERF_H
#define PICTUREINTERF_H

#include "functordispatch1.h"
#include "functordispatch2.h"
#include "interface.h"

namespace game {

struct IMqImage2;
struct CMqPoint;
struct CPictureInterf;

struct CPictureInterfData
{
    int widgetChildIndex;
    SmartPtr<CBFunctorDispatch2<std::uint32_t, CPictureInterf*>> onMouseButtonPressed;
    SmartPtr<CBFunctorDispatch1<CPictureInterf*>> onMouseDoubleClicked;
};

assert_size(CPictureInterfData, 20);

/**
 * Picture ui element.
 * Represents IMAGE from Interf.dlg or ScenEdit.dlg files.
 */
struct CPictureInterf : public CInterface
{
    CPictureInterfData* data;
};

assert_size(CPictureInterf, 12);

namespace CPictureInterfApi {

struct Api
{
    /**
     * Sets specified image to be shown by picture ui element.
     * Passing nullptr hides image.
     */
    using SetImage = void(__thiscall*)(CPictureInterf* thisptr,
                                       IMqImage2* image,
                                       const CMqPoint* offset);
    SetImage setImage;

    /** Assigns mouse button press functor. */
    using AssignFunctor = void(__thiscall*)(CPictureInterf* thisptr, SmartPointer* functor);
    AssignFunctor assignFunctor;
};

Api& get();

} // namespace CPictureInterfApi

} // namespace game

#endif // PICTUREINTERF_H
