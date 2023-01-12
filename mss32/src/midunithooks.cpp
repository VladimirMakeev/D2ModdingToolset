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
#include "campaignstream.h"
#include "custommodifier.h"
#include "custommodifiers.h"
#include "dynamiccast.h"
#include "midgardstream.h"
#include "modifierutils.h"
#include "originalfunctions.h"
#include "settings.h"
#include "streamutils.h"
#include "stringandid.h"
#include "stringpairarrayptr.h"
#include "ummodifier.h"
#include "unitmodifier.h"
#include "unitutils.h"
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

    if (userSettings().modifiers.notifyModifiersChanged) {
        notifyModifiersChanged(thisptr->unitImpl);
    }

    return true;
}

bool __fastcall removeModifierHooked(game::CMidUnit* thisptr,
                                     int /*%edx*/,
                                     const game::CMidgardID* modifierId)
{
    using namespace game;

    const auto& umModifierApi = CUmModifierApi::get();

    if (!thisptr) {
        return false;
    }

    CUmModifier* modifier = nullptr;
    for (auto curr = thisptr->unitImpl; curr; curr = modifier->data->prev) {
        modifier = castUnitToUmModifier(curr);
        if (!modifier) {
            break;
        }

        if (modifier->data->modifierId == *modifierId) {
            auto prev = modifier->data->prev;
            auto next = modifier->data->next;
            if (next) {
                umModifierApi.setPrev(next, prev);
            } else {
                thisptr->unitImpl = prev;
            }

            auto prevModifier = castUnitToUmModifier(prev);
            if (prevModifier) {
                prevModifier->data->next = next;
            }

            if (userSettings().modifiers.notifyModifiersChanged) {
                notifyModifiersChanged(thisptr->unitImpl);
            }

            modifier->vftable->destructor(modifier, true);
            return true;
        }
    }

    return false;
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

bool __fastcall upgradeHooked(game::CMidUnit* thisptr,
                              int /*%edx*/,
                              const game::CScenarioVisitor* visitor,
                              const game::CMidgardID* upgradeImplId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& unitApi = CMidUnitApi::get();
    const auto& listApi = IdListApi::get();
    const auto& rttiApi = RttiApi::get();
    const auto& typeIdOperator = *rttiApi.typeIdOperator;
    const auto& typeInfoInequalityOperator = *rttiApi.typeInfoInequalityOperator;

    auto upgradeUnitImpl = getUnitImpl(upgradeImplId);
    if (!upgradeUnitImpl)
        return false;

    auto unitImpl = getUnitImpl(thisptr->unitImpl);
    if (typeInfoInequalityOperator(typeIdOperator(unitImpl), typeIdOperator(upgradeUnitImpl)))
        return false;

    IdList modifierIds{};
    listApi.constructor(&modifierIds);
    if (!unitApi.getModifiers(&modifierIds, thisptr) || !unitApi.removeModifiers(&thisptr->unitImpl)
        || !unitApi.replaceImpl(&thisptr->unitImpl, upgradeUnitImpl)
        || !unitApi.addModifiers(&modifierIds, thisptr, nullptr, true)) {
        listApi.destructor(&modifierIds);
        return false;
    }
    listApi.destructor(&modifierIds);

    auto soldier = fn.castUnitImplToSoldier(thisptr->unitImpl);
    thisptr->currentHp = soldier->vftable->getHitPoints(soldier);
    thisptr->currentXp = 0;
    return true;
}

bool __fastcall initWithSoldierImplHooked(game::CMidUnit* thisptr,
                                          int /*%edx*/,
                                          const game::IMidgardObjectMap* objectMap,
                                          const game::CMidgardID* unitImplId,
                                          const int* turn)
{
    using namespace game;

    if (!getOriginalFunctions().initWithSoldierImpl(thisptr, objectMap, unitImplId, turn)) {
        return false;
    }

    for (const auto& modifierId : getNativeModifiers(thisptr->unitImpl->id)) {
        CMidUnitApi::get().addModifier(thisptr, &modifierId);
    }

    return true;
}

void __fastcall midUnitStreamHooked(game::CMidUnit* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    game::IMidgardStreamEnv* streamEnv)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& stringPairArrayPtrApi = StringPairArrayPtrApi::get();
    const auto& campaignStreamApi = CampaignStreamApi::get();
    const auto& midUnitApi = CMidUnitApi::get();

    StringPairArrayPtr arrayPtr;
    stringPairArrayPtrApi.constructor(&arrayPtr);

    CampaignStream campaignStream;
    static const char* dbfFileName = "SUnit.dbf";
    campaignStreamApi.constructor(&campaignStream, streamEnv, &dbfFileName, &arrayPtr);
    stringPairArrayPtrApi.setData(&arrayPtr, nullptr);
    if (campaignStreamApi.hasErrors(&campaignStream)) {
        fn.throwScenarioException("", "Invalid Stream");
    }

    auto& stream = campaignStream.stream;
    auto& streamVPtr = stream->vftable;
    streamVPtr->enterRecord(stream);
    streamVPtr->streamId(stream, "UNIT_ID", &thisptr->id);
    midUnitApi.streamImpl(&stream, &thisptr->unitImpl);
    midUnitApi.streamModifiers(&stream, &thisptr->id, thisptr);
    streamTurn(stream, "CREATION", &thisptr->creation);
    streamVPtr->streamText(stream, "NAME_TXT", &thisptr->name, 0);
    streamVPtr->streamBool(stream, "TRANSF", &thisptr->transformed);

    if (thisptr->transformed) {
        streamVPtr->streamId(stream, "ORIGTYPEID", &thisptr->origTypeId);
        streamVPtr->streamBool(stream, "KEEP_HP", &thisptr->keepHp);
        streamVPtr->streamInt(stream, "ORIG_XP", &thisptr->origXp);
        streamVPtr->streamInt(stream, "HP_BEFORE", &thisptr->hpBefore);
        streamVPtr->streamInt(stream, "HP_BEF_MAX", &thisptr->hpBefMax);
    }

    if (streamVPtr->hasValue(stream, "DYNLEVEL")) {
        streamVPtr->streamBool(stream, "DYNLEVEL", &thisptr->dynLevel);
    } else {
        thisptr->dynLevel = false;
    }

    streamVPtr->streamInt(stream, "HP", &thisptr->currentHp);
    if (thisptr->currentHp < 0) {
        thisptr->currentHp = 0;
    }

    streamVPtr->streamInt(stream, "XP", &thisptr->currentXp);
    if (thisptr->currentXp < 0) {
        thisptr->currentXp = 0;
    }

    // Moved calls of soldier->getHitPoints / getXpNext to later phase of scenario load
    // (see loadScenarioMapHooked and scenarioMapStreamHooked), because of an issue with custom
    // modifiers where corresponding scripts are trying to access unit stack or other scenario
    // objects while the scenario is being loaded and expected objects are not accessible yet.

    streamVPtr->leaveRecord(stream);
    campaignStreamApi.destructor(&campaignStream);
}

bool __stdcall getModifiersHooked(game::IdList* value, const game::CMidUnit* unit)
{
    using namespace game;

    const auto& listApi = IdListApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    listApi.clear(value);

    NativeModifiers nativeModifiers = getNativeModifiers(unit->unitImpl->id);

    CUmModifier* modifier = nullptr;
    for (auto curr = unit->unitImpl; curr; curr = modifier->data->prev) {
        modifier = (CUmModifier*)dynamicCast(curr, 0, rtti.IUsUnitType, rtti.CUmModifierType, 0);
        if (!modifier)
            break;

        auto modifierId = modifier->data->modifierId;
        auto it = std::find(nativeModifiers.begin(), nativeModifiers.end(), modifierId);
        if (it != nativeModifiers.end()) {
            // Count skipped in case if native mod is duplicated by normal one
            nativeModifiers.erase(it);
            continue;
        }

        listApi.pushFront(value, &modifierId);
    }

    return true;
}

bool __stdcall addModifiersHooked(const game::IdList* value,
                                  game::CMidUnit* unit,
                                  char* errorBuffer,
                                  bool skipInapplicable)
{
    using namespace game;

    for (const auto& modifierId : getNativeModifiers(unit->unitImpl->id)) {
        CMidUnitApi::get().addModifier(unit, &modifierId);
    }

    return getOriginalFunctions().addModifiers(value, unit, errorBuffer, skipInapplicable);
}

bool __stdcall removeModifiersHooked(game::IUsUnit** unitImpl)
{
    using namespace game;

    for (auto curr = *unitImpl; curr; curr = *unitImpl) {
        auto modifier = castUnitToUmModifier(curr);
        if (!modifier) {
            break;
        }

        *unitImpl = modifier->data->prev;

        auto prevModifier = castUnitToUmModifier(*unitImpl);
        if (prevModifier) {
            prevModifier->data->next = nullptr;
        }

        modifier->vftable->destructor(modifier, true);
    }

    return true;
}

} // namespace hooks
