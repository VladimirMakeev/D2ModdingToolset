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
#include "difficultylevel.h"
#include "fortcategory.h"
#include "fortification.h"
#include "game.h"
#include "gameutils.h"
#include "globalvariables.h"
#include "log.h"
#include "midgardid.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midscenvariables.h"
#include "midvillage.h"
#include "originalfunctions.h"
#include "racecategory.h"
#include "racetype.h"
#include "scenarioinfo.h"
#include "settings.h"
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
    const auto raceId = player->raceType->data->raceType.id;
    const char* racePrefix{};
    CurrencyType manaType;
    std::int32_t manaIncome;

    if (raceId == races.human->id) {
        racePrefix = "EMPIRE_";
        manaType = CurrencyType::LifeMana;
        manaIncome = income->lifeMana;
    } else if (raceId == races.heretic->id) {
        racePrefix = "LEGIONS_";
        manaType = CurrencyType::InfernalMana;
        manaIncome = income->infernalMana;
    } else if (raceId == races.dwarf->id) {
        racePrefix = "CLANS_";
        manaType = CurrencyType::RunicMana;
        manaIncome = income->runicMana;
    } else if (raceId == races.undead->id) {
        racePrefix = "HORDES_";
        manaType = CurrencyType::DeathMana;
        manaIncome = income->deathMana;
    } else if (raceId == races.elf->id) {
        racePrefix = "ELVES_";
        manaType = CurrencyType::GroveMana;
        manaIncome = income->groveMana;
    } else if (raceId == races.neutral->id) {
        // Skip neutrals
        return income;
    }

    if (!racePrefix) {
        logError("mssProxyError.log",
                 fmt::format("Trying to compute daily income for unknown race. "
                             "LRace.dbf id: {:d}",
                             raceId));
        return income;
    }

    std::array<int, 6> cityGoldIncome = {0, 0, 0, 0, 0, 0};
    std::array<int, 6> cityManaIncome = {0, 0, 0, 0, 0, 0};

    // No variables implemented for mana, get incomes from settings
    const auto& economy = userSettings().economy;
    cityManaIncome[0] = economy.cityManaIncome.capital;
    cityManaIncome[1] = economy.cityManaIncome.tier1;
    cityManaIncome[2] = economy.cityManaIncome.tier2;
    cityManaIncome[3] = economy.cityManaIncome.tier3;
    cityManaIncome[4] = economy.cityManaIncome.tier4;
    cityManaIncome[5] = economy.cityManaIncome.tier5;

    auto variables{getScenarioVariables(objectMap)};
    if (variables && variables->variables.length) {

        logDebug("cityIncome.log",
                 fmt::format("Loop through {:d} scenario variables", variables->variables.length));

        std::uint32_t listIndex{};
        for (const auto& variable : variables->variables) {
            const auto& name = variable.second.name;

            // Additional income for specific race
            if (!strncmp(name, racePrefix, std::strlen(racePrefix))) {
                for (int i = 0; i < 6; ++i) {
                    const auto expectedName{
                        fmt::format("{:s}TIER_{:d}_CITY_INCOME", racePrefix, i)};

                    if (!strncmp(name, expectedName.c_str(), sizeof(name))) {
                        cityGoldIncome[i] += variable.second.value;
                        break;
                    }
                }
            }

            // Additional income for all races
            if (!strncmp(name, "TIER", 4)) {
                for (int i = 0; i < 6; ++i) {
                    const auto expectedName{fmt::format("TIER_{:d}_CITY_INCOME", i)};

                    if (!strncmp(name, expectedName.c_str(), sizeof(name))) {
                        cityGoldIncome[i] += variable.second.value;
                        break;
                    }
                }
            }

            listIndex++;
        }

        logDebug("cityIncome.log", fmt::format("Loop done in {:d} iterations", listIndex));
    }

    // No variables defined for gold, get incomes from settings
    if (std::all_of(std::begin(cityGoldIncome), std::end(cityGoldIncome),
                    [](int value) { return value == 0; })) {
        cityGoldIncome[0] = economy.cityGoldIncome.capital;
        cityGoldIncome[1] = economy.cityGoldIncome.tier1;
        cityGoldIncome[2] = economy.cityGoldIncome.tier2;
        cityGoldIncome[3] = economy.cityGoldIncome.tier3;
        cityGoldIncome[4] = economy.cityGoldIncome.tier4;
        cityGoldIncome[5] = economy.cityGoldIncome.tier5;
    }

    // No incomes defined, skip
    if (std::all_of(std::begin(cityGoldIncome), std::end(cityGoldIncome),
                    [](int value) { return value == 0; }) && 
        std::all_of(std::begin(cityManaIncome), std::end(cityManaIncome),
                       [](int value) { return value == 0; })) {
        return income;
    }

    int goldIncomeTotal = 0;
    int manaIncomeTotal = 0;

    auto getVillageIncome = [playerId, &cityGoldIncome, &cityManaIncome, &goldIncomeTotal, 
                            &manaIncomeTotal](const IMidScenarioObject* obj) {
        auto fortification = static_cast<const CFortification*>(obj);

        if (fortification->ownerId == *playerId) {
            auto vftable = static_cast<const CFortificationVftable*>(fortification->vftable);
            auto category = vftable->getCategory(fortification);

            if (category->id == FortCategories::get().village->id) {
                auto village = static_cast<const CMidVillage*>(fortification);
                goldIncomeTotal += cityGoldIncome[village->tierLevel];
                manaIncomeTotal += cityManaIncome[village->tierLevel];
            }
        }
    };

    forEachScenarioObject(objectMap, IdType::Fortification, getVillageIncome);

    // Additional gold and mana income for capital city
    goldIncomeTotal += cityGoldIncome[0];
    manaIncomeTotal += cityManaIncome[0];

    // Income increase based on current game difficulty
    int incomeBonus = 0;
    const auto& globalApi = GlobalDataApi::get();
    const auto globalData = *globalApi.getGlobalData();
    const auto globalVars = *globalData->globalVariables;
    const auto& difficulties = DifficultyLevelCategories::get();
    const auto difficultyId = getScenarioInfo(objectMap)->gameDifficulty.id;
    if (difficultyId == difficulties.easy->id) {
        incomeBonus = globalVars->incomeEasy;
    } else if (difficultyId == difficulties.average->id) {
        incomeBonus = globalVars->incomeAverage;
    } else if (difficultyId == difficulties.hard->id) {
        incomeBonus = globalVars->incomeHard;
    } else if (difficultyId == difficulties.veryHard->id) {
        incomeBonus = globalVars->incomeVeryHard;
    }
    if (incomeBonus != 0) {
        goldIncomeTotal = goldIncomeTotal * (100 + incomeBonus) / 100;
        manaIncomeTotal = manaIncomeTotal * (100 + incomeBonus) / 100;
    }

    // Sum additional city incomes with original ones
    const int gold{income->gold + goldIncomeTotal};
    const int mana{manaIncome + manaIncomeTotal};
    BankApi::get().set(income, CurrencyType::Gold, std::clamp(gold, 0, 9999));
    BankApi::get().set(income, manaType, std::clamp(mana, 0, 9999));

    return income;
}

} // namespace hooks
