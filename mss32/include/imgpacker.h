/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef IMGPACKER_H
#define IMGPACKER_H

#include "d2string.h"
#include "d2vector.h"

namespace game {

struct CImgPacker
{
    struct PackerLoaderVftable;

    struct PackerLoader
    {
        PackerLoaderVftable* vftable;
    };
};

struct CImgPacker::PackerLoaderVftable
{
    using Destructor = void(__thiscall*)(CImgPacker::PackerLoader* thisptr, char flags);
    Destructor destructor;

    using Method1 = Vector<String>*(__thiscall*)(CImgPacker::PackerLoader* thisptr);
    Method1 method1;
};

assert_vftable_size(CImgPacker::PackerLoaderVftable, 2);

struct CImgPackerLoaderWeakDup : public CImgPacker::PackerLoader
{
    CImgPacker::PackerLoader* packerLoader;
};

assert_size(CImgPackerLoaderWeakDup, 8);

} // namespace game

#endif // IMGPACKER_H
