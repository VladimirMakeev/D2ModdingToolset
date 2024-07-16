/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef SCENPROPINTERF_H
#define SCENPROPINTERF_H

#include "popupinterf.h"

namespace game {
struct CSpinButtonInterf;

namespace editor {

struct CScenPropInterfData
{
    char unknown[156];
};

/** Represents DLG_PROP from ScenEdit.dlg. */
struct CScenPropInterf : public CPopupInterf
{
    CScenPropInterfData* data;
};

assert_size(CScenPropInterf, 28);

namespace CScenPropInterfApi {

struct Api
{
    using Constructor = CScenPropInterf*(__thiscall*)(CScenPropInterf* thisptr,
                                                      ITask* task,
                                                      char* a3);
    Constructor constructor;

    struct SpinButtonCallback
    {
        using Callback = void(__thiscall*)(void* thisptr, CSpinButtonInterf* spinButton);

        Callback callback;
        int unknown;
    };

    /** Reuse function from CCapitalInterf. */
    using CreateSpinButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                              int dummy,
                                                              void* interf,
                                                              SpinButtonCallback* callback);
    CreateSpinButtonFunctor createSpinButtonFunctor;
};

Api& get();

} // namespace CScenPropInterfApi

} // namespace editor

} // namespace game

#endif // SCENPROPINTERF_H
