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

#include "globalvariableshooks.h"
#include "dbffile.h"
#include "globalvariables.h"
#include "log.h"
#include "mempool.h"
#include "originalfunctions.h"
#include <cstring>
#include <filesystem>
#include <fmt/format.h>

namespace hooks {

static const char stealRmktColumn[]{"STEAL_RMKT"};
static const char rmktRiotMinColumn[]{"RMKT_R_MIN"};
static const char rmktRiotMaxColumn[]{"RMKT_R_MAX"};

game::GlobalVariables* __fastcall globalVariablesCtorHooked(game::GlobalVariables* thisptr,
                                                            int /*%edx*/,
                                                            const char* directory,
                                                            game::CProxyCodeBaseEnv* proxy)
{
    using namespace game;

    static const char dbfFileName[]{"GVars.dbf"};

    const auto originalCtor{getOriginalFunctions().globalVariablesCtor};

    // We can not open database right after original c-tor, file is still being locked
    utils::DbfFile dbfFile;
    if (!dbfFile.open(std::filesystem::path{directory} / dbfFileName)) {
        logError("mssProxyError.log", fmt::format("Could not open {:s}", dbfFileName));
        return originalCtor(thisptr, directory, proxy);
    }

    if (!dbfFile.column(stealRmktColumn) || !dbfFile.column(rmktRiotMinColumn)
        || !dbfFile.column(rmktRiotMaxColumn)) {
        return originalCtor(thisptr, directory, proxy);
    }

    utils::DbfRecord record;
    if (!dbfFile.record(record, 0u) || record.isDeleted()) {
        return originalCtor(thisptr, directory, proxy);
    }

    const auto extendedDataSize{sizeof(GlobalVariablesDataHooked)};

    auto extendedData = (GlobalVariablesDataHooked*)Memory::get().allocate(extendedDataSize);
    std::memset(extendedData, 0, extendedDataSize);

    int stealAmount{};
    if (record.value(stealAmount, stealRmktColumn)) {
        extendedData->stealRmkt = std::clamp(stealAmount, 0, INT_MAX);
    }

    int riotMin{};
    if (record.value(riotMin, rmktRiotMinColumn)) {
        extendedData->rmktRiotMin = std::clamp(riotMin, 0, INT_MAX);
    }

    int riotMax{};
    if (record.value(riotMax, rmktRiotMaxColumn)) {
        extendedData->rmktRiotMax = std::clamp(riotMax, 0, INT_MAX);
    }

    originalCtor(thisptr, directory, proxy);

    // Copy original variables into extended data structure and assign it instead of original.
    // We do not free memory of GlobalVariablesData::tutorialName here, original d-tor will do
    std::memcpy(extendedData, thisptr->data, sizeof(GlobalVariablesData));
    Memory::get().freeNonZero(thisptr->data);

    thisptr->data = extendedData;

    return thisptr;
}

} // namespace hooks
