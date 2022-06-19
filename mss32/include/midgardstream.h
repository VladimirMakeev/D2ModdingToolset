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

#ifndef MIDGARDSTREAM_H
#define MIDGARDSTREAM_H

#include "d2assert.h"

namespace game {

struct IMidgardStreamVftable;
struct IMidgardStreamEnv;
struct CMidgardID;
struct Bank;
struct StringAndId;

/**
 * Interface for various stream classes.
 * Child classes are used to read and write scenario objects from files (CMidStreamFile),
 * dbf tables (CMidStreamDB) and network (CMidStreamNet).
 */
struct IMidgardStream
{
    IMidgardStreamVftable* vftable;
};

struct IMidgardStreamVftable
{
    using Destructor = void(__thiscall*)(IMidgardStream* thisptr, char flags);
    Destructor destructor;

    /** Returns pointer to underlying IMidgardStreamEnv object. */
    using GetStreamEnv = IMidgardStreamEnv*(__thiscall*)(IMidgardStream* thisptr);
    GetStreamEnv getStreamEnv;

    using GetBool = bool(__thiscall*)(IMidgardStream* thisptr);

    /**
     * Returns true if stream opened for writing.
     * In write mode stream writes existing scenario objects data.
     */
    GetBool writeMode;

    /**
     * Returns true if stream opened for reading.
     * In read mode stream reads data into scenario objects.
     */
    GetBool readMode;

    /** Returns error message string. */
    using GetError = const char*(__thiscall*)(IMidgardStream* thisptr);
    GetError getError;

    /** Sets error message. */
    using SetError = void(__thiscall*)(IMidgardStream* thisptr, const char* message);
    SetError setError;

    using CheckRecord = void(__thiscall*)(IMidgardStream* thisptr);

    /**
     * Tries to enter record.
     * Throws an exception if already in a record.
     */
    CheckRecord enterRecord;

    /**
     * Tries to leave record.
     * Throws an exception if stream is not in a record.
     */
    CheckRecord leaveRecord;

    /** Returns true if data with specified name could be read from stream. */
    using HasValue = bool(__thiscall*)(IMidgardStream* thisptr, const char* name);
    HasValue hasValue;

    template <typename T>
    using StreamData = void(__thiscall*)(IMidgardStream* thisptr, const char* name, T* data);

    /** Streams arrays of characters. */
    StreamData<char> streamString;

    /** Streams 4 byte integer value. */
    StreamData<int> streamInt;

    /** Streams byte value. */
    StreamData<unsigned char> streamByte;

    /** Streams boolean value */
    StreamData<bool> streamBool;

    /**
     * Streams array of bytes.
     * @param[in] thisptr pointer to stream object.
     * @param[in] name name of the array to stream.
     * @param[inout] data pointer to array of bytes to stream.
     * @param[inout] byteCount number of bytes to stream.
     */
    using StreamBinary = void(__thiscall*)(IMidgardStream* thisptr,
                                           const char* name,
                                           unsigned char* data,
                                           int* byteCount);
    StreamBinary streamBinary;

    /** Streams identifier. */
    StreamData<CMidgardID> streamId;

    /** Streams ingame currency. */
    StreamData<Bank> streamCurrency;

    /** Streams text. */
    using StreamText = void(__thiscall*)(IMidgardStream* thisptr,
                                         const char* name,
                                         StringAndId* data,
                                         int unknown);
    StreamText streamText;
};

assert_vftable_size(IMidgardStreamVftable, 17);

} // namespace game

#endif // MIDGARDSTREAM_H
