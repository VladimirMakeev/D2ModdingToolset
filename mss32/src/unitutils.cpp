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

#include "unitutils.h"
#include "attack.h"
#include "attacksourcecat.h"
#include "attacksourcelist.h"
#include "customattacks.h"
#include "dynamiccast.h"
#include "game.h"
#include "globaldata.h"
#include "immunecat.h"
#include "log.h"
#include "midgardid.h"
#include "midunit.h"
#include "ummodifier.h"
#include "unitgenerator.h"
#include "ussoldier.h"
#include "ussoldierimpl.h"
#include "usunitimpl.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void generateUnitImplByAttackId(const game::CMidgardID* attackId)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();
    int typeIndex = id.getTypeIndex(attackId);
    int categoryIndex = id.getCategoryIndex(attackId);
    IdCategory category = id.getCategory(attackId);

    CMidgardID unitImplId{};
    id.fromParts(&unitImplId, category, categoryIndex, IdType::UnitGenerated, typeIndex);

    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->generateUnitImpl(unitGenerator, &unitImplId);
}

game::IUsSoldier* castUnitImplToSoldierWithLogging(const game::IUsUnit* unitImpl)
{
    using namespace game;

    auto soldier = gameFunctions().castUnitImplToSoldier(unitImpl);
    if (!soldier) {
        hooks::logError("mssProxyError.log", fmt::format("Failed to cast unit impl {:s} to soldier",
                                                         hooks::idToString(&unitImpl->unitId)));
    }

    return soldier;
}

void getSoldierAttackSourceImmunities(const game::LImmuneCat* immuneCat,
                                      const game::IUsSoldier* soldier,
                                      game::LinkedList<game::LAttackSource>* value)
{
    using namespace game;

    const auto& sources = AttackSourceCategories::get();
    const auto& sourceListApi = AttackSourceListApi::get();
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.weapon)->id)
        sourceListApi.add(value, sources.weapon);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.mind)->id)
        sourceListApi.add(value, sources.mind);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.life)->id)
        sourceListApi.add(value, sources.life);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.death)->id)
        sourceListApi.add(value, sources.death);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.fire)->id)
        sourceListApi.add(value, sources.fire);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.water)->id)
        sourceListApi.add(value, sources.water);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.air)->id)
        sourceListApi.add(value, sources.air);
    if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, sources.earth)->id)
        sourceListApi.add(value, sources.earth);

    for (const auto& custom : getCustomAttacks().sources) {
        if (immuneCat->id == soldier->vftable->getImmuneByAttackSource(soldier, &custom.source)->id)
            sourceListApi.add(value, &custom.source);
    }
}

bool isUnitSmall(const game::CMidUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    return soldier->vftable->getSizeSmall(soldier);
}

game::CMidgardID getGlobalUnitImplId(const game::CMidUnit* unit)
{
    using namespace game;

    CMidgardID globalImplId{};
    CUnitGenerator* unitGenerator = (*(GlobalDataApi::get().getGlobalData()))->unitGenerator;
    unitGenerator->vftable->getGlobalUnitImplId(unitGenerator, &globalImplId,
                                                &unit->unitImpl->unitId);

    return globalImplId;
}

game::TUsUnitImpl* getGlobalUnitImpl(const game::CMidUnit* unit)
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    const CMidgardID globalImplId{getGlobalUnitImplId(unit)};

    auto globalData = *globalApi.getGlobalData();
    auto result = (TUsUnitImpl*)globalApi.findById(globalData->units, &globalImplId);
    if (!result) {
        logError("mssProxyError.log",
                 fmt::format("Could not find unit impl {:s}", idToString(&globalImplId)));
    }

    return result;
}

game::TUsSoldierImpl* getSoldierImpl(const game::IUsSoldier* soldier)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto current = soldier;
    while (current) {
        auto soldierImpl = (TUsSoldierImpl*)dynamicCast(current, 0, rtti.IUsSoldierType,
                                                        rtti.TUsSoldierImplType, 0);
        if (soldierImpl)
            return soldierImpl;

        auto modifier = (CUmModifier*)dynamicCast(current, 0, rtti.IUsSoldierType,
                                                  rtti.CUmModifierType, 0);
        current = modifier ? fn.castUnitImplToSoldier(modifier->data->underlying) : nullptr;
    }

    return nullptr;
}

game::IAttack* getAttack(const game::IUsSoldier* soldier, bool primary, bool checkAltAttack)
{
    using namespace game;

    auto attack = primary ? soldier->vftable->getAttackById(soldier)
                          : soldier->vftable->getSecondAttackById(soldier);
    if (attack == nullptr)
        return nullptr;

    if (!checkAltAttack)
        return attack;

    auto altAttackId = attack->vftable->getAltAttackId(attack);
    if (*altAttackId == emptyId)
        return attack;

    auto soldierImpl = getSoldierImpl(soldier);

    // HACK: temporary swapping attack ids to get alt attack wrapped in CAttackModified
    auto attackId = soldierImpl->data->attackId;
    soldierImpl->data->attackId = *altAttackId;
    attack = soldier->vftable->getAttackById(soldier);
    soldierImpl->data->attackId = attackId;

    return attack;
}

} // namespace hooks
