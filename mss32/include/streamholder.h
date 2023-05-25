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

#ifndef STREAMHOLDER_H
#define STREAMHOLDER_H

#include "uievent.h"

namespace game {

struct CSoundSystemStream;
struct CSoundSystemImpl;

struct CStreamHolderData
{
    SmartPtr<CSoundSystemStream> soundSystemStream;
    SmartPtr<CSoundSystemImpl> soundSystemImpl;
    unsigned int holderFadeMsgId;
    UiEvent holderFadeEvent;
    UiEvent updateEvent;
    int nextTime;
    int maybeDuration;
    char* filePath;
    char* filePath2;
    int currentStreamPlaybackPos;
    void* streamHandle; /**< Actual type is HSTREAM from Miles Sound system. */
    bool unknown6;
    char padding[3];
};

assert_size(CStreamHolderData, 96);

struct CStreamHolder
{
    CStreamHolderData* data;
};

assert_size(CStreamHolder, 4);

} // namespace game

#endif // STREAMHOLDER_H
