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

#ifndef MQUIKERNEL_H
#define MQUIKERNEL_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

namespace game {

struct IMqUIKernelVftable;
struct CMqPoint;
struct CMqRect;
struct IMqUIController;

struct IMqUIKernel
{
    IMqUIKernelVftable* vftable;
};

struct IMqUIKernelVftable
{
    using Destructor = void(__thiscall*)(IMqUIKernel* thisptr, char flags);
    Destructor destructor;

    using SetWindowPosition = bool(__thiscall*)(IMqUIKernel* thisptr, const CMqPoint* position);
    SetWindowPosition setWindowPosition;

    /** Dispatches messages, updates window, processes IMqUIController. */
    using ProcessMessages = WPARAM(__thiscall*)(IMqUIKernel* thisptr);
    ProcessMessages processMessages;

    /**
     * Closes the application.
     * CMqUIKernelSimple posts WM_CLOSE message here.
     */
    using Close = void(__thiscall*)(IMqUIKernel* thisptr);
    Close close;

    using SetUiController = void(__thiscall*)(IMqUIKernel* thisptr, IMqUIController* uiController);
    SetUiController setUiController;

    using GetUiController = IMqUIController*(__thiscall*)(const IMqUIKernel* thisptr);
    GetUiController getUiController;

    /** Usage case unknown.*/
    using ShowEmptyMessage = int(__thiscall*)(const IMqUIKernel* thisptr,
                                              int a2,
                                              std::uint32_t type);
    ShowEmptyMessage showEmptyMessage;

    /**
     * Shows message box with specified text.
     * @param type determines contents and behaviour of the message box. One of the 'MB_' Windows
     * api macros.
     * CMqUIKernelSimple calls MessageBoxA here.
     */
    using ShowMessage = int(__thiscall*)(const IMqUIKernel* thisptr,
                                         const char* text,
                                         std::uint32_t type);
    ShowMessage showMessage;

    using GetWindowTitle = const char*(__thiscall*)(const IMqUIKernel* thisptr);
    GetWindowTitle getWindowTitle;

    /** Flashes window to inform the user that window requires attention. */
    using FlashNotification = bool(__thiscall*)(IMqUIKernel* thisptr, bool changeWndState);
    FlashNotification flashNotification;

    /** Returns handle of a game window. */
    using GetWindowHandle = HWND(__thiscall*)(const IMqUIKernel* thisptr);
    GetWindowHandle getWindowHandle;

    using GetInstance = HINSTANCE(__thiscall*)(const IMqUIKernel* thisptr);
    /** Returns application instance. */
    GetInstance getInstance;
    /** Assumption: returns copy of application instance. */
    GetInstance getInstanceCopy;

    using SetInstanceCopy = void(__thiscall*)(IMqUIKernel* thisptr, HINSTANCE instance);
    SetInstanceCopy setInstanceCopy;

    /**
     * Returns system time in milliseconds.
     * CMqUIKernelSimple returns systemTime acquired from timeGetTime() in a message loop.
     */
    using GetTime = int(__thiscall*)(const IMqUIKernel* thisptr);
    GetTime getTime;

    using GetMousePosition = CMqPoint*(__thiscall*)(const IMqUIKernel* thisptr,
                                                    CMqPoint* mousePosition);
    /** Assumption: returns mouse position in client coordinates. */
    GetMousePosition getMousePosClient;
    /** Assumption: returns mouse position in screen coordinates. */
    GetMousePosition getMousePosScreen;

    /**
     * Sets current mouse position according to specified client coordinates.
     * CMqUIKernelSimple updates client and screen coordinates, sends WM_MOUSEMOVE message.
     */
    using SetMousePosition = void(__thiscall*)(IMqUIKernel* thisptr,
                                               const CMqPoint* mousePosClient);
    SetMousePosition setMousePosition;

    /** Clips mouse cursor using specified area. */
    using ClipCursor = void(__thiscall*)(IMqUIKernel* thisptr, const CMqRect* area);
    ClipCursor clipCursor;

    /**
     * Meaning unknown.
     * CMqUIKernelSimple sets unknown17 to false.
     */
    using ResetUnknown17 = void(__thiscall*)(IMqUIKernel* thisptr);
    ResetUnknown17 resetUnknown17;

    /** Returns game window area. */
    using GetClientArea = CMqRect*(__thiscall*)(const IMqUIKernel* thisptr, CMqRect* clientArea);
    GetClientArea getClientArea;

    /** Returns true if window has focus and is not minimised. */
    using HasFocusAndShown = bool(__thiscall*)(const IMqUIKernel* thisptr);
    HasFocusAndShown hasFocusAndShown;

    /** Returns true if specified virtual key is pressed. */
    using IsKeyDown = bool(__thiscall*)(const IMqUIKernel* thistr, int key);
    IsKeyDown isKeyDown;

    using InvalidateAndUpdate = void(__thiscall*)(IMqUIKernel* thisptr);
    InvalidateAndUpdate invalidateAndUpdate;

    /**
     * Registers new window message.
     * CMqUIKernelSimple calls RegisterWindowMessageA here.
     */
    using RegisterMessage = std::uint32_t(__thiscall*)(IMqUIKernel* thisptr, const char* message);
    RegisterMessage registerMessage;

    /**
     * Posts specified message.
     * CMqUIKernelSimple calls PostMessageA here.
     */
    using PostWndMessage = bool(__thiscall*)(const IMqUIKernel* thisptr,
                                             UINT Msg,
                                             WPARAM wParam,
                                             LPARAM lParam);
    PostWndMessage postMessage;

    /**
     * Adds timer with specified timout value in milliseconds.
     * CMqUIKernelSimple calls SetTimer here.
     * @returns id of created timer.
     */
    using AddTimer = std::uint32_t(__thiscall*)(IMqUIKernel* thisptr, std::uint32_t timeoutMs);
    AddTimer addTimer;

    /**
     * Removes timer with specfied id.
     * CMqUIKernelSimple calls KillTimer here.
     */
    using RemoveTimer = void(__thiscall*)(IMqUIKernel* thisptr, std::uint32_t timerId);
    RemoveTimer removeTimer;
};

static_assert(sizeof(IMqUIKernelVftable) == 28 * sizeof(void*),
              "IMqUIKernel vftable must have exactly 28 methods");

} // namespace game

#endif // MQUIKERNEL_H
