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

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "d2list.h"
#include "smartptr.h"
#include <cstdint>

namespace game {

struct IMqAnimation;
struct CUIManager;

struct AnimationLists
{
    SmartPtr<CUIManager> uiManagerPtr;
    List<IMqAnimation*> animationList;
    ListIterator<IMqAnimation*> iterator;
    List<IMqAnimation*> deferredAnimations;
    std::uint32_t updateInterval;
    std::uint32_t nextUpdateTime;
    std::uint32_t updatesCount;
    bool updateInProcess;
    char padding[3];
};

static_assert(sizeof(AnimationLists) == 68,
              "Size of AnimationLists structure must be exactly 68 bytes");

struct AnimationsData
{
    AnimationLists* slowAnimations; /**< Update interval 66ms, hence 'slow'. */
    AnimationLists* fastAnimations; /**< Update interval 33ms, fast. */
};

static_assert(sizeof(AnimationsData) == 8,
              "Size of AnimationsData structure must be exactly 8 bytes");

struct Animations
{
    AnimationsData* data;
};

static_assert(sizeof(Animations) == 4, "Size of Animations structure must be exactly 4 bytes");

} // namespace game

#endif // ANIMATIONS_H
