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

#include "fontshooks.h"
#include "fonts.h"
#include "mempool.h"
#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace hooks {

/** Wrapper is needed to safely use std::vector<FontListPtrPairWrapper>  */
struct FontListPtrPairWrapper
{
    FontListPtrPairWrapper()
    {
        game::FontListPtrPairApi::get().defaultCtor(&pair);
    }

    FontListPtrPairWrapper(const game::FontListPtrPair& listPtrPair)
    {
        game::FontListPtrPairApi::get().copyConstructor(&pair, &listPtrPair);
    }

    FontListPtrPairWrapper(const FontListPtrPairWrapper& other)
    {
        game::FontListPtrPairApi::get().copyConstructor(&pair, &other.pair);
    }

    FontListPtrPairWrapper& operator=(const FontListPtrPairWrapper& other) = delete;

    FontListPtrPairWrapper(FontListPtrPairWrapper&& other) noexcept
    {
        // Emulate move semantics by copying and deleting
        game::FontListPtrPairApi::get().copyConstructor(&pair, &other.pair);
        game::FontListPtrPairApi::get().destructor(&other.pair);
    }

    ~FontListPtrPairWrapper()
    {
        game::FontListPtrPairApi::get().destructor(&pair);
    }

    game::FontListPtrPair pair{};
};

struct CustomFontCacheData : public game::FontCacheData
{
    CustomFontCacheData()
    {
        game::FontCacheApi::get().dataConstructor(this);
    }

    std::vector<FontListPtrPairWrapper> customFonts;
};

bool __stdcall loadFontFilesHooked(const char* interfFolder)
{
    using namespace game;

    const auto& cacheApi = FontCacheApi::get();

    if (cacheApi.fontCache->data != nullptr) {
        // Font cache exists, fonts are already loaded
        return false;
    }

    const auto& fontApi = FontListPtrPairApi::get();

    // Load default fonts
    FontListPtrPair small;
    fontApi.loadFromFile(&small, interfFolder, "small");

    FontListPtrPair normal;
    fontApi.loadFromFile(&normal, interfFolder, "normal");

    FontListPtrPair medium;
    fontApi.loadFromFile(&medium, interfFolder, "medium");

    FontListPtrPair large;
    fontApi.loadFromFile(&large, interfFolder, "large");

    FontListPtrPair vLarge;
    fontApi.loadFromFile(&vLarge, interfFolder, "vlarge");

    FontListPtrPair medbold;
    fontApi.loadFromFile(&medbold, interfFolder, "medbold");

    FontListPtrPair menu;
    fontApi.loadFromFile(&menu, interfFolder, "menu");

    if (!fontApi.isLoaded(&small) || !fontApi.isLoaded(&normal) || !fontApi.isLoaded(&medium)
        || !fontApi.isLoaded(&large) || !fontApi.isLoaded(&vLarge) || !fontApi.isLoaded(&medbold)
        || !fontApi.isLoaded(&menu)) {
        fontApi.destructor(&menu);
        fontApi.destructor(&medbold);
        fontApi.destructor(&vLarge);
        fontApi.destructor(&large);
        fontApi.destructor(&medium);
        fontApi.destructor(&normal);
        fontApi.destructor(&small);
        return false;
    }

    FontCache* cache = (FontCache*)Memory::get().allocate(sizeof(FontCache));

    auto* cacheData = (CustomFontCacheData*)Memory::get().allocate(sizeof(CustomFontCacheData));
    new (cacheData) CustomFontCacheData();

    cache->data = cacheData;

    // Set global pointer
    cacheApi.smartPtrSetData(cacheApi.fontCache, cache);
    // Setup default fonts
    fontApi.copyConstructor(&cache->data->small, &small);
    fontApi.copyConstructor(&cache->data->normal, &normal);
    fontApi.copyConstructor(&cache->data->medium, &medium);
    fontApi.copyConstructor(&cache->data->large, &large);
    fontApi.copyConstructor(&cache->data->veryLarge, &vLarge);
    fontApi.copyConstructor(&cache->data->mediumBold, &medbold);
    fontApi.copyConstructor(&cache->data->menu, &menu);

    // clang-format off
    static const std::array<const char*, 7u> defaultFonts{{
        "small",
        "normal",
        "medium",
        "large",
        "vlarge",
        "medbold",
        "menu"
    }};
    // clang-format on

    // Load custom fonts
    const std::filesystem::path directory{interfFolder};

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        const std::filesystem::path& path = entry.path();
        if (path.extension().string() != ".mft") {
            continue;
        }

        const std::string name = path.stem().string();
        if (std::find(defaultFonts.cbegin(), defaultFonts.cend(), name) != defaultFonts.cend()) {
            // Do not load default fonts twice
            continue;
        }

        FontListPtrPairWrapper font;
        fontApi.loadFromFile(&font.pair, interfFolder, name.c_str());
        if (fontApi.isLoaded(&font.pair)) {
            cacheData->customFonts.emplace_back(font);
        }
    }

    fontApi.destructor(&menu);
    fontApi.destructor(&medbold);
    fontApi.destructor(&vLarge);
    fontApi.destructor(&large);
    fontApi.destructor(&medium);
    fontApi.destructor(&normal);
    fontApi.destructor(&small);
    return true;
}

void __fastcall fontCacheDataDtorHooked(game::FontCacheData* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& destructor = FontListPtrPairApi::get().destructor;

    CustomFontCacheData* data = static_cast<CustomFontCacheData*>(thisptr);

    data->customFonts.~vector();
    destructor(&data->menu);
    destructor(&data->mediumBold);
    destructor(&data->veryLarge);
    destructor(&data->large);
    destructor(&data->medium);
    destructor(&data->normal);
    destructor(&data->small);
}

} // namespace hooks
