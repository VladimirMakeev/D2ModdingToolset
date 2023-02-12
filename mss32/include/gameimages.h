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

#include "d2assert.h"
#include "mqdb.h"
#include "smartptr.h"
#include "stringintlist.h"

namespace game {

struct CLogFile;
struct IMqImage2;
struct LRaceCategory;
struct CMidgardID;
struct IMidgardObjectMap;
struct MQDBDataWrapper;
struct GameImageDataWrapper;

/** Stores game images information read from .ff files. */
struct GameImages
{
    CLogFile* log;
    GameImageDataWrapper* thief;
    GameImageDataWrapper* lords;
    MQDBDataWrapper* ground;
    MQDBDataWrapper* grBorder;
    MQDBRecordMap** groundTiles;
    MQDBRecordMap** borderImages;
    MQDBDataWrapper* faces;
    GameImageDataWrapper* batUnits;
    GameImageDataWrapper* batItems;
    GameImageDataWrapper* battle;
    GameImageDataWrapper* isoCursr;
    GameImageDataWrapper* isoSpell;
    GameImageDataWrapper* isoTerrn;
    GameImageDataWrapper* isoCmon;
    GameImageDataWrapper* isoAnim;
    GameImageDataWrapper* isoStill;
    GameImageDataWrapper* isoUnit;
    GameImageDataWrapper* events;
    GameImageDataWrapper* city;
    GameImageDataWrapper* iconSpel;
    GameImageDataWrapper* iconBld;
    GameImageDataWrapper* iconItem;
    GameImageDataWrapper* icons;
    bool animatedIso;
    char padding[3];
};

assert_size(GameImages, 100);

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

    /** Returns icon image from Icons.ff by its name. */
    using GetIconImageByName = IMqImage2**(__stdcall*)(IMqImage2** value, const char* name);
    GetIconImageByName getIconImageByName;

    /** Returns unit portrait from Events.ff. */
    using GetUnitIcon = IMqImage2*(__stdcall*)(const CMidgardID* unitImplId);
    GetUnitIcon getUnitIcon;
};

Api& get();

} // namespace GameImagesApi

} // namespace game

#endif // GAMEIMAGES_H
