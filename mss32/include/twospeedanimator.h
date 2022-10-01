/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef TWOSPEEDANIMATOR_H
#define TWOSPEEDANIMATOR_H

#include "d2assert.h"
#include "mqanimation.h"
#include "smartptr.h"

namespace game {

struct CMqPresentationManager;

struct CTwoSpeedAnimator : public IMqAnimation
{
    struct ITwoSpeedAnimationVftable;

    struct ITwoSpeedAnimation
    {
        ITwoSpeedAnimationVftable* vftable;
    };

    bool fastAnimation;
    char padding[3];
    SmartPtr<CMqPresentationManager> presentationMgr;
    ITwoSpeedAnimation* twoSpeedAnimation;
};

assert_size(CTwoSpeedAnimator, 20);

// Virtual table does not contain destructor
struct CTwoSpeedAnimator::ITwoSpeedAnimationVftable
{
    /** Updates animations by a single frame. */
    using Update = void(__thiscall*)(CTwoSpeedAnimator::ITwoSpeedAnimation* thisptr,
                                     bool smoothScroll);
    Update update;
};

assert_vftable_size(CTwoSpeedAnimator::ITwoSpeedAnimationVftable, 1);

} // namespace game

#endif // TWOSPEEDANIMATOR_H
