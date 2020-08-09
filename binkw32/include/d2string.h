/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef D2STRING_H
#define D2STRING_H

#include <cstdint>

namespace game {

struct String
{
    std::uint32_t length;
    std::uint32_t lengthAllocated;
    char* string;
    void* allocator;
};

static_assert(sizeof(String) == 16, "String structure must be exactly 16 bytes");

namespace StringApi {

struct Api
{
    /**
     * Initializes existing String using C-style string.
     * Allocates memory and copies string data.
     * @param[in] thisptr pointer to already existing String.
     * @param[in] string pointer to C-style string to copy.
     * @returns thisptr.
     */
    using InitFromString = String*(__thiscall*)(String* thisptr, const char* string);
    InitFromString initFromString;

    /**
     * Initializes existing String using C-style string and length.
     * Allocates memory and copies length characters of string.
     * @param[in] thisptr pointer to already existing String.
     * @param[in] string pointer to C-style string to copy.
     * @param length number of characters to copy.
     * @returns thisptr.
     */
    using InitFromStringN = String*(__thiscall*)(String* thisptr,
                                                 const char* string,
                                                 std::uint32_t length);
    InitFromStringN initFromStringN;

    /** Frees memory allocated for thisptr->string. */
    using Free = int(__thiscall*)(String* thisptr);
    Free free;

    /**
     * Appends string of specified length.
     * @param[in] thisptr pointer to existing String to append to.
     * @param[in] string pointer to C-style string to append.
     * @param length number of characters to append.
     * @returns thisptr->string.
     */
    using Append = char*(__thiscall*)(String* thisptr, const char* string, std::uint32_t length);
    Append append;

    /**
     * Appends count number of characters ch to String.
     * @param[in] thisptr pointer to existing String to append to.
     * @param ch character to append.
     * @param count number of times to append.
     * @returns thisptr->string.
     */
    using AppendChar = char*(__thiscall*)(String* thisptr, char ch, std::uint32_t count);
    AppendChar appendChar;
};

/** Returns String functions according to determined version of the game. */
Api& get();

} // namespace StringApi

} // namespace game

#endif // D2STRING_H
