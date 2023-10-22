/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef DIPLOMACYVIEW_H
#define DIPLOMACYVIEW_H

#include <cstdint>

namespace sol {
class state;
}

namespace game {
struct CMidDiplomacy;
}

namespace bindings {

enum class RelationType : int
{
    War,
    Neutral,
    Peace,
};

class DiplomacyView
{
public:
    DiplomacyView(const game::CMidDiplomacy* diplomacy);

    static void bind(sol::state& lua);

    std::uint32_t getCurrentRelation(int race1CategoryId, int race2CategoryId) const;
    std::uint32_t getPreviousRelation(int race1CategoryId, int race2CategoryId) const;

    bool getAlliance(int race1CategoryId, int race2CategoryId) const;
    std::uint32_t getAllianceTurn(int race1CategoryId, int race2CategoryId) const;
    
    bool getAlwaysAtWar(int race1CategoryId, int race2CategoryId) const;
    bool getAiCouldNotBreakAlliance(int race1CategoryId, int race2CategoryId) const;

    int getRelationType(std::uint32_t relations) const;

private:
    const game::CMidDiplomacy* diplomacy;
};

}

#endif // DIPLOMACYVIEW_H