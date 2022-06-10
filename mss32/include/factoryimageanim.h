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

#ifndef FACTORYIMAGEANIM_H
#define FACTORYIMAGEANIM_H

#include "d2vector.h"
#include "imgfactory.h"
#include "mqanimation.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CMqPresentationManager;

struct FactoryImageAnim
    : public CImgFactory::Image
    , public IMqAnimation
{
    SmartPtr<CMqPresentationManager> presentationMgr;
    Vector<IMqImage2*> frames;
    int currentFrame;
    char unknown;
    char padding[3];
};

assert_size(FactoryImageAnim, 40);
assert_offset(FactoryImageAnim, presentationMgr, 8);

} // namespace game

#endif // FACTORYIMAGEANIM_H
