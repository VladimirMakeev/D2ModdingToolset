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

#ifndef GAMEIMAGES_H
#define GAMEIMAGES_H

#include "d2list.h"
#include "d2string.h"
#include "smartptr.h"

namespace game {

struct CLogFile;
struct IMqImage2;
struct LRaceCategory;

/** Stores game images information read from .ff files. */
struct GameImages
{
    CLogFile* log;
    void* thief;
    void* lords;
    void* ground;
    void* grBorder;
    void* groundTiles;
    void* unknown;
    void* faces;
    void* batUnits;
    void* batItems;
    void* battle;
    void* isoCur;
    void* isoSpell;
    void* isoTerrn;
    void* isoCmon;
    void* isoAnim;
    void* isoStill;
    void* isoUnit;
    void* events;
    void* city;
    void* iconSpell;
    void* iconBld;
    void* iconItem;
    void* icons;
    bool animatedIso;
    char padding[3];
};

static_assert(sizeof(GameImages) == 100, "Size of GameImages structure must be exactly 100 bytes");

using GameImagesPtr = SmartPtr<GameImages*>;

namespace GameImagesApi {

struct Api
{
    using GetGameImages = GameImagesPtr*(__stdcall*)(GameImagesPtr* images);
    GetGameImages getGameImages;

    using GetImage = IMqImage2*(__stdcall*)(void* storage,
                                            const char* imageName,
                                            int a3,
                                            bool randomStartingFrame,
                                            CLogFile* log);
    GetImage getImage;

    using CreateOrFreeGameImages = void(__thiscall*)(GameImagesPtr* thisptr, GameImages** data);
    CreateOrFreeGameImages createOrFreeGameImages;

    /** Searches storage for specific image names and stores them in list. */
    using GetImageNames = void(
        __stdcall*)(List<String>* imageNames, void* storage, const char* imageName, int a4, int a5);
    GetImageNames getImageNames;

    /** Returns race logo image name(s) for specified race category. */
    using GetRaceLogoImageName = void(__stdcall*)(List<String>* imageNames,
                                                  void* storage,
                                                  const LRaceCategory* race);
    GetRaceLogoImageName getRaceLogoImageName;
};

Api& get();

} // namespace GameImagesApi

} // namespace game

#endif // GAMEIMAGES_H
