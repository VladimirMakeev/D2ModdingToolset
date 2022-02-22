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

#ifndef MENUPROTOCOL_H
#define MENUPROTOCOL_H

#include "d2pair.h"
#include "d2string.h"
#include "linkedlist.h"
#include "menubase.h"
#include <guiddef.h>

namespace game {

struct ProtocolInfo
{
    bool unknown;
    char padding[3];
    Pair<size_t, void*> info;
    GUID guid;
    String name;
};

static_assert(sizeof(ProtocolInfo) == 44,
              "Size of ProtocolInfo structure must be exactly 44 bytes");

struct CMenuProtocolData
{
    LinkedList<ProtocolInfo> protocols;
};

static_assert(sizeof(CMenuProtocolData) == 16,
              "Size of CMenuProtocolData structure must be exactly 16 bytes");

/**
 * Network protocol selection screen.
 * Represents DLG_PROTOCOL from Interf.dlg.
 */
struct CMenuProtocol : public CMenuBase
{
    CMenuProtocolData* data;
};

static_assert(sizeof(CMenuProtocol) == 16,
              "Size of CMenuProtocol structure must be exactly 16 bytes");

namespace CMenuProtocolApi {

struct Api
{
    using Constructor = CMenuProtocol*(__thiscall*)(CMenuProtocol* thisptr, CMenuPhase* menuPhase);
    Constructor constructor;

    /** Shows existing network protocols. */
    using DisplayCallback = void(__thiscall*)(CMenuProtocol* thisptr,
                                              String* string,
                                              bool a3,
                                              int selectedIndex);
    DisplayCallback displayCallback;

    /** Creates IMqNetService depending on player choice and advances to a next menu screen. */
    using ButtonHandler = void(__thiscall*)(CMenuProtocol* thisptr);
    ButtonHandler continueHandler;

    /** Allocates memory and calls CMenuProtocol constructor. */
    using CreateMenu = CMenuProtocol*(__stdcall*)(CMenuPhase* menuPhase);
    CreateMenu createMenu;
};

Api& get();

} // namespace CMenuProtocolApi

} // namespace game

#endif // MENUPROTOCOL_H
