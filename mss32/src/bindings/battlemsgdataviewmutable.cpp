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

#include "battlemsgdataviewmutable.h"
#include "battlemsgdata.h"
#include "gameutils.h"
#include "idview.h"
#include "playerview.h"
#include "stackview.h"
#include "unitview.h"
#include <sol/sol.hpp>

namespace bindings {

BattleMsgDataViewMutable::BattleMsgDataViewMutable(game::BattleMsgData* battleMsgData,
                                                   const game::IMidgardObjectMap* objectMap)
    : BattleMsgDataView(battleMsgData, objectMap)
    , battleMsgData{battleMsgData}
    , objectMap{objectMap}
{ }

void BattleMsgDataViewMutable::bind(sol::state& lua)
{
    auto view = lua.new_usertype<BattleMsgDataViewMutable>("BattleViewMutable", sol::base_classes,
                                                           sol::bases<BattleMsgDataView>());
    bindAccessMethods(view);

    view["setRetreatStatus"] = &BattleMsgDataViewMutable::setRetreatStatus;
    view["setDecidedToRetreat"] = &BattleMsgDataViewMutable::setDecidedToRetreat;
}

void BattleMsgDataViewMutable::setRetreatStatus(bool attacker, std::uint8_t value)
{
    game::BattleMsgDataApi::get().setRetreatStatus(battleMsgData, attacker,
                                                   static_cast<game::RetreatStatus>(value & 3u));
}

void BattleMsgDataViewMutable::setDecidedToRetreat()
{
    game::BattleMsgDataApi::get().setRetreatDecisionWasMade(battleMsgData);
}

} // namespace bindings
