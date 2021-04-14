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

#ifndef IMAGE2TEXT_H
#define IMAGE2TEXT_H

#include "d2string.h"
#include "mqimage2surface16.h"

namespace game {

/** Renders text string as an image. */
struct CImage2Text : public CMqImage2Surface16
{
    String text;
    int unk;
    int unk2;
    bool unk3;
    char pad[3];
    int unk4;
    int unk5;
    CMqPoint size2;
};

static_assert(sizeof(CImage2Text) == 88, "Size of CImage2Text structure must be exactly 88 bytes");

namespace CImage2TextApi {

struct Api
{
    using Constructor = CImage2Text*(__thiscall*)(CImage2Text* thisptr, int width, int height);
    Constructor constructor;

    using SetText = void(__thiscall*)(CImage2Text* thisptr, const char* text);
    SetText setText;
};

Api& get();

} // namespace CImage2TextApi

} // namespace game

#endif // IMAGE2TEXT_H
