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

#include "netmsgutils.h"
#include "battlemsgdata.h"
#include "mqstream.h"

namespace hooks {

void serializeMsgWithBattleMsgData(game::CNetMsg* msg,
                                   game::BattleMsgData* battleMsgData,
                                   game::CNetMsgVftable::Serialize method,
                                   game::CMqStream* stream)
{
    using namespace game;

    if (stream->read) {
        const size_t count = sizeof(battleMsgData->unitsInfo) / sizeof(*battleMsgData->unitsInfo);
        ModifiedUnitInfo* prev[count];
        for (size_t i = 0; i < count; i++) {
            prev[i] = battleMsgData->unitsInfo[i].modifiedUnits.patched;
        }

        method(msg, stream);

        for (size_t i = 0; i < count; i++) {
            battleMsgData->unitsInfo[i].modifiedUnits.patched = prev[i];
        }
    } else {
        method(msg, stream);
    }

    for (auto& unitInfo : battleMsgData->unitsInfo) {
        stream->vftable->serialize(stream, unitInfo.modifiedUnits.patched,
                                   sizeof(ModifiedUnitInfo) * ModifiedUnitInfoCountPatched);
    }
}

} // namespace hooks
