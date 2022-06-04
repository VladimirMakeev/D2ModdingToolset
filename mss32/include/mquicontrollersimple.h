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

#ifndef MQUICONTROLLERSIMPLE_H
#define MQUICONTROLLERSIMPLE_H

#include "d2list.h"
#include "d2set.h"
#include "mquicontroller.h"

namespace game {

template <typename T>
struct UiEventData
{
    int eventId;
    SmartPtr<T> callback;
};

assert_size(UiEventData<void>, 12);

struct UiMessageEventData
{
    unsigned int messageId;
    int eventId;
    SmartPtr<UiMessageCallback> callback;
};

assert_size(UiMessageEventData, 16);

struct CallbackPointerPair
{
    bool unknown;
    char padding[3];
    void* begin;
    void* end;
};

assert_size(CallbackPointerPair, 12);

struct CMqUIControllerSimpleData
{
    int freeEventId;
    Set<UiEventData<UiTimerCallback>> timerCallbacks;
    List<UiEventData<UiUpdateCallback>> updateCallbacks;
    Set<UiEventData<UiPaintCallback>> paintCallbacks;
    Set<UiEventData<UiVisibilityCallback>> visibilityCallbacks;
    Set<UiEventData<UiKeypressCallback>> keypressCallbacks;
    Set<UiEventData<UiMousePressCallback>> mousePressCallbacks;
    Set<UiEventData<UiMouseMoveCallback>> mouseMoveCallbacks;
    Set<UiEventData<UiCloseCallback>> closeCallbacks;
    Set<UiMessageEventData> messageCallbacks;
    CallbackPointerPair unknown;
    CallbackPointerPair unknown2;
    CallbackPointerPair unknown3;
    CallbackPointerPair unknown4;
    CallbackPointerPair unknown5;
    CallbackPointerPair unknown6;
    CallbackPointerPair unknown7;
    CallbackPointerPair unknown8;
};

assert_size(CMqUIControllerSimpleData, 340);

struct CMqUIControllerSimple : public IMqUIController
{
    CMqUIControllerSimpleData* data;
};

assert_size(CMqUIControllerSimple, 8);

} // namespace game

#endif // MQUICONTROLLERSIMPLE_H
