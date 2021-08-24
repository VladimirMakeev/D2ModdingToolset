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

#include "midcondgamemode.h"
#include "button.h"
#include "condinterf.h"
#include "condinterfhandler.h"
#include "d2string.h"
#include "dialoginterf.h"
#include "eventconditioncathooks.h"
#include "functor.h"
#include "game.h"
#include "interfmanager.h"
#include "mempool.h"
#include "midevcondition.h"
#include "midevent.h"
#include "mideventhooks.h"
#include "midgard.h"
#include "midgardobjectmap.h"
#include "midgardstream.h"
#include "radiobuttoninterf.h"
#include "testcondition.h"
#include "textids.h"
#include "utils.h"

namespace hooks {

/** Same as RAD_GAME_MODE button indices in DLG_COND_GAME_MODE from ScenEdit.dlg */
enum class GameMode : int
{
    Single,
    Hotseat,
    Online,
};

/** Custom event condition that checks current game mode. */
struct CMidCondGameMode : public game::CMidEvCondition
{
    GameMode gameMode;
};

void __fastcall condGameModeDestructor(CMidCondGameMode* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall condGameModeIsIdsEqual(const CMidCondGameMode*,
                                       int /*%edx*/,
                                       const game::CMidgardID*)
{
    return false;
}

void __fastcall condGameModeAddToList(const CMidCondGameMode*,
                                      int /*%edx*/,
                                      game::SortedList<game::CMidgardID>*)
{ }

bool __fastcall condGameModeIsValid(const CMidCondGameMode* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap*)
{
    const auto mode = thisptr->gameMode;
    return mode == GameMode::Single || mode == GameMode::Hotseat || mode == GameMode::Online;
}

bool __fastcall condGameModeMethod4(const CMidCondGameMode*, int /*%edx*/, int)
{
    return false;
}

void __fastcall condGameModeStream(CMidCondGameMode* thisptr,
                                   int /*%edx*/,
                                   game::IMidgardStream** stream)
{
    auto streamPtr = *stream;
    auto vftable = streamPtr->vftable;

    vftable->streamInt(streamPtr, "MODE", (int*)&thisptr->gameMode);
}

// clang-format off
static game::CMidEvConditionVftable condGameModeVftable{
    (game::CMidEvConditionVftable::Destructor)condGameModeDestructor,
    (game::CMidEvConditionVftable::IsIdsEqual)condGameModeIsIdsEqual,
    (game::CMidEvConditionVftable::AddToList)condGameModeAddToList,
    (game::CMidEvConditionVftable::IsValid)condGameModeIsValid,
    (game::CMidEvConditionVftable::Method4)condGameModeMethod4,
    (game::CMidEvConditionVftable::Stream)condGameModeStream,
};
// clang-format on

game::CMidEvCondition* createMidCondGameMode()
{
    using namespace game;

    auto gameMode = (CMidCondGameMode*)Memory::get().allocate(sizeof(CMidCondGameMode));

    gameMode->category.vftable = EventCondCategories::vftable();
    gameMode->category.id = customEventConditions().gameMode.category.id;
    gameMode->category.table = customEventConditions().gameMode.category.table;
    gameMode->gameMode = GameMode::Single;
    gameMode->vftable = &condGameModeVftable;

    return gameMode;
}

void __stdcall midCondGameModeGetInfoString(game::String* info,
                                            const game::IMidgardObjectMap* objectMap,
                                            const game::CMidEvCondition* eventCondition)
{
    const auto textInfoId = &customEventConditions().gameMode.infoText;
    std::string str{game::gameFunctions().getInterfaceText(textInfoId)};

    const auto gameMode = static_cast<const CMidCondGameMode*>(eventCondition)->gameMode;
    std::string modeName;

    switch (gameMode) {
    case GameMode::Single:
        modeName = getTranslatedText(textIds().interf.gameMode.single.c_str());
        if (modeName.empty()) {
            modeName = "single player";
        }
        break;
    case GameMode::Hotseat:
        modeName = getTranslatedText(textIds().interf.gameMode.hotseat.c_str());
        if (modeName.empty()) {
            modeName = "hotseat";
        }
        break;
    case GameMode::Online:
        modeName = getTranslatedText(textIds().interf.gameMode.online.c_str());
        if (modeName.empty()) {
            modeName = "online";
        }
        break;
    }

    replace(str, "%MODE%", modeName);
    game::StringApi::get().initFromStringN(info, str.c_str(), str.length());
}

struct CCondGameModeInterf : public game::editor::CCondInterf
{
    void* unknown;
    CMidCondGameMode* gameModeCondition;
    game::CMidgardID eventId;
};

void __fastcall condGameModeInterfDestructor(CCondGameModeInterf* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::CInterfaceVftable::OnVisibilityChanged onVisibilityChanged{};

void __fastcall condGameModeInterfOnVisibilityChanged(CCondGameModeInterf* thisptr,
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

    if (!thisptr->gameModeCondition) {
        return;
    }

    auto gameMode = thisptr->gameModeCondition->gameMode;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto radioButton = dialogApi.findRadioButton(dialog, "RAD_GAME_MODE");
    if (radioButton) {
        CRadioButtonInterfApi::get().setCheckedButton(radioButton, (int)gameMode);
    }
}

bool __fastcall condGameModeInterfSetEventCondition(CCondGameModeInterf* thisptr,
                                                    int /*%edx*/,
                                                    game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().gameMode.category.id) {
        thisptr->gameModeCondition = static_cast<CMidCondGameMode*>(eventCondition);
        return true;
    }

    return false;
}

static game::editor::CCondInterfVftable condGameModeInterfVftable{};

void __fastcall condGameModeInterfCancelButtonHandler(CCondGameModeInterf* thisptr, int /*%edx*/)
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

void __fastcall condGameModeInterfOkButtonHandler(CCondGameModeInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

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

    auto midEvent = (CMidEvent*)objectMap->vftable
                        ->findScenarioObjectByIdForChange(objectMap, &thisptr->eventId);

    auto gameMode = static_cast<CMidCondGameMode*>(createMidCondGameMode());

    auto radioButton = dialogApi.findRadioButton(dialog, "RAD_GAME_MODE");
    if (radioButton) {
        gameMode->gameMode = static_cast<GameMode>(radioButton->data->selectedButton);
    }

    CMidEventApi::get().addCondition(midEvent, nullptr, gameMode);

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->hideInterface(ptr.data, thisptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);

    if (thisptr) {
        thisptr->CInterface::vftable->destructor(thisptr, 1);
    }
}

game::editor::CCondInterf* createCondGameModeInterf(game::ITask* task,
                                                    void* a2,
                                                    const game::CMidgardID* eventId)
{
    using namespace game;
    using namespace editor;

    auto thisptr = (CCondGameModeInterf*)Memory::get().allocate(sizeof(CCondGameModeInterf));

    static const char dialogName[]{"DLG_COND_GAME_MODE"};

    CCondInterfApi::get().constructor(thisptr, dialogName, task);

    static bool initialized{false};
    if (!initialized) {
        onVisibilityChanged = thisptr->CInterface::vftable->onVisibilityChanged;

        initialized = true;
        std::memcpy(&condGameModeInterfVftable, thisptr->CInterface::vftable,
                    sizeof(CInterfaceVftable));

        // Change methods that are specific for our custom class
        condGameModeInterfVftable
            .destructor = (CInterfaceVftable::Destructor)&condGameModeInterfDestructor;
        condGameModeInterfVftable
            .onVisibilityChanged = (CInterfaceVftable::
                                        OnVisibilityChanged)&condGameModeInterfOnVisibilityChanged;
        condGameModeInterfVftable
            .setEventCondition = (CCondInterfVftable::
                                      SetEventCondition)&condGameModeInterfSetEventCondition;
    }

    thisptr->CInterface::vftable = &condGameModeInterfVftable;
    thisptr->eventId = *eventId;
    thisptr->unknown = a2;
    thisptr->gameModeCondition = nullptr;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    if (dialogApi.findButton(dialog, "BTN_OK")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condGameModeInterfOkButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_CANCEL")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condGameModeInterfCancelButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    return thisptr;
}

struct CTestGameMode : public game::ITestCondition
{
    CMidCondGameMode* condition;
};

void __fastcall testGameModeDestructor(CTestGameMode* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall testGameModeDoTest(const CTestGameMode* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap*,
                                   const game::CMidgardID*,
                                   const game::CMidgardID*)
{
    const auto* data = game::CMidgardApi::get().instance()->data;

    switch (thisptr->condition->gameMode) {
    case GameMode::Single:
        return !data->hotseatGame && !data->multiplayerGame;

    case GameMode::Hotseat:
        return data->hotseatGame;

    case GameMode::Online:
        return data->multiplayerGame && !data->hotseatGame;
    }

    return false;
}

static game::ITestConditionVftable testGameModeVftable{
    (game::ITestConditionVftable::Destructor)testGameModeDestructor,
    (game::ITestConditionVftable::Test)testGameModeDoTest,
};

game::ITestCondition* createTestGameMode(game::CMidEvCondition* eventCondition)
{
    auto thisptr = (CTestGameMode*)game::Memory::get().allocate(sizeof(CTestGameMode));
    thisptr->condition = static_cast<CMidCondGameMode*>(eventCondition);
    thisptr->vftable = &testGameModeVftable;

    return thisptr;
}

bool checkGameModeConditionValid(game::CDialogInterf*,
                                 const game::IMidgardObjectMap* objectMap,
                                 const game::CMidgardID* eventId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, eventId);
    if (!obj) {
        return false;
    }

    const auto& category = customEventConditions().gameMode.category;
    auto event = static_cast<const CMidEvent*>(obj);

    std::vector<const CMidEvCondition*> conditions{};
    getConditionsOfType(event, &category, conditions);

    if (conditions.size() > 1) {
        auto message = getTranslatedText(textIds().interf.gameMode.tooMany.c_str());
        if (message.empty()) {
            message = "Only one condition of type \"Game mode\" is allowed per event.";
        }

        showMessageBox(message);
        return false;
    }

    return true;
}

} // namespace hooks
