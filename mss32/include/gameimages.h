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

#include "smartptr.h"
#include "stringintlist.h"

namespace game {

struct CLogFile;
struct IMqImage2;
struct LRaceCategory;
struct CMidgardID;
struct IMidgardObjectMap;

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

    using GetImage = IMqImage2*(__stdcall*)(const void* storage,
                                            const char* imageName,
                                            int a3,
                                            bool randomStartingFrame,
                                            CLogFile* log);
    GetImage getImage;

    using CreateOrFreeGameImages = void(__thiscall*)(GameImagesPtr* thisptr, GameImages** data);
    CreateOrFreeGameImages createOrFreeGameImages;

    /** Searches storage for specific image names and stores them in list. */
    using GetImageNames = void(__stdcall*)(List<String>* imageNames,
                                           const void* storage,
                                           const char* imageName,
                                           int a4,
                                           int a5);
    GetImageNames getImageNames;

    /** Returns race logo image name(s) for specified race category. */
    using GetRaceLogoImageName = void(__stdcall*)(List<String>* imageNames,
                                                  void* storage,
                                                  const LRaceCategory* race);
    GetRaceLogoImageName getRaceLogoImageName;

    /** Returns race flag image to show near village entry when its controlled by specified race. */
    using GetFlagImage = IMqImage2*(__stdcall*)(const LRaceCategory* race);
    GetFlagImage getFlagImage;

    /** Returns objective circle image with specified size. */
    using GetObjectiveImage = IMqImage2*(__stdcall*)(int objectSize);
    GetObjectiveImage getObjectiveImage;

    /** Returns village riot image. */
    using GetRiotImage = IMqImage2*(__cdecl*)();
    GetRiotImage getRiotImage;

    /** Populates list with city image names and animation flag pairs. */
    using GetCityImageNames = void(__stdcall*)(StringIntList* list,
                                               const void* isoCmon,
                                               const void* isoImg,
                                               const char* cityImageName,
                                               size_t nameLength,
                                               size_t nameLength2);
    GetCityImageNames getCityImageNames;

    /** Returns village image depending on tier, shadow and animation settings. */
    using GetVillageImage = IMqImage2*(__stdcall*)(char villageTier, bool animatedIso, bool shadow);
    GetVillageImage getVillageImage;

    /**
     * Returns city lagre preview image names.
     * Large preview images are used in CCityStackInterf.
     */
    using GetCityPreviewLargeImageNames = void(__stdcall*)(List<String>* imageNames,
                                                           const void* cityFF,
                                                           const LRaceCategory* race,
                                                           int cityTier);
    GetCityPreviewLargeImageNames getCityPreviewLargeImageNames;

    /**
     * Returns city icon image names.
     * City icon images are shown in CStratInterf when selecting a city.
     */
    using GetCityIconImageNames = void(__stdcall*)(List<String>* imageNames,
                                                   const void* iconsFF,
                                                   const CMidgardID* fortificationId,
                                                   const IMidgardObjectMap* objectMap);
    GetCityIconImageNames getCityIconImageNames;

    using GetIconImageByName = IMqImage2**(__stdcall*)(IMqImage2** value, const char* name);
    GetIconImageByName getIconImageByName;
};

Api& get();

} // namespace GameImagesApi

} // namespace game

#endif // GAMEIMAGES_H
