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

#include "movepathhooks.h"
#include "dynamiccast.h"
#include "game.h"
#include "gameimages.h"
#include "gamesettings.h"
#include "image2text.h"
#include "isolayers.h"
#include "log.h"
#include "mempool.h"
#include "midgard.h"
#include "midgardobjectmap.h"
#include "midstack.h"
#include "midunit.h"
#include "multilayerimg.h"
#include "pathinfolist.h"
#include "settings.h"
#include "ussoldier.h"
#include "utils.h"
#include <cmath>
#include <fmt/format.h>

namespace hooks {

void __stdcall showMovementPathHooked(const game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* stackId,
                                      game::LinkedList<game::CMqPoint>* path,
                                      const game::CMqPoint* lastReachablePoint,
                                      const game::CMqPoint* pathEnd,
                                      bool a6)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto plan = fn.getMidgardPlan(objectMap);

    const auto& dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();

    auto stackObj = objectMap->vftable->findScenarioObjectById(objectMap, stackId);
    auto stack = static_cast<const CMidStack*>(
        dynamicCast(stackObj, 0, rtti.IMidScenarioObjectType, rtti.CMidStackType, 0));

    auto leaderObj = objectMap->vftable->findScenarioObjectById(objectMap, &stack->leaderId);
    auto leader = static_cast<const CMidUnit*>(leaderObj);
    auto unitImpl = leader->unitImpl;
    const bool noble = fn.castUnitImplToNoble(unitImpl) != nullptr;

    auto soldier = fn.castUnitImplToSoldier(unitImpl);
    const bool waterOnly = soldier->vftable->getWaterOnly(soldier);

    const CMqPoint* positionPtr{};
    bool pathLeadsToAction{};
    if (!a6) {
        positionPtr = lastReachablePoint;
    } else {
        positionPtr = lastReachablePoint;

        pathLeadsToAction = fn.getBlockingPathNearbyStackId(objectMap, plan, stack,
                                                            lastReachablePoint, pathEnd, 0)
                            != nullptr;

        if (!pathLeadsToAction) {
            CMqPoint entrance{};
            if (fn.getFortOrRuinEntrance(objectMap, plan, stack, pathEnd, &entrance)
                && std::abs(lastReachablePoint->x - entrance.x) <= 1
                && std::abs(lastReachablePoint->y - entrance.y) <= 1) {
                pathLeadsToAction = true;
            }
        }
    }

    const auto& pathApi = PathInfoListApi::get();

    PathInfoList pathInfo;
    pathApi.constructor(&pathInfo);

    {
        CMqPoint point{};
        point.x = positionPtr->x;
        point.y = positionPtr->y;
        pathApi.populateFromPath(objectMap, stack, path, &point, waterOnly, &pathInfo);
    }

    const auto& imagesApi = GameImagesApi::get();

    GameImagesPtr imagesPtr;
    imagesApi.getGameImages(&imagesPtr);
    auto images = *imagesPtr.data;

    const auto& memAlloc = Memory::get().allocate;

    auto gameSettings = *CMidgardApi::get().instance()->data->settings;
    const bool displayPathTurn{gameSettings->displayPathTurn};
    CMidgardID turnStringId{};
    CMidgardIDApi::get().fromString(&turnStringId, "X005TA0935");
    const char* turnString{fn.getInterfaceText(&turnStringId)};

    const auto& moveCostColor{userSettings().movementCost.textColor};
    const auto& moveCostOutline{userSettings().movementCost.outlineColor};

    bool firstNode{true};

    bool pathAllowed{};
    bool waterOnlyToLand{};

    const auto stackPosition = fn.getStackPositionById(objectMap, stackId);
    bool v61 = *positionPtr != stackPosition;

    int turnNumber{};
    bool manyTurnsToTravel{};

    std::uint32_t index{};
    for (auto node = pathInfo.head->next; node != pathInfo.head;
         node = node->next, firstNode = false, ++index) {
        const auto& currentPosition = node->data.position;

        if (!fn.stackCanMoveToPosition(objectMap, &currentPosition, stack, plan)) {
            continue;
        }

        pathAllowed = !waterOnlyToLand;
        if (waterOnly && !fn.isWaterTileSurroundedByWater(&currentPosition, objectMap)) {
            pathAllowed = false;
            waterOnlyToLand = true;
        }

        // Blue flag
        const char* imageName = "MOVENORMAL";
        if (!v61) {
            // Red flag
            imageName = "MOVEOUT";
            if (!pathLeadsToAction) {
                // White flag
                imageName = "MOVEACTION";
            }
        }

        const bool endOfPath{currentPosition == *positionPtr};
        if (endOfPath) {
            v61 = false;
            if (pathLeadsToAction) {
                // Red flag with a scroll, noble actions
                imageName = "MOVENEGO";

                if (!noble) {
                    // Red flag with a sword, battle
                    imageName = "MOVEBATTLE";
                }
            }
        }

        if (!pathAllowed) {
            // Crossed out white flag, when path of water only stack leads to the land
            imageName = "MOVEINCMP";
        }

        auto flagImage = imagesApi.getImage(images->isoCmon, imageName, 0, true, images->log);
        if (!flagImage) {
            continue;
        }

        const auto imagesCount{flagImage->vftable->getImagesCount(flagImage)};
        flagImage->vftable->setImageIndex(flagImage, index % imagesCount);

        CImage2Text* turnNumberImage{};

        if (displayPathTurn && !firstNode) {
            bool drawTurnNumber{};
            turnNumber = 0;

            const auto prev = node->prev;
            const auto prevTurnsToReach{prev->data.turnsToReach};
            const auto currTurnsToReach{node->data.turnsToReach};
            const bool differ{prevTurnsToReach != currTurnsToReach};

            // Check previous node, draw turn number only if number of turns to reach is different
            // and previous one is not 0
            if (differ) {
                manyTurnsToTravel = true;

                if (prevTurnsToReach != 0) {
                    drawTurnNumber = true;
                    turnNumber = prevTurnsToReach;
                }
            }

            const bool lastNode{node->next == pathInfo.head};
            if (lastNode && manyTurnsToTravel) {
                // Always draw turn number on the last path node,
                // but only when path is longer than single turn
                drawTurnNumber = true;

                // Special case: end of the path is the first node of a new travel turn
                // (previous node has different turns to reach), we draw _previous_ day turn number.
                // Exception: do not draw 0 as turn number
                if (!differ || prev->data.turnsToReach == 0) {
                    turnNumber = currTurnsToReach;
                } else {
                    turnNumber = prevTurnsToReach;
                }
            }

            if (drawTurnNumber) {
                turnNumberImage = static_cast<CImage2Text*>(memAlloc(sizeof(CImage2Text)));
                CImage2TextApi::get().constructor(turnNumberImage, 32, 64);

                std::string text{turnString};
                replace(text, "%TURN%", fmt::format("{:d}", turnNumber));
                CImage2TextApi::get().setText(turnNumberImage, text.c_str());
            }
        }

        CImage2Text* moveCostImage{};

        if (pathAllowed && !turnNumberImage) {
            moveCostImage = static_cast<CImage2Text*>(memAlloc(sizeof(CImage2Text)));
            CImage2TextApi::get().constructor(moveCostImage, 32, 64);

            const auto moveCostString{fmt::format(
                "\\fmedium;\\hC;\\vT;\\c{:03d};{:03d};{:03d};\\o{:03d};{:03d};{:03d};{:d}",
                (int)moveCostColor.r, (int)moveCostColor.g, (int)moveCostColor.b,
                (int)moveCostOutline.r, (int)moveCostOutline.g, (int)moveCostOutline.b,
                node->data.moveCostTotal)};

            CImage2TextApi::get().setText(moveCostImage, moveCostString.c_str());
        }

        auto multilayerImg = static_cast<CMultiLayerImg*>(memAlloc(sizeof(CMultiLayerImg)));
        CMultiLayerImgApi::get().constructor(multilayerImg);

        CMultiLayerImgApi::get().addImage(multilayerImg, flagImage, -999, -999);

        if (turnNumberImage) {
            CMultiLayerImgApi::get().addImage(multilayerImg, turnNumberImage, -999, -999);
        }

        if (moveCostImage) {
            CMultiLayerImgApi::get().addImage(multilayerImg, moveCostImage, -999, -999);
        }

        CMqPoint pos;
        pos.x = currentPosition.x;
        pos.y = currentPosition.y;
        fn.showImageOnGround(&pos, isoLayers().symMovePath, multilayerImg, 0, 0);
    }

    imagesApi.createOrFreeGameImages(&imagesPtr, nullptr);

    pathApi.freeNodes(&pathInfo);
    pathApi.freeNode(&pathInfo, pathInfo.head);
}

} // namespace hooks
