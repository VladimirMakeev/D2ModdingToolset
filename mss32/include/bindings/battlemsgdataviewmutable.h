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

#ifndef BATTLEMSGDATAVIEWMUTABLE_H
#define BATTLEMSGDATAVIEWMUTABLE_H

#include "battlemsgdataview.h"

namespace bindings {

class BattleMsgDataViewMutable : public BattleMsgDataView
{
public:
    BattleMsgDataViewMutable(game::BattleMsgData* battleMsgData,
                             const game::IMidgardObjectMap* objectMap);

    static void bind(sol::state& lua);

    void setRetreatStatus(bool attacker, std::uint8_t value);
    void setDecidedToRetreat();

private:
    game::BattleMsgData* battleMsgData;
    const game::IMidgardObjectMap* objectMap;
};

} // namespace bindings

#endif // BATTLEMSGDATAVIEWMUTABLE_H
