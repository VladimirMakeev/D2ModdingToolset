/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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

#ifndef DBFHEADER_H
#define DBFHEADER_H

#include <cstddef>
#include <cstdint>

namespace utils {

enum class CodePage : std::uint8_t
{
    DosUsa = 1,          /**< Code page 437 */
    DosMultilingual = 2, /**< Code page 850 */
    WinAnsi = 3,         /**< Code page 1252 */
    StandardMac = 4,
    EEMsDos = 0x64,      /**< Code page 852 */
    NordicMsDos = 0x65,  /**< Code page 865 */
    RussianMsDos = 0x66, /**< Code page 866 */
    IcelandicMsDos = 0x67,
    CzechMsDos = 0x68,
    PolishMsDos = 0x69,
    GreekMsDos = 0x6a,
    TurkishMsDos = 0x6b,
    RussianMac = 0x96,
    EEMac = 0x97,
    GreekMac = 0x98,
    WinEE = 0xc8, /**< Code page 1250 */
    RussianWin = 0xc9,
    TurkishWin = 0xca,
    GreekWin = 0xcb
};

struct DbfHeader
{
    union Version
    {
        struct
        {
            std::uint8_t version : 3;
            std::uint8_t memoFile : 1;
            std::uint8_t sqlTable : 3;
            std::uint8_t dbtFlag : 1;
        } parts;
        std::uint8_t data;
    } version;

    struct UpdateTime
    {
        std::uint8_t year; /**< Added to a base year of 1900. */
        std::uint8_t month;
        std::uint8_t day;
    } lastUpdate;

    std::uint32_t recordsTotal;
    std::uint16_t headerLength;
    std::uint16_t recordLength;

    char reserved[2];

    bool incompleteTransaction;
    bool encryption;

    char multiUserReserved[12];

    bool mdxPresent;

    CodePage language;

    char reserved2[2];
};

static_assert(sizeof(DbfHeader) == 32, "Size of DbfHeader structure must be exactly 32 bytes");

static_assert(offsetof(DbfHeader, recordsTotal) == 4,
              "DbfHeader::recordsTotal offset must be 4 bytes");
static_assert(offsetof(DbfHeader, language) == 29, "DbfHeader::language offset must be 29 bytes");

} // namespace utils

#endif // DBFHEADER_H
