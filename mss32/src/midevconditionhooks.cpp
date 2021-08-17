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
#include "midcondownresource.h"
#include "originalfunctions.h"
#include <fmt/format.h>

namespace hooks {

game::CMidEvCondition* __stdcall createEventConditionFromCategoryHooked(
    const game::LEventCondCategory* category)
{
    using namespace game;

    if (category->id != customEventConditions().ownResource.category.id) {
        return getOriginalFunctions().createEventConditionFromCategory(category);
    }

    return createMidCondOwnResource();
}

void __stdcall eventConditionGetInfoStringHooked(game::String* info,
                                                 const game::IMidgardObjectMap* objectMap,
                                                 const game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().ownResource.category.id) {
        midCondOwnResourceGetInfoString(info, objectMap, eventCondition);
        return;
    }

    getOriginalFunctions().eventConditionGetInfoString(info, objectMap, eventCondition);
}

void __stdcall eventConditionGetDescriptionHooked(game::String* description,
                                                  const game::LEventCondCategory* category)
{
    const auto& ownResource = customEventConditions().ownResource;

    if (category->id == ownResource.category.id) {
        auto text = game::gameFunctions().getInterfaceText(&ownResource.description);

        game::StringApi::get().initFromStringN(description, text, std::strlen(text));
        return;
    }

    getOriginalFunctions().eventConditionGetDescription(description, category);
}

void __stdcall eventConditionGetBriefHooked(game::String* brief,
                                            const game::LEventCondCategory* category)
{
    const auto& ownResource = customEventConditions().ownResource;

    if (category->id == ownResource.category.id) {
        auto text = game::gameFunctions().getInterfaceText(&ownResource.brief);

        game::StringApi::get().initFromStringN(brief, text, std::strlen(text));
        return;
    }

    getOriginalFunctions().eventConditionGetBrief(brief, category);
}

} // namespace hooks
