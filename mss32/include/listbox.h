/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef LISTBOX_H
#define LISTBOX_H

#include "functordispatch1.h"
#include "interface.h"
#include "smartptr.h"
#include <cstddef>

namespace game {

struct CDialogInterf;
struct CImage2TextBackground;
struct CListBoxInterfData;

/**
 * List box ui element.
 * Represents LBOX and TLBOX from Interf.dlg or ScenEdit.dlg files.
 */
struct CListBoxInterf : public CInterface
{
    CListBoxInterfData* listBoxData;

    /** Assumption: interface for list box elements rendering. */
    struct LBDisplay
    {
        const void* vftable;
    };
};

static_assert(sizeof(CListBoxInterf) == 12,
              "Size of CListBoxInterf structure must be exactly 12 bytes");

static_assert(offsetof(CListBoxInterf, listBoxData) == 8,
              "CListBoxInterf::listBoxData offset must be 8 bytes");

struct CListBoxInterfData
{
    int unknown;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    CListBoxInterf::LBDisplay* display;
    SmartPointer ptr;
    SmartPointer ptr2;
    int unknown7;
    int unknown8;
    int selectedElement;
    int elementsTotal;
    int unknown13[10];
    Vector<int> childIndices;
    SmartPtr<CBFunctorDispatch1<int>> onSelectionConfirmed;
    SmartPtr<CBFunctorDispatch1<int>> onRightMouseButtonPressed;
    SmartPointer ptr5;
    CImage2TextBackground* image2TextBgnd;
    CMqPoint bgndImagePos;
};

static_assert(sizeof(CListBoxInterfData) == 148,
              "Size of CListBoxInterfData structure must be exactly 148 bytes");

struct CListBoxDisplayTextData
{
    char unknown[48];
};

static_assert(sizeof(CListBoxDisplayTextData) == 48,
              "Size of CListBoxDisplayTextData structure must be exactly 48 bytes");

/** Assumption: implements rendering for list box containing text elements. */
struct CListBoxDisplayText : public CListBoxInterf::LBDisplay
{
    CListBoxDisplayTextData* data;
};

static_assert(sizeof(CListBoxDisplayText) == 8,
              "Size of CListBoxDisplayText structure must be exactly 8 bytes");

namespace CListBoxInterfApi {

struct Api
{
    /**
     * Assigns functor with callback to a list box.
     * Searches for list box element by its and dialog names.
     * Callback will be called each time when list box selection changes.
     * @param[in] dialog used for list box search.
     * @param[in] buttonName list box to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for list box.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @returns list box found with callback set or nullptr if not found.
     */
    using AssignFunctor = CListBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                      const char* listBoxName,
                                                      const char* dialogName,
                                                      SmartPointer* functor);
    AssignFunctor assignFunctor;

    /**
     * Assigns functor with callback to a list box display text element.
     * Searches for list box element by its and dialog names.
     * Callback will be called each time when list box selection changes.
     * @param[in] dialog used for list box search.
     * @param[in] buttonName list box to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for list box.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @param addTextShortenedMark if true, adds '[...]' to shortened list box text elements.
     * @returns list box found with callback set or nullptr if not found.
     */
    using AssignDisplayTextFunctor = CListBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                                 const char* listBoxName,
                                                                 const char* dialogName,
                                                                 SmartPointer* functor,
                                                                 bool addTextShortenedMark);
    AssignDisplayTextFunctor assignDisplayTextFunctor;

    /**
     * Assigns functor with callback to a list box display surface element.
     * Searches for list box element by its and dialog names.
     * Callback will be called each time when list box selection changes.
     * @param[in] dialog used for list box search.
     * @param[in] buttonName list box to assign functor to.
     * @param[in] dialogName dialog (menu) where to search for list box.
     * @param[in] functor pointer to existing functor initialized with callback.
     * @returns list box found with callback set or nullptr if not found.
     */
    using AssignDisplaySurfaceFunctor = CListBoxInterf*(__stdcall*)(CDialogInterf* dialog,
                                                                    const char* listBoxName,
                                                                    const char* dialogName,
                                                                    SmartPointer* functor);
    AssignDisplaySurfaceFunctor assignDisplaySurfaceFunctor;

    /**
     * Initializes list box with specified number of elements.
     * @param[in] thisptr list box to initialize.
     * @param elementsTotal max number of elements to show in list box.
     * @returns unknown.
     */
    using SetElementsTotal = bool(__thiscall*)(CListBoxInterf* thisptr, int elementsTotal);
    SetElementsTotal setElementsTotal;

    /**
     * Assumption: returns index of currently selected list box element.
     * Called many times in game code, actual types and meaning unknown.
     */
    using SelectedIndex = int(__thiscall*)(CListBoxInterf* thisptr);
    SelectedIndex selectedIndex;

    using SetSelectedIndex = void(__thiscall*)(CListBoxInterf* thisptr, int selectedIndex);
    SetSelectedIndex setSelectedIndex;
};

Api& get();

} // namespace CListBoxInterfApi

} // namespace game

#endif // LISTBOX_H
