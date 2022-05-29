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

#include "midunithooks.h"
#include "custommodifier.h"
#include "modifierutils.h"
#include "originalfunctions.h"
#include "stringandid.h"
#include "ummodifier.h"
#include "unitmodifier.h"
#include "ussoldier.h"
#include "usunit.h"
#include "usunitimpl.h"

namespace hooks {

bool __fastcall addModifierHooked(game::CMidUnit* thisptr,
                                  int /*%edx*/,
                                  const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto unitModifier = getUnitModifier(modifierId);
    if (!unitModifier || !unitModifier->vftable->canApplyToUnit(unitModifier, thisptr->unitImpl))
        return false;

    auto modifier = unitModifier->vftable->createModifier(unitModifier);

    auto prevModifier = castUnitToUmModifier(thisptr->unitImpl);
    if (prevModifier)
        prevModifier->data->next = modifier;

    CUmModifierApi::get().setPrev(modifier, thisptr->unitImpl);

    auto customModifier = castModifierToCustomModifier(modifier);
    if (customModifier)
        customModifier->setUnit(thisptr);

    thisptr->unitImpl = castUmModifierToUnit(modifier);
    return true;
}

bool __fastcall removeModifierHooked(game::CMidUnit* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* modifierId)
{
    if (!thisptr) {
        return false;
    }

    return getOriginalFunctions().removeModifier(thisptr, modifierId);
}

bool __fastcall transformHooked(game::CMidUnit* thisptr,
                                int /*%edx*/,
                                const game::CScenarioVisitor* visitor,
                                const game::IMidgardObjectMap* objectMap,
                                const game::CMidgardID* transformImplId,
                                bool keepHp)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& unitApi = CMidUnitApi::get();
    const auto& global = GlobalDataApi::get();

    if (thisptr->transformed && !unitApi.untransform(thisptr, visitor))
        return false;

    if (thisptr->origTypeId == emptyId) {
        auto soldier = fn.castUnitImplToSoldier(thisptr->unitImpl);

        thisptr->transformed = true;
        thisptr->origTypeId = thisptr->unitImpl->id;
        thisptr->keepHp = keepHp;
        thisptr->hpBefore = thisptr->currentHp;
        thisptr->hpBefMax = soldier->vftable->getHitPoints(soldier);
        thisptr->origXp = thisptr->currentXp;
        unitApi.getModifiers(&thisptr->origModifiers, thisptr);
    }

    auto globalData = *global.getGlobalData();
    auto globalUnitImpl = static_cast<TUsUnitImpl*>(
        global.findById(globalData->units, &thisptr->unitImpl->id));

    auto globalTransformImpl = static_cast<TUsUnitImpl*>(
        global.findById(globalData->units, transformImplId));

    if (!fn.castUnitImplToStackLeader(globalUnitImpl)
        && fn.castUnitImplToStackLeader(globalTransformImpl)) {
        auto globalTransformSoldier = fn.castUnitImplToSoldier(globalTransformImpl);
        auto transformName = globalTransformSoldier->vftable->getName(globalTransformSoldier);
        StringAndIdApi::get().setString(&thisptr->name, transformName);
    }

    thisptr->currentXp = 0;

    if (!unitApi.removeModifiers(&thisptr->unitImpl)
        || !unitApi.replaceImpl(&thisptr->unitImpl, globalTransformImpl)
        || !unitApi.addModifiers(&thisptr->origModifiers, thisptr, nullptr, true)) {
        return false;
    }

    if (!keepHp) {
        // Fix unit transformation to include hp mods into current hp recalculation
        auto transformedSoldier = fn.castUnitImplToSoldier(thisptr->unitImpl);
        int transformedHp = transformedSoldier->vftable->getHitPoints(transformedSoldier);
        thisptr->currentHp = transformedHp * thisptr->hpBefore / thisptr->hpBefMax;
        if (thisptr->currentHp == 0)
            thisptr->currentHp = 1;
    }

    return true;
}

} // namespace hooks
