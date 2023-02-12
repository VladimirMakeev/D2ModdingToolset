/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef BORDEREDIMG_H
#define BORDEREDIMG_H

#include "d2vector.h"
#include "mqimage2.h"
#include "mqpoint.h"

namespace game {

struct CBorderedImgData;

struct CBorderedImg : public IMqImage2
{
    CBorderedImgData* data;
};

assert_size(CBorderedImg, 8);

struct CBorderedImgData
{
    CMqPoint borderSize;
    Vector<IMqImage2*> images;
};

assert_size(CBorderedImgData, 24);

/** Corresponds to border image from Icons.ff. */
enum class BorderType : int
{
    None = 0,
    UnitSmall, // BORDERUNITSMALL
    UnitLarge, // BORDERUNITLARGE
    LordSmall, // BORDERLORDSMALL
    LordLarge, // BORDERLORDLARGE
    Item,      // BORDERITEM
};

namespace CBorderedImgApi {

struct Api
{
    using Constructor = CBorderedImg*(__thiscall*)(CBorderedImg* thisptr, BorderType borderType);
    Constructor constructor;

    using AddImage = void(__thiscall*)(CBorderedImg* thisptr, IMqImage2* image);
    AddImage addImage;
};

Api& get();

} // namespace CBorderedImgApi

} // namespace game

#endif // BORDEREDIMG_H
