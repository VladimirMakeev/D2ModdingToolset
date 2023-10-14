/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "generationresultinterf.h"
#include "autodialog.h"
#include "button.h"
#include "dialoginterf.h"
#include "image2memory.h"
#include "mempool.h"
#include "menubase.h"
#include "menurandomscenario.h"
#include "multilayerimg.h"
#include "pictureinterf.h"

namespace hooks {

static void __fastcall acceptButtonHandler(CGenerationResultInterf* thisptr, int /* %edx */)
{
    thisptr->onAccept(thisptr->menu);
}

static void __fastcall rejectButtonHandler(CGenerationResultInterf* thisptr, int /* %edx */)
{
    thisptr->onReject(thisptr->menu);
}

static void __fastcall cancelButtonHandler(CGenerationResultInterf* thisptr, int /* %edx */)
{
    thisptr->onCancel(thisptr->menu);
}

static int pixelIndex(int x, int y, int size, float scaleFactor = 1.0f)
{
    const int i = static_cast<int>(x / scaleFactor);
    const int j = static_cast<int>(y / scaleFactor);

    return i + size * j;
}

static CImage2Memory* createPreviewImage(CMenuRandomScenario* menu)
{
    using namespace game;

    const int size = menu->scenario->size;
    const int width = size;
    const int height = size;
    const int previewSize = 144;
    const float scaleFactor = static_cast<float>(previewSize) / static_cast<float>(size);

    CImage2Memory* preview = createImage2Memory(previewSize, previewSize);

    rsg::MapGenerator* generator{menu->generator.get()};
    // Each pixel represents map tile that is colored according to its zone
    std::vector<Color> tileColoring(width * height);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            rsg::Position pos{i, j};

            static const Color colors[] = {
                Color{255, 0, 0, 255},     // Zone id 0: red
                Color{0, 255, 0, 255},     // 1: green
                Color{0, 0, 255, 255},     // 2: blue
                Color{255, 255, 255, 255}, // 3: white
                Color{0, 0, 0, 255},       // 4: black
                Color{127, 127, 127, 255}, // 5: gray
                Color{255, 255, 0, 255},   // 6: yellow
                Color{0, 255, 255, 255},   // 7: cyan
                Color{255, 0, 255, 255},   // 8: magenta
                Color{255, 153, 0, 255},   // 9: orange
                Color{0, 158, 10, 255},    // 10: dark green
                Color{0, 57, 158, 255},    // 11: dark blue
                Color{158, 57, 0, 255},    // 12: dark red
            };

            const int index{pixelIndex(i, j, width)};
            const auto zoneId{generator->zoneColoring[generator->posToIndex(pos)]};
            const int colorsTotal{static_cast<int>(std::size(colors))};

            if (zoneId < colorsTotal) {
                tileColoring[index] = colors[zoneId];
            } else {
                const std::uint8_t c = 32 + 10 * (zoneId - colorsTotal);

                tileColoring[index] = Color(c, c, c, 255);
            }
        }
    }

    // Scale tile coloring up to 144 x 144 pixels.
    // It is better to show preview images of the same size regardless of scenario size
    for (int i = 0; i < previewSize; ++i) {
        for (int j = 0; j < previewSize; ++j) {
            const int sourceIndex{pixelIndex(i, j, size, scaleFactor)};
            const int destIndex{pixelIndex(i, j, previewSize)};

            preview->pixels[destIndex] = tileColoring[sourceIndex];
        }
    }

    return preview;
}

CGenerationResultInterf* createGenerationResultInterf(CMenuRandomScenario* menu,
                                                      OnGenerationResultAccepted onAccept,
                                                      OnGenerationResultRejected onReject,
                                                      OnGenerationResultCanceled onCancel)
{
    using namespace game;

    static const char dialogName[] = "DLG_GENERATION_RESULT";

    const auto& allocateMemory = Memory::get().allocate;

    auto popup = (CGenerationResultInterf*)allocateMemory(sizeof(CGenerationResultInterf));
    CPopupDialogInterfApi::get().constructor(popup, dialogName, nullptr);

    popup->onAccept = onAccept;
    popup->onReject = onReject;
    popup->onCancel = onCancel;
    popup->menu = menu;

    CDialogInterf* dialog{*popup->dialog};

    using ButtonCallback = CMenuBaseApi::Api::ButtonCallback;

    const auto& createButtonFunctor = CMenuBaseApi::get().createButtonFunctor;
    const auto& assignFunctor = CButtonInterfApi::get().assignFunctor;
    const auto& smartPtrFree = SmartPointerApi::get().createOrFreeNoDtor;

    SmartPointer functor;

    {
        auto callback = (ButtonCallback)acceptButtonHandler;
        createButtonFunctor(&functor, 0, (CMenuBase*)popup, &callback);
        assignFunctor(dialog, "BTN_ACCEPT", dialogName, &functor, 0);
        smartPtrFree(&functor, nullptr);
    }

    {
        auto callback = (ButtonCallback)rejectButtonHandler;
        createButtonFunctor(&functor, 0, (CMenuBase*)popup, &callback);
        assignFunctor(dialog, "BTN_REJECT", dialogName, &functor, 0);
        smartPtrFree(&functor, nullptr);
    }

    {
        auto callback = (ButtonCallback)cancelButtonHandler;
        createButtonFunctor(&functor, 0, (CMenuBase*)popup, &callback);
        assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        smartPtrFree(&functor, nullptr);
    }

    {
        IMqImage2* border = AutoDialogApi::get().loadImage("ZONES_BORDER");
        CImage2Memory* preview = createPreviewImage(menu);

        CMultiLayerImg* image{(CMultiLayerImg*)allocateMemory(sizeof(CMultiLayerImg))};

        const auto& multilayerApi{CMultiLayerImgApi::get()};
        multilayerApi.constructor(image);

        // Make sure preview is inside border image
        const int borderOffset = 7;

        multilayerApi.addImage(image, border, -999, -999);
        multilayerApi.addImage(image, preview, borderOffset, borderOffset);

        CPictureInterf* zonesImage = CDialogInterfApi::get().findPicture(dialog, "IMG_ZONES");

        CMqPoint offset{};
        CPictureInterfApi::get().setImage(zonesImage, image, &offset);
    }

    return popup;
}

} // namespace hooks
