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

#include "taskobjprophooks.h"
#include "taskobjprop.h"
#include "midgardplan.h"
#include "gameutils.h"
#include "scenedit.h"
#include "midgardscenariomap.h"
#include "dynamiccast.h"
#include "mapelement.h"
#include "midsiteresourcemarket.h"
#include "interfmanager.h"
#include "originalfunctions.h"
#include "resourcemarketinterface.h"

namespace hooks {

static game::IMapElement* getMapElementAtPosition(const game::CMqPoint* mapPosition)
{
    using namespace game;
    using namespace editor;

    const CScenEdit* scenEdit{CScenEditApi::get().instance()};
    const IMidgardObjectMap* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    const auto& dynamicCast{RttiApi::get().dynamicCast};
    const auto* mapElementType{RttiApi::rtti().IMapElementType};

    const CMidgardPlan* plan{getMidgardPlan(objectMap)};

    const auto& list{IdListApi::get()};
    IdList objectIds;
    list.constructor(&objectIds);

    IMapElement* mapElement{};
    if (CMidgardPlanApi::get().getObjectsAtPoint(plan, &objectIds, mapPosition)) {
        const auto* objectType{RttiApi::rtti().IMidScenarioObjectType};

        for (const auto& objectId : objectIds) {
            auto* object{objectMap->vftable->findScenarioObjectById(objectMap, &objectId)};

            mapElement = (IMapElement*)dynamicCast(object, 0, objectType, mapElementType, 0);

            if (!dynamicCast(mapElement, 0, mapElementType, RttiApi::rtti().CMidRoadType, 0)) {
                break;
            }
        }
    }

    list.destructor(&objectIds);
    return mapElement;
}

bool __fastcall taskObjPropDoActionHooked(game::editor::CTaskObjProp* thisptr,
                                          int /*%edx*/,
                                          const game::CMqPoint* mapPosition)
{
    using namespace game;
    using namespace editor;

    const auto& dynamicCast{RttiApi::get().dynamicCast};
    const auto* mapElementType{RttiApi::rtti().IMapElementType};

    IMapElement* mapElement{getMapElementAtPosition(mapPosition)};
    if (!mapElement) {
        return false;
    }

    auto* market{(CMidSiteResourceMarket*)dynamicCast(mapElement, 0, mapElementType,
                                                      getResourceMarketTypeDescriptor(), 0)};
    if (!market) {
        // Map element is not resource market, let the game handle it
        return getOriginalFunctions().taskObjPropDoAction(thisptr, mapPosition);
    }

    CInterface* marketProps = createResourceMarketInterface(thisptr, market);

    thisptr->propertiesInterface = marketProps;
    CInterfManagerImpl* interfManager{thisptr->interfManager.data};
    interfManager->CInterfManagerImpl::CInterfManager::vftable->showInterface(interfManager,
                                                                              marketProps);

    return true;
}

}