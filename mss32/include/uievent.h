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

#ifndef UIEVENT_H
#define UIEVENT_H

#include "d2assert.h"
#include "smartptr.h"
#include <cstdint>

namespace game {

struct CUIManager;

/**
 * Ui related event types used in game.
 * Determines callback signature.
 */
enum class UiEventType : int
{
    /** Timer events, callback is called at the specified interval. */
    Timer = 1,

    /** Constant updates, callback is called within game message processing loop. */
    Update = 2,

    /**
     * Paint events, callback is called after the BeginPaint call during WM_PAINT processing.
     * No usages found in game. Supported in UiEvent destructor and game message loop,
     */
    Paint = 3,

    /** Game window visibility changes, callback is called when window loses or receives focus. */
    VisibilityChange = 4,

    /** Callback is called when key is pressed on the keyboard. */
    KeyPress = 5,

    /** Callback is called when mouse button is pressed. */
    MousePress = 6,

    /** Callback is called when mouse changes position. */
    MouseMove = 7,

    /** Callback is called when player attempts to close game window. */
    Close = 8,

    /**
     * Callback is called when message previously created by RegisterWindowMessage is received.
     */
    Message = 9,
};

/**
 * Used for managing various ui related events in game.
 * Identifiers are created and used solely by CUIManager.
 */
struct UiEvent
{
    SmartPtr<CUIManager> uiManager;
    UiEventType type;
    std::uint32_t messageId; /**< Used when type == UiEventType::Message. */
    int* eventId;
    int* referenceCount;
};

assert_size(UiEvent, 24);

namespace UiEventApi {

struct Api
{
    using CopyConstructor = UiEvent*(__thiscall*)(UiEvent* thisptr, const UiEvent* other);
    CopyConstructor copy;

    using Destructor = void(__thiscall*)(UiEvent* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace UiEventApi

} // namespace game

#endif // UIEVENT_H
