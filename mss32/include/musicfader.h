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

#ifndef MUSICFADER_H
#define MUSICFADER_H

#include "smartptr.h"
#include "uievent.h"
#include <cstddef>

namespace game {

struct CMusicFaderData
{
    SmartPointer unknownPtr;
    UiEvent uiEvent;
    char unknown[12];
    SmartPointer functor;
    char unknown2[24];
};

assert_size(CMusicFaderData, 76);
assert_offset(CMusicFaderData, uiEvent, 8);
assert_offset(CMusicFaderData, functor, 44);

struct CMusicFader
{
    const void* vftable;
    CMusicFaderData* data;
};

assert_size(CMusicFader, 8);

namespace CMusicFaderApi {

struct Api
{
    /** Gets singleton object. */
    using Get = CMusicFader*(__stdcall*)();
    Get get;

    /** Checks that uiEvent::eventId is not null. */
    using HasEventId = bool(__thiscall*)(CMusicFader* thisptr);
    HasEventId hasEventId;

    using Callback = bool(__thiscall*)(CMusicFader* thisptr);
    Callback callback;
};

Api& get();

} // namespace CMusicFaderApi

} // namespace game

#endif // MUSICFADER_H
