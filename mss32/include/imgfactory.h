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

#ifndef IMGFACTORY_H
#define IMGFACTORY_H

#include "mqimage2.h"

namespace game {

struct CImgFactory
{
    struct Image
    {
        struct Vftable : public IMqImage2Vftable
        {
            using GetImage = IMqImage2*(__thiscall*)(const Image* thisptr,
                                                     std::uint32_t imageIndex);
            GetImage getImage;

            using IsUnknownReset = bool(__thiscall*)(const Image* thisptr);
            IsUnknownReset isUnknownReset;
        };

        Vftable* vftable;
    };
};

static_assert(sizeof(CImgFactory::Image::Vftable) == 11 * sizeof(void*),
              "CImgFactory::Image vftable must have exactly 11 methods");

} // namespace game

#endif // IMGFACTORY_H
