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

#ifndef WIDGETINTERF_H
#define WIDGETINTERF_H

#include "d2list.h"
#include "functordispatch1.h"
#include "functordispatch2.h"
#include "functordispatch3.h"
#include "functordispatch4.h"
#include "interface.h"
#include "uievent.h"

namespace game {

struct CWidgetInterf;
struct IMqImage2;

enum class WidgetMouseState : std::uint32_t
{
    MouseOver = 1,
    MouseLeftDown = 2,
    MouseRightDown = 4,
    MouseBtnPressed = 8,
};

struct CWidgetInterfState
{
    CMqPoint mousePosition;
    WidgetMouseState mouseState;
    bool mouseOver;
    bool wasPressed;
    char padding[2];
};

static_assert(sizeof(CWidgetInterfState) == 16,
              "Size of CWidgetInterfState structure must be exactly 16 bytes");

struct WidgetImage
{
    SmartPtr<IMqImage2> image;
    CMqPoint offset;
    int unknown;
};

static_assert(sizeof(WidgetImage) == 20, "Size of WidgetImage structure must be exactly 20 bytes");

using WidgetMouseMoveFunctor = CBFunctorDispatch3<CWidgetInterf*, const CMqPoint*, unsigned int>;
using WidgetMousePressFunctor = CBFunctorDispatch2<CWidgetInterf*, const CMqPoint*>;
using WidgetLeftUpFunctor = CBFunctorDispatch4<CWidgetInterf*, const CMqPoint*, bool, bool>;

struct CWidgetInterfData
{
    List<WidgetImage> images;
    SmartPtr<WidgetMouseMoveFunctor> mouseMoveFunctor;
    SmartPtr<WidgetMousePressFunctor> mouseLeftDownFunctor;
    SmartPtr<WidgetMousePressFunctor> mouseRightDownFunctor;
    SmartPtr<WidgetLeftUpFunctor> mouseLeftUpFunctor;
    SmartPtr<WidgetMousePressFunctor> mouseRightUpFunctor;
    SmartPtr<CBFunctorDispatch1<CWidgetInterf*>> mouseDblClickFunctor;
    std::uint32_t mouseClickTime;
    bool mouseOver;
    bool mousePressed;
    bool mouseLeftUp;
    bool mouseRightUp;
    UiEvent mouseMoveEvent;
    UiEvent mouseLeftDownEvent;
    UiEvent mouseRightDownEvent;
    UiEvent mouseLeftUpEvent;
    UiEvent mouseRightUpEvent;
    UiEvent mouseDblClickEvent;
    std::uint32_t mouseMoveMsgId;
    std::uint32_t mouseLeftDownMsgId;
    std::uint32_t mouseRightDownMsgId;
    std::uint32_t mouseLeftUpMsgId;
    std::uint32_t mouseRightUpMsgId;
    std::uint32_t mouseDblClickMsgId;
    CWidgetInterfState* state;
};

static_assert(sizeof(CWidgetInterfData) == 244,
              "Size of CWidgetInterfData structure must be exactly 244 bytes");

struct CWidgetInterf : public CInterface
{
    CWidgetInterfData* data;
};

static_assert(sizeof(CWidgetInterf) == 12,
              "Size of CPictureInterf structure must be exactly 12 bytes");

} // namespace game

#endif // WIDGETINTERF_H
