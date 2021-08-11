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

#ifndef EDITBOXINTERF_H
#define EDITBOXINTERF_H

#include "d2string.h"
#include "interface.h"

namespace game {

struct CEditBoxFocus;
struct CImage2Fill;
struct CImage2TextBackground;
struct CDialogInterf;

enum class EditFilter : int
{
    NoFilter,        /**< Any input allowed. */
    TextOnly,        /**< Only text characters, punctuation marks are not allowed. */
    AlphaNum,        /**< Text, digits, punctuation marks, some special characters. */
    DigitsOnly,      /**< Only digits. */
    AlphaNumNoSlash, /**< Text, digits, some punctuation marks. '\', '/' not allowed. */
    NamesDot,        /**< Same as EditFilter::Names, but allows punctuation marks. */
    Names,           /**< Only characters suitable for Windows file names. */
};

struct EditBoxData
{
    EditFilter filter;
    SmartPointer ptr;
    int maxInputLength; /**< Allowed range is [1 : 4096]. */
    String formatString;
    String inputString;
    int editCursorPos;
    bool allowEnter;
    char padding[3];
};

struct CEditBoxInterfData
{
    SmartPtr<CEditBoxFocus> editBoxFocus;
    int textBoxChildIndex;
    int unknown2;
    EditBoxData editBoxData;
    CImage2Fill* image2Fill;
    char unknown[12];
    CImage2TextBackground* background;
    CMqPoint bgndImagePos;
};

static_assert(sizeof(CEditBoxInterfData) == 100,
              "Size of CEditBoxInterfData structure must be exactly 100 bytes");

/**
 * Edit box ui element.
 * Represents EDIT from Interf.dlg or ScenEdit.dlg files.
 */
struct CEditBoxInterf : public CInterface
{
    CEditBoxInterfData* data;
};

static_assert(sizeof(CEditBoxInterf) == 12,
              "Size of CEditBoxInterf structure must be exactly 12 bytes");

namespace CEditBoxInterfApi {

struct Api
{
    /**
     * Searches for specified edit box and sets its filter and max input length.
     * @returns found edit box.
     */
    using SetFilterAndLength = CEditBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                           const char* controlName,
                                                           const char* dialogName,
                                                           EditFilter filter,
                                                           int inputLength);
    SetFilterAndLength setFilterAndLength;

    /**
     * Sets max input length of edit box.
     * Updates edit box state after changing length.
     */
    using SetInputLength = void(__thiscall*)(CEditBoxInterf* thisptr, int length);
    SetInputLength setInputLength;
};

Api& get();

} // namespace CEditBoxInterfApi

} // namespace game

#endif // EDITBOXINTERF_H
