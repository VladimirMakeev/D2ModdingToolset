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

#ifndef CUSTOMUNITENCYCLOPEDIA_H
#define CUSTOMUNITENCYCLOPEDIA_H

#include "categoryids.h"
#include "unitimplview.h"
#include "unitview.h"
#include <optional>
#include <sol/sol.hpp>
#include <string>
#include <vector>

namespace game {
struct IUsUnit;
struct CMidUnit;
} // namespace game

namespace hooks {

struct CustomUnitEncyclopediaFunctions;

class CustomUnitEncyclopedia
{
public:
    CustomUnitEncyclopedia();
    virtual ~CustomUnitEncyclopedia();

    std::string getTxtStatsText(const game::CMidUnit* unit,
                                const game::IUsUnit* unitImpl,
                                bool isMaxLevel,
                                bool isInBattle,
                                int fortificationArmor,
                                int shatteredArmor,
                                const std::vector<game::AttackSourceId>& removedSourceWards,
                                const std::vector<game::AttackClassId>& removedClassWards) const;
    std::string getTxtStats2Text(const game::CMidUnit* unit,
                                 const game::IUsUnit* unitImpl,
                                 bool isInBattle,
                                 int fortificationArmor,
                                 int shatteredArmor,
                                 int unitRegen,
                                 const std::string& pattern) const;
    std::string getTxtLeaderInfoText(const game::CMidUnit* unit,
                                     const game::IUsUnit* unitImpl) const;
    std::string getTxtAttackInfoText(const game::CMidUnit* unit,
                                     const game::IUsUnit* unitImpl,
                                     int boostDamageLevel,
                                     int lowerDamageLevel,
                                     int lowerInitiativeLevel) const;

protected:
    template <typename... Args>
    std::string getValue(std::optional<sol::function> function,
                         const game::CMidUnit* unit,
                         const game::IUsUnit* unitImpl,
                         Args&&... args) const
    {
        try {
            if (function && unitImpl) {
                auto unitView = unit ? std::optional<bindings::UnitView>(unit) : std::nullopt;
                bindings::UnitImplView unitImplView{unitImpl};
                return (*function)(unitView, unitImplView, std::forward<Args>(args)...);
            }
        } catch (const std::exception& e) {
            showScriptErrorMessage(e.what());
        }

        return "";
    }

    void showScriptErrorMessage(const char* reason) const;

private:
    const CustomUnitEncyclopediaFunctions& getFunctions() const;

    mutable CustomUnitEncyclopediaFunctions* mainThreadFunctions;
    mutable CustomUnitEncyclopediaFunctions* workerThreadFunctions;
};

CustomUnitEncyclopedia& getCustomUnitEncyclopedia();

} // namespace hooks

#endif // CUSTOMUNITENCYCLOPEDIA_H
