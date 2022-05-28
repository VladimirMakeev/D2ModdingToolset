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

#include "midunitdescriptorhooks.h"
#include "attack.h"
#include "dynamiccast.h"
#include "game.h"
#include "midgardobjectmap.h"
#include "midstack.h"
#include "midunit.h"
#include "midunitdescriptor.h"
#include "unitutils.h"

namespace hooks {

game::IAttack* __fastcall midUnitDescriptorGetAttackHooked(const game::CMidUnitDescriptor* thisptr,
                                                           int /*%edx*/)
{
    auto unit = thisptr->unit;
    if (!unit)
        return nullptr;

    return getAttack(unit->unitImpl, true, true);
}

int __fastcall midUnitDescriptorGetAttackInitiativeHooked(const game::CMidUnitDescriptor* thisptr,
                                                          int /*%edx*/)
{
    using namespace game;

    const auto& descriptorApi = CMidUnitDescriptorApi::get();

    auto attack = descriptorApi.getAttack(thisptr);

    return attack->vftable->getInitiative(attack);
}

bool __fastcall midUnitDescriptorIsUnitLeaderHooked(const game::CMidUnitDescriptor* thisptr,
                                                    int /*%edx*/)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (CMidgardIDApi::get().getType(&thisptr->groupId) != IdType::Stack) {
        // Only stacks can contain leader units
        return false;
    }

    // Fix crash on viewing stack leader transformed to ordinary soldier
    if (fn.castUnitImplToStackLeader(thisptr->unit->unitImpl) == nullptr) {
        return false;
    }

    auto objectMap{thisptr->objectMap};
    auto stackObject{objectMap->vftable->findScenarioObjectById(objectMap, &thisptr->groupId)};
    if (!stackObject) {
        return false;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto stack{(const CMidStack*)dynamicCast(stackObject, 0, rtti.IMidScenarioObjectType,
                                             rtti.CMidStackType, 0)};
    if (!stack) {
        return false;
    }

    return stack->leaderId == thisptr->unitId;
}

} // namespace hooks
