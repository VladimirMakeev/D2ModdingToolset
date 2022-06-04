/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef SCENARIOINFO_H
#define SCENARIOINFO_H

#include "d2list.h"
#include "difficultylevel.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "racecategory.h"
#include <cstddef>

namespace game {

/** Holds general information about scenario in scenario file and game. */
struct CScenarioInfo : public IMidScenarioObject
{
    char* name;
    char* description;
    char* briefing;
    char* briefLong;
    char* debunkW;
    char* debunkL;
    int currentTurn;
    char official;
    char sourceM;
    char padding[2];
    char* creator;
    int mapSize;
    int qtyCities;
    LDifficultyLevel scenarioDifficulty;
    LDifficultyLevel gameDifficulty;
    List<LRaceCategory> races;
    CMidgardID campaignId;
    int spellMaxTier;
    int unitMaxTier;
    int leaderMaxLevel;
    int cityMaxTier;
    int suggestedLevel;
    int mapSeed;
};

assert_size(CScenarioInfo, 120);
assert_offset(CScenarioInfo, scenarioDifficulty, 52);
assert_offset(CScenarioInfo, unitMaxTier, 100);

} // namespace game

#endif // SCENARIOINFO_H
