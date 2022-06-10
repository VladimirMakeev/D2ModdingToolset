/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef BATBIGFACE_H
#define BATBIGFACE_H

#include "battlemsgdata.h"
#include "d2map.h"
#include "image2text.h"
#include "mqimage2.h"
#include "mqrect.h"
#include "smartptr.h"

namespace game {

struct IMidgardObjectMap;
struct CBatImagesLoader;

struct CBatBigFaceUnitData
{
    CMidgardID unitImplId;
    bool showItems;
    char padding[3];
    SmartPtr<CImage2Text> textImage;
    CMidgardID itemIds[2];
    /** Might contain additional members. */
};

struct CBatBigFaceUnitImplData
{
    SmartPtr<IMqImage2> faceImage;
    /** Might contain additional members. */
};

struct CBatBigFaceItemData
{
    SmartPtr<IMqImage2> itemImage;
    /** Might contain additional members. */
};

struct CBatBigFaceData
{
    IMidgardObjectMap* objectMap;
    bool flippedBattle;
    char padding[3];
    CBatImagesLoader* imagesLoader;
    CMqPoint itemBgndImagePos;
    SmartPointer itemBgndImage;
    CMqRect bigFaceImgArea;
    CMqRect bigFaceTextArea;
    CMqRect itemImgAreas[2];
    int unknown;
    BattleMsgData battleMsgData;
    CMidgardID unitId;
    Map<CMidgardID, CBatBigFaceUnitData> unitData;         // Key is unit id
    Map<CMidgardID, CBatBigFaceUnitImplData> unitImplData; // Key is unit impl id
    Map<CMidgardID, CBatBigFaceItemData> itemData;         // Key is item id
    bool unknown2;
    char padding2[3];
    char unknown3[4];
};

assert_size(CBatBigFaceData, 4112);

struct CBatBigFace : public IMqImage2
{
    CBatBigFaceData* data;
};

assert_size(CBatBigFace, 8);

namespace BatBigFaceApi {

struct Api
{
    using SetUnitId = void(__thiscall*)(CBatBigFace* thisptr, const CMidgardID* unitId);
    SetUnitId setUnitId;

    using SetUnknown = void(__thiscall*)(CBatBigFace* thisptr, bool a2);
    SetUnknown setUnknown;

    using Update = void(__thiscall*)(CBatBigFace* thisptr, const BattleMsgData* battleMsgData);
    Update update;

    using UnitDataMapErase = void(__thiscall*)(Map<CMidgardID, CBatBigFaceUnitData>* thisptr,
                                               MapIterator<CMidgardID, CBatBigFaceUnitData> pos);
    UnitDataMapErase unitDataMapErase;

    using UnitDataMapAccess = CBatBigFaceUnitData*(
        __thiscall*)(Map<CMidgardID, CBatBigFaceUnitData>* thisptr, const CMidgardID* key);
    UnitDataMapAccess unitDataMapAccess;

    using UnitImplDataMapAccess = CBatBigFaceUnitImplData*(
        __thiscall*)(Map<CMidgardID, CBatBigFaceUnitImplData>* thisptr, const CMidgardID* key);
    UnitImplDataMapAccess unitImplDataMapAccess;

    using ItemDataMapAccess = CBatBigFaceItemData*(
        __thiscall*)(Map<CMidgardID, CBatBigFaceItemData>* thisptr, const CMidgardID* key);
    ItemDataMapAccess itemDataMapAccess;
};

Api& get();

} // namespace BatBigFaceApi

} // namespace game

#endif // BATBIGFACE_H
