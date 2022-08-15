/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "itemutils.h"
#include "globaldata.h"
#include "itembattle.h"
#include "itemequipment.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "miditem.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

const game::CItemBase* getGlobalItemById(const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* itemId)
{
    using namespace game;

    auto item = static_cast<CMidItem*>(
        objectMap->vftable->findScenarioObjectById(objectMap, itemId));
    if (!item) {
        logError("mssProxyError.log", fmt::format("Could not find item {:s}", idToString(itemId)));
        return nullptr;
    }

    const auto& global = GlobalDataApi::get();
    auto globalData = *global.getGlobalData();

    auto globalItem = global.findItemById(globalData->itemTypes, &item->globalItemId);
    if (!globalItem) {
        logError("mssProxyError.log",
                 fmt::format("Could not find global item {:s}", idToString(&item->globalItemId)));
        return nullptr;
    }

    return globalItem;
}

game::CMidgardID getAttackId(const game::IItem* item)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto itemBattle = (CItemBattle*)dynamicCast(item, 0, rtti.IItemType, rtti.CItemBattleType, 0);
    if (itemBattle) {
        return itemBattle->attackId;
    }

    auto itemEquipment = (CItemEquipment*)dynamicCast(item, 0, rtti.IItemType,
                                                      rtti.CItemEquipmentType, 0);
    if (itemEquipment) {
        return itemEquipment->attackId;
    }

    return emptyId;
}

} // namespace hooks
