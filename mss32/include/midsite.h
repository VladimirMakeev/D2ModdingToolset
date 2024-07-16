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

#ifndef MIDSITE_H
#define MIDSITE_H

#include "aipriority.h"
#include "d2set.h"
#include "mapelement.h"
#include "midgardid.h"
#include "midscenarioobject.h"
#include "sitecategories.h"
#include "stringandid.h"
#include <cstddef>

namespace game {

struct IMidgardObjectMap;
struct CampaignStream;

/** Base class for site objects. */
struct CMidSite : public IMidScenarioObject
{
    IMapElement mapElement;
    IAiPriority aiPriority;
    int imgIso;
    char imgIntf[9];
    char padding[3];
    StringAndId title;
    StringAndId description;
    CMidgardID unknownId;
    Set<CMidgardID> visitors;
    LSiteCategory siteCategory;
};

assert_size(CMidSite, 120);
assert_offset(CMidSite, mapElement, 8);
assert_offset(CMidSite, title, 52);

struct CMidSiteVftable : public IMidScenarioObjectVftable
{
    using GetEntrancePoint = CMqPoint*(__thiscall*)(const CMidSite* thisptr, CMqPoint* entrance);
    GetEntrancePoint getEntrancePoint;

    using StreamSiteData = void(__thiscall*)(CMidSite* thisptr,
                                             CampaignStream* stream,
                                             const CMidgardID* siteId);
    StreamSiteData streamSiteData;
};

assert_vftable_size(CMidSiteVftable, 6);

namespace CMidSiteApi {

struct Api
{
    using Constructor = CMidSite*(__thiscall*)(CMidSite* thisptr,
                                               const CMidgardID* siteId,
                                               const LSiteCategory* siteCategory);
    Constructor constructor;

    using SetData = bool(__thiscall*)(CMidSite* thisptr,
                                      IMidgardObjectMap* objectMap,
                                      int imgIso,
                                      const char* imgIntf,
                                      const CMqPoint* position,
                                      const char* title,
                                      const char* description);
    SetData setData;
};

Api& get();

} // namespace CMidSiteApi

} // namespace game

#endif // MIDSITE_H
