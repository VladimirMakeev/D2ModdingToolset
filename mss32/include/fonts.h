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

#ifndef FONTS_H
#define FONTS_H

#include "d2list.h"
#include "d2pair.h"
#include "d2string.h"
#include "smartptr.h"

namespace game {

/** Holds bitmap data and metrics of a single font symbol. */
struct FontSymbol
{
    std::uint32_t width;
    std::uint32_t height;
    int unknown;
    std::uint32_t rowLength;
    char* bitmapData;
};

assert_size(FontSymbol, 20);

struct FontData
{
    char* name;
    int height;
    int lineSpacing;
    int unknown2;
    FontSymbol* symbols[256];
};

assert_size(FontData, 1040);

/** Holds data and metrics of an entire font. */
struct Font
{
    FontData* data;
};

assert_size(Font, 4);

/** Holds information about loaded font. */
struct FontInfo
{
    String name;
    String path;
    SmartPtr<Font> font;
    int valid;
};

assert_size(FontInfo, 44);

using FontInfoList = List<FontInfo>;

using FontListPtrPair = Pair<Font*, SmartPtr<FontInfoList>>;
assert_size(FontListPtrPair, 12);

namespace FontListPtrPairApi {

struct Api
{
    using LoadFromFile = FontListPtrPair*(__stdcall*)(FontListPtrPair* pair,
                                                      const char* directory,
                                                      const char* filename);
    LoadFromFile loadFromFile;

    using IsLoaded = bool(__thiscall*)(const FontListPtrPair* thisptr);
    IsLoaded isLoaded;

    using DefaultCtor = FontListPtrPair*(__thiscall*)(FontListPtrPair* thisptr);
    DefaultCtor defaultCtor;

    using Destructor = void(__thiscall*)(FontListPtrPair* thisptr);
    Destructor destructor;

    using CopyConstructor = FontListPtrPair*(__thiscall*)(FontListPtrPair* thisptr,
                                                          const FontListPtrPair* other);
    CopyConstructor copyConstructor;
};

Api& get();

} // namespace FontListPtrPairApi

struct FontCacheData
{
    FontListPtrPair small;
    FontListPtrPair normal;
    FontListPtrPair medium;
    FontListPtrPair large;
    FontListPtrPair veryLarge;
    FontListPtrPair mediumBold;
    FontListPtrPair menu;
};

assert_size(FontCacheData, 84);

/** Holds loaded fonts. */
struct FontCache
{
    FontCacheData* data;
};

assert_size(FontCache, 4);

using FontCachePtr = SmartPtr<FontCache>;

namespace FontCacheApi {

struct Api
{
    using LoadFontFiles = bool(__stdcall*)(const char* interfFolder);
    LoadFontFiles loadFontFiles;

    using DataConstructor = FontCacheData*(__thiscall*)(FontCacheData* thisptr);
    DataConstructor dataConstructor;

    using DataDestructor = void(__thiscall*)(FontCacheData* thisptr);
    DataDestructor dataDestructor;

    using SmartPtrSetData = void(__thiscall*)(FontCachePtr* thisptr, FontCache* data);
    SmartPtrSetData smartPtrSetData;

    FontCachePtr* fontCache;
};

Api& get();

} // namespace FontCacheApi

} // namespace game

#endif // FONTS_H
