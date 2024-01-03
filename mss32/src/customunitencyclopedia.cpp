/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "customunitencyclopedia.h"
#include "customunitencyclopediafunctions.h"
#include <fmt/format.h>
#include <thread>

extern std::thread::id mainThreadId;

namespace hooks {

static const char* SCRIPT_FILE_NAME = "unitEncyclopedia.lua";

CustomUnitEncyclopedia::CustomUnitEncyclopedia()
    : mainThreadFunctions(nullptr)
    , workerThreadFunctions(nullptr)
{ }

CustomUnitEncyclopedia ::~CustomUnitEncyclopedia()
{
    delete mainThreadFunctions;
    delete workerThreadFunctions;
}

std::string CustomUnitEncyclopedia::getTxtStatsText(
    const game::CMidUnit* unit,
    const game::IUsUnit* unitImpl,
    bool isMaxLevel,
    bool isInBattle,
    int fortificationArmor,
    int shatteredArmor,
    const std::vector<game::AttackSourceId>& removedSourceWards,
    const std::vector<game::AttackClassId>& removedClassWards) const
{
    return getValue(getFunctions().getTxtStatsText, unit, unitImpl, isMaxLevel, isInBattle,
                    fortificationArmor, shatteredArmor, removedSourceWards, removedClassWards);
}

std::string CustomUnitEncyclopedia::getTxtStats2Text(const game::CMidUnit* unit,
                                                     const game::IUsUnit* unitImpl,
                                                     bool isInBattle,
                                                     int fortificationArmor,
                                                     int shatteredArmor,
                                                     int unitRegen,
                                                     const std::string& pattern) const
{
    return getValue(getFunctions().getTxtStats2Text, unit, unitImpl, isInBattle, fortificationArmor,
                    shatteredArmor, unitRegen, pattern);
}

std::string CustomUnitEncyclopedia::getTxtLeaderInfoText(const game::CMidUnit* unit,
                                                         const game::IUsUnit* unitImpl) const
{
    return getValue(getFunctions().getTxtLeaderInfoText, unit, unitImpl);
}

std::string CustomUnitEncyclopedia::getTxtAttackInfoText(const game::CMidUnit* unit,
                                                         const game::IUsUnit* unitImpl,
                                                         int boostDamageLevel,
                                                         int lowerDamageLevel,
                                                         int lowerInitiativeLevel) const
{
    return getValue(getFunctions().getTxtAttackInfoText, unit, unitImpl, boostDamageLevel,
                    lowerDamageLevel, lowerInitiativeLevel);
}

void CustomUnitEncyclopedia::showScriptErrorMessage(const char* reason) const
{
    showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                    "Reason: '{:s}'",
                                    SCRIPT_FILE_NAME, reason));
}

const CustomUnitEncyclopediaFunctions& CustomUnitEncyclopedia::getFunctions() const
{
    auto& functions = std::this_thread::get_id() == mainThreadId ? mainThreadFunctions
                                                                 : workerThreadFunctions;
    if (functions == nullptr) {
        // Functions need to be initialized in a corresponding thread to use its own Lua instance
        functions = new CustomUnitEncyclopediaFunctions(SCRIPT_FILE_NAME);
    }

    return *functions;
}

CustomUnitEncyclopedia& getCustomUnitEncyclopedia()
{
    static CustomUnitEncyclopedia value{};

    return value;
}

} // namespace hooks
