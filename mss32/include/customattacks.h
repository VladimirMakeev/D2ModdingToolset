/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef CUSTOMATTACKS_H
#define CUSTOMATTACKS_H

#include "attackreachcat.h"
#include "attacksourcecat.h"
#include <string>
#include <vector>

namespace hooks {

static const char damageRatioColumnName[] = "DAM_RATIO";
static const char damageRatioPerTargetColumnName[] = "DR_REPEAT";

struct CustomAttackSource
{
    game::LAttackSource source;
    std::string text;
    std::string nameId;
    double immunityAiRating;
    std::uint32_t wardFlagPosition;
};

using CustomAttackSources = std::vector<CustomAttackSource>;

struct CustomAttackReach
{
    game::LAttackReach reach;
    std::string text;
    std::string reachTxt;
    std::string targetsTxt;
    std::string selectionScript;
    std::string attackScript;
    bool markAttackTargets;
};

using CustomAttackReaches = std::vector<CustomAttackReach>;

struct CustomAttacks
{
    CustomAttackSources sources;
    CustomAttackReaches reaches;
    bool customizeDamageRatio;
};

CustomAttacks& getCustomAttacks();

} // namespace hooks

#endif // CUSTOMATTACKS_H
