/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#include "netsingleplayerhooks.h"
#include "commandmsg.h"
#include "log.h"
#include "logutils.h"
#include "netsingleplayer.h"
#include "originalfunctions.h"
#include <fmt/format.h>
#include <process.h>

namespace hooks {

bool __fastcall netSinglePlayerSendMessageHooked(game::CNetSinglePlayer* thisptr,
                                                 int /*%edx*/,
                                                 int idTo,
                                                 const game::NetMessageHeader* message)
{
    auto logFileName = fmt::format("netMessages{:d}.log", _getpid());
    logDebug(logFileName,
             fmt::format("{:08x} ({:s})\t-> {:08x} ({:s})\t{:s}\tsender name: {:s}", thisptr->netId,
                         getNetPlayerIdDesc(thisptr->netId), idTo, getNetPlayerIdDesc(idTo),
                         message->messageClassName, thisptr->playerName.string));

    return getOriginalFunctions().netSinglePlayerSendMessage(thisptr, idTo, message);
}

} // namespace hooks
