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

#include "categories.h"
#include "currency.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "sortedlist.h"
#include "stringandid.h"

namespace game {

struct TRaceType;

/** Holds player related data in scenario file and game. */
struct CMidPlayer : public IMidScenarioObject
{
    CMidgardID playerId;
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
    SortedList<CMidgardID> spyData;
    Bank bank;
    Bank spellBank;
    bool isHuman;
    char padding[3];
    CMidgardID capturedById;
    bool alwaysAi;
    char padding2[3];
    SortedList<std::uint64_t> exmapData; // Each node contains 8 bytes of data
};

static_assert(sizeof(CMidPlayer) == 176, "Size of CMidPlayer structure must be exactly 176 bytes");

static_assert(offsetof(CMidPlayer, bank) == 112, "CMidPlayer::bank offset must be 112 bytes");

static_assert(offsetof(CMidPlayer, spellBank) == 124,
              "CMidPlayer::spellBank offset must be 124 bytes");

static_assert(offsetof(CMidPlayer, isHuman) == 136, "CMidPlayer::isHuman offset must be 136 bytes");

} // namespace game

#endif // MIDPLAYER_H
