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

#ifndef CAPITALDATA_H
#define CAPITALDATA_H

#include "smartptr.h"
#include "sortedcapitalracelist.h"

namespace game {

/** Holds capitals information read from Capital.dat. */
struct CapitalData
{
    CapitalDatList heretic;
    CapitalDatList human;
    CapitalDatList dwarf;
    CapitalDatList undead;
    CapitalDatList elf;
    SortedCapitalRaceList races;
};

static_assert(sizeof(CapitalData) == 108,
              "Size of CapitalData structure must be exactly 108 bytes");

using CapitalDataPtr = SmartPtr<CapitalData*>;

namespace CapitalDataApi {

struct Api
{
    /** Allocates memory for CapitalData structure and initializes races list. */
    using Allocate = CapitalData**(__thiscall*)(CapitalData** thisptr);
    Allocate allocate;

    /**
     * Returns capital data using smart pointer.
     * Creates CapitalData global object on first function call.
     */
    using GetCapitalData = CapitalDataPtr*(__stdcall*)(CapitalDataPtr* capitalData);
    GetCapitalData getCapitalData;

    /**
     * Sets smart pointer data.
     * Decrements reference counter if data is nullptr;
     */
    using CapitalDataPtrSetData = void(__thiscall*)(CapitalDataPtr* thisptr, CapitalData** data);
    CapitalDataPtrSetData capitalDataPtrSetData;

    /** Reads Capital.dat information into CapitalData. */
    using Read = void(__stdcall*)(const char* imagesFolderPath, const char* filename);
    Read read;

    /** Reads single Capital.dat entry for specified race. */
    using ReadEntry = bool(__thiscall*)(CapitalData**** thisptr,
                                        const char* configFilePath,
                                        const LRaceCategory* race);
    ReadEntry readEntry;
};

Api& get();

} // namespace CapitalDataApi

} // namespace game

#endif // CAPITALDATA_H
