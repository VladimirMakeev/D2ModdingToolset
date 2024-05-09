/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef DISPLAYHANDLERS_H
#define DISPLAYHANDLERS_H

#include "d2map.h"
#include "idset.h"
#include "imagelayerlist.h"
#include "midgardid.h"
#include "smartptr.h"

namespace game {

struct IMidgardObjectMap;
struct CMidVillage;
struct IMapElement;
struct CMidSite;
struct alignas(8) TypeDescriptor;

namespace DisplayHandlersApi {

struct Api
{
    template <typename T>
    using DisplayHandler = void(__stdcall*)(ImageLayerList* list,
                                            const T* scenarioObject,
                                            const IMidgardObjectMap* objectMap,
                                            const CMidgardID* playerId,
                                            const IdSet* objectives,
                                            int a6,
                                            bool animatedIso);

    DisplayHandler<CMidVillage> villageHandler;

    DisplayHandler<CMidSite> siteHandler;
};

Api& get();

} // namespace DisplayHandlersApi

struct ImageDisplayHandlerVftable;

struct ImageDisplayHandler
{
    ImageDisplayHandlerVftable* vftable;
    DisplayHandlersApi::Api::DisplayHandler<void> handler;
};

assert_size(ImageDisplayHandler, 8);

struct ImageDisplayHandlerVftable
{
    using Destructor = void(__thiscall*)(ImageDisplayHandler* thisptr, char flags);
    Destructor destructor;

    using RunHandler = void(__thiscall*)(ImageDisplayHandler* thisptr,
                                         ImageLayerList* list,
                                         const IMapElement* mapElement,
                                         const IMidgardObjectMap* objectMap,
                                         const CMidgardID* playerId,
                                         const IdSet* objectives,
                                         int a6,
                                         bool animatedIso);
    RunHandler runHandler;
};

assert_vftable_size(ImageDisplayHandlerVftable, 2);

using ImageDisplayHandlerPtr = SmartPtr<ImageDisplayHandler>;
using ImageDisplayHandlerMap = Map<const TypeDescriptor*, ImageDisplayHandlerPtr>;
using ImageDisplayHandlerMapIt = MapIterator<const TypeDescriptor*, ImageDisplayHandlerPtr>;
using ImageDisplayHandlerMapInsertIt = Pair<ImageDisplayHandlerMapIt, bool>;

using ImageDisplayHandlerMapFind =
    ImageDisplayHandlerMapIt*(__thiscall*)(ImageDisplayHandlerMap* thisptr,
                                           ImageDisplayHandlerMapIt* iterator,
                                           const TypeDescriptor** descriptor);

namespace ImageDisplayHandlerApi {

struct Api
{
    using AddHandler =
        ImageDisplayHandlerMapInsertIt*(__thiscall*)(ImageDisplayHandlerMap* thisptr,
                                                     ImageDisplayHandlerMapInsertIt* iterator,
                                                     const TypeDescriptor** descriptor,
                                                     const ImageDisplayHandlerPtr* handler);
    AddHandler addHandler;
};

Api& get();

ImageDisplayHandlerMap* instance();

} // namespace ImageDisplayHandlerApi

} // namespace game

#endif // DISPLAYHANDLERS_H
