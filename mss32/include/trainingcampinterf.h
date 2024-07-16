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

#ifndef TRAININGCAMPINTERF_H
#define TRAININGCAMPINTERF_H

#include "draganddropinterf.h"
#include "intvector.h"

namespace game {

struct CMidSiteTrainer;

namespace editor {

struct CTaskObjProp;

struct CTrainingCampInterfData
{
    CTaskObjProp* taskObjProp;
    CMidSiteTrainer* trainingCamp;
    char unknown2[20];
    IntVector siteIndices;
    int selectedImgIsoIndex;
};

assert_size(CTrainingCampInterfData, 48);

/** Represents DLG_TRAINING_CAMP from ScenEdit.dlg */
struct CTrainingCampInterf : public CDragAndDropInterf
{
    CTrainingCampInterfData* campData;
};

assert_size(CTrainingCampInterf, 28);

namespace CTrainingCampInterfApi {

struct Api
{
    struct ButtonCallback
    {
        using Callback = void(__thiscall*)(CTrainingCampInterf* thisptr);

        Callback callback;
        int unknown;
    };

    using CreateButtonFunctor = SmartPointer*(__stdcall*)(SmartPointer* functor,
                                                          int dummy,
                                                          CTrainingCampInterf* interf,
                                                          ButtonCallback* callback);
    CreateButtonFunctor createButtonFunctor;
};

Api& get();

} // namespace CTrainingCampInterfApi

}

}

#endif // TRAININGCAMPINTERF_H
