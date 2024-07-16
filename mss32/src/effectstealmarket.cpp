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

#include "effectstealmarket.h"
#include "eventeffectbase.h"
#include "mempool.h"
#include "midgardobjectmap.h"
#include "midmsgsender.h"
#include "midsiteresourcemarket.h"

namespace hooks {

struct CEffectStealMarket : public game::CEventEffectBase
{
    game::CMidgardID marketId;
    game::CurrencyType resource;
    std::int16_t amount;
};

static void __fastcall effectStealDtor(CEffectStealMarket* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static bool __fastcall effectStealApply(CEffectStealMarket* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::IMidMsgSender* msgSender,
                                        game::IdVector* triggerers)
{
    using namespace game;

    auto constMarket{(const CMidSiteResourceMarket*)
                         objectMap->vftable->findScenarioObjectById(objectMap, &thisptr->marketId)};

    if (isMarketStockInfinite(constMarket->infiniteStock, thisptr->resource)) {
        return true;
    }

    auto market{(CMidSiteResourceMarket*)objectMap->vftable
                    ->findScenarioObjectByIdForChange(objectMap, &thisptr->marketId)};

    const std::int16_t initialAmount{BankApi::get().get(&market->stock, thisptr->resource)};
    BankApi::get().set(&market->stock, thisptr->resource, initialAmount - thisptr->amount);

    msgSender->vftable->sendObjectsChanges(msgSender);
    return true;
}

static bool __fastcall effectStealMethod2(CEffectStealMarket* thisptr, int /*%edx*/, int)
{
    return true;
}

static bool __fastcall effectStealMethod3(CEffectStealMarket* thisptr, int /*%edx*/)
{
    return true;
}

static bool __fastcall effectStealStopProcessing(const CEffectStealMarket* thisptr, int /*%edx*/)
{
    return false;
}

// clang-format off
static game::IEventEffectVftable effectStealVftable{
    (game::IEventEffectVftable::Destructor)effectStealDtor,
    (game::IEventEffectVftable::Apply)effectStealApply,
    (game::IEventEffectVftable::Method2)effectStealMethod2,
    (game::IEventEffectVftable::Method3)effectStealMethod3,
    (game::IEventEffectVftable::StopProcessing)effectStealStopProcessing,
};
// clang-format on

game::IEventEffect* createEffectStealMarket(const game::CMidgardID& marketId,
                                            game::CurrencyType resource,
                                            std::int16_t amount)
{
    auto effect = (CEffectStealMarket*)game::Memory::get().allocate(sizeof(CEffectStealMarket));
    effect->vftable = &effectStealVftable;
    effect->marketId = marketId;
    effect->resource = resource;
    effect->amount = amount;

    return effect;
}

} // namespace hooks
