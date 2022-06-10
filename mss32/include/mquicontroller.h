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

#ifndef MQUICONTROLLER_H
#define MQUICONTROLLER_H

#include "functordispatch0.h"
#include "functordispatch1.h"
#include "functordispatch2.h"
#include "functordispatch3.h"
#include "smartptr.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace game {

using UiUpdateCallback = CBFunctorDispatch0wRet<bool>;
using UiPaintCallback = CBFunctorDispatch1<HDC>;
using UiVisibilityCallback = CBFunctorDispatch1<bool>;
using UiKeypressCallback = CBFunctorDispatch3<unsigned int, unsigned int, unsigned int>;
using UiMousePressCallback = CBFunctorDispatch3<unsigned short, unsigned short, const POINT*>;
using UiMouseMoveCallback = CBFunctorDispatch2<unsigned short, const POINT*>;
using UiCloseCallback = CBFunctorDispatch0wRet<bool>;
using UiMessageCallback = CBFunctorDispatch2<unsigned int, long>;
using UiTimerCallback = CBFunctorDispatch0;

struct IMqUIControllerVftable;

struct IMqUIController
{
    IMqUIControllerVftable* vftable;
};

struct IMqUIControllerVftable
{
    using Destructor = void(__thiscall*)(IMqUIController* thisptr, char flags);
    Destructor destructor;

    /** Adds callback and returns event id associated with it. */
    template <typename T>
    using AddCallback = int(__thiscall*)(IMqUIController* thisptr, SmartPtr<T>* functor);

    /** Removes callback by specified event id. */
    using RemoveCallback = void(__thiscall*)(IMqUIController* thisptr, int eventId);

    AddCallback<UiUpdateCallback> addUpdateCallback;
    RemoveCallback removeUpdateCallback;

    AddCallback<UiPaintCallback> addPaintCallback;
    RemoveCallback removePaintCallback;

    AddCallback<UiVisibilityCallback> addVisibilityChangeCallback;
    RemoveCallback removeVisibilityChangeCallback;

    AddCallback<UiKeypressCallback> addKeypressCallback;
    RemoveCallback removeKeypressCallback;

    AddCallback<UiMousePressCallback> addMousePressCallback;
    RemoveCallback removeMousePressCallback;

    AddCallback<UiMouseMoveCallback> addMouseMoveCallback;
    RemoveCallback removeMouseMoveCallback;

    AddCallback<UiCloseCallback> addCloseCallback;
    RemoveCallback removeCloseCallback;

    using AddTimerCallback = int(__thiscall*)(IMqUIController* thisptr,
                                              unsigned int timerId,
                                              SmartPtr<UiTimerCallback>* functor);
    AddTimerCallback addTimerCallback;
    RemoveCallback removeTimerCallback;

    using AddMessageCallback = int(__thiscall*)(IMqUIController* thisptr,
                                                unsigned int msgIdentifier,
                                                SmartPtr<UiMessageCallback>* functor);
    AddMessageCallback addMessageCallback;

    using RemoveMessageCallback = void(__thiscall*)(IMqUIController* thisptr,
                                                    int messageId,
                                                    int eventId);
    RemoveMessageCallback removeMessageCallback;

    using RunUpdateCallbacks = bool(__thiscall*)(IMqUIController* thisptr);
    RunUpdateCallbacks runUpdateCallbacks;

    using RunTimerCallbacks = int(__thiscall*)(IMqUIController* thisptr, unsigned int timerId);
    RunTimerCallbacks runTimerCallbacks;

    using RunPaintCallbacks = bool(__thiscall*)(IMqUIController* thisptr, HDC hdc);
    RunPaintCallbacks runPaintCallbacks;

    using RunCloseCallbacks = bool(__thiscall*)(IMqUIController* thisptr);
    RunCloseCallbacks runCloseCallbacks;

    using RunVisibilityCallbacks = bool(__thiscall*)(IMqUIController* thisptr, bool hasFocus);
    RunVisibilityCallbacks runVisibilityCallbacks;

    using RunKeypressCallbacks = bool(__thiscall*)(IMqUIController* thisptr,
                                                   unsigned short msg,
                                                   unsigned short wParam,
                                                   int dummy,
                                                   unsigned short lParam);
    RunKeypressCallbacks runKeypressCallbacks;

    using RunMousePressCallbacks = bool(__thiscall*)(IMqUIController* thisptr,
                                                     int a2,
                                                     int a3,
                                                     const POINT* point);
    RunMousePressCallbacks runMousePressCallbacks;

    using RunMouseMoveCallbacks = bool(__thiscall*)(IMqUIController* thisptr,
                                                    int wParam,
                                                    const POINT* point);
    RunMouseMoveCallbacks runMouseMoveCallbacks;

    using RunMessageCallbacks = bool(__thiscall*)(IMqUIController* thisptr,
                                                  void* dummy,
                                                  unsigned short msg,
                                                  int wParam,
                                                  int lParam);
    RunMessageCallbacks runMessageCallbacks;
};

assert_vftable_size(IMqUIControllerVftable, 28);

} // namespace game

#endif // MQUICONTROLLER_H
