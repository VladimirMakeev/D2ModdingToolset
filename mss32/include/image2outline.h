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

#ifndef IMAGE2OUTLINE_H
#define IMAGE2OUTLINE_H

#include "d2color.h"
#include "mqimage2surface16.h"

namespace game {

/** Renders thin rectangle outline. */
struct CImage2Outline : public CMqImage2Surface16
{
    Color color;
    CMqPoint size2;
};

assert_size(CImage2Outline, 56);

namespace CImage2OutlineApi {

struct Api
{
    using Constructor = CImage2Outline*(__thiscall*)(CImage2Outline* thisptr,
                                                     const CMqPoint* size,
                                                     const Color* color,
                                                     std::uint32_t opacity);
    Constructor constructor;
};

Api& get();

} // namespace CImage2OutlineApi

} // namespace game

#endif // IMAGE2OUTLINE_H
