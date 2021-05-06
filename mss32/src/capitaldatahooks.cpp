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

#include "capitaldatahooks.h"
#include "capitaldata.h"
#include "log.h"
#include "mempool.h"
#include "racehooks.h"
#include "utils.h"
#include <filesystem>
#include <fmt/format.h>
#include <vector>

namespace hooks {

struct CustomCapitalData : public game::CapitalData
{
    std::vector<game::CapitalDatList*> newRacesData;
};

game::CapitalData** __fastcall allocateCapitalDataHooked(game::CapitalData** thisptr, int /*%edx*/)
{
    using namespace game;

    logDebug("capitalDat.log", "Allocate capital data c-tor hook started");

    auto customData = (CustomCapitalData*)Memory::get().allocate(sizeof(CustomCapitalData));

    logDebug("capitalDat.log", "Allocated custom capital data structure");

    {
        std::vector<game::CapitalDatList*> newRacesData(newRaces().size());
        logDebug("capitalDat.log", "Created tmp races data vector");

        customData->newRacesData = newRacesData;
        logDebug("capitalDat.log", "Created new races data vector");
    }

    const auto& listConstructor = CapitalDatListApi::get().constructor;

    listConstructor(&customData->heretic);
    listConstructor(&customData->human);
    listConstructor(&customData->dwarf);
    listConstructor(&customData->undead);
    listConstructor(&customData->elf);

    logDebug("capitalDat.log", "CapitalDatList c-tors for vanilla races called");

    for (auto& race : customData->newRacesData) {
        race = (CapitalDatList*)Memory::get().allocate(sizeof(CapitalDatList));
        logDebug("capitalDat.log", "Allocate CapitalDatList for a new race");

        listConstructor(race);
        logDebug("capitalDat.log", "CapitalDatList c-tor called for a new race");
    }

    const auto& raceList = SortedCapitalRaceListApi::get();
    raceList.constructor(&customData->races);

    logDebug("capitalDat.log", "SortedCapitalRaceList c-tors called");

    const auto& races = RaceCategories::get();

    SortedCapitalRaceListIterator iterator{};

    CapitalDatList* ptr{&customData->human};
    raceList.add(&customData->races, &iterator, races.human, &ptr);
    logDebug("capitalDat.log", "SortedCapitalRaceList added empire");

    ptr = &customData->heretic;
    raceList.add(&customData->races, &iterator, races.heretic, &ptr);
    logDebug("capitalDat.log", "SortedCapitalRaceList added legions");

    ptr = &customData->dwarf;
    raceList.add(&customData->races, &iterator, races.dwarf, &ptr);
    logDebug("capitalDat.log", "SortedCapitalRaceList added clans");

    ptr = &customData->undead;
    raceList.add(&customData->races, &iterator, races.undead, &ptr);
    logDebug("capitalDat.log", "SortedCapitalRaceList added undead");

    ptr = &customData->elf;
    raceList.add(&customData->races, &iterator, races.elf, &ptr);
    logDebug("capitalDat.log", "SortedCapitalRaceList added elves");

    for (size_t i = 0; i < customData->newRacesData.size(); ++i) {
        ptr = customData->newRacesData[i];
        raceList.add(&customData->races, &iterator, &newRaces()[i].category, &ptr);
        logDebug("capitalDat.log", "SortedCapitalRaceList added new race");
    }

    *thisptr = customData;

    logDebug("capitalDat.log", "Allocate capital data c-tor hook finished");
    return thisptr;
}

void __stdcall readCapitalDataHooked(const char* imagesFolderPath, const char* filename)
{
    using namespace game;

    const std::filesystem::path filePath{std::filesystem::path{imagesFolderPath} / filename};

    const auto& capitalData = CapitalDataApi::get();

    CapitalDataPtr ptr{};
    capitalData.getCapitalData(&ptr);

    const auto& races = RaceCategories::get();
    CapitalData*** dataPtr = &ptr.data;

    if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), races.human)) {
        showErrorMessageBox(fmt::format("Failed to read Capital.dat entry for empire"));
    }

    if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), races.heretic)) {
        showErrorMessageBox(
            fmt::format("Failed to read Capital.dat entry for legions of the damned"));
    }

    if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), races.dwarf)) {
        showErrorMessageBox(fmt::format("Failed to read Capital.dat entry for mountain clans"));
    }

    if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), races.undead)) {
        showErrorMessageBox(fmt::format("Failed to read Capital.dat entry for undead hordes"));
    }

    if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), races.elf)) {
        showErrorMessageBox(fmt::format("Failed to read Capital.dat entry for elves"));
    }

    for (const auto& race : newRaces()) {
        if (!capitalData.readEntry(&dataPtr, filePath.string().c_str(), &race.category)) {
            showErrorMessageBox(fmt::format("Failed to read Capital.dat entry for new race"));
        }
    }

    capitalData.capitalDataPtrSetData(&ptr, nullptr);
}

} // namespace hooks
