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

#include "midcondvarcmp.h"
#include "button.h"
#include "condinterf.h"
#include "condinterfhandler.h"
#include "d2string.h"
#include "dialoginterf.h"
#include "eventconditioncathooks.h"
#include "functor.h"
#include "game.h"
#include "gameutils.h"
#include "interfmanager.h"
#include "listbox.h"
#include "mempool.h"
#include "midevcondition.h"
#include "midevent.h"
#include "mideventhooks.h"
#include "midgardobjectmap.h"
#include "midgardstream.h"
#include "midscenvariables.h"
#include "radiobuttoninterf.h"
#include "testcondition.h"
#include "textids.h"
#include "utils.h"
#include <fmt/format.h>
#include <functional>
#include <vector>

namespace hooks {

/** Same as RAD_OPERATOR button indices in DLG_COND_VAR_CMP from ScenEdit.dlg */
enum class CompareType : int
{
    Equal,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
};

/** Custom event condition that compares two scenario variables. */
struct CMidCondVarCmp : game::CMidEvCondition
{
    int variableId1;
    int variableId2;
    CompareType compareType;
};

void __fastcall condVarCmpDestructor(CMidCondVarCmp* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall condVarCmpIsIdsEqual(const CMidCondVarCmp*, int /*%edx*/, const game::CMidgardID*)
{
    return false;
}

void __fastcall condVarCmpAddToList(const CMidCondVarCmp*,
                                    int /*%edx*/,
                                    game::Set<game::CMidgardID>*)
{ }

bool __fastcall condVarCmpIsValid(const CMidCondVarCmp* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap*)
{
    return true;
}

bool __fastcall condVarCmpMethod4(const CMidCondVarCmp*, int /*%edx*/, int)
{
    return false;
}

void __fastcall condVarCmpStream(CMidCondVarCmp* thisptr,
                                 int /*%edx*/,
                                 game::IMidgardStream** stream)
{
    auto streamPtr = *stream;
    auto vftable = streamPtr->vftable;

    vftable->streamInt(streamPtr, "VAR1", &thisptr->variableId1);
    vftable->streamInt(streamPtr, "VAR2", &thisptr->variableId2);
    vftable->streamInt(streamPtr, "CMP", (int*)&thisptr->compareType);
}

// clang-format off
static game::CMidEvConditionVftable condVarCmpVftable{
    (game::CMidEvConditionVftable::Destructor)condVarCmpDestructor,
    (game::CMidEvConditionVftable::IsIdsEqual)condVarCmpIsIdsEqual,
    (game::CMidEvConditionVftable::AddToList)condVarCmpAddToList,
    (game::CMidEvConditionVftable::IsValid)condVarCmpIsValid,
    (game::CMidEvConditionVftable::Method4)condVarCmpMethod4,
    (game::CMidEvConditionVftable::Stream)condVarCmpStream,
};
// clang-format on

game::CMidEvCondition* createMidCondVarCmp()
{
    using namespace game;

    auto varCmp = (CMidCondVarCmp*)Memory::get().allocate(sizeof(CMidCondVarCmp));

    varCmp->category.vftable = EventCondCategories::vftable();
    varCmp->category.id = customEventConditions().variableCmp.category.id;
    varCmp->category.table = customEventConditions().variableCmp.category.table;
    varCmp->compareType = CompareType::Equal;
    varCmp->vftable = &condVarCmpVftable;

    return varCmp;
}

void __stdcall midCondVarCmpGetInfoString(game::String* info,
                                          const game::IMidgardObjectMap* objectMap,
                                          const game::CMidEvCondition* eventCondition)
{
    auto variables = getScenarioVariables(objectMap);
    if (!variables) {
        // Sanity check, this should never happen
        return;
    }

    auto* condition = static_cast<const CMidCondVarCmp*>(eventCondition);
    auto& findById = game::CMidScenVariablesApi::get().findById;

    auto data1 = findById(variables, condition->variableId1);
    auto data2 = findById(variables, condition->variableId2);
    std::string comparison;

    const auto& varCmpTextIds = textIds().events.conditions.variableCmp;

    switch (condition->compareType) {
    case CompareType::Equal:
        comparison = getInterfaceText(varCmpTextIds.equal.c_str());
        if (comparison.empty()) {
            comparison = "is equal to";
        }
        break;
    case CompareType::NotEqual:
        comparison = getInterfaceText(varCmpTextIds.notEqual.c_str());
        if (comparison.empty()) {
            comparison = "is not equal to";
        }
        break;
    case CompareType::Greater:
        comparison = getInterfaceText(varCmpTextIds.greater.c_str());
        if (comparison.empty()) {
            comparison = "is greater than";
        }
        break;
    case CompareType::GreaterEqual:
        comparison = getInterfaceText(varCmpTextIds.greaterEqual.c_str());
        if (comparison.empty()) {
            comparison = "is greater or equal to";
        }
        break;
    case CompareType::Less:
        comparison = getInterfaceText(varCmpTextIds.less.c_str());
        if (comparison.empty()) {
            comparison = "is less than";
        }
        break;
    case CompareType::LessEqual:
        comparison = getInterfaceText(varCmpTextIds.lessEqual.c_str());
        if (comparison.empty()) {
            comparison = "is less or equal to";
        }
        break;
    }

    const auto str{fmt::format("{:s} {:s} {:s}", data1->name, comparison, data2->name)};
    game::StringApi::get().initFromStringN(info, str.c_str(), str.length());
}

using Variables = std::vector<const game::ScenarioVariable*>;

struct CCondVarCmpInterf : public game::editor::CCondInterf
{
    void* unknown;
    CMidCondVarCmp* condition;
    game::CMidgardID eventId;
    Variables variables;
};

void __fastcall condVarCmpInterfDestructor(CCondVarCmpInterf* thisptr, int /*%edx*/, char flags)
{
    Variables().swap(thisptr->variables);

    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::CInterfaceVftable::OnVisibilityChanged onVisibilityChanged{};

void __fastcall condVarCmpInterfOnVisibilityChanged(CCondVarCmpInterf* thisptr,
                                                    int /*%edx*/,
                                                    int a2,
                                                    int a3)
{
    using namespace game;

    if (onVisibilityChanged) {
        onVisibilityChanged(thisptr, a2, a3);
    }

    if (!a2) {
        return;
    }

    if (!thisptr->condition) {
        return;
    }

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto radioButton = dialogApi.findRadioButton(dialog, "RAD_OPERATOR");
    if (radioButton) {
        CRadioButtonInterfApi::get().setCheckedButton(radioButton,
                                                      (int)thisptr->condition->compareType);
    }

    const auto variableId1 = thisptr->condition->variableId1;
    const auto variableId2 = thisptr->condition->variableId2;

    const auto& variables = thisptr->variables;
    for (size_t i = 0; i < variables.size(); ++i) {
        const auto id = variables[i]->first;
        if (variableId1 == id) {
            auto listBox = dialogApi.findListBox(dialog, "TLBOX_VARIABLES1");
            if (listBox) {
                CListBoxInterfApi::get().setSelectedIndex(listBox, i);
            }
        }

        if (variableId2 == id) {
            auto listBox = dialogApi.findListBox(dialog, "TLBOX_VARIABLES2");
            if (listBox) {
                CListBoxInterfApi::get().setSelectedIndex(listBox, i);
            }
        }
    }
}

bool __fastcall condVarCmpInterfSetEventCondition(CCondVarCmpInterf* thisptr,
                                                  int /*%edx*/,
                                                  game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().variableCmp.category.id) {
        thisptr->condition = static_cast<CMidCondVarCmp*>(eventCondition);
        return true;
    }

    return false;
}

static game::editor::CCondInterfVftable condVarCmpInterfVftable{};

void __fastcall condVarCmpInterfCancelButtonHandler(CCondVarCmpInterf* thisptr, int /*%edx*/)
{
    using namespace game;

    auto handler = thisptr->condData->interfHandler;
    if (handler) {
        handler->vftable->runCallback(handler, false);
    }

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->hideInterface(ptr.data, thisptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);

    if (thisptr) {
        thisptr->CInterface::vftable->destructor(thisptr, 1);
    }
}

void __fastcall condVarCmpInterfOkButtonHandler(CCondVarCmpInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto listBox1 = dialogApi.findListBox(dialog, "TLBOX_VARIABLES1");
    auto listBox2 = dialogApi.findListBox(dialog, "TLBOX_VARIABLES2");

    if (!listBox1 || !listBox2) {
        return;
    }

    const int total = static_cast<int>(thisptr->variables.size());

    const int index1 = CListBoxInterfApi::get().selectedIndex(listBox1);
    if (index1 < 0 || index1 >= total) {
        return;
    }

    const int index2 = CListBoxInterfApi::get().selectedIndex(listBox2);
    if (index2 < 0 || index2 >= total) {
        return;
    }

    auto handler = thisptr->condData->interfHandler;
    if (handler) {
        handler->vftable->runCallback(handler, true);
    }

    auto objectMap = CCondInterfApi::get().getObjectMap(thisptr->unknown);

    {
        auto midEvent = (const CMidEvent*)objectMap->vftable
                            ->findScenarioObjectById(objectMap, &thisptr->eventId);

        const int conditionsTotal = midEvent->conditions.end - midEvent->conditions.bgn;
        if (conditionsTotal >= 10) {
            // Could not create new condition
            showMessageBox(getInterfaceText("X100TA0631"));
            return;
        }
    }

    auto condition = static_cast<CMidCondVarCmp*>(createMidCondVarCmp());

    auto radioButton = dialogApi.findRadioButton(dialog, "RAD_OPERATOR");
    if (radioButton) {
        condition->compareType = static_cast<CompareType>(radioButton->data->selectedButton);
    }

    condition->variableId1 = thisptr->variables[index1]->first;
    condition->variableId2 = thisptr->variables[index2]->first;

    auto midEvent = (CMidEvent*)objectMap->vftable
                        ->findScenarioObjectByIdForChange(objectMap, &thisptr->eventId);

    CMidEventApi::get().addCondition(midEvent, nullptr, condition);

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->hideInterface(ptr.data, thisptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);

    if (thisptr) {
        thisptr->CInterface::vftable->destructor(thisptr, 1);
    }
}

void __fastcall condVarCmpInterfListBoxDisplayHandler(CCondVarCmpInterf* thisptr,
                                                      int /*%edx*/,
                                                      game::String* string,
                                                      bool a3,
                                                      int selectedIndex)
{
    const auto total = static_cast<int>(thisptr->variables.size());
    if (selectedIndex < 0 || selectedIndex >= total) {
        return;
    }

    const auto variable = thisptr->variables[selectedIndex];

    game::StringApi::get().initFromStringN(string, variable->second.name,
                                           std::strlen(variable->second.name));
}

game::editor::CCondInterf* createCondVarCmpInterf(game::ITask* task,
                                                  void* a2,
                                                  const game::CMidgardID* eventId)
{
    using namespace game;
    using namespace editor;

    auto thisptr = (CCondVarCmpInterf*)Memory::get().allocate(sizeof(CCondVarCmpInterf));
    // Set all values to zero, so std::vector variables in CCondVarCmpInterf can work properly
    std::memset(thisptr, 0, sizeof(CCondVarCmpInterf));

    static const char dialogName[]{"DLG_COND_VAR_CMP"};

    CCondInterfApi::get().constructor(thisptr, dialogName, task);

    static bool initialized{false};
    if (!initialized) {
        initialized = true;
        onVisibilityChanged = thisptr->CInterface::vftable->onVisibilityChanged;

        std::memcpy(&condVarCmpInterfVftable, thisptr->CInterface::vftable,
                    sizeof(CInterfaceVftable));

        // Change methods that are specific for our custom class
        condVarCmpInterfVftable
            .destructor = (CInterfaceVftable::Destructor)&condVarCmpInterfDestructor;
        condVarCmpInterfVftable
            .onVisibilityChanged = (CInterfaceVftable::
                                        OnVisibilityChanged)&condVarCmpInterfOnVisibilityChanged;
        condVarCmpInterfVftable
            .setEventCondition = (CCondInterfVftable::
                                      SetEventCondition)&condVarCmpInterfSetEventCondition;
    }

    thisptr->CInterface::vftable = &condVarCmpInterfVftable;
    thisptr->eventId = *eventId;
    thisptr->unknown = a2;
    thisptr->condition = nullptr;

    {
        // Cache scenario variables
        auto objectMap = CCondInterfApi::get().getObjectMap(thisptr->unknown);
        Variables variables;

        auto scenVariables = getScenarioVariables(objectMap);
        if (scenVariables) {
            for (const auto& variable : scenVariables->variables) {
                variables.push_back(&variable);
            }
        }

        variables.swap(thisptr->variables);
    }

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    if (dialogApi.findButton(dialog, "BTN_OK")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condVarCmpInterfOkButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_CANCEL")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condVarCmpInterfCancelButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    const auto& listApi = CListBoxInterfApi::get();
    auto variablesList1 = dialogApi.findListBox(dialog, "TLBOX_VARIABLES1");
    if (variablesList1) {
        using ListBoxCallback = CCondInterfApi::Api::ListBoxDisplayCallback;
        ListBoxCallback callback{};
        Functor functor{};

        callback.callback = (ListBoxCallback::Callback)condVarCmpInterfListBoxDisplayHandler;
        CCondInterfApi::get().createListBoxDisplayFunctor(&functor, 0, thisptr, &callback);

        listApi.assignDisplayTextFunctor(dialog, "TLBOX_VARIABLES1", dialogName, &functor, false);
        FunctorApi::get().createOrFree(&functor, nullptr);
        listApi.setElementsTotal(variablesList1, thisptr->variables.size());
    }

    auto variablesList2 = dialogApi.findListBox(dialog, "TLBOX_VARIABLES2");
    if (variablesList2) {
        using ListBoxCallback = CCondInterfApi::Api::ListBoxDisplayCallback;
        ListBoxCallback callback{};
        Functor functor{};

        callback.callback = (ListBoxCallback::Callback)condVarCmpInterfListBoxDisplayHandler;
        CCondInterfApi::get().createListBoxDisplayFunctor(&functor, 0, thisptr, &callback);

        listApi.assignDisplayTextFunctor(dialog, "TLBOX_VARIABLES2", dialogName, &functor, false);
        FunctorApi::get().createOrFree(&functor, nullptr);
        listApi.setElementsTotal(variablesList2, thisptr->variables.size());
    }

    return thisptr;
}

struct CTestVarCmp : public game::ITestCondition
{
    CMidCondVarCmp* condition;
};

void __fastcall testVarCmpDestructor(CTestVarCmp* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall testVarCmpDoTest(const CTestVarCmp* thisptr,
                                 int /*%edx*/,
                                 const game::IMidgardObjectMap* objectMap,
                                 const game::CMidgardID* playerId,
                                 const game::CMidgardID* eventId)
{
    auto variables = getScenarioVariables(objectMap);
    if (!variables) {
        // Sanity check, this should never happen
        return false;
    }

    const auto& findById = game::CMidScenVariablesApi::get().findById;
    const auto* condition = thisptr->condition;

    const auto value1 = findById(variables, condition->variableId1)->value;
    const auto value2 = findById(variables, condition->variableId2)->value;

    switch (condition->compareType) {
    case CompareType::Equal:
        return value1 == value2;

    case CompareType::NotEqual:
        return value1 != value2;

    case CompareType::Greater:
        return value1 > value2;

    case CompareType::GreaterEqual:
        return value1 >= value2;

    case CompareType::Less:
        return value1 < value2;

    case CompareType::LessEqual:
        return value1 <= value2;
    }

    return false;
}

static game::ITestConditionVftable testVarCmpVftable{
    (game::ITestConditionVftable::Destructor)testVarCmpDestructor,
    (game::ITestConditionVftable::Test)testVarCmpDoTest,
};

game::ITestCondition* createTestVarCmp(game::CMidEvCondition* eventCondition)
{
    auto thisptr = (CTestVarCmp*)game::Memory::get().allocate(sizeof(CTestVarCmp));
    thisptr->condition = static_cast<CMidCondVarCmp*>(eventCondition);
    thisptr->vftable = &testVarCmpVftable;

    return thisptr;
}

} // namespace hooks
