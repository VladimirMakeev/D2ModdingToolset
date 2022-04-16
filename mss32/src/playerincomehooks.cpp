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

#include "playerincomehooks.h"
#include "currency.h"
#include "fortcategory.h"
#include "fortification.h"
#include "game.h"
#include "gameutils.h"
#include "iterators.h"
#include "log.h"
#include "midgardid.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midscenvariables.h"
#include "midvillage.h"
#include "originalfunctions.h"
#include "racecategory.h"
#include "racetype.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <fmt/format.h>

namespace hooks {

game::Bank* __stdcall computePlayerDailyIncomeHooked(game::Bank* income,
                                                     game::IMidgardObjectMap* objectMap,
                                                     const game::CMidgardID* playerId)
{
    using namespace game;

    getOriginalFunctions().computePlayerDailyIncome(income, objectMap, playerId);

    auto playerObj = objectMap->vftable->findScenarioObjectById(objectMap, playerId);
    if (!playerObj) {
        logError("mssProxyError.log",
                 fmt::format("Could not find player {:s}", idToString(playerId)));
        return income;
    }

    const auto& races = RaceCategories::get();
    auto player = static_cast<const CMidPlayer*>(playerObj);
    if (player->raceType->data->raceType.id == races.neutral->id) {
        // Skip neutrals
        return income;
    }

    auto variables{getScenarioVariables(objectMap)};
    if (!variables || !variables->variables.length) {
        // No variables defined, skip
        return income;
    }

    const auto raceId = player->raceType->data->raceType.id;
    const char* racePrefix{};

    if (raceId == races.human->id) {
        racePrefix = "EMPIRE_";
    } else if (raceId == races.heretic->id) {
        racePrefix = "LEGIONS_";
    } else if (raceId == races.dwarf->id) {
        racePrefix = "CLANS_";
    } else if (raceId == races.undead->id) {
        racePrefix = "HORDES_";
    } else if (raceId == races.elf->id) {
        racePrefix = "ELVES_";
    }

    if (!racePrefix) {
        logError("mssProxyError.log",
                 fmt::format("Trying to compute daily income for unknown race. "
                             "LRace.dbf id: {:d}",
                             raceId));
        return income;
    }

    std::array<int, 6> cityIncome = {0, 0, 0, 0, 0, 0};

    logDebug("cityIncome.log",
             fmt::format("Loop through {:d} scenario variables", variables->variables.length));

    std::uint32_t listIndex{};
    for (const auto& variable : variables->variables) {
        const auto& name = variable.second.name;

        // Additional income for specific race
        if (!strncmp(name, racePrefix, std::strlen(racePrefix))) {
            for (int i = 0; i < 6; ++i) {
                const auto expectedName{fmt::format("{:s}TIER_{:d}_CITY_INCOME", racePrefix, i)};

                if (!strncmp(name, expectedName.c_str(), sizeof(name))) {
                    cityIncome[i] += variable.second.value;
                    break;
                }
            }
        }

        // Additional income for all races
        if (!strncmp(name, "TIER", 4)) {
            for (int i = 0; i < 6; ++i) {
                const auto expectedName{fmt::format("TIER_{:d}_CITY_INCOME", i)};

                if (!strncmp(name, expectedName.c_str(), sizeof(name))) {
                    cityIncome[i] += variable.second.value;
                    break;
                }
            }
        }

        listIndex++;
    }

    logDebug("cityIncome.log", fmt::format("Loop done in {:d} iterations", listIndex));

    if (std::all_of(std::begin(cityIncome), std::end(cityIncome),
                    [](int value) { return value == 0; })) {
        return income;
    }

    IteratorPtr iteratorPtr;
    Iterators::get().createFortificationsIterator(&iteratorPtr, objectMap);
    IteratorPtr endIteratorPtr;
    Iterators::get().createFortificationsEndIterator(&endIteratorPtr, objectMap);

    auto iterator = iteratorPtr.data;

    while (!iterator->vftable->end(iterator, endIteratorPtr.data)) {
        auto id = iterator->vftable->getObjectId(iterator);
        auto obj = objectMap->vftable->findScenarioObjectById(objectMap, id);
        auto fortification = static_cast<CFortification*>(obj);

        if (fortification->ownerId == *playerId) {
            auto vftable = static_cast<const CFortificationVftable*>(fortification->vftable);
            auto category = vftable->getCategory(fortification);

            if (category->id == FortCategories::get().village->id) {
                auto village = static_cast<const CMidVillage*>(fortification);

                const int gold{income->gold + cityIncome[village->tierLevel]};
                BankApi::get().set(income, CurrencyType::Gold, std::clamp(gold, 0, 9999));
            }
        }

        iterator->vftable->advance(iterator);
    }

    // Custom capital city income
    const int gold{income->gold + cityIncome[0]};
    BankApi::get().set(income, CurrencyType::Gold, std::clamp(gold, 0, 9999));

    auto& freeSmartPtr = SmartPointerApi::get().createOrFree;
    freeSmartPtr((SmartPointer*)&iteratorPtr, nullptr);
    freeSmartPtr((SmartPointer*)&endIteratorPtr, nullptr);

    return income;
}

} // namespace hooks
