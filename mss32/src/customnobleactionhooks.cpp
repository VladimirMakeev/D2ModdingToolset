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

#include "customnobleactionhooks.h"
#include "customnobleactioncategories.h"
#include "midsite.h"
#include "midsiteresourcemarket.h"
#include "nobleactioncategoryset.h"
#include "nobleactionresultstealmarket.h"
#include "originalfunctions.h"
#include "sitecategories.h"
#include "sitecategoryhooks.h"
#include "textids.h"
#include "utils.h"

namespace hooks {

static bool canStealFromResourceMarket(const CMidSiteResourceMarket* market)
{
    using namespace game;

    // If any of market resources are infinite, noble can always steal them
    if (isMarketStockInfinite(market->infiniteStock, CurrencyType::Gold)
        || isMarketStockInfinite(market->infiniteStock, CurrencyType::InfernalMana)
        || isMarketStockInfinite(market->infiniteStock, CurrencyType::LifeMana)
        || isMarketStockInfinite(market->infiniteStock, CurrencyType::DeathMana)
        || isMarketStockInfinite(market->infiniteStock, CurrencyType::RunicMana)
        || isMarketStockInfinite(market->infiniteStock, CurrencyType::GroveMana)) {
        return true;
    }

    // Otherwise check if stock is not empty
    return !BankApi::get().isZero(&market->stock);
}

game::INobleActionResult* __stdcall createNobleActionResultHooked(
    game::IMidgardObjectMap* objectMap,
    const game::LNobleActionCat* actionCategory,
    const game::CMidgardID* targetObjectId,
    const game::CMidgardID* id)
{
    using namespace game;

    const auto& customActions{getCustomNobleActionCategories()};

    if (customActions.stealMarket.second
        && customActions.stealMarket.first.id == actionCategory->id) {
        return createStealMarketActionResult(objectMap, *targetObjectId);
    }

    return getOriginalFunctions().createNobleActionResult(objectMap, actionCategory, targetObjectId,
                                                          id);
}

bool __stdcall getSiteNobleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* objectId,
                                         game::Set<game::LNobleActionCat>* nobleActions)
{
    using namespace game;

    const auto& customSites{customSiteCategories()};
    const auto& customActions{getCustomNobleActionCategories()};

    if (customSites.exists && customActions.stealMarket.second) {
        auto site{(const CMidSite*)objectMap->vftable->findScenarioObjectById(objectMap, objectId)};

        if (site->siteCategory.id == customSites.resourceMarket.id
            && canStealFromResourceMarket((const CMidSiteResourceMarket*)site)) {
            NobleActionCatSetInsertIterator it{};
            NobleActionCatSetApi::get().insert(nobleActions, &it, &customActions.stealMarket.first);
            return true;
        }
    }

    return getOriginalFunctions().getSiteNobleActions(objectMap, playerId, objectId, nobleActions);
}

bool __stdcall getPossibleNobleActionsHooked(const game::IMidgardObjectMap* objectMap,
                                             const game::CMidgardID* playerId,
                                             const game::CMidgardID* objectId,
                                             game::Set<game::LNobleActionCat>* nobleActions)
{
    using namespace game;

    const auto& customSites{customSiteCategories()};
    const auto& customActions{getCustomNobleActionCategories()};

    if (customSites.exists && customActions.stealMarket.second) {
        auto site{(const CMidSite*)objectMap->vftable->findScenarioObjectById(objectMap, objectId)};

        if (site->siteCategory.id == customSites.resourceMarket.id) {
            NobleActionCatSetInsertIterator it{};
            NobleActionCatSetApi::get().insert(nobleActions, &it, &customActions.stealMarket.first);
            return true;
        }
    }

    return getOriginalFunctions().getSiteNobleActions(objectMap, playerId, objectId, nobleActions);
}

game::String* __stdcall getNobleActionResultDescriptionHooked(
    game::String* description,
    const game::LNobleActionCat nobleActionCat,
    const game::CCmdNobleResultMsg* nobleResultMsg,
    const game::CPhaseGame* phaseGame,
    const game::CMidPlayer* player)
{
    using namespace game;

    const auto& customActions{getCustomNobleActionCategories()};

    if (customActions.stealMarket.second
        && nobleActionCat.id == customActions.stealMarket.first.id) {
        static const char fallback[]{
            "\\c000;000;000;\\hC;\\vC;\\fNormal;Resources have been stolen from the market!"};
        const auto text{
            getInterfaceText(textIds().nobleActions.stealMarketSuccess.c_str(), fallback)};

        StringApi::get().initFromString(description, text.c_str());
        return description;
    }

    return getOriginalFunctions().getNobleActionResultDescription(description, nobleActionCat,
                                                                  nobleResultMsg, phaseGame,
                                                                  player);
}

} // namespace hooks
