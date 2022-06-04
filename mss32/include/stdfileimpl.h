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

#ifndef STDFILEIMPL_H
#define STDFILEIMPL_H

#include "d2assert.h"

namespace game {

struct IStdFileImplVftable;

/**
 * Interface for low-level data streaming.
 * Derived types used for streaming into memory buffer (CStdFileBuffer) or file (CStdFileHandle).
 */
struct IStdFileImpl
{
    IStdFileImplVftable* vftable;
};

struct IStdFileImplVftable
{
    using Destructor = void(__thiscall*)(IStdFileImpl* thisptr, char flags);
    Destructor destructor;

    /** Returns true if stream is opened and ready. */
    using GetBool = bool(__thiscall*)(const IStdFileImpl* thisptr);
    GetBool isOpened;

    /** Closes the stream. */
    using Close = void(__thiscall*)(IStdFileImpl* thisptr);
    Close close;

    /** Sets stream position. */
    using SetFilePointer = void(__thiscall*)(IStdFileImpl* thisptr,
                                             unsigned int distance,
                                             unsigned int moveMethod);
    SetFilePointer setFilePointer;

    using GetUint = unsigned int(__thiscall*)(const IStdFileImpl* thisptr);

    /** Returns current stream position. */
    GetUint getFilePointer;

    /** Returns size of file or buffer. */
    GetUint getFileSize;

    /** Returns true if stream position points at the end. */
    GetBool isFilePointerAtEnd;

    /**
     * Writes specified number of bytes from buffer into stream.
     * @returns number of bytes written.
     */
    using Write = unsigned int(__thiscall*)(IStdFileImpl* thisptr,
                                            const void* buffer,
                                            unsigned int numBytes);
    Write write;

    /**
     * Reads stream contents into buffer with specified size.
     * @returns number of bytes read.
     */
    using Read = unsigned int(__thiscall*)(IStdFileImpl* thisptr,
                                           void* buffer,
                                           unsigned int numBytes);
    Read read;

    /**
     * Writes specified string, adds '\r' symbols after each '\n' if needed.
     * @returns number of bytes written.
     */
    using WriteString = unsigned int(__thiscall*)(IStdFileImpl* thisptr, const char* string);
    WriteString writeString;

    /**
     * Reads string contents into bufer with specified size.
     * @returns number of bytes read.
     */
    using ReadString = unsigned int(__thiscall*)(IStdFileImpl* thisptr,
                                                 char* string,
                                                 unsigned int numBytes);
    ReadString readString;

    /**
     * Returns stream handle.
     * Returns nullptr for CStdFileBuffer, Win32 HANDLE for CStdFileHandle.
     */
    using GetHandle = void*(__thiscall*)(IStdFileImpl* thisptr);
    GetHandle getHandle;
};

assert_vftable_size(IStdFileImplVftable, 12);

} // namespace game

#endif // STDFILEIMPL_H
