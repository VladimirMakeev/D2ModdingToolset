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

#include "battlemsgdatahooks.h"
#include "log.h"
#include "modifierutils.h"
#include "originalfunctions.h"
#include <atomic>
#include <fmt/format.h>

namespace hooks {

class ModifiedUnitsPatchedFactory
{
public:
    ModifiedUnitsPatchedFactory()
        : count(0)
    { }

    ~ModifiedUnitsPatchedFactory()
    {
        if (count != 0) {
            logError("mssProxyError.log",
                     fmt::format("{:d} instances of ModifiedUnitsPatched remained on finalization",
                                 count));
        }
    }

    game::ModifiedUnitInfo* create()
    {
        count++;
        return new game::ModifiedUnitInfo[game::ModifiedUnitCountPatched];
    }

    void destroy(game::ModifiedUnitInfo* value)
    {
        count--;
        delete[] value;
    }

private:
    std::atomic<int> count;
} modifiedUnitsPatchedFactory;

game::BattleMsgData* __fastcall battleMsgDataCtorHooked(game::BattleMsgData* thisptr, int /*%edx*/)
{
    using namespace game;

    getOriginalFunctions().battleMsgDataCtor(thisptr);

    for (auto& unitInfo : thisptr->unitsInfo) {
        memset(&unitInfo.modifiedUnits, 0, sizeof(ModifiedUnitsPatched));
        unitInfo.modifiedUnits.patched = modifiedUnitsPatchedFactory.create();
        resetModifiedUnitsInfo(&unitInfo);
    }

    return thisptr;
}

game::BattleMsgData* __fastcall battleMsgDataCopyCtorHooked(game::BattleMsgData* thisptr,
                                                            int /*%edx*/,
                                                            const game::BattleMsgData* src)
{
    using namespace game;

    getOriginalFunctions().battleMsgDataCtor(thisptr);

    for (auto& unitInfo : thisptr->unitsInfo) {
        memset(&unitInfo.modifiedUnits, 0, sizeof(ModifiedUnitsPatched));
    }

    return battleMsgDataCopyHooked(thisptr, 0, src);
}

game::BattleMsgData* __fastcall battleMsgDataCopyAssignHooked(game::BattleMsgData* thisptr,
                                                              int /*%edx*/,
                                                              const game::BattleMsgData* src)
{
    using namespace game;

    if (thisptr == src)
        return thisptr;

    const size_t count = std::size(thisptr->unitsInfo);
    std::vector<ModifiedUnitInfo*> prev(count);
    for (size_t i = 0; i < count; i++) {
        prev[i] = thisptr->unitsInfo[i].modifiedUnits.patched;
    }

    *thisptr = *src;

    for (size_t i = 0; i < count; i++) {
        memcpy(prev[i], src->unitsInfo[i].modifiedUnits.patched,
               sizeof(ModifiedUnitInfo) * ModifiedUnitCountPatched);
        thisptr->unitsInfo[i].modifiedUnits.patched = prev[i];
    }

    return thisptr;
}

game::BattleMsgData* __fastcall battleMsgDataCopyHooked(game::BattleMsgData* thisptr,
                                                        int /*%edx*/,
                                                        const game::BattleMsgData* src)
{
    using namespace game;

    *thisptr = *src;

    const size_t count = std::size(thisptr->unitsInfo);
    for (size_t i = 0; i < count; i++) {
        auto modifiedUnits = modifiedUnitsPatchedFactory.create();
        memcpy(modifiedUnits, src->unitsInfo[i].modifiedUnits.patched,
               sizeof(ModifiedUnitInfo) * ModifiedUnitCountPatched);
        thisptr->unitsInfo[i].modifiedUnits.patched = modifiedUnits;
    }

    return thisptr;
}

void __fastcall battleMsgDataDtorHooked(game::BattleMsgData* thisptr, int /*%edx*/)
{
    using namespace game;

    for (auto& unitInfo : thisptr->unitsInfo) {
        modifiedUnitsPatchedFactory.destroy(unitInfo.modifiedUnits.patched);
        unitInfo.modifiedUnits.patched = nullptr;
    }
}

void resetUnitInfo(game::UnitInfo* unitInfo)
{
    using namespace game;

    unitInfo->unitId1 = invalidId;
    resetModifiedUnitsInfo(unitInfo);
    for (auto& modifierId : unitInfo->modifierIds) {
        modifierId = invalidId;
    }
}

void __fastcall removeUnitInfoHooked(game::BattleMsgData* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& battle = BattleMsgDataApi::get();

    size_t index;
    ModifiedUnitsPatched modifiedUnits{};
    const size_t count = std::size(thisptr->unitsInfo);
    for (index = 0; index < count; ++index) {
        if (thisptr->unitsInfo[index].unitId1 == *unitId) {
            modifiedUnits = thisptr->unitsInfo[index].modifiedUnits;
            break;
        }
    }

    for (size_t i = index; i + 1 < count; ++i) {
        memcpy(&thisptr->unitsInfo[i], &thisptr->unitsInfo[i + 1], sizeof(UnitInfo));
    }

    if (index < count) {
        auto lastInfo = &thisptr->unitsInfo[count - 1];
        lastInfo->modifiedUnits = modifiedUnits;
        resetUnitInfo(lastInfo);
    }

    while (battle.decreaseUnitAttacks(thisptr, unitId))
        ;
}

} // namespace hooks
