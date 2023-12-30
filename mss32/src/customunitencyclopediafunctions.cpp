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

#include "customunitencyclopediafunctions.h"
#include "attackimpl.h"
#include "attackutils.h"
#include "attackview.h"
#include "customattackutils.h"
#include "game.h"
#include "idview.h"
#include "scripts.h"
#include "utils.h"

namespace hooks {

CustomUnitEncyclopediaFunctions::CustomUnitEncyclopediaFunctions(const std::string& scriptFileName)
{
    environment = executeScriptFile(scriptsFolder() / scriptFileName, false, true);
    if (!environment)
        return;

    auto& env = *environment;
    env["getInterfaceText"] = sol::overload<>(
        [](const std::string& id) -> std::string {
            return game::gameFunctions().getInterfaceText(&bindings::IdView(id).id);
        },
        [](const bindings::IdView& id) -> std::string {
            return game::gameFunctions().getInterfaceText(&id.id);
        });
    env["replace"] = [](const std::string& string, const std::string& keyword,
                        const std::string& replacement) {
        auto result = string;
        replace(result, keyword, replacement);
        return result;
    };
    env["applyAttackDamageRatio"] = &applyAttackDamageRatio;
    env["computeAttackDamageRatio"] = [](const bindings::AttackView& attack, int targetCount) {
        return computeAttackDamageRatio(getCustomAttackData(attack.getAttack()), targetCount);
    };
    env["getBoostDamage"] = &getBoostDamage;
    env["getLowerDamage"] = &getLowerDamage;
    env["getLowerInitiative"] = &getLowerInitiative;
    env["getCustomAttackSources"] = []() {
        std::vector<game::AttackSourceId> result;
        result.reserve(getCustomAttacks().sources.size());
        for (const auto& custom : getCustomAttacks().sources) {
            result.push_back(custom.source.id);
        }
        return result;
    };
    env["getCustomAttackSourceName"] = [](game::AttackSourceId id) {
        for (const auto& custom : getCustomAttacks().sources) {
            if (id == custom.source.id)
                return getInterfaceText(custom.nameId.c_str());
        }
        return std::string();
    };
    env["getCustomAttackReachName"] = [](game::AttackReachId id) {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id)
                return getInterfaceText(custom.reachTxt.c_str());
        }
        return std::string();
    };
    env["getCustomAttackTargetsName"] = [](game::AttackReachId id) {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id)
                return getInterfaceText(custom.targetsTxt.c_str());
        }
        return std::string();
    };

#define FUNCTION(_NAME_) this->##_NAME_ = getScriptFunction(env, #_NAME_);
    FUNCTION(getTxtStatsText)
    FUNCTION(getTxtStats2Text)
    FUNCTION(getTxtLeaderInfoText)
    FUNCTION(getTxtAttackInfoText)
}

} // namespace hooks
