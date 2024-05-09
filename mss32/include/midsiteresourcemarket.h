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

#ifndef MIDSITERESOURCEMARKET_H
#define MIDSITERESOURCEMARKET_H

#include "currency.h"
#include "midsite.h"
#include <string>
#include <vector>

namespace game {
struct alignas(8) TypeDescriptor;
struct IMidgardObjectMap;
} // namespace game

namespace hooks {

struct ExchangeRates
{
    game::CurrencyType resource2; /**< Resource to buy. */
    int amount1;                  /**< Amount of resource1 needed to get amount2 of resource2. */
    int amount2;                  /**< Amount of resource2 needed to get amount1 of resource1. */
};

struct ResourceExchange
{
    game::CurrencyType resource1;     /**< Resource to sell. */
    std::vector<ExchangeRates> rates; /**< Resources to buy and exchange rates. */
};

using MarketExchangeRates = std::vector<ResourceExchange>;

union InfiniteStock
{
    struct
    {
        bool lifeMana : 1;
        bool infernalMana : 1;
        bool runicMana : 1;
        bool deathMana : 1;
        bool groveMana : 1;
        bool gold : 1;
    } parts;

    std::uint8_t value;
};

assert_size(InfiniteStock, 1);

struct CMidSiteResourceMarket : public game::CMidSite
{
    /** User defined exchange rates script, if set. */
    std::string exchangeRatesScript;
    /** Market resources, if finite. */
    game::Bank stock;
    /** Specifies whether certain resource types are infinite or not. */
    InfiniteStock infiniteStock;
    /** If true, resource market uses custom exchange rates script. */
    bool customExchangeRates;
};

game::TypeDescriptor* getResourceMarketTypeDescriptor();

CMidSiteResourceMarket* createResourceMarket(const game::CMidgardID* siteId);

void addResourceMarketStreamRegister();

bool isMarketStockInfinite(const InfiniteStock& stock, game::CurrencyType currency);

bool getExchangeRates(const game::IMidgardObjectMap* objectMap,
                      const game::CMidgardID& marketId,
                      const game::CMidgardID& visitorStackId,
                      MarketExchangeRates& exchangeRates);

const ExchangeRates* findExchangeRates(const MarketExchangeRates& marketRates,
                                       game::CurrencyType playerCurrency,
                                       game::CurrencyType marketCurrency);

bool exchangeResources(game::IMidgardObjectMap* objectMap,
                       const game::CMidgardID& marketId,
                       const game::CMidgardID& visitorStackId,
                       game::CurrencyType playerCurrency,
                       game::CurrencyType marketCurrency,
                       std::uint16_t amount);

} // namespace hooks

#endif // MIDSITERESOURCEMARKET_H
