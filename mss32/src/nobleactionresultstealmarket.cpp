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

#include "nobleactionresultstealmarket.h"
#include "effectgiveresources.h"
#include "effectstealmarket.h"
#include "game.h"
#include "gameutils.h"
#include "globalvariables.h"
#include "idlist.h"
#include "mempool.h"
#include "midgardobjectmap.h"
#include "midsiteresourcemarket.h"
#include "midstack.h"
#include "nobleactionresult.h"
#include <array>

namespace hooks {

struct CNobleActionResultStealMarket : public game::INobleActionResult
{
    game::CMidgardID marketId;
    game::CurrencyType resource;
    std::int16_t amount;
};

static void __fastcall stealMarketDtor(CNobleActionResultStealMarket* thisptr,
                                       int /*%edx*/,
                                       char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static int __fastcall stealMarketGetValue(const CNobleActionResultStealMarket*, int /*%edx*/)
{
    return 0;
}

static const game::CMidgardID* __fastcall stealMarketGetId(
    const CNobleActionResultStealMarket* thisptr,
    int /*%edx*/)
{
    return &thisptr->marketId;
}

static bool __fastcall stealMarketApply(CNobleActionResultStealMarket* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::List<game::IEventEffect*>* effects,
                                        const game::CMidgardID* stackId,
                                        const game::CMidgardID* targetObjectId)
{
    using namespace game;

    auto steal{createEffectStealMarket(thisptr->marketId, thisptr->resource, thisptr->amount)};

    // Reuse IdList::pushBack as the game does in other INobleActionResult::Apply methods
    IdListApi::get().pushBack((IdList*)effects, (const CMidgardID*)&steal);

    const CMidStack* noble{getStack(objectMap, stackId)};

    Bank resources;
    BankApi::get().setZero(&resources);
    BankApi::get().set(&resources, thisptr->resource, thisptr->amount);

    auto giveResources{createEffectGiveResources(noble->ownerId, resources, true)};
    IdListApi::get().pushBack((IdList*)effects, (const CMidgardID*)&giveResources);

    return true;
}

// clang-format off
static game::INobleActionResultVftable stealMarketVftable{
    (game::INobleActionResultVftable::Destructor)stealMarketDtor,
    (game::INobleActionResultVftable::GetValue)stealMarketGetValue,
    (game::INobleActionResultVftable::GetId)stealMarketGetId,
    (game::INobleActionResultVftable::Apply)stealMarketApply,
};
// clang-format on

game::INobleActionResult* createStealMarketActionResult(const game::IMidgardObjectMap* objectMap,
                                                        const game::CMidgardID& marketId)
{
    using namespace game;

    auto stealMarket = (CNobleActionResultStealMarket*)Memory::get().allocate(
        sizeof(CNobleActionResultStealMarket));
    stealMarket->vftable = &stealMarketVftable;
    stealMarket->marketId = marketId;

    auto market{
        (const CMidSiteResourceMarket*)objectMap->vftable->findScenarioObjectById(objectMap,
                                                                                  &marketId)};

    // clang-format off
    const std::array<CurrencyType, 6> resources{
        CurrencyType::Gold,      CurrencyType::InfernalMana,
        CurrencyType::LifeMana,  CurrencyType::DeathMana,
        CurrencyType::RunicMana, CurrencyType::GroveMana
    };
    // clang-format on

    std::vector<CurrencyType> availableResources;
    availableResources.reserve(resources.size());

    for (const auto resource : resources) {
        if (isMarketStockInfinite(market->infiniteStock, resource)
            || BankApi::get().get(&market->stock, resource) > 0) {
            availableResources.push_back(resource);
        }
    }

    if (availableResources.empty()) {
        // This should never happen
        stealMarket->resource = CurrencyType::Gold;
        stealMarket->amount = 0;
    } else {
        // Pick random resource from those available and compute amount to steal
        const int resourceIndex{gameFunctions().generateRandomNumber(availableResources.size())};
        const CurrencyType randomResource{availableResources[resourceIndex]};

        const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
        const GlobalVariables* variables{global->globalVariables};

        int maxStealAmount{variables->data->stealRmkt};
        if (!isMarketStockInfinite(market->infiniteStock, randomResource)) {
            maxStealAmount = std::min(maxStealAmount,
                                      (int)BankApi::get().get(&market->stock, randomResource));
        }

        stealMarket->resource = randomResource;
        stealMarket->amount = gameFunctions().generateRandomNumber(maxStealAmount) + 1;
    }

    return stealMarket;
}

} // namespace hooks
