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

#include "midsiteresourcemarket.h"
#include "bindings/stackview.h"
#include "campaignstream.h"
#include "dynamiccast.h"
#include "gameutils.h"
#include "log.h"
#include "mempool.h"
#include "midgardobjectmap.h"
#include "midgardstream.h"
#include "midplayer.h"
#include "midstack.h"
#include "scenarioobjectstreams.h"
#include "scripts.h"
#include "sitecategoryhooks.h"
#include "streamregister.h"
#include "utils.h"
#include <algorithm>
#include <cstring>
#include <fmt/format.h>

namespace hooks {

// Rtti info below is put into functions so it will be initialized
// the first time CMidSiteResourceMarket object is created.
// Delayed initialization of rtti structures is needed
// because of typeInfoVftable() implementation which expects gameVersion() to work correctly.
// gameVersion() can be called only after determineGameVersion() is called from DllMain.

static game::ClassHierarchyDescriptor* getResourceMarketHierarchyDescriptor()
{
    // clang-format off
    static game::BaseClassDescriptor baseClassDescriptor{
        getResourceMarketTypeDescriptor(),
        5u, // base class array has 5 more elements after this descriptor
        game::PMD{ 0, -1, 0 },
        0u
    };

    static game::BaseClassArray baseClassArray{
        &baseClassDescriptor,
        game::RttiApi::rtti().CMidSiteDescriptor,
        game::RttiApi::rtti().IMidScenarioObjectDescriptor,
        game::RttiApi::rtti().IMidObjectDescriptor,
        game::RttiApi::rtti().IMapElementDescriptor,
        game::RttiApi::rtti().IAiPriorityDescriptor,
    };

    static game::ClassHierarchyDescriptor hierarchyDescriptor{
        0u,
        1u,
        6u, // base class array has 6 elements
        &baseClassArray
    };
    // clang-format on

    return &hierarchyDescriptor;
}

static game::RttiInfo<game::CMidSiteVftable>& getMarketSiteRttiInfo()
{
    // clang-format off
    // CompleteObjectLocator for IMidScenarioObject base class
    static const game::CompleteObjectLocator objectLocator{
        0u,
        offsetof(CMidSiteResourceMarket, vftable),
        0u,
        getResourceMarketTypeDescriptor(),
        getResourceMarketHierarchyDescriptor()
    };

    // CompleteObjectLocator and vftable for IMidScenarioObject base class
    static game::RttiInfo<game::CMidSiteVftable> rttiInfo{
        &objectLocator
    };
    // clang-format on

    return rttiInfo;
}

static game::RttiInfo<game::IMapElementVftable>& getMarketMapElementRttiInfo()
{
    // clang-format off
    // CompleteObjectLocator for IMapElement base class
    static const game::CompleteObjectLocator objectLocator{
        0u,
        offsetof(CMidSiteResourceMarket, CMidSiteResourceMarket::mapElement),
        0u,
        getResourceMarketTypeDescriptor(),
        getResourceMarketHierarchyDescriptor()
    };

    // CompleteObjectLocator and vftable for IMapElement base class
    static game::RttiInfo<game::IMapElementVftable> rttiInfo{
        &objectLocator
    };
    // clang-format on

    return rttiInfo;
}

static game::RttiInfo<game::IAiPriorityVftable>& getMarketAiPriorityRttiInfo()
{
    // clang-format off
    // CompleteObjectLocator for IAiPriority base class
    static const game::CompleteObjectLocator objectLocator{
        0u,
        offsetof(CMidSiteResourceMarket, CMidSiteResourceMarket::aiPriority),
        0u,
        getResourceMarketTypeDescriptor(),
        getResourceMarketHierarchyDescriptor()
    };

    // CompleteObjectLocator and vftable for IAiPriority base class
    static game::RttiInfo<game::IAiPriorityVftable> rttiInfo{
        &objectLocator
    };
    // clang-format on

    return rttiInfo;
}

static game::IMidScenarioObjectVftable::Destructor midSiteDtor = nullptr;

static void __fastcall marketSiteDtor(CMidSiteResourceMarket* thisptr, int /*%edx*/, char flags)
{
    // Destroy market specific data
    thisptr->exchangeRatesScript.~basic_string();

    if (midSiteDtor) {
        // Allow CMidSite d-tor clear itself and free our memory
        midSiteDtor((game::IMidObject*)thisptr, flags);
    }
}

static void __fastcall marketStream(CMidSiteResourceMarket* thisptr,
                                    int /* %edx */,
                                    game::CampaignStream* campaignStream,
                                    const game::CMidgardID* siteId)
{
    using namespace game;

    IMidgardStream* stream{campaignStream->stream};
    IMidgardStreamVftable* vftable{stream->vftable};

    vftable->streamBool(stream, "CUSTOM", &thisptr->customExchangeRates);
    if (thisptr->customExchangeRates) {
        int length{static_cast<int>(thisptr->exchangeRatesScript.length())};
        vftable->streamInt(stream, "CODE_LEN", &length);

        if (vftable->readMode(stream)) {
            thisptr->exchangeRatesScript.resize(length);
        }

        vftable->streamString(stream, "CODE", thisptr->exchangeRatesScript.data());
    }

    vftable->streamCurrency(stream, "BANK", &thisptr->stock);
    vftable->streamByte(stream, "INF", &thisptr->infiniteStock.value);
}

static void __fastcall marketMapElementDtor(game::IMapElement* thisptr, int /*%edx*/, char flags)
{
    constexpr auto offset = offsetof(CMidSiteResourceMarket, CMidSiteResourceMarket::mapElement);

    const auto ptr = reinterpret_cast<std::uintptr_t>(thisptr) - offset;
    CMidSiteResourceMarket* site = reinterpret_cast<CMidSiteResourceMarket*>(ptr);
    marketSiteDtor(site, 0, flags);
}

static void __fastcall marketAiPriorityDtor(game::IAiPriority* thisptr, int /*%edx*/, char flags)
{
    constexpr auto offset = offsetof(CMidSiteResourceMarket, CMidSiteResourceMarket::aiPriority);

    const auto ptr = reinterpret_cast<std::uintptr_t>(thisptr) - offset;
    CMidSiteResourceMarket* site = reinterpret_cast<CMidSiteResourceMarket*>(ptr);
    marketSiteDtor(site, 0, flags);
}

/** Get vftables from CMidSite and update methods specific to resource market */
static void setupResourceMarketRttiInfo(const CMidSiteResourceMarket* market)
{
    using namespace game;

    {
        // For IMidScenarioObject
        auto& rttiInfo = getMarketSiteRttiInfo();

        std::memcpy(&rttiInfo.vftable, market->vftable, sizeof(CMidSiteVftable));
        // Remember base class destructor to destroy CMidSite data properly
        midSiteDtor = market->vftable->destructor;
        // Use our own d-tor
        rttiInfo.vftable.destructor = (IMidScenarioObjectVftable::Destructor)marketSiteDtor;
        // Make sure we use market-specific site data stream method
        rttiInfo.vftable.streamSiteData = (CMidSiteVftable::StreamSiteData)marketStream;
    }

    {
        // For IMapElement
        auto& rttiInfo = getMarketMapElementRttiInfo();

        std::memcpy(&rttiInfo.vftable, market->mapElement.vftable, sizeof(IMapElementVftable));
        rttiInfo.vftable.destructor = (IMapElementVftable::Destructor)marketMapElementDtor;
    }

    {
        // For IAiPriority
        auto& rttiInfo = getMarketAiPriorityRttiInfo();

        std::memcpy(&rttiInfo.vftable, market->aiPriority.vftable, sizeof(IAiPriorityVftable));
        rttiInfo.vftable.destructor = (IAiPriorityVftable::Destructor)marketAiPriorityDtor;
    }
}

static void readExchangeRates(const sol::table& table, MarketExchangeRates& exchangeRates)
{
    exchangeRates.clear();
    // if table size is > 6 we 100% have duplicates
    exchangeRates.reserve(table.size());

    bool resourceExchangeExists[6]{};

    for (std::size_t i = 0u; i < table.size(); ++i) {
        const sol::table& exchangeTable = table[i + 1];

        ResourceExchange exchange;
        exchange.resource1 = exchangeTable[1];

        if (resourceExchangeExists[(int)exchange.resource1]) {
            throw std::runtime_error("Duplicate resource exchange found!");
        }

        resourceExchangeExists[(int)exchange.resource1] = true;

        const sol::table& ratesArray = exchangeTable[2];
        // if rates array size is > 6 we 100% have duplicates
        exchange.rates.reserve(ratesArray.size());

        bool exchangeRateExists[6]{};

        for (std::size_t j = 0u; j < ratesArray.size(); ++j) {
            const sol::table& ratesTable = ratesArray[j + 1];

            ExchangeRates rates;
            rates.resource2 = ratesTable[1];

            if (exchangeRateExists[(int)rates.resource2]) {
                throw std::runtime_error("Duplicate exchange rate found!");
            }

            exchangeRateExists[(int)rates.resource2] = true;

            rates.amount1 = ratesTable[2];
            rates.amount2 = ratesTable[3];

            exchange.rates.push_back(rates);
        }

        exchangeRates.push_back(exchange);
    }
}

game::TypeDescriptor* getResourceMarketTypeDescriptor()
{
    // clang-format off
    static game::TypeDescriptor descriptor{
        game::RttiApi::typeInfoVftable(),
        nullptr,
        ".?AVCMidSiteResourceMarket@@"
    };
    // clang-format on

    return &descriptor;
}

CMidSiteResourceMarket* createResourceMarket(const game::CMidgardID* siteId)
{
    if (!customSiteCategories().exists) {
        return nullptr;
    }

    using namespace game;

    auto market = (CMidSiteResourceMarket*)Memory::get().allocate(sizeof(CMidSiteResourceMarket));
    CMidSiteApi::get().constructor(market, siteId, &customSiteCategories().resourceMarket);

    static bool firstTime = true;
    if (firstTime) {
        firstTime = false;
        setupResourceMarketRttiInfo(market);
    }

    new (&market->exchangeRatesScript) std::string();
    market->customExchangeRates = false;

    BankApi::get().setZero(&market->stock);
    market->infiniteStock.value = 0u;

    // Use our own rtti info and vftables
    market->vftable = &getMarketSiteRttiInfo().vftable;
    market->mapElement.vftable = &getMarketMapElementRttiInfo().vftable;
    market->aiPriority.vftable = &getMarketAiPriorityRttiInfo().vftable;
    return market;
}

struct CResourceMarketStreamRegister : public game::CStreamRegisterBase
{ };

void __fastcall resMarketStreamRegisterDtor(CResourceMarketStreamRegister* thisptr,
                                            int /*%edx*/,
                                            char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::IMidScenarioObject* __fastcall resMargetStreamRegisterStream(
    CResourceMarketStreamRegister* thisptr,
    int /*%edx*/,
    const game::CMidgardID* objectId)
{
    return createResourceMarket(objectId);
}

// clang-format off
static game::CStreamRegisterBaseVftable resMarketStreamRegisterVftable{
    (game::CStreamRegisterBaseVftable::Destructor)resMarketStreamRegisterDtor,
    (game::CStreamRegisterBaseVftable::Stream)resMargetStreamRegisterStream

};
// clang-format on

void addResourceMarketStreamRegister()
{
    using namespace game;

    static bool firstTime = true;
    if (!firstTime) {
        return;
    }

    firstTime = false;

    static CResourceMarketStreamRegister streamRegister;
    // Make sure we use our own vftable
    streamRegister.vftable = &resMarketStreamRegisterVftable;

    const auto& typeInfoRawName = *RttiApi::get().typeInfoRawName;
    const char* rawName = typeInfoRawName(getResourceMarketTypeDescriptor());

    // Associate stream register with a new site
    ScenarioObjectStreamsApi::get().addStreamRegister(&streamRegister, rawName);
}

bool isMarketStockInfinite(const InfiniteStock& stock, game::CurrencyType currency)
{
    using namespace game;

    switch (currency) {
    case CurrencyType::Gold:
        return stock.parts.gold;
    case CurrencyType::InfernalMana:
        return stock.parts.infernalMana;
    case CurrencyType::LifeMana:
        return stock.parts.lifeMana;
    case CurrencyType::DeathMana:
        return stock.parts.deathMana;
    case CurrencyType::RunicMana:
        return stock.parts.runicMana;
    case CurrencyType::GroveMana:
        return stock.parts.groveMana;
    }

    return false;
}

bool getExchangeRates(const game::IMidgardObjectMap* objectMap,
                      const game::CMidgardID& marketId,
                      const game::CMidgardID& visitorStackId,
                      MarketExchangeRates& exchangeRates)
{
    using namespace game;

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, &marketId)};
    auto market{(const CMidSiteResourceMarket*)obj};
    if (!market) {
        return false;
    }

    static const char functionName[]{"getExchangeRates"};
    const bool bindScenario{true};
    const bool alwaysExist{true};

    std::optional<sol::environment> env;
    std::optional<sol::function> getExchangeRates;
    if (market->customExchangeRates) {
        sol::protected_function_result result;
        env = executeScript(market->exchangeRatesScript, result, bindScenario);
        if (!result.valid()) {
            const CMqPoint& pos{market->mapElement.position};
            const sol::error err = result;
            logError("mssProxyError.log", fmt::format("Failed to load custom exchange rates"
                                                      " for resource market {:s} at ({:d}, {:d}).\n"
                                                      "Script:\n'{:s}'\n"
                                                      "Reason: {:s}",
                                                      idToString(&market->id), pos.x, pos.y,
                                                      market->exchangeRatesScript, err.what()));
            return false;
        }

        getExchangeRates = getProtectedScriptFunction(env.value(), functionName, alwaysExist);
    } else {
        // Resource market uses default exchange rates
        const auto& path{scriptsFolder() / customSiteCategories().exchangeRatesScript};
        getExchangeRates = getScriptFunction(path, functionName, env, alwaysExist, bindScenario);
    }

    try {
        const CMidStack* visitorStack{getStack(objectMap, &visitorStackId)};
        if (!visitorStack) {
            return false;
        }

        const bindings::StackView visitor{visitorStack, objectMap};
        const sol::table table = (*getExchangeRates)(visitor);

        readExchangeRates(table, exchangeRates);
    } catch (const std::exception& e) {
        const CMqPoint& pos{market->mapElement.position};
        logError("mssProxyError.log", fmt::format("Failed to get exchange rates"
                                                  " for resource market {:s} at ({:d}, {:d}).\n"
                                                  "Reason: '{:s}'",
                                                  idToString(&market->id), pos.x, pos.y, e.what()));
        return false;
    }

    return true;
}

const ExchangeRates* findExchangeRates(const MarketExchangeRates& marketRates,
                                       game::CurrencyType playerCurrency,
                                       game::CurrencyType marketCurrency)
{
    auto playerResourceMatch = [playerCurrency](const ResourceExchange& exchange) {
        return exchange.resource1 == playerCurrency;
    };

    const auto ratesIt{std::find_if(marketRates.cbegin(), marketRates.cend(), playerResourceMatch)};
    if (ratesIt == marketRates.cend()) {
        return nullptr;
    }

    auto marketResourceMatch = [marketCurrency](const ExchangeRates& exchRates) {
        return exchRates.resource2 == marketCurrency;
    };

    const ResourceExchange& exchange{*ratesIt};
    const auto exchangeIt{
        std::find_if(exchange.rates.cbegin(), exchange.rates.cend(), marketResourceMatch)};
    if (exchangeIt == exchange.rates.cend()) {
        // Could not exchange, bug ?
        return nullptr;
    }

    return &(*exchangeIt);
}

bool exchangeResources(game::IMidgardObjectMap* objectMap,
                       const game::CMidgardID& marketId,
                       const game::CMidgardID& visitorStackId,
                       game::CurrencyType playerCurrency,
                       game::CurrencyType marketCurrency,
                       std::uint16_t amount)
{
    using namespace game;

    MarketExchangeRates rates;
    if (!getExchangeRates(objectMap, marketId, visitorStackId, rates)) {
        // Failed to get exchange rates from script. Bug.
        return false;
    }

    auto foundRate{findExchangeRates(rates, playerCurrency, marketCurrency)};
    if (!foundRate) {
        return false;
    }

    const ExchangeRates& rate{*foundRate};

    const std::uint16_t playerExchangeAmount = amount * rate.amount1;

    const auto* stack{getStack(objectMap, &visitorStackId)};
    const auto* player{getPlayer(objectMap, &stack->ownerId)};

    const auto& bankApi{BankApi::get()};
    const auto playerResource{bankApi.get(&player->bank, playerCurrency)};
    const bool playerCanExchange{playerResource >= playerExchangeAmount};

    if (!playerCanExchange) {
        // Not enough resources. Why we are here?
        // Was it a net message from cheater who tries to exchange?
        return false;
    }

    // Check if market has enough resources, or they are infinite

    auto site{objectMap->vftable->findScenarioObjectById(objectMap, &marketId)};
    auto market{static_cast<const CMidSiteResourceMarket*>(site)};

    const std::uint16_t marketExchangeAmount = amount * rate.amount2;

    const bool infiniteResource{isMarketStockInfinite(market->infiniteStock, marketCurrency)};

    bool marketCanExchange{};
    if (infiniteResource) {
        marketCanExchange = true;
    } else {
        // Check market stock amount
        const auto marketResource{bankApi.get(&market->stock, marketCurrency)};

        marketCanExchange = marketResource >= marketExchangeAmount;
    }

    if (!marketCanExchange) {
        // Same question as above: why we are here if market can't exchange resources?
        // Why net message was sent at all?
        return false;
    }

    auto playerToChange{getPlayerToChange(objectMap, &stack->ownerId)};
    // Player sells its resource
    bankApi.set(&playerToChange->bank, playerCurrency, playerResource - playerExchangeAmount);
    // And gets resource from market
    const auto current{bankApi.get(&playerToChange->bank, marketCurrency)};
    bankApi.set(&playerToChange->bank, marketCurrency, current + marketExchangeAmount);

    const bool infiniteResource2{isMarketStockInfinite(market->infiniteStock, playerCurrency)};

    // Change resource market data only when necessary
    if (!infiniteResource || !infiniteResource2) {
        auto obj{objectMap->vftable->findScenarioObjectByIdForChange(objectMap, &marketId)};
        auto marketToChange{static_cast<CMidSiteResourceMarket*>(obj)};

        if (!infiniteResource) {
            // Market sells its resource
            const auto marketResource{bankApi.get(&market->stock, marketCurrency)};
            bankApi.set(&marketToChange->stock, marketCurrency,
                        marketResource - marketExchangeAmount);
        }

        if (!infiniteResource2) {
            // And gets resource from player
            const auto marketResource{bankApi.get(&market->stock, playerCurrency)};
            bankApi.set(&marketToChange->stock, playerCurrency,
                        marketResource + playerExchangeAmount);
        }
    }

    return true;
}

} // namespace hooks
