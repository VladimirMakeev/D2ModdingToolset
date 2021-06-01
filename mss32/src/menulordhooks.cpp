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

#include "menulordhooks.h"
#include "autodialog.h"
#include "menuphase.h"
#include "originalfunctions.h"
#include "racehooks.h"
#include <array>
#include <fmt/format.h>
#include <string>

namespace hooks {

game::CMenuLord* __fastcall menuLordCtorHooked(game::CMenuLord* thisptr,
                                               int /*%edx*/,
                                               game::CMenuPhase* menuPhase)
{
    using namespace game;

    getOriginalFunctions().menuLordCtor(thisptr, menuPhase);

    if (!thisptr->data->backgroundImage) {
        // Empty background image means it was not loaded for a custom race.
        // Load it here
        const auto raceCatId{menuPhase->data->race.id};

        for (const auto& race : newRaces()) {
            if (raceCatId == race.category.id) {
                const auto imageName{
                    fmt::format("DLG_CHOOSE_LORD_{:s}_BG", race.categoryName.substr(2))};

                thisptr->data->backgroundImage = AutoDialogApi::get().loadImage(imageName.c_str());
                break;
            }
        }
    }

    return thisptr;
}

using ImageNames = std::array<const char*, 4>;

static ImageNames* getLordImageNames(const game::LRaceCategory* raceCategory)
{
    const auto& races = game::RaceCategories::get();
    const auto raceCatId{raceCategory->id};

    if (raceCatId == races.human->id) {
        static ImageNames humanImages{"FACE_HU_0", "FACE_HU_1", "FACE_HU_2", "FACE_HU_3"};
        return &humanImages;
    }

    if (raceCatId == races.heretic->id) {
        static ImageNames hereticImages{"FACE_HE_0", "FACE_HE_1", "FACE_HE_2", "FACE_HE_3"};
        return &hereticImages;
    }

    if (raceCatId == races.dwarf->id) {
        static ImageNames dwarfImages{"FACE_DW_0", "FACE_DW_1", "FACE_DW_2", "FACE_DW_3"};
        return &dwarfImages;
    }

    if (raceCatId == races.undead->id) {
        static ImageNames undeadImages{"FACE_UN_0", "FACE_UN_1", "FACE_UN_2", "FACE_UN_3"};
        return &undeadImages;
    }

    if (raceCatId == races.elf->id) {
        static ImageNames elfImages{"FACE_EL_0", "FACE_EL_1", "FACE_EL_2", "FACE_EL_3"};
        return &elfImages;
    }

    // We need to dynamically create custom names
    // and make sure their contents will outlive loadFaceImages function call
    static std::array<std::string, 4> customNames;
    static ImageNames customImages;

    bool found{false};
    for (const auto& race : newRaces()) {
        if (raceCatId == race.category.id) {
            for (int i = 0; i < 4; ++i) {
                customNames[i] = fmt::format("FACE_{:s}_{:d}", race.abbreviation, i);
                customImages[i] = customNames[i].c_str();
            }

            found = true;
            break;
        }
    }

    if (found) {
        return &customImages;
    }

    return nullptr;
}

void __stdcall getLordFacesByRaceHooked(const game::LRaceCategory* raceCategory,
                                        game::ImagePtrPairs* faceImages)
{
    using namespace game;

    auto imageNames = getLordImageNames(raceCategory);
    if (!imageNames) {
        return;
    }

    const auto& loadFaceImages = CMenuLordApi::get().loadLordFaceImages;
    loadFaceImages(imageNames->data(), imageNames->size(), faceImages);
}

void __stdcall getLordAnimationHooked(const game::CMenuPhase* menuPhase,
                                      int selectedFace,
                                      game::ImagePtrPair* animations)
{
    using namespace game;

    auto imageNames = getLordImageNames(&menuPhase->data->race);
    if (!imageNames) {
        return;
    }

    ImagePtr animation{};
    CMenuLordApi::get().loadLordAnimation(&animation, (*imageNames)[selectedFace]);

    const auto& smartPtr = SmartPointerApi::get();

    smartPtr.copy(reinterpret_cast<SmartPointer*>(&animations->first),
                  reinterpret_cast<SmartPointer*>(&animation));

    smartPtr.createOrFree(reinterpret_cast<SmartPointer*>(&animation), nullptr);

    smartPtr.copy(reinterpret_cast<SmartPointer*>(&animations->second),
                  reinterpret_cast<SmartPointer*>(&animations->first));
}

} // namespace hooks
