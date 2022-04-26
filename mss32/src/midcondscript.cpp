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

#include "midcondscript.h"
#include "bindings/scenarioview.h"
#include "button.h"
#include "condinterf.h"
#include "condinterfhandler.h"
#include "d2string.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "editor.h"
#include "eventconditioncathooks.h"
#include "functor.h"
#include "game.h"
#include "interfmanager.h"
#include "iterators.h"
#include "listbox.h"
#include "mempool.h"
#include "midbag.h"
#include "midevcondition.h"
#include "midevent.h"
#include "midgardstream.h"
#include "radiobuttoninterf.h"
#include "scripts.h"
#include "testcondition.h"
#include "textboxinterf.h"
#include "utils.h"
#include <Windows.h>
#include <fmt/format.h>
#include <string>

namespace hooks {

static const std::string scriptSignature{"function checkEventCondition(scenario)"};

/** Custom event condition which logic is controlled entirely by lua script. */
struct CMidCondScript : public game::CMidEvCondition
{
    std::string code;
    std::string description;
};

void __fastcall condScriptDestructor(CMidCondScript* thisptr, int /*%edx*/, char flags)
{
    // Manually call the destructors
    thisptr->code.~basic_string();
    thisptr->description.~basic_string();

    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall condScriptIsIdsEqual(const CMidCondScript*, int /*%edx*/, const game::CMidgardID*)
{
    return false;
}

void __fastcall condScriptAddToList(const CMidCondScript*,
                                    int /*%edx*/,
                                    game::Set<game::CMidgardID>*)
{ }

bool __fastcall condScriptIsValid(const CMidCondScript* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap*)
{
    return true;
}

bool __fastcall condScriptMethod4(const CMidCondScript*, int /*%edx*/, int)
{
    return false;
}

void __fastcall condScriptStream(CMidCondScript* thisptr,
                                 int /*%edx*/,
                                 game::IMidgardStream** stream)
{
    auto streamPtr = *stream;
    auto vftable = streamPtr->vftable;

    int codeLength = static_cast<int>(thisptr->code.size());
    vftable->streamInt(streamPtr, "CODE_LEN", &codeLength);

    if (vftable->readMode(streamPtr)) {
        thisptr->code.resize(codeLength);
    }

    vftable->streamString(streamPtr, "CODE", thisptr->code.data());

    int descrLength = static_cast<int>(thisptr->description.size());
    vftable->streamInt(streamPtr, "DESCR_LEN", &descrLength);

    if (vftable->readMode(streamPtr)) {
        thisptr->description.resize(descrLength);
    }

    vftable->streamString(streamPtr, "DESCR", thisptr->description.data());
}

// clang-format off
static game::CMidEvConditionVftable condScriptVftable{
    (game::CMidEvConditionVftable::Destructor)condScriptDestructor,
    (game::CMidEvConditionVftable::IsIdsEqual)condScriptIsIdsEqual,
    (game::CMidEvConditionVftable::AddToList)condScriptAddToList,
    (game::CMidEvConditionVftable::IsValid)condScriptIsValid,
    (game::CMidEvConditionVftable::Method4)condScriptMethod4,
    (game::CMidEvConditionVftable::Stream)condScriptStream,
};
// clang-format on

game::CMidEvCondition* createMidCondScript()
{
    using namespace game;

    auto script = (CMidCondScript*)Memory::get().allocate(sizeof(CMidCondScript));
    std::memset(script, 0, sizeof(CMidCondScript));

    script->category.vftable = EventCondCategories::vftable();
    script->category.id = customEventConditions().script.category.id;
    script->category.table = customEventConditions().script.category.table;
    std::string().swap(script->code);
    std::string().swap(script->description);

    script->vftable = &condScriptVftable;
    return script;
}

void __stdcall midCondScriptGetInfoString(game::String* info,
                                          const game::IMidgardObjectMap* objectMap,
                                          const game::CMidEvCondition* eventCondition)
{
    const auto textInfoId = &customEventConditions().script.infoText;
    std::string str{game::gameFunctions().getInterfaceText(textInfoId)};

    replace(str, "%DESC%", static_cast<const CMidCondScript*>(eventCondition)->description);
    game::StringApi::get().initFromStringN(info, str.c_str(), str.length());
}

/** Toggle button indices, must match with RAD_FILTER in DLG_COND_SCRIPT in ScenEdit.dlg. */
enum class ObjectFilter : int
{
    Stack,
    City,
    Ruin,
    Location,
    Bag,
    Other,
};

using IdVector = std::vector<game::CMidgardID>;

struct CCondScriptInterf : public game::editor::CCondInterf
{
    void* unknown;
    CMidCondScript* condition;
    game::CMidgardID eventId;
    IdVector stacks;
    IdVector cities;
    IdVector ruins;
    IdVector locations;
    IdVector bags;
};

void __fastcall condScriptInterfDestructor(CCondScriptInterf* thisptr, int /*%edx*/, char flags)
{
    // Manually call destructors
    thisptr->stacks.~vector();
    thisptr->cities.~vector();
    thisptr->ruins.~vector();
    thisptr->locations.~vector();
    thisptr->bags.~vector();

    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::CInterfaceVftable::OnVisibilityChanged onVisibilityChanged{};

void __fastcall condScriptInterfOnVisibilityChanged(CCondScriptInterf* thisptr,
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

    auto editBox = dialogApi.findEditBox(dialog, "EDIT_CODE");
    if (editBox) {
        CEditBoxInterfApi::get().setString(editBox, thisptr->condition->code.c_str());
    }

    auto editComment = dialogApi.findEditBox(dialog, "EDIT_COMMENT");
    if (editComment) {
        CEditBoxInterfApi::get().setString(editComment, thisptr->condition->description.c_str());
    }
}

bool __fastcall condScriptInterfSetEventCondition(CCondScriptInterf* thisptr,
                                                  int /*%edx*/,
                                                  game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().script.category.id) {
        thisptr->condition = static_cast<CMidCondScript*>(eventCondition);
        return true;
    }

    return false;
}

static game::editor::CCondInterfVftable condScriptInterfVftable{};

void __fastcall condScriptInterfCancelButtonHandler(CCondScriptInterf* thisptr, int /*%edx*/)
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

void __fastcall condScriptInterfOkButtonHandler(CCondScriptInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto editBox = dialogApi.findEditBox(dialog, "EDIT_CODE");
    if (!editBox) {
        return;
    }

    const auto& code = editBox->data->editBoxData.inputString;
    if (!code.length) {
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
            showMessageBox(getTranslatedText("X100TA0631"));
            return;
        }
    }

    auto condition = static_cast<CMidCondScript*>(createMidCondScript());
    condition->code = std::string(code.string);

    auto editComment = dialogApi.findEditBox(dialog, "EDIT_COMMENT");
    if (editComment) {
        condition->description = std::string(editComment->data->editBoxData.inputString.string);
    }

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

void __fastcall condScriptInterfLoadButtonHandler(CCondScriptInterf* thisptr, int /*%edx*/)
{
    using namespace game;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto editBox = dialogApi.findEditBox(dialog, "EDIT_CODE");
    if (!editBox) {
        return;
    }

    std::string script;
    if (readUserSelectedFile(script, "Lua scripts (*.lua)\0*.lua\0\0",
                             scriptsFolder().string().c_str())) {
        CEditBoxInterfApi::get().setString(editBox, script.c_str());
    }
}

static const IdVector* getObjectIds(const CCondScriptInterf* interf, ObjectFilter filter)
{
    switch (filter) {
    case ObjectFilter::Stack:
        return &interf->stacks;

    case ObjectFilter::City:
        return &interf->cities;

    case ObjectFilter::Ruin:
        return &interf->ruins;

    case ObjectFilter::Location:
        return &interf->locations;

    case ObjectFilter::Bag:
        return &interf->bags;
    }

    return nullptr;
}

void __fastcall condScriptInterfPasteIdButtonHandler(CCondScriptInterf* thisptr, int /*%edx*/)
{
    using namespace game;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto editBox = dialogApi.findEditBox(dialog, "EDIT_CODE");
    if (!editBox) {
        return;
    }

    const auto& listApi = CListBoxInterfApi::get();
    auto listBox = dialogApi.findListBox(dialog, "TLBOX_OBJECTS");
    if (!listBox) {
        return;
    }

    auto radioButton = CDialogInterfApi::get().findRadioButton(dialog, "RAD_FILTER");
    if (!radioButton) {
        return;
    }

    auto* ids = getObjectIds(thisptr, (ObjectFilter)radioButton->data->selectedButton);
    if (!ids) {
        return;
    }

    const auto selectedIndex = CListBoxInterfApi::get().selectedIndex(listBox);
    const auto total = static_cast<int>(ids->size());
    if (selectedIndex < 0 || selectedIndex >= total) {
        return;
    }

    const CMidgardID* objectId = &(*ids)[selectedIndex];
    const auto idString{fmt::format("'{:s}'", idToString(objectId))};

    std::string code{editBox->data->editBoxData.inputString.string};
    const auto cursor = editBox->data->editBoxData.editCursorPos;

    code.insert(cursor, idString);

    const auto& editBoxApi = CEditBoxInterfApi::get();
    editBoxApi.setString(editBox, code.c_str());

    // Set cursor position after inserted id string
    editBox->data->editBoxData.editCursorPos = cursor + idString.size();
    editBoxApi.updateFocus(editBox->data->editBoxFocus.data);
    editBoxApi.update(editBox);
}

void __fastcall condScriptInterfDocsButtonHandler(CCondScriptInterf* thisptr, int /*%edx*/)
{
    ShellExecute(nullptr, "open",
                 "https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md",
                 nullptr, nullptr, SW_SHOWNORMAL);
}

void __fastcall condScriptInterfOnFilterChanged(CCondScriptInterf* thisptr,
                                                int /*%edx*/,
                                                int selectedButton)
{
    using namespace game;

    auto dialog = thisptr->popupData->dialog;
    const auto& listApi = CListBoxInterfApi::get();
    auto listBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_OBJECTS");
    if (!listBox) {
        return;
    }

    auto* ids = getObjectIds(thisptr, (ObjectFilter)selectedButton);
    CListBoxInterfApi::get().setElementsTotal(listBox, ids ? static_cast<int>(ids->size()) : 0);
}

void __fastcall condScriptInterfStackListBoxDisplayHandler(CCondScriptInterf* thisptr,
                                                           int /*%edx*/,
                                                           game::String* string,
                                                           bool a3,
                                                           int selectedIndex)
{
    using namespace game;
    using namespace editor;

    auto dialog = thisptr->popupData->dialog;
    auto radioButton = CDialogInterfApi::get().findRadioButton(dialog, "RAD_FILTER");
    if (!radioButton) {
        return;
    }

    const auto filter = (ObjectFilter)radioButton->data->selectedButton;
    auto* ids = getObjectIds(thisptr, filter);
    if (!ids) {
        return;
    }

    const auto total = static_cast<int>(ids->size());
    if (selectedIndex < 0 || selectedIndex >= total) {
        return;
    }

    auto objectMap = CCondInterfApi::get().getObjectMap(thisptr->unknown);
    const CMidgardID* objectId = &(*ids)[selectedIndex];

    if (filter == ObjectFilter::Bag) {
        auto bag = (const CMidBag*)objectMap->vftable->findScenarioObjectById(objectMap, objectId);
        if (!bag) {
            // This should never happen
            return;
        }

        const auto& pos = bag->mapElement.position;
        const auto description{
            fmt::format("{:s} ({:d}, {:d})", idToString(objectId), pos.x, pos.y)};

        StringApi::get().initFromStringN(string, description.c_str(), description.size());
        return;
    }

    String tmp{};
    editorFunctions.getObjectNamePos(&tmp, objectMap, objectId);

    if (tmp.string != string->string) {
        StringApi::get().initFromStringN(string, tmp.string, tmp.length);
    }

    StringApi::get().free(&tmp);
}

using CreateIterator = game::Iterators::Api::CreateIterator;

static void getObjectIds(IdVector& ids,
                         game::IMidgardObjectMap* objectMap,
                         const CreateIterator& create,
                         const CreateIterator& createEnd)
{
    using namespace game;

    IteratorPtr iteratorPtr;
    IteratorPtr endIteratorPtr;

    create(&iteratorPtr, objectMap);
    createEnd(&endIteratorPtr, objectMap);

    auto iterator = iteratorPtr.data;

    while (!iterator->vftable->end(iterator, endIteratorPtr.data)) {
        ids.push_back(*iterator->vftable->getObjectId(iterator));

        iterator->vftable->advance(iterator);
    }

    auto& freeSmartPtr = SmartPointerApi::get().createOrFree;
    freeSmartPtr((SmartPointer*)&iteratorPtr, nullptr);
    freeSmartPtr((SmartPointer*)&endIteratorPtr, nullptr);
}

game::editor::CCondInterf* createCondScriptInterf(game::ITask* task,
                                                  void* a2,
                                                  const game::CMidgardID* eventId)
{
    using namespace game;
    using namespace editor;

    auto thisptr = (CCondScriptInterf*)Memory::get().allocate(sizeof(CCondScriptInterf));
    std::memset(thisptr, 0, sizeof(CCondScriptInterf));

    static const char dialogName[]{"DLG_COND_SCRIPT"};

    CCondInterfApi::get().constructor(thisptr, dialogName, task);

    static bool initialized{false};
    if (!initialized) {
        initialized = true;

        onVisibilityChanged = thisptr->CInterface::vftable->onVisibilityChanged;

        std::memcpy(&condScriptInterfVftable, thisptr->CInterface::vftable,
                    sizeof(CInterfaceVftable));

        // Change methods that are specific for our custom class
        condScriptInterfVftable
            .destructor = (CInterfaceVftable::Destructor)&condScriptInterfDestructor;
        condScriptInterfVftable
            .onVisibilityChanged = (CInterfaceVftable::
                                        OnVisibilityChanged)&condScriptInterfOnVisibilityChanged;
        condScriptInterfVftable
            .setEventCondition = (CCondInterfVftable::
                                      SetEventCondition)&condScriptInterfSetEventCondition;
    }

    thisptr->CInterface::vftable = &condScriptInterfVftable;
    thisptr->eventId = *eventId;
    thisptr->unknown = a2;
    thisptr->condition = nullptr;

    auto objectMap = CCondInterfApi::get().getObjectMap(thisptr->unknown);
    auto iterators = Iterators::get();

    IdVector stacks;
    getObjectIds(stacks, objectMap, iterators.createStacksIterator,
                 iterators.createStacksEndIterator);
    stacks.swap(thisptr->stacks);

    IdVector cities;
    getObjectIds(cities, objectMap, iterators.createFortificationsIterator,
                 iterators.createFortificationsEndIterator);
    cities.swap(thisptr->cities);

    IdVector ruins;
    getObjectIds(ruins, objectMap, iterators.createRuinsIterator, iterators.createRuinsEndIterator);
    ruins.swap(thisptr->ruins);

    IdVector locations;
    getObjectIds(locations, objectMap, iterators.createLocationsIterator,
                 iterators.createLocationsEndIterator);
    locations.swap(thisptr->locations);

    IdVector bags;
    getObjectIds(bags, objectMap, iterators.createBagsIterator, iterators.createBagsEndIterator);
    bags.swap(thisptr->bags);

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    if (dialogApi.findButton(dialog, "BTN_OK")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condScriptInterfOkButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_CANCEL")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condScriptInterfCancelButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_LOAD")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condScriptInterfLoadButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_PASTE_ID")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condScriptInterfPasteIdButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_PASTE_ID", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_DOCS")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condScriptInterfDocsButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_DOCS", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    const auto& radioApi = CRadioButtonInterfApi::get();
    auto radioButton = dialogApi.findRadioButton(dialog, "RAD_FILTER");
    if (radioButton) {
        radioApi.setCheckedButton(radioButton, (int)ObjectFilter::Stack);

        using Callback = CCondInterfApi::Api::RadioButtonCallback;
        Callback callback{};
        Functor functor{};

        callback.callback = (Callback::Callback)condScriptInterfOnFilterChanged;
        CCondInterfApi::get().createRadioButtonFunctor(&functor, 0, thisptr, &callback);

        radioApi.setOnButtonPressed(dialog, "RAD_FILTER", dialogName, &functor);
        FunctorApi::get().createOrFree(&functor, nullptr);

        // Disable button since 'other' list is not implemented yet
        radioApi.setButtonEnabled(radioButton, (int)ObjectFilter::Other, false);
    }

    const auto& listApi = CListBoxInterfApi::get();
    auto listBox = dialogApi.findListBox(dialog, "TLBOX_OBJECTS");
    if (listBox) {
        using ListBoxCallback = CCondInterfApi::Api::ListBoxDisplayCallback;
        ListBoxCallback callback{};
        Functor functor{};

        callback.callback = (ListBoxCallback::Callback)condScriptInterfStackListBoxDisplayHandler;
        CCondInterfApi::get().createListBoxDisplayFunctor(&functor, 0, thisptr, &callback);

        listApi.assignDisplayTextFunctor(dialog, "TLBOX_OBJECTS", dialogName, &functor, false);
        FunctorApi::get().createOrFree(&functor, nullptr);
        // Init with stacks, since its toggle button selected by default
        listApi.setElementsTotal(listBox, thisptr->stacks.size());
    }

    auto signature = dialogApi.findTextBox(dialog, "TXT_SIGNATURE");
    if (signature) {
        CTextBoxInterfApi::get().setString(signature, scriptSignature.c_str());
    }

    CEditBoxInterfApi::get().setFilterAndLength(dialog, "EDIT_CODE", dialogName,
                                                EditFilter::NoFilter, 4096);

    // Disable buttons that are not implemented yet
    auto objectives = dialogApi.findButton(dialog, "BTN_OBJECTIVES");
    if (objectives) {
        objectives->vftable->setEnabled(objectives, false);
    }

    auto buttonCheck = dialogApi.findButton(dialog, "BTN_CHECK");
    if (buttonCheck) {
        buttonCheck->vftable->setEnabled(buttonCheck, false);
    }

    auto codeUp = dialogApi.findButton(dialog, "BTN_CODE_UP");
    if (codeUp) {
        codeUp->vftable->setEnabled(codeUp, false);
    }

    auto codeDown = dialogApi.findButton(dialog, "BTN_CODE_DOWN");
    if (codeDown) {
        codeDown->vftable->setEnabled(codeDown, false);
    }

    return thisptr;
}

struct CTestScript : public game::ITestCondition
{
    CMidCondScript* condition;
};

void __fastcall testScriptDestructor(CTestScript* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall testScriptDoTest(const CTestScript* thisptr,
                                 int /*%edx*/,
                                 const game::IMidgardObjectMap* objectMap,
                                 const game::CMidgardID* playerId,
                                 const game::CMidgardID* eventId)
{
    const auto& body = thisptr->condition->code;
    if (body.empty()) {
        return false;
    }

    const auto code{fmt::format("{:s}\n{:s}\nend\n", scriptSignature, body)};
    sol::protected_function_result result;
    auto env = executeScript(code, result);
    if (!result.valid()) {
        const sol::error err = result;
        logError("mssProxyError.log",
                 fmt::format("Failed to load scriptable event condition.\n"
                             "Event id {:s}\n"
                             "Description: '{:s}'\n"
                             "Script:\n'{:s}'\n"
                             "Reason: '{:s}'",
                             idToString(eventId), thisptr->condition->description, code,
                             err.what()));
        return false;
    }

    using CheckCondition = std::function<bool(const bindings::ScenarioView&)>;

    auto checkCondition = getScriptFunction<CheckCondition>(env, "checkEventCondition", true);
    if (!checkCondition) { // Sanity check, this should never happen
        return false;
    }

    try {
        const bindings::ScenarioView scenario{objectMap};

        return (*checkCondition)(scenario);
    } catch (const std::exception& e) {
        logError("mssProxyError.log",
                 fmt::format("Failed to execute scriptable event condition.\n"
                             "Event id {:s}\n"
                             "Description: '{:s}'\n"
                             "Script:\n'{:s}'\n"
                             "Reason: '{:s}'",
                             idToString(eventId), thisptr->condition->description, code, e.what()));
    }

    return false;
}

static game::ITestConditionVftable testScriptVftable{
    (game::ITestConditionVftable::Destructor)testScriptDestructor,
    (game::ITestConditionVftable::Test)testScriptDoTest,
};

game::ITestCondition* createTestScript(game::CMidEvCondition* eventCondition)
{
    auto thisptr = (CTestScript*)game::Memory::get().allocate(sizeof(CTestScript));
    thisptr->condition = static_cast<CMidCondScript*>(eventCondition);
    thisptr->vftable = &testScriptVftable;

    return thisptr;
}

} // namespace hooks
