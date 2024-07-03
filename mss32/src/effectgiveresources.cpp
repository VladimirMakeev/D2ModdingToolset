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

#include "effectgiveresources.h"
#include "eventeffectbase.h"
#include "gameutils.h"
#include "mempool.h"
#include "midgardobjectmap.h"
#include "midmsgsender.h"
#include "midplayer.h"

namespace hooks {

struct CEffectGiveResources : public game::CEventEffectBase
{
    game::Bank resources;
    game::CMidgardID playerId;
    bool add;
};

static void __fastcall effectGiveDtor(CEffectGiveResources* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static bool __fastcall effectGiveApply(CEffectGiveResources* thisptr,
                                       int /*%edx*/,
                                       game::IMidgardObjectMap* objectMap,
                                       game::IMidMsgSender* msgSender,
                                       game::IdVector* triggerers)
{
    using namespace game;

    const auto& bankApi{BankApi::get()};
    if (!bankApi.isValid(&thisptr->resources)) {
        return false;
    }

    CMidPlayer* player{getPlayerToChange(objectMap, &thisptr->playerId)};

    Bank bank;
    bankApi.setZero(&bank);

    if (thisptr->add) {
        bankApi.add(&bank, &player->bank);
        bankApi.add(&bank, &thisptr->resources);
    } else {
        bankApi.copy(&bank, &player->bank);
        bankApi.subtract(&bank, &thisptr->resources);
    }

    if (bankApi.isValid(&bank)) {
        bankApi.copy(&player->bank, &bank);
    }

    msgSender->vftable->sendObjectsChanges(msgSender);
    return true;
}

static bool __fastcall effectGiveMethod2(CEffectGiveResources* thisptr, int /*%edx*/, int)
{
    return true;
}

static bool __fastcall effectGiveMethod3(CEffectGiveResources* thisptr, int /*%edx*/)
{
    return true;
}

static bool __fastcall effectGiveStopProcessing(const CEffectGiveResources* thisptr, int /*%edx*/)
{
    return false;
}

// clang-format off
static game::IEventEffectVftable effectGiveVftable{
    (game::IEventEffectVftable::Destructor)effectGiveDtor,
    (game::IEventEffectVftable::Apply)effectGiveApply,
    (game::IEventEffectVftable::Method2)effectGiveMethod2,
    (game::IEventEffectVftable::Method3)effectGiveMethod3,
    (game::IEventEffectVftable::StopProcessing)effectGiveStopProcessing,
};
// clang-format on

game::IEventEffect* createEffectGiveResources(const game::CMidgardID& playerId,
                                              const game::Bank& resources,
                                              bool add)
{
    using namespace game;

    auto effect{(CEffectGiveResources*)Memory::get().allocate(sizeof(CEffectGiveResources))};
    effect->vftable = &effectGiveVftable;
    BankApi::get().setZero(&effect->resources);
    BankApi::get().copy(&effect->resources, &resources);
    effect->playerId = playerId;
    effect->add = add;

    return effect;
}

} // namespace hooks
