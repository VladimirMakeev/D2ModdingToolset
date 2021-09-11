/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "midevconditionhooks.h"
#include "d2string.h"
#include "eventconditioncathooks.h"
#include "game.h"
#include "midcondgamemode.h"
#include "midcondownresource.h"
#include "midcondplayertype.h"
#include "midcondscript.h"
#include "midcondvarcmp.h"
#include "originalfunctions.h"
#include <fmt/format.h>

namespace hooks {

game::CMidEvCondition* __stdcall createEventConditionFromCategoryHooked(
    const game::LEventCondCategory* category)
{
    const auto& conditions = customEventConditions();
    const auto id = category->id;

    if (id == conditions.ownResource.category.id) {
        return createMidCondOwnResource();
    }

    if (id == conditions.gameMode.category.id) {
        return createMidCondGameMode();
    }

    if (id == conditions.playerType.category.id) {
        return createMidCondPlayerType();
    }

    if (id == conditions.variableCmp.category.id) {
        return createMidCondVarCmp();
    }

    if (id == conditions.script.category.id) {
        return createMidCondScript();
    }

    return getOriginalFunctions().createEventConditionFromCategory(category);
}

void __stdcall eventConditionGetInfoStringHooked(game::String* info,
                                                 const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidEvCondition* eventCondition)
{
    const auto& conditions = customEventConditions();
    const auto id = eventCondition->category.id;

    if (id == conditions.ownResource.category.id) {
        midCondOwnResourceGetInfoString(info, objectMap, eventCondition);
        return;
    }

    if (id == conditions.gameMode.category.id) {
        midCondGameModeGetInfoString(info, objectMap, eventCondition);
        return;
    }

    if (id == conditions.playerType.category.id) {
        midCondPlayerTypeGetInfoString(info, objectMap, eventCondition);
        return;
    }

    if (id == conditions.variableCmp.category.id) {
        midCondVarCmpGetInfoString(info, objectMap, eventCondition);
        return;
    }

    if (id == conditions.script.category.id) {
        midCondScriptGetInfoString(info, objectMap, eventCondition);
        return;
    }

    getOriginalFunctions().eventConditionGetInfoString(info, objectMap, eventCondition);
}

void __stdcall eventConditionGetDescriptionHooked(game::String* description,
                                                  const game::LEventCondCategory* category)
{
    using namespace game;

    const auto& conditions = customEventConditions();
    const auto id = category->id;
    const CMidgardID* descriptionId{nullptr};

    if (id == conditions.ownResource.category.id) {
        descriptionId = &conditions.ownResource.description;
    } else if (id == conditions.gameMode.category.id) {
        descriptionId = &conditions.gameMode.description;
    } else if (id == conditions.playerType.category.id) {
        descriptionId = &conditions.playerType.description;
    } else if (id == conditions.variableCmp.category.id) {
        descriptionId = &conditions.variableCmp.description;
    } else if (id == conditions.script.category.id) {
        descriptionId = &conditions.script.description;
    }

    if (descriptionId) {
        auto text = gameFunctions().getInterfaceText(descriptionId);

        StringApi::get().initFromStringN(description, text, std::strlen(text));
        return;
    }

    getOriginalFunctions().eventConditionGetDescription(description, category);
}

void __stdcall eventConditionGetBriefHooked(game::String* brief,
                                            const game::LEventCondCategory* category)
{
    using namespace game;

    const auto& conditions = customEventConditions();
    const auto id = category->id;
    const CMidgardID* briefId{nullptr};

    if (id == conditions.ownResource.category.id) {
        briefId = &conditions.ownResource.brief;
    } else if (id == conditions.gameMode.category.id) {
        briefId = &conditions.gameMode.brief;
    } else if (id == conditions.playerType.category.id) {
        briefId = &conditions.playerType.brief;
    } else if (id == conditions.variableCmp.category.id) {
        briefId = &conditions.variableCmp.brief;
    } else if (id == conditions.script.category.id) {
        briefId = &conditions.script.brief;
    }

    if (briefId) {
        auto text = gameFunctions().getInterfaceText(briefId);

        StringApi::get().initFromStringN(brief, text, std::strlen(text));
        return;
    }

    getOriginalFunctions().eventConditionGetBrief(brief, category);
}

} // namespace hooks
