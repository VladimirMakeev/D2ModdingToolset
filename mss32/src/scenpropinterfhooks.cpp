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

#include "scenpropinterfhooks.h"
#include "aiattitudes.h"
#include "aiattitudestable.h"
#include "dialoginterf.h"
#include "globaldata.h"
#include "midgardscenariomap.h"
#include "midplayer.h"
#include "originalfunctions.h"
#include "racetype.h"
#include "scenedit.h"
#include "scenpropinterf.h"
#include "smartptr.h"
#include "spinbuttoninterf.h"
#include "stringarray.h"
#include "utils.h"
#include "visitors.h"

namespace hooks {

static const game::CMidPlayer* getNeutralPlayer(const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    const auto neutralRaceId{RaceCategories::get().neutral->id};
    const CMidPlayer* neutrals{};

    auto checkNeutralPlayer = [neutralRaceId, &neutrals](const IMidScenarioObject* obj) {
        const auto* player{static_cast<const CMidPlayer*>(obj)};
        const auto raceCatId{player->raceType->data->raceType.id};

        if (raceCatId == neutralRaceId) {
            neutrals = player;
        }
    };

    forEachScenarioObject(objectMap, IdType::Player, checkNeutralPlayer);
    return neutrals;
}

static void __fastcall onSpinButtonSelectionChanged(game::editor::CScenPropInterf* thisptr,
                                                    int /*%edx*/,
                                                    game::CSpinButtonInterf* spinButton)
{
    using namespace game;

    const CScenEdit* scenEdit{CScenEditApi::get().instance()};
    auto* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    const auto* neutralPlayer{getNeutralPlayer(objectMap)};

    const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
    const auto* table{global->attitudesCategories};

    const auto* tableRecord{table->bgn + spinButton->data->selectedOption};
    int id{static_cast<int>(tableRecord->id)};

    LAttitudesCategory attitude{};
    LAttitudesCategoryTableApi::get().findCategoryById(table, &attitude, &id);

    VisitorApi::get().playerSetAttitude(&neutralPlayer->id, &attitude, objectMap, 1);
}

game::editor::CScenPropInterf* __fastcall scenPropInterfCtorHooked(
    game::editor::CScenPropInterf* thisptr,
    int /*%edx*/,
    game::ITask* task,
    char* a3)
{
    using namespace game;

    getOriginalFunctions().scenPropInterfCtor(thisptr, task, a3);

    static const char spinButtonName[]{"SPIN_ATTITUDE"};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{*thisptr->dialog};

    if (!dialogApi.findControl(dialog, spinButtonName)) {
        return thisptr;
    }

    // Spinbox related to Neutrals attitude found, setup its options
    CSpinButtonInterf* spinButton{dialogApi.findSpinButton(dialog, spinButtonName)};

    const auto& globalApi{GlobalDataApi::get()};
    GlobalData* global{*globalApi.getGlobalData()};
    const auto* table{global->attitudesCategories};

    const CScenEdit* scenEdit{CScenEditApi::get().instance()};
    const auto* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    const auto* neutralPlayer{getNeutralPlayer(objectMap)};
    const auto neutralsAttitude{neutralPlayer->attitude.id};

    const auto& findCategory{LAttitudesCategoryTableApi::get().findCategoryById};
    LAttitudesCategory category{};

    const auto& stringArrayApi{StringArrayApi::get()};
    StringArray attitudeNames{};
    stringArrayApi.reserve(&attitudeNames, 1u);

    std::size_t currentAttitudeIndex = 0u;
    std::size_t index = 0u;
    for (auto* record = table->bgn; record != table->end; ++record, ++index) {
        int id{static_cast<int>(record->id)};
        findCategory(table, &category, &id);

        const CAiAttitudes* attitudes{
            CAiAttitudesTableApi::get().find(global->aiAttitudes, &category)};

        const auto& nameText{attitudes->data->name};
        const char* name{globalApi.findTextById(nameText.texts, &nameText.id)};

        String attitudeName{};
        StringApi::get().initFromString(&attitudeName, name);

        stringArrayApi.pushBack(&attitudeNames, &attitudeName);
        StringApi::get().free(&attitudeName);

        if (category.id == neutralsAttitude) {
            currentAttitudeIndex = index;
        }
    }

    const auto& spinApi{CSpinButtonInterfApi::get()};
    spinApi.setOptions(spinButton, &attitudeNames);
    spinApi.setSelectedOption(spinButton, currentAttitudeIndex);

    stringArrayApi.destructor(&attitudeNames);

    using SpinCallback = editor::CScenPropInterfApi::Api::SpinButtonCallback;

    SpinCallback callback{};
    callback.callback = (SpinCallback::Callback)onSpinButtonSelectionChanged;

    SmartPointer functor{};
    editor::CScenPropInterfApi::get().createSpinButtonFunctor(&functor, 0, thisptr, &callback);

    static const char dialogName[]{"DLG_PROP"};
    spinApi.assignFunctor(dialog, spinButtonName, dialogName, &functor);

    SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);

    return thisptr;
}

} // namespace hooks
