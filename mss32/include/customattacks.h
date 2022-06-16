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
#include "midgardid.h"
#include <map>
#include <string>
#include <vector>

namespace hooks {

static const char damageRatioColumnName[] = "DAM_RATIO";
static const char damageRatioPerTargetColumnName[] = "DR_REPEAT";
static const char damageSplitColumnName[] = "DAM_SPLIT";
static const char critDamageColumnName[] = "CRIT_DAM";
static const char critPowerColumnName[] = "CRIT_POWER";

struct CustomAttackData
{
    std::uint8_t damageRatio;
    bool damageRatioPerTarget;
    bool damageSplit;
    std::uint8_t critDamage;
    std::uint8_t critPower;
};

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
    bool melee;
    std::uint32_t maxTargets;
};

using CustomAttackReaches = std::vector<CustomAttackReach>;

using CustomAttackDamageRatios = std::map<game::CMidgardID, double>;

struct CustomAttacks
{
    CustomAttackSources sources;
    CustomAttackReaches reaches;
    std::map<game::CMidgardID, CustomAttackDamageRatios> damageRatios; // Mapped by attack id
    std::vector<game::CMidgardID> targets;
    struct
    {
        game::CMidgardID unitId;
        std::uint32_t turnCount;
        bool used;
    } freeTransformSelf;
    bool damageRatiosEnabled;
    bool critSettingsEnabled;
};

void initializeCustomAttacks();
CustomAttacks& getCustomAttacks();

} // namespace hooks

#endif // CUSTOMATTACKS_H
