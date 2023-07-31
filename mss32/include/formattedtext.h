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

#ifndef FORMATTEDTEXT_H
#define FORMATTEDTEXT_H

#include "d2color.h"
#include "d2list.h"
#include "d2vector.h"
#include "smartptr.h"

namespace game {

struct CMqRect;
struct CMqPoint;
struct SurfaceDecompressData;
struct IFormatString;

struct IFormattedTextVftable;

struct IFormattedText
{
    IFormattedTextVftable* vftable;
};

struct IFormattedTextVftable
{
    using Destructor = void(__thiscall*)(IFormattedText* thisptr, char flags);
    Destructor destructor;

    using GetFormatString = IFormatString*(__thiscall*)(IFormattedText* thisptr);
    GetFormatString getFormatString;

    using Draw = CMqPoint*(__thiscall*)(IFormattedText* thisptr,
                                        CMqPoint* textSize,
                                        SurfaceDecompressData* surfaceData,
                                        const char* text,
                                        CMqRect* area,
                                        int a6);
    Draw draw;

    using Method2 =
        int*(__thiscall*)(IFormattedText* thisptr, int a2, char* str, unsigned int a4, int a5);
    Method2 method2;

    using Method3 = unsigned int(__thiscall*)(IFormattedText* thisptr,
                                              int a2,
                                              unsigned int a3,
                                              int a4);
    Method3 method3;

    using Method4 = unsigned int(__thiscall*)(IFormattedText* thisptr, int a2, char* str, int a4);
    Method4 method4;

    using GetFirstTokenWithNoSpecifierParts = void(__stdcall*)(Vector<char*>* charPtrArray,
                                                               char* str,
                                                               int dummy);
    GetFirstTokenWithNoSpecifierParts getFirstTokenWithNoSpecifierParts;

    using Method6 = void(__thiscall*)(IFormattedText* thisptr,
                                      Vector<void*>* ptrArray,
                                      char* str,
                                      CMqRect* area);
    Method6 method6;

    using GetFirstTokenWithNoSpecifierPartsInRange = void(__stdcall*)(Vector<char*>* stringPtrArray,
                                                                      int dummy,
                                                                      const char* string,
                                                                      const char* end);
    GetFirstTokenWithNoSpecifierPartsInRange getFirstTokenWithNoSpecifierPartsInRange;

    using GetHeightWithLineSpacingUntil = int(__thiscall*)(IFormattedText* thisptr,
                                                           char* textString,
                                                           unsigned int a3);
    GetHeightWithLineSpacingUntil getHeightWithLineSpacingUntil;

    using GetTextSize = CMqPoint*(__thiscall*)(IFormattedText* thisptr,
                                               CMqPoint* size,
                                               const char* text);
    GetTextSize getTextSize;

    using GetTextWidth = int(__thiscall*)(IFormattedText* thisptr, const char* text);
    GetTextWidth getTextWidth;

    using GetTextHeight = int(__thiscall*)(IFormattedText* thisptr, const char* text, int width);
    GetTextHeight getTextHeight;

    using GetTextColorsFromString = void(__stdcall*)(const char* string, List<Color>* textColors);
    GetTextColorsFromString getTextColorsFromString;

    using GetTextBgndColorsFromString = void(__stdcall*)(const char* string,
                                                         List<Color>* bgndColors);
    GetTextBgndColorsFromString getTextBgndColorsFromString;

    using GetTextOutlineColorsFromString = void(__stdcall*)(const char* string,
                                                            List<Color>* outlineColors);
    GetTextOutlineColorsFromString getTextOutlineColorsFromString;

    using GetFirstFmtSpecifierPtr = char*(__stdcall*)(char* str);
    GetFirstFmtSpecifierPtr getFirstFmtSpecifierPtr;

    using SetTextColor = void(__thiscall*)(IFormattedText* thisptr, Color value);
    SetTextColor setTextColor;

    using SetBgndColor = void(__thiscall*)(IFormattedText* thisptr, Color value);
    SetBgndColor setBgndColor;

    using SetOutlineColor = void(__thiscall*)(IFormattedText* thisptr, Color value);
    SetOutlineColor setOutlineColor;

    using SetUnknown4 = void(__thiscall*)(IFormattedText* thisptr, bool value);
    SetUnknown4 setUnknown4;
};

assert_vftable_size(IFormattedTextVftable, 21);

using FormattedTextPtr = SmartPtr<IFormattedText>;

namespace IFormattedTextApi {

struct Api
{
    using GetFormattedText = FormattedTextPtr*(__stdcall*)(FormattedTextPtr* fmtTextPtr);
    GetFormattedText getFormattedText;
};

Api& get();

} // namespace IFormattedTextApi

} // namespace game

#endif // FORMATTEDTEXT_H
