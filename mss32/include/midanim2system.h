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

#ifndef MIDANIM2SYSTEM_H
#define MIDANIM2SYSTEM_H

#include "midcommandqueue2.h"
#include "twospeedanimator.h"

namespace game {

struct IMidgardObjectMap;
struct IMidAnim2;

using IMidAnim2Ptr = SmartPtr<IMidAnim2>;

struct CMidAnim2System
{
    struct Impl
        : public CTwoSpeedAnimator::ITwoSpeedAnimation
        , public CMidCommandQueue2::INotifyCQ
    {
        List<IMidAnim2Ptr> animations;
        ListIterator<IMidAnim2Ptr> currentAnimation;
        CMidgardID playerId;
        IdList playerIdList;
        CMidgardID playerIdCopy;
        CTwoSpeedAnimator* slowAnimator;
        CTwoSpeedAnimator* fastAnimator;
        CMidCommandQueue2* commandQueue;
        IMidgardObjectMap* objectMap;
        bool unknown9;
        bool unknown10;
        char padding2[2];
        CMidAnim2System* animSystem;
    };

    CMidAnim2System::Impl* impl;
};

assert_size(CMidAnim2System::Impl, 84);
assert_size(CMidAnim2System, 4);

} // namespace game

#endif // MIDANIM2SYSTEM_H
