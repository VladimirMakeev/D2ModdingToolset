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

#ifndef SCENARIOHEADER_H
#define SCENARIOHEADER_H

#include "categoryids.h"
#include "d2assert.h"
#include <cstddef>
#include <cstdint>

namespace game {

struct CMidgardID;

/** Describes race in scenario file header. */
struct RaceInfo
{
    RaceId race;
    char unknown[36];
};

assert_size(RaceInfo, 40);

#pragma pack(push)
#pragma pack(1)
/**
 * Describes scenario file.
 * Structure size should be about 3156 bytes, according to header validity checks code.
 * See https://github.com/VladimirMakeev/D2SGFormat for additional info about header format.
 */
struct ScenarioFileHeader
{
    char description[256];
    char author[21];
    bool official;
    char name[256];
    int mapSize;
    DifficultyLevelId difficulty;
    int turnNumber;
    int unknown;
    char campaignId[11];
    int unknown2;
    char unknown3;
    char defaultPlayerName[256];
    RaceId race;
    char unknown4[815];
    int unknownData;
    int prngValues[250]; /**< Values used for pseudo-random number generation. */
    int paddingSize;
    int racesTotal;
    RaceInfo races[5];
    char padding[43];
    char padding2[260];
};
#pragma pack(pop)

assert_size(ScenarioFileHeader, 3156);
assert_offset(ScenarioFileHeader, author, 256);
assert_offset(ScenarioFileHeader, official, 277);
assert_offset(ScenarioFileHeader, name, 278);
assert_offset(ScenarioFileHeader, mapSize, 534);
assert_offset(ScenarioFileHeader, difficulty, 538);
assert_offset(ScenarioFileHeader, campaignId, 550);
assert_offset(ScenarioFileHeader, racesTotal, 2649);
assert_offset(ScenarioFileHeader, races, 2653);

namespace ScenarioFileHeaderApi {

struct Api
{
    /**
     * Reads scenario file and parses its header performing validity checks.
     * @param[in] a1 meaning unknown.
     * @param[in] filePath full path to scenario file (.sg).
     * @param[inout] scenarioFileId pointer parsed scenario file id from header is stored here.
     * @param[inout] header scenario file header to populate.
     * @param a5 unknown
     * @param a6 unknown
     * @param a7 unknown
     * @returns true on success, false if scenario file can't be read or header data invalid.
     */
    using ReadAndCheckHeader = bool(__stdcall*)(void* a1,
                                                const char* filePath,
                                                CMidgardID* scenarioFileId,
                                                ScenarioFileHeader* header,
                                                int a5,
                                                int a6,
                                                int a7);
    ReadAndCheckHeader readAndCheckHeader;
};

Api& get();

} // namespace ScenarioFileHeaderApi

} // namespace game

#endif // SCENARIOHEADER_H
