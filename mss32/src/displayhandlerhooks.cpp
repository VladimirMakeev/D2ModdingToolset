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

#include "displayhandlershooks.h"
#include "game.h"
#include "gameimages.h"
#include "gameutils.h"
#include "isolayers.h"
#include "midgardobjectmap.h"
#include "midplayer.h"
#include "midvillage.h"
#include "racetype.h"
#include "stringintlist.h"
#include <fmt/format.h>

namespace hooks {

static std::string getVillageImageName(game::RaceId raceId, int villageTier, bool shadow)
{
    using game::RaceId;

    const char tierLetter = '0' + static_cast<char>(villageTier);
    auto imageName{fmt::format("G000FT0000NE{:c}", tierLetter)};

    switch (raceId) {
    case RaceId::Dwarf:
        imageName += "DW";
        break;
    case RaceId::Elf:
        imageName += "EL";
        break;
    case RaceId::Heretic:
        imageName += "HE";
        break;
    case RaceId::Human:
        imageName += "HU";
        break;
    case RaceId::Undead:
        imageName += "UN";
        break;
    default:
        break;
    }

    if (shadow) {
        imageName += 'S';
    }

    return imageName;
}

static game::IMqImage2* getVillageImageForRace(game::RaceId raceId,
                                               int villageTier,
                                               bool animatedIso,
                                               bool shadow)
{
    using namespace game;

    const auto& listApi{StringIntListApi::get()};

    StringIntList list{};
    listApi.constructor(&list);

    const auto& imagesApi{GameImagesApi::get()};

    GameImagesPtr imagesPtr;
    imagesApi.getGameImages(&imagesPtr);

    auto images{*imagesPtr.data};
    auto isoCmon{images->isoCmon};
    auto isoImg{animatedIso ? images->isoAnim : images->isoStill};

    const auto imageName{getVillageImageName(raceId, villageTier, shadow)};
    const auto length{imageName.length()};

    imagesApi.getCityImageNames(&list, isoCmon, isoImg, imageName.c_str(), length, length);

    IMqImage2* villageImage{};
    if (list.length == 1) {
        auto node{list.head->next};
        const auto& name{node->data.first};
        const auto animated{node->data.second};

        villageImage = imagesApi.getImage(animated ? isoImg : isoCmon, name.string, 0, true,
                                          images->log);
    }

    imagesApi.createOrFreeGameImages(&imagesPtr, nullptr);
    listApi.destructor(&list);

    return villageImage;
}

static game::IMqImage2* getVillageImage(game::RaceId raceId,
                                        int villageTier,
                                        bool animatedIso,
                                        bool shadow)
{
    auto imageForRace{getVillageImageForRace(raceId, villageTier, animatedIso, shadow)};

    return imageForRace
               ? imageForRace
               : game::GameImagesApi::get().getVillageImage(villageTier, animatedIso, shadow);
}

static const game::LRaceCategory* getVillageOwnerRace(const game::IMidgardObjectMap* objectMap,
                                                      const game::CMidgardID& ownerId)
{
    using namespace game;

    const LRaceCategory* ownerRace{RaceCategories::get().neutral};

    if (ownerId != emptyId) {
        auto player{getPlayer(objectMap, &ownerId)};
        if (player) {
            ownerRace = &player->raceType->data->raceType;
        }
    }

    return ownerRace;
}

static bool villageIsObjective(const game::IdSet* objectives, const game::CMidgardID& villageId)
{
    using namespace game;

    IdSetIterator iterator{};
    IdSetApi::get().find(objectives, &iterator, &villageId);
    return iterator != objectives->end();
}

void __stdcall displayHandlerVillageHooked(game::ImageLayerList* list,
                                           const game::CMidVillage* village,
                                           const game::IMidgardObjectMap* objectMap,
                                           const game::CMidgardID* playerId,
                                           const game::IdSet* objectives,
                                           int a6,
                                           bool animatedIso)
{
    using namespace game;

    const auto& listApi{ImageLayerListApi::get()};
    listApi.clear(list);

    if (!village) {
        return;
    }

    auto ownerRace{getVillageOwnerRace(objectMap, village->ownerId)};

    ImageLayerPair imagePair{getVillageImage(ownerRace->id, village->tierLevel, animatedIso, false),
                             isoLayers().villages};
    listApi.pushBack(list, &imagePair);

    ImageLayerPair shadowPair{getVillageImage(ownerRace->id, village->tierLevel, animatedIso, true),
                              isoLayers().stacksShadow};
    listApi.pushBack(list, &shadowPair);

    const auto& fn{gameFunctions()};
    if (!fn.isRaceCategoryUnplayable(ownerRace)) {
        ImageLayerPair flagPair{GameImagesApi::get().getFlagImage(ownerRace), isoLayers().flags};
        listApi.pushBack(list, &flagPair);
    }

    if (playerId && villageIsObjective(objectives, village->id)) {
        ImageLayerPair objectivePair{GameImagesApi::get().getObjectiveImage(
                                         village->mapElement.sizeX),
                                     isoLayers().symObjective};
        listApi.pushBack(list, &objectivePair);
    }

    if (village->stackId != emptyId) {
        listApi.addShieldImageLayer(list, village, 0, objectMap, playerId);
    }

    if (village->riotTurn) {
        ImageLayerPair riotPair{GameImagesApi::get().getRiotImage(), isoLayers().riots};
        listApi.pushBack(list, &riotPair);
    }
}

} // namespace hooks
