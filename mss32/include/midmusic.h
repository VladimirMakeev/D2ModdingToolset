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

#ifndef MIDMUSIC_H
#define MIDMUSIC_H

#include "d2string.h"
#include "racecategory.h"
#include "smartptr.h"

namespace game {

struct CLogFile;
struct CSoundSystemStream;

struct CMidMusicData
{
    char musicFolder[260];
    char customMusicFolder[260];
    CLogFile* logFile;
    int unknown;
    SmartPtr<CSoundSystemStream> soundSystemStream;
    int unknown2;
    int unknown3;
    LRaceCategory raceCategory;
    String string;
    String string2;
    String string3;
    String string4;
    char unknown4[12];
};

static_assert(sizeof(CMidMusicData) == 632,
              "Size of CMidMusicData structure must be exactly 632 bytes");

struct CMidMusic
{
    CMidMusicData* data;
};

static_assert(sizeof(CMidMusic) == 4, "Size of CMidMusic structure must be exactly 4 bytes");

namespace CMidMusicApi {

struct Api
{
    /**
     * Frees memory allocated for strings in CMidMusicData.
     * Actual meaning unknown.
     */
    using FreeStrings = void(__thiscall*)(CMidMusic* thisptr);
    FreeStrings freeStrings;

    /**
     * Plays specified wav or mp3 file.
     * @param[in] thisptr pointer to music object.
     * @param[in] folder string containing path to folder with file.
     * @param[in] fileName name of file without extension to play.
     * @param a4 unknown.
     * @param mp3 if set to true '.mp3' used as file extension, otherwise '.wav'.
     */
    using PlayFile = void(__thiscall*)(CMidMusic* thisptr,
                                       const char* folder,
                                       const char* fileName,
                                       int a4,
                                       bool mp3);
    PlayFile playFile;

    /** Used to switch between music tracks in battle. */
    using PlayBattleTrack = void(__thiscall*)(CMidMusic* thisptr);
    PlayBattleTrack playBattleTrack;

    /** Used to switch between music tracks in capital cities. */
    using PlayCapitalTrack = void(__thiscall*)(CMidMusic* thisptr,
                                               const LRaceCategory* raceCategory);
    PlayCapitalTrack playCapitalTrack;
};

Api& get();

} // namespace CMidMusicApi

} // namespace game

#endif // MIDMUSIC_H
