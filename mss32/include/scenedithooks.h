/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef SCENEDITHOOKS_H
#define SCENEDITHOOKS_H

#include <vector>
#include <string>
#include <utility>

namespace game {
struct CScenEdit;
}

namespace hooks {

using NameDescPair = std::pair<std::string /* name */, std::string /* description */>;
using MarketNames = std::vector<NameDescPair>;

bool __fastcall readScenDataHooked(game::CScenEdit* thisptr, int /*%edx*/);

const MarketNames& getMarketNames();

}

#endif // SCENEDITHOOKS_H
