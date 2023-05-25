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

#ifndef SOUNDSYSTEMSTREAM_H
#define SOUNDSYSTEMSTREAM_H

#include "d2list.h"
#include "uievent.h"

namespace game {

struct CSoundSystemImpl;

struct CSoundSystemStreamData
{
    SmartPtr<CSoundSystemImpl> soundSystemImpl;
    unsigned int mqCbStreamMsgId;
    UiEvent streamEvent;
    SmartPointer ptr2;
    SmartPointer ptr3;
    bool musicOn;
    char padding[3];
    int unknown3;
    int fxVolume;
    List<void*> list;
};

assert_size(CSoundSystemStreamData, 80);

struct CSoundSystemStream
{
    CSoundSystemStreamData* data;
};

assert_size(CSoundSystemStream, 4);

} // namespace game

#endif // SOUNDSYSTEMSTREAM_H
