/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#include "netmsgmapentryexchangeresourcesmsg.h"
#include "dynamiccast.h"
#include "exchangeresourcesmsg.h"
#include "mempool.h"
#include "netmsg.h"
#include "streambits.h"

namespace hooks {

struct CNetMsgMapEntryExchangeResources : public game::CNetMsgMapEntry_member
{ };

static void __fastcall destructor(CNetMsgMapEntryExchangeResources* thisptr,
                                  int /*%edx*/,
                                  char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static const char* __fastcall getRawName(CNetMsgMapEntryExchangeResources* thisptr, int /*%edx*/)
{
    const auto& rawName{*game::RttiApi::get().typeInfoRawName};

    return rawName(getExchangeResourcesMsgTypeDescriptor());
}

static bool __fastcall process(CNetMsgMapEntryExchangeResources* thisptr,
                               int /*%edx*/,
                               game::NetMessageHeader* header,
                               std::uint32_t idFrom,
                               std::uint32_t playerNetId)
{
    using namespace game;

    const auto& streamApi{CStreamBitsApi::get()};

    CStreamBits stream;
    streamApi.readConstructor(&stream, 0, header, header->length, false);

    CExchangeResourcesMsg message;
    message.vftable->serialize(&message, &stream);

    const bool result{thisptr->vftable->runCallback(thisptr, &message, idFrom, playerNetId)};

    streamApi.destructor(&stream);
    return result;
}

static bool __fastcall runCallback(CNetMsgMapEntryExchangeResources* thisptr,
                                   int /*%edx*/,
                                   game::CNetMsg* netMessage,
                                   std::uint32_t idFrom,
                                   std::uint32_t)
{
    return thisptr->callback(thisptr->data, netMessage, idFrom);
}

static game::TypeDescriptor* getNetMsgMapEntryExchangeResourcesTypeDescriptor()
{
    using namespace game;

    // clang-format off
    static game::TypeDescriptor descriptor{
        game::RttiApi::typeInfoVftable(),
        nullptr,
        ".?AVCNetMsgMapEntryExchangeResources@@",
    };
    // clang-format on

    return &descriptor;
}

static game::ClassHierarchyDescriptor* getHierarchyDescriptor()
{
    using namespace game;

    // clang-format off
    static game::BaseClassDescriptor baseClassDescriptor{
        getNetMsgMapEntryExchangeResourcesTypeDescriptor(),
        1u, // base class array has 1 more element after this descriptor
        game::PMD{ 0, -1, 0 },
        0u
    };

    static game::BaseClassArray baseClassArray{
        &baseClassDescriptor,
        RttiApi::rtti().CNetMsgMapEntryDescriptor
    };

    static game::ClassHierarchyDescriptor hierarchyDescriptor{
        0,
        0,
        2u,
        &baseClassArray
    };
    // clang-format on

    return &hierarchyDescriptor;
}

using MapEntryRttiInfo = game::RttiInfo<game::CNetMsgMapEntry_memberVftable>;

static MapEntryRttiInfo& getMsgRttiInfo()
{
    using namespace game;

    // clang-format off
    static const game::CompleteObjectLocator objectLocator{
        0u,
        offsetof(CNetMsgMapEntryExchangeResources, vftable),
        0u,
        getNetMsgMapEntryExchangeResourcesTypeDescriptor(),
        getHierarchyDescriptor()
    };
    // clang-format on

    static MapEntryRttiInfo rttiInfo{&objectLocator};
    return rttiInfo;
}

game::CNetMsgMapEntry_member* createNetMsgMapEntryExchangeResourcesMsg(
    game::CMidServerLogic* serverLogic,
    game::CNetMsgMapEntry_member::Callback callback)
{
    using namespace game;

    auto entry{(CNetMsgMapEntryExchangeResources*)Memory::get().allocate(
        sizeof(CNetMsgMapEntryExchangeResources))};

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;

        // Use our own vftable
        auto& vftable{getMsgRttiInfo().vftable};
        vftable.destructor = (CNetMsgMapEntry_memberVftable::Destructor)destructor;
        vftable.getName = (CNetMsgMapEntry_memberVftable::GetName)getRawName;
        vftable.process = (CNetMsgMapEntry_memberVftable::Process)process;
        vftable.runCallback = (CNetMsgMapEntry_memberVftable::RunCallback)runCallback;
    }

    entry->vftable = &getMsgRttiInfo().vftable;
    entry->data = serverLogic;
    entry->callback = callback;

    return entry;
}

} // namespace hooks
