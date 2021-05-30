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

#ifndef MENULORD_H
#define MENULORD_H

#include "d2pair.h"
#include "difficultylevel.h"
#include "lordcat.h"
#include "menubase.h"
#include "mqimage2.h"

namespace game {

struct CMenuPhase;
struct LRaceCategory;

using ImagePtr = SmartPtr<IMqImage2>;
using ImagePtrPair = Pair<ImagePtr, ImagePtr>;
using ImagePtrPairs = Vector<ImagePtrPair>;

struct CMenuLordData
{
    LLordCategory lordCategory;
    LDifficultyLevel difficulty;
    int selectedFace;
    char unknown2[24];
    ImagePtrPairs lordFaces;
    int unknown3;
    int unknown4;
    int unknown5;
    char unknown6[24];
    int unknown7;
    int unknown8;
    IMqImage2* backgroundImage;
    int unknown9;
    int unknown10;
    int unknown11;
    int unknown12;
    int unknown13;
    int unknown14;
    int unknown15;
};

static_assert(sizeof(CMenuLordData) == 144,
              "Size of CMenuLordData structure must be exactly 144 bytes");

static_assert(offsetof(CMenuLordData, lordFaces) == 52,
              "CMenuLordData::lordFaces offset must be 52 bytes");

static_assert(offsetof(CMenuLordData, backgroundImage) == 112,
              "CMenuLordData::backgroundImage offset must be 112 bytes");

/**
 * Lord selection screen.
 * Represents DLG_CHOOSE_LORD from Interf.dlg.
 */
struct CMenuLord : public CMenuBase
{
    CMenuLordData* data;
};

static_assert(sizeof(CMenuLord) == 16, "Size of CMenuLord structure must be exactly 16 bytes");

namespace CMenuLordApi {

struct Api
{
    using Constructor = CMenuLord*(__thiscall*)(CMenuLord* thisptr, CMenuPhase* menuPhase);
    Constructor constructor;

    /** Returns lord face images for specified race category. */
    using GetLordFacesByRace = void(__stdcall*)(const LRaceCategory* raceCategory,
                                                ImagePtrPairs* faceImages);
    GetLordFacesByRace getLordFacesByRace;

    /** Loads lord face images using specified image names array. */
    using LoadLordFaceImages = void(__stdcall*)(const char** imageNames,
                                                size_t imagesTotal,
                                                ImagePtrPairs* faceImages);
    LoadLordFaceImages loadLordFaceImages;

    /** Returns lord animation for selected lord face. */
    using GetLordAnimation = void(__stdcall*)(const CMenuPhase* menuPhase,
                                              int selectedFace,
                                              ImagePtrPair* animations);
    GetLordAnimation getLordAnimation;

    /** Loads lord portrait transition animation by name. */
    using LoadLordAnimation = ImagePtr*(__stdcall*)(ImagePtr* animation, const char* animationName);
    LoadLordAnimation loadLordAnimation;
};

Api& get();

} // namespace CMenuLordApi

} // namespace game

#endif // MENULORD_H
