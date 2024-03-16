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

#ifndef MIDPLAYER_H
#define MIDPLAYER_H

#include "aiattitudescat.h"
#include "currency.h"
#include "d2set.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "stringandid.h"

namespace game {

struct TRaceType;

/** Holds player related data in scenario file and game. */
struct CMidPlayer : public IMidScenarioObject
{
    StringAndId name;
    StringAndId description;
    int face;
    CMidgardID raceId;
    CMidgardID lordId;
    TRaceType* raceType;
    LAttitudesCategory attitude;
    int qtyBreaks;
    CMidgardID fogId;
    CMidgardID knownSpellsId;
    CMidgardID buildingsId;
    int researchTurn;
    int constructionTurn;
    Set<CMidgardID> spyData;
    Bank bank;
    Bank spellBank;
    bool isHuman;
    char padding[3];
    CMidgardID capturedById;
    bool alwaysAi;
    char padding2[3];
    Set<std::uint64_t> exmapData; // Each node contains 8 bytes of data
};

assert_size(CMidPlayer, 176);
assert_offset(CMidPlayer, bank, 112);
assert_offset(CMidPlayer, spellBank, 124);
assert_offset(CMidPlayer, isHuman, 136);

} // namespace game

#endif // MIDPLAYER_H
