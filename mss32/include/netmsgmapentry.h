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

#ifndef NETMSGMAPENTRY_H
#define NETMSGMAPENTRY_H

#include "d2assert.h"
#include <cstdint>

namespace game {

struct NetMessageHeader;
struct CNetMsg;
struct CNetMsgMapEntryVftable;
struct CMenusAnsInfoMsg;
struct CGameVersionMsg;

template <typename T>
struct CNetMsgMapEntryT
{
    T* vftable;
};

/** Base class for net message map entries. */
struct CNetMsgMapEntry : public CNetMsgMapEntryT<CNetMsgMapEntryVftable>
{ };

struct CNetMsgMapEntryVftable
{
    using Destructor = void(__thiscall*)(CNetMsgMapEntry* thisptr, char flags);
    Destructor destructor;

    /** Returns CNetMsg raw name. */
    using GetName = const char*(__thiscall*)(const CNetMsgMapEntry* thisptr);
    GetName getName;

    /** Serializes message from header, runs callback. */
    using Process = bool(__thiscall*)(CNetMsgMapEntry* thisptr,
                                      NetMessageHeader* header,
                                      std::uint32_t idFrom,
                                      std::uint32_t playerNetId);
    Process process;
};

struct CNetMsgMapEntry_memberVftable;

/**
 * Net message map entry.
 * Runs callback upon receiving net message of specific type.
 * Actual class uses templates for specifying messages and callbacks.
 */
struct CNetMsgMapEntry_member : public CNetMsgMapEntryT<CNetMsgMapEntry_memberVftable>
{
    void* data;
    bool(__thiscall* callback)(void* thisptr, CNetMsg* netMessage, std::uint32_t idFrom);
};

assert_size(CNetMsgMapEntry_member, 12);

struct CNetMsgMapEntry_memberVftable : public CNetMsgMapEntryVftable
{
    /**
     * Runs callback with CNetMsg being cast to appropriate type.
     * Ignores playerNetId parameter.
     */
    using RunCallback = bool(__thiscall*)(CNetMsgMapEntry_member* thisptr,
                                          CNetMsg* netMessage,
                                          std::uint32_t idFrom,
                                          std::uint32_t playerNetId);
    RunCallback runCallback;
};

struct CNetMsgMapEntryWReceiver_memberVftable;

/**
 * Net message map entry that passes receiver player net id to callback.
 * Runs callback upon receiving net message of specific type.
 * Actual class uses templates for specifying messages and callbacks.
 */
struct CNetMsgMapEntryWReceiver_member
    : public CNetMsgMapEntryT<CNetMsgMapEntryWReceiver_memberVftable>
{
    void* data;
    bool(__thiscall* callback)(void* thisptr,
                               CNetMsg* netMessage,
                               std::uint32_t idFrom,
                               std::uint32_t playerNetId);
    int unknown;
};

assert_size(CNetMsgMapEntryWReceiver_member, 16);

struct CNetMsgMapEntryWReceiver_memberVftable : public CNetMsgMapEntryVftable
{
    using RunCallback = bool(__thiscall*)(CNetMsgMapEntry_member* thisptr,
                                          CNetMsg* netMessage,
                                          std::uint32_t idFrom,
                                          std::uint32_t playerNetId);
    RunCallback runCallback;
};

namespace CNetMsgMapEntryApi {

struct Api
{
    template <typename T>
    using EntryCallback = bool(__thiscall*)(void* thisptr, const T* message, std::uint32_t idFrom);

    using MenusAnsInfoCallback = EntryCallback<CMenusAnsInfoMsg>;
    using GameVersionCallback = EntryCallback<CGameVersionMsg>;

    template <typename T>
    using AllocateEntry = CNetMsgMapEntry*(__stdcall*)(void* userData, T callback);

    AllocateEntry<MenusAnsInfoCallback> allocateMenusAnsInfoEntry;
    AllocateEntry<GameVersionCallback> allocateGameVersionEntry;
};

Api& get();

} // namespace CNetMsgMapEntryApi

} // namespace game

#endif // NETMSGMAPENTRY_H
