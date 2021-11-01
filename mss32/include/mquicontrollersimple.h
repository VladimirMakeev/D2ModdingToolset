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

#include "linkedlist.h"
#include "mquicontroller.h"
#include "sortedlist.h"

namespace game {

template <typename T>
struct UiEventData
{
    int eventId;
    SmartPtr<T> callback;
};

static_assert(sizeof(UiEventData<void>) == 12,
              "Size of UiEventData structure must be exactly 12 bytes");

struct UiMessageEventData
{
    unsigned int messageId;
    int eventId;
    SmartPtr<UiMessageCallback> callback;
};

static_assert(sizeof(UiMessageEventData) == 16,
              "Size of UiMessageEventData structure must be exactly 12 bytes");

struct CallbackPointerPair
{
    bool unknown;
    char padding[3];
    void* begin;
    void* end;
};

static_assert(sizeof(CallbackPointerPair) == 12,
              "Size of CallbackPointerPair structure must be exactly 12 bytes");

struct CMqUIControllerSimpleData
{
    int freeEventId;
    SortedList<UiEventData<UiTimerCallback>> timerCallbacks;
    LinkedList<UiEventData<UiUpdateCallback>> updateCallbacks;
    SortedList<UiEventData<UiPaintCallback>> paintCallbacks;
    SortedList<UiEventData<UiVisibilityCallback>> visibilityCallbacks;
    SortedList<UiEventData<UiKeypressCallback>> keypressCallbacks;
    SortedList<UiEventData<UiMousePressCallback>> mousePressCallbacks;
    SortedList<UiEventData<UiMouseMoveCallback>> mouseMoveCallbacks;
    SortedList<UiEventData<UiCloseCallback>> closeCallbacks;
    SortedList<UiMessageEventData> messageCallbacks;
    CallbackPointerPair unknown;
    CallbackPointerPair unknown2;
    CallbackPointerPair unknown3;
    CallbackPointerPair unknown4;
    CallbackPointerPair unknown5;
    CallbackPointerPair unknown6;
    CallbackPointerPair unknown7;
    CallbackPointerPair unknown8;
};

static_assert(sizeof(CMqUIControllerSimpleData) == 340,
              "Size of CMqUIControllerSimpleData structure must be exactly 340 bytes");

struct CMqUIControllerSimple : public IMqUIController
{
    CMqUIControllerSimpleData* data;
};

static_assert(sizeof(CMqUIControllerSimple) == 8,
              "Size of CMqUIControllerSimple structure must be exactly 8 bytes");

} // namespace game

#endif // MQUICONTROLLERSIMPLE_H
