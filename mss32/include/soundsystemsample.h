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

#ifndef SOUNDSYSTEMSAMPLE_H
#define SOUNDSYSTEMSAMPLE_H

#include "d2pair.h"
#include "d2vector.h"
#include "smartptr.h"
#include "uievent.h"
#include <Windows.h>

namespace game {

struct CSoundSystemImpl;

struct SoundSampleInfo
{
    void* sampleHandle; /**< Miles sound system handle */
    void* soundData;    /**< Raw data from .wdb file */
    int soundId;
    Pair<int /* volume */, int /* pan */> volumePan;
    SmartPointer functor;
};

assert_size(SoundSampleInfo, 28);

struct CSoundSystemSampleData
{
    SmartPtr<CSoundSystemImpl> soundSystemImpl;
    std::uint32_t mqCbSampleMsgId;
    UiEvent sampleEvent;
    bool soundFx;
    char padding[3];
    int fxVolume;
    SoundSampleInfo infos[10];
    int infosCount;
    int freeId;
    Vector<char[12]> array;
    RTL_CRITICAL_SECTION criticalSection;
};

assert_size(CSoundSystemSampleData, 372);

struct CSoundSystemSample
{
    CSoundSystemSampleData* data;
};

assert_size(CSoundSystemSample, 4);

} // namespace game

#endif // SOUNDSYSTEMSAMPLE_H
