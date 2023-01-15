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

#ifndef MIDGARDSTREAMENV_H
#define MIDGARDSTREAMENV_H

#include "d2assert.h"
#include "smartptr.h"

namespace game {

struct IMidgardStreamEnvVftable;
struct IMidgardStreamCount;
struct IMidgardStream;
struct CMidgardID;

struct IMidgardStreamEnv
{
    IMidgardStreamEnvVftable* vftable;
};

struct IMidgardStreamEnvVftable
{
    using Destructor = void(__thiscall*)(IMidgardStreamEnv* thisptr, char flags);
    Destructor destructor;

    using Method1 = int(__thiscall*)(IMidgardStreamEnv* thisptr);
    Method1 method1;

    using GetBool = bool(__thiscall*)(const IMidgardStreamEnv* thisptr);

    GetBool writeMode;
    GetBool readMode;

    // See description of CMidServerLogicCoreData::isExpansionContent
    GetBool isExpansionContent;

    using GetError = const char*(__thiscall*)(const IMidgardStreamEnv* thisptr);
    GetError getError;

    using SetError = void(__thiscall*)(IMidgardStreamEnv* thisptr, const char* errorMessage);
    SetError setError;

    using GetStreamCount = IMidgardStreamCount*(__thiscall*)(IMidgardStreamEnv* thisptr);
    GetStreamCount getStreamCount;

    using FindStream = IMidgardStream*(__thiscall*)(IMidgardStreamEnv* thisptr,
                                                    const char* fileName,
                                                    SmartPointer* ptr);
    FindStream findStream;

    using OpenStream = void(__thiscall*)(IMidgardStreamEnv* thisptr,
                                         char* fileName,
                                         SmartPointer* ptr);
    OpenStream openStream;

    using CloseStream = void(__thiscall*)(IMidgardStreamEnv* thisptr, void* a1, void* a2, void* a3);
    CloseStream closeStream;

    using StreamObjectGuard = void(__thiscall*)(IMidgardStreamEnv* thisptr,
                                                const CMidgardID* objectId);

    StreamObjectGuard streamBeginObject;
    StreamObjectGuard streamEndObject;
};

assert_vftable_size(IMidgardStreamEnvVftable, 13);

} // namespace game

#endif // MIDGARDSTREAMENV_H
