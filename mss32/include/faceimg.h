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

#ifndef FACEIMG_H
#define FACEIMG_H

#include "mqimage2.h"

namespace game {

struct IFaceImgVftable;

struct CFaceImg
{

    struct IFaceImg : public IMqImage2T<IFaceImgVftable>
    { };

    assert_size(IFaceImg, 4);
};

/** Unknown names map to CFaceImgImpl fields. */
struct IFaceImgVftable : public IMqImage2Vftable
{
    /** Probably image index. */
    using SetInt = void(__thiscall*)(CFaceImg::IFaceImg* thisptr, int value);
    SetInt setUnknown68;

    using GetInt = int(__thiscall*)(CFaceImg::IFaceImg* thisptr);
    GetInt getUnknown68;

    SetInt setPercentHp;

    using SetBool = void(__thiscall*)(CFaceImg::IFaceImg* thisptr, bool value);
    SetBool setLeftSide;
};

assert_vftable_size(IFaceImgVftable, 13);

} // namespace game

#endif // FACEIMG_H
