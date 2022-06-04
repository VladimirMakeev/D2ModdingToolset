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

#ifndef CURSORIMPL_H
#define CURSORIMPL_H

#include "interfaddon.h"
#include "interfcursor.h"
#include "mqpresentationmanager.h"
#include "smartptr.h"

namespace game {

struct CInterfManagerImpl;
struct CUIManager;
struct CursorHandle;

struct CCursorImplData
{
    CInterfManagerImpl* interfManager;
    SmartPtr<CMqPresentationManager> presentationMgr;
    SmartPtr<CUIManager> uiManager;
    SmartPtr<CursorHandle> cursorHandle;
    bool unknown;
    char padding[3];
};

assert_size(CCursorImplData, 32);

struct CCursorImpl
    : public IInterfCursor
    , public IInterfAddOn
    , public CMqPresentationManager::IPresentation
{
    CCursorImplData* data;
};

assert_size(CCursorImpl, 16);

} // namespace game

#endif // CURSORIMPL_H
