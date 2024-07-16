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

#include "exchangeresourcesmsg.h"
#include "dynamiccast.h"
#include "mempool.h"
#include "midclient.h"
#include "midclientcore.h"
#include "midgard.h"
#include "midobjectlock.h"
#include "phasegame.h"
#include "streambits.h"

namespace hooks {

static void __fastcall exchangeResourcesMsgDestructor(CExchangeResourcesMsg* thisptr,
                                                      int /*%edx*/,
                                                      char flags);

static void __fastcall exchangeResourcesMsgSerialize(CExchangeResourcesMsg* thisptr,
                                                     int /*%edx*/,
                                                     game::CStreamBits* stream);

static game::ClassHierarchyDescriptor* getMsgHierarchyDescriptor()
{
    using namespace game;

    // clang-format off
    static game::BaseClassDescriptor baseClassDescriptor{
        getExchangeResourcesMsgTypeDescriptor(),
        1u, // base class array has 1 more element after this descriptor
        game::PMD{ 0, -1, 0 },
        0u
    };

    static game::BaseClassArray baseClassArray{
        &baseClassDescriptor,
        RttiApi::rtti().CNetMsgDescriptor
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

using MsgRttiInfo = game::RttiInfo<game::CNetMsgVftable>;

static void setupRttiInfo(MsgRttiInfo& rttiInfo)
{
    using namespace game;

    // Use our own vftable
    rttiInfo.vftable.destructor = (CNetMsgVftable::Destructor)exchangeResourcesMsgDestructor;
    rttiInfo.vftable.serialize = (CNetMsgVftable::Serialize)exchangeResourcesMsgSerialize;
}

static MsgRttiInfo& getMsgRttiInfo()
{
    using namespace game;

    // clang-format off
    static const game::CompleteObjectLocator objectLocator{
        0u,
        offsetof(CExchangeResourcesMsg, vftable),
        0u,
        getExchangeResourcesMsgTypeDescriptor(),
        getMsgHierarchyDescriptor()
    };
    // clang-format on

    static MsgRttiInfo rttiInfo{&objectLocator};

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;
        setupRttiInfo(rttiInfo);
    }

    return rttiInfo;
}

CExchangeResourcesMsg::CExchangeResourcesMsg()
    : siteId{game::invalidId}
    , visitorStackId{game::invalidId}
    , playerCurrency{game::CurrencyType::Gold}
    , siteCurrency{game::CurrencyType::Gold}
    , amount{0u}
{
    vftable = &getMsgRttiInfo().vftable;
}

CExchangeResourcesMsg::CExchangeResourcesMsg(const game::CMidgardID& siteId,
                                             const game::CMidgardID& visitorStackId,
                                             game::CurrencyType playerCurrency,
                                             game::CurrencyType siteCurrency,
                                             std::uint16_t amount)
    : siteId{siteId}
    , visitorStackId{visitorStackId}
    , playerCurrency{playerCurrency}
    , siteCurrency{siteCurrency}
    , amount{amount}
{
    vftable = &getMsgRttiInfo().vftable;
}

static void __fastcall exchangeResourcesMsgDestructor(CExchangeResourcesMsg* thisptr,
                                                      int /*%edx*/,
                                                      char flags)
{
    using namespace game;

    CNetMsgApi::get().destructor(thisptr);
    if (flags & 1) {
        Memory::get().freeNonZero(thisptr);
    }
}

static void __fastcall exchangeResourcesMsgSerialize(CExchangeResourcesMsg* thisptr,
                                                     int /*%edx*/,
                                                     game::CStreamBits* stream)
{
    using namespace game;

    CNetMsgApi::get().serialize(thisptr, stream);

    const auto& serializeId{CStreamBitsApi::get().serializeId};

    serializeId(stream, &thisptr->siteId);
    serializeId(stream, &thisptr->visitorStackId);

    stream->vftable->serialize(stream, &thisptr->playerCurrency, sizeof(thisptr->playerCurrency));
    stream->vftable->serialize(stream, &thisptr->siteCurrency, sizeof(thisptr->siteCurrency));
    stream->vftable->serialize(stream, &thisptr->amount, sizeof(thisptr->amount));
}

game::TypeDescriptor* getExchangeResourcesMsgTypeDescriptor()
{
    using namespace game;

    // clang-format off
    static game::TypeDescriptor descriptor{
        game::RttiApi::typeInfoVftable(),
        nullptr,
        ".?AVCExchangeResourcesMsg@@",
    };
    // clang-format on

    return &descriptor;
}

void sendExchangeResourcesMsg(game::CPhaseGame* phaseGame,
                              const game::CMidgardID& siteId,
                              const game::CMidgardID& visitorStackId,
                              game::CurrencyType playerCurrency,
                              game::CurrencyType marketCurrency,
                              std::uint16_t amount)
{
    using namespace game;

    if (!phaseGame->data->clientTakesTurn) {
        return;
    }

    ++phaseGame->data->midObjectLock->pendingNetworkUpdates;

    CExchangeResourcesMsg message{siteId, visitorStackId, playerCurrency, marketCurrency, amount};

    CMidClient* client{phaseGame->data->midClient};
    CMidgard* midgard{client->core.data->midgard};

    CMidgardApi::get().sendNetMsgToServer(midgard, &message);
}

} // namespace hooks
