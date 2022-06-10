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

#ifndef MQPRESENTATIONMANAGER_H
#define MQPRESENTATIONMANAGER_H

#include "d2list.h"
#include "d2pair.h"
#include "uievent.h"

namespace game {

struct CRendererImpl;
struct CLogFile;
struct IMqRenderer2;
struct CMqPresentationManagerData;

struct CMqPresentationManager
{
    struct IPresentationVftable;

    struct IPresentation
    {
        IPresentationVftable* vftable;
    };

    CMqPresentationManagerData* data;
};

assert_size(CMqPresentationManager, 4);

struct CMqPresentationManagerData
{
    UiEvent updateEvent;
    UiEvent visibilityEvent;
    CRendererImpl* renderer;
    /**
     * Assumption: presentations are drawn from highest layer value to lowest.
     * Thus CMqFps with layer -1 is drawn above all.
     */
    List<Pair<CMqPresentationManager::IPresentation*, int /* layer */>> presentationList;
    bool renderingFrame;
    bool unknown17;
    char padding[2];
    CLogFile* logFile;
};

assert_size(CMqPresentationManagerData, 76);

struct CMqPresentationManager::IPresentationVftable
{
    using Destructor = void(__thiscall*)(CMqPresentationManager::IPresentation* thisptr,
                                         char flags);
    Destructor destructor;

    using Draw = void(__thiscall*)(CMqPresentationManager::IPresentation* thisptr,
                                   IMqRenderer2* renderer);
    Draw draw;
};

assert_vftable_size(CMqPresentationManager::IPresentationVftable, 2);

} // namespace game

#endif // MQPRESENTATIONMANAGER_H
