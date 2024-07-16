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

#include "visitorcreatesitehooks.h"
#include "editor.h"
#include "gameutils.h"
#include "itemcategory.h"
#include "mempool.h"
#include "midgardobjectmap.h"
#include "midgardplan.h"
#include "midsitemage.h"
#include "midsitemerchant.h"
#include "midsitemercs.h"
#include "midsiteresourcemarket.h"
#include "midsitetrainer.h"
#include "sitecategoryhooks.h"
#include "visitorcreatesite.h"

namespace hooks {

bool __fastcall visitorCreateSiteCanApplyHooked(const game::editor::CVisitorCreateSite* thisptr,
                                                int /* %edx */)
{
    using namespace game;

    const CMidgardMap* map = getMidgardMap(thisptr->objectMap);
    if (!editorFunctions.canPlaceSite(&thisptr->position, map, thisptr->objectMap)) {
        return false;
    }

    const CMidgardPlan* plan = getMidgardPlan(thisptr->objectMap);
    if (!CMidgardPlanApi::get().canPlaceSite(&thisptr->position, plan, nullptr)) {
        return false;
    }

    const auto& categories = SiteCategories::get();
    const auto id = thisptr->category.id;
    if (id == categories.merchant->id || id == categories.mageTower->id
        || id == categories.mercenaries->id || id == categories.trainer->id) {
        return true;
    }

    if (customSiteCategories().exists) {
        return id == customSiteCategories().resourceMarket.id;
    }

    return false;
}

bool __fastcall visitorCreateSiteApplyHooked(const game::editor::CVisitorCreateSite* thisptr,
                                             int /* %edx */)
{
    using namespace game;

    IMidgardObjectMap* objectMap = thisptr->objectMap;

    CMidgardPlan* plan = getMidgardPlanToChange(objectMap);
    CMidgardID siteId;
    objectMap->vftable->createScenarioIdIncFreeIndex(objectMap, &siteId, IdType::Site);

    const auto& memory = Memory::get();
    const auto& categories = SiteCategories::get();
    const auto id = thisptr->category.id;

    CMidSite* site = nullptr;
    if (id == categories.merchant->id) {
        auto merchant = (CMidSiteMerchant*)memory.allocate(sizeof(CMidSiteMerchant));

        site = CMidSiteMerchantApi::get().constructor(merchant, &siteId);
    } else if (id == categories.mageTower->id) {
        auto mage = (CMidSiteMage*)memory.allocate(sizeof(CMidSiteMage));

        site = CMidSiteMageApi::get().constructor(mage, &siteId);
    } else if (id == categories.mercenaries->id) {
        auto mercs = (CMidSiteMercs*)memory.allocate(sizeof(CMidSiteMercs));

        site = CMidSiteMercsApi::get().constructor(mercs, &siteId);
    } else if (id == categories.trainer->id) {
        auto trainer = (CMidSiteTrainer*)memory.allocate(sizeof(CMidSiteTrainer));

        site = CMidSiteTrainerApi::get().constructor(trainer, &siteId);
    } else if (customSiteCategories().exists && id == customSiteCategories().resourceMarket.id) {
        site = createResourceMarket(&siteId);
    }

    if (!site) {
        return false;
    }

    if (!objectMap->vftable->insertObject(objectMap, site)) {
        return false;
    }

    const char* imgIntf = thisptr->interfaceImage.string ? thisptr->interfaceImage.string : "";
    const char* title = thisptr->name.string ? thisptr->name.string : "";
    const char* description = thisptr->description.string ? thisptr->description.string : "";

    if (!CMidSiteApi::get().setData(site, objectMap, thisptr->imgIso, imgIntf, &thisptr->position,
                                    title, description)) {
        return false;
    }

    if (!CMidgardPlanApi::get().addMapElement(plan, &site->mapElement, false)) {
        return false;
    }

    if (id == categories.merchant->id) {
        const auto& addBuyCategory = VisitorApi::get().merchantAddBuyCategory;
        const auto& items = ItemCategories::get();

        addBuyCategory(&siteId, items.armor, objectMap, 1);
        addBuyCategory(&siteId, items.jewel, objectMap, 1);
        addBuyCategory(&siteId, items.weapon, objectMap, 1);
        addBuyCategory(&siteId, items.banner, objectMap, 1);
        addBuyCategory(&siteId, items.potionBoost, objectMap, 1);
        addBuyCategory(&siteId, items.potionHeal, objectMap, 1);
        addBuyCategory(&siteId, items.potionRevive, objectMap, 1);
        addBuyCategory(&siteId, items.potionPermanent, objectMap, 1);
        addBuyCategory(&siteId, items.scroll, objectMap, 1);
        addBuyCategory(&siteId, items.wand, objectMap, 1);
        addBuyCategory(&siteId, items.valuable, objectMap, 1);
    }

    return true;
}

} // namespace hooks
