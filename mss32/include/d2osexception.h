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

#ifndef D2OSEXCEPTION_H
#define D2OSEXCEPTION_H

namespace game {

/** Base class for all exceptions. */
struct os_exception
{
    void* vftable;
    const char* message;
};

namespace os_exceptionApi {

struct Api
{
    /** Passes its arguments to c++ standard library function that handles exception throw. */
    using ThrowException = void(__stdcall*)(const os_exception* thisptr, const void* throwInfo);
    ThrowException throwException;
};

Api& get();

} // namespace os_exceptionApi

} // namespace game

#endif // D2OSEXCEPTION_H
