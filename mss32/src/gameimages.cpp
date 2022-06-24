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

#include "gameimages.h"
#include "version.h"
#include <array>

namespace game::GameImagesApi {

// clang-format off
static std::array<Api, 4> functions = {{
    // Akella
    Api{
        (Api::GetGameImages)0x5ae7bc,
        (Api::GetImage)0x5aedfb,
        (Api::CreateOrFreeGameImages)0x4080e4,
        (Api::GetImageNames)0x5c817c,
        (Api::GetRaceLogoImageName)0x5c649b,
        (Api::GetFlagImage)0x5c138e,
        (Api::GetObjectiveImage)0x5b3c04,
        (Api::GetRiotImage)0x5b9616,
        (Api::GetCityImageNames)0x5c8252,
        (Api::GetVillageImage)0x5b93e7,
        (Api::GetCityPreviewLargeImageNames)0x5c66b8,
        (Api::GetCityIconImageNames)0x5c7046,
        (Api::GetIconImageByName)0x5b37b3,
        (Api::GetUnitIcon)0x5b996b,
    },
    // Russobit
    Api{
        (Api::GetGameImages)0x5ae7bc,
        (Api::GetImage)0x5aedfb,
        (Api::CreateOrFreeGameImages)0x4080e4,
        (Api::GetImageNames)0x5c817c,
        (Api::GetRaceLogoImageName)0x5c649b,
        (Api::GetFlagImage)0x5c138e,
        (Api::GetObjectiveImage)0x5b3c04,
        (Api::GetRiotImage)0x5b9616,
        (Api::GetCityImageNames)0x5c8252,
        (Api::GetVillageImage)0x5b93e7,
        (Api::GetCityPreviewLargeImageNames)0x5c66b8,
        (Api::GetCityIconImageNames)0x5c7046,
        (Api::GetIconImageByName)0x5b37b3,
        (Api::GetUnitIcon)0x5b996b,
    },
    // Gog
    Api{
        (Api::GetGameImages)0x5ada44,
        (Api::GetImage)0x5ae0bf,
        (Api::CreateOrFreeGameImages)0x407d6f,
        (Api::GetImageNames)0x5c7165,
        (Api::GetRaceLogoImageName)0x5c5484,
        (Api::GetFlagImage)0x5c0328,
        (Api::GetObjectiveImage)0x5b2eca,
        (Api::GetRiotImage)0x5b85b1,
        (Api::GetCityImageNames)0x5c723b,
        (Api::GetVillageImage)0x5b8382,
        (Api::GetCityPreviewLargeImageNames)0x5c56a1,
        (Api::GetCityIconImageNames)0x5c602f,
        (Api::GetIconImageByName)0x5b2a79,
        (Api::GetUnitIcon)0x5b891e,
    },
    // Scenario Editor
    Api{
        (Api::GetGameImages)0x5581c3,
        (Api::GetImage)0x558806,
        (Api::CreateOrFreeGameImages)0x43724a,
        (Api::GetImageNames)0x566a05,
        (Api::GetRaceLogoImageName)nullptr,
        (Api::GetFlagImage)0x562de2,
        (Api::GetObjectiveImage)0x554c7a,
        (Api::GetRiotImage)0x55571e,
        (Api::GetCityImageNames)0x566adb,
        (Api::GetVillageImage)0x5554ef,
        (Api::GetCityPreviewLargeImageNames)nullptr,
        (Api::GetCityIconImageNames)nullptr,
        (Api::GetIconImageByName)0x55498d,
        (Api::GetUnitIcon)0x55a52a,
    },
}};
// clang-format on

Api& get()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::GameImagesApi
