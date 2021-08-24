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

#include "midcondplayertype.h"
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
#include "midgardobjectmap.h"
#include "midgardstream.h"
#include "midplayer.h"
#include "testcondition.h"
#include "textids.h"
#include "togglebutton.h"
#include "utils.h"

namespace hooks {

/** Custom event condition that checks whether affected player is controlled by AI or not. */
struct CMidCondPlayerType : public game::CMidEvCondition
{
    bool playerTypeAi;
};

void __fastcall condPlayerTypeDestructor(CMidCondPlayerType* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall condPlayerTypeIsIdsEqual(const CMidCondPlayerType*,
                                         int /*%edx*/,
                                         const game::CMidgardID*)
{
    return false;
}

void __fastcall condPlayerTypeAddToList(const CMidCondPlayerType*,
                                        int /*%edx*/,
                                        game::SortedList<game::CMidgardID>*)
{ }

bool __fastcall condPlayerTypeIsValid(const CMidCondPlayerType* thisptr,
                                      int /*%edx*/,
                                      const game::IMidgardObjectMap*)
{
    return true;
}

bool __fastcall condPlayerTypeMethod4(const CMidCondPlayerType*, int /*%edx*/, int)
{
    return false;
}

void __fastcall condPlayerTypeStream(CMidCondPlayerType* thisptr,
                                     int /*%edx*/,
                                     game::IMidgardStream** stream)
{
    auto streamPtr = *stream;
    auto vftable = streamPtr->vftable;

    vftable->streamBool(streamPtr, "AI", &thisptr->playerTypeAi);
}

// clang-format off
static game::CMidEvConditionVftable condPlayerTypeVftable{
    (game::CMidEvConditionVftable::Destructor)condPlayerTypeDestructor,
    (game::CMidEvConditionVftable::IsIdsEqual)condPlayerTypeIsIdsEqual,
    (game::CMidEvConditionVftable::AddToList)condPlayerTypeAddToList,
    (game::CMidEvConditionVftable::IsValid)condPlayerTypeIsValid,
    (game::CMidEvConditionVftable::Method4)condPlayerTypeMethod4,
    (game::CMidEvConditionVftable::Stream)condPlayerTypeStream,
};
// clang-format on

game::CMidEvCondition* createMidCondPlayerType()
{
    using namespace game;

    auto playerType = (CMidCondPlayerType*)Memory::get().allocate(sizeof(CMidCondPlayerType));

    playerType->category.vftable = EventCondCategories::vftable();
    playerType->category.id = customEventConditions().playerType.category.id;
    playerType->category.table = customEventConditions().playerType.category.table;
    playerType->playerTypeAi = false;
    playerType->vftable = &condPlayerTypeVftable;

    return playerType;
}

void __stdcall midCondPlayerTypeGetInfoString(game::String* info,
                                              const game::IMidgardObjectMap* objectMap,
                                              const game::CMidEvCondition* eventCondition)
{
    const auto textInfoId = &customEventConditions().playerType.infoText;
    std::string str{game::gameFunctions().getInterfaceText(textInfoId)};

    const auto ai = static_cast<const CMidCondPlayerType*>(eventCondition)->playerTypeAi;
    std::string type;

    const auto& playerTypeTextIds = textIds().events.conditions.playerType;

    if (ai) {
        type = getTranslatedText(playerTypeTextIds.ai.c_str());
        if (type.empty()) {
            type = "AI";
        }
    } else {
        type = getTranslatedText(playerTypeTextIds.human.c_str());
        if (type.empty()) {
            type = "human";
        }
    }

    replace(str, "%TYPE%", type);
    game::StringApi::get().initFromStringN(info, str.c_str(), str.length());
}

struct CCondPlayerTypeInterf : public game::editor::CCondInterf
{
    void* unknown;
    CMidCondPlayerType* condition;
    game::CMidgardID eventId;
};

void __fastcall condPlayerTypeInterfDestructor(CCondPlayerTypeInterf* thisptr,
                                               int /*%edx*/,
                                               char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::CInterfaceVftable::OnVisibilityChanged onVisibilityChanged{};

void __fastcall condPlayerTypeInterfOnVisibilityChanged(CCondPlayerTypeInterf* thisptr,
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

    auto toggleButton = dialogApi.findToggleButton(dialog, "TOG_AI");
    if (toggleButton) {
        CToggleButtonApi::get().setChecked(toggleButton, thisptr->condition->playerTypeAi);
    }
}

bool __fastcall condPlayerTypeInterfSetEventCondition(CCondPlayerTypeInterf* thisptr,
                                                      int /*%edx*/,
                                                      game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().playerType.category.id) {
        thisptr->condition = static_cast<CMidCondPlayerType*>(eventCondition);
        return true;
    }

    return false;
}

static game::editor::CCondInterfVftable condPlayerTypeInterfVftable{};

void __fastcall condPlayerTypeInterfCancelButtonHandler(CCondPlayerTypeInterf* thisptr,
                                                        int /*%edx*/)
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

void __fastcall condPlayerTypeInterfOkButtonHandler(CCondPlayerTypeInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

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

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    auto condition = static_cast<CMidCondPlayerType*>(createMidCondPlayerType());

    auto toggleButton = dialogApi.findToggleButton(dialog, "TOG_AI");
    if (toggleButton) {
        condition->playerTypeAi = toggleButton->data->checked;
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

game::editor::CCondInterf* createCondPlayerTypeInterf(game::ITask* task,
                                                      void* a2,
                                                      const game::CMidgardID* eventId)
{
    using namespace game;
    using namespace editor;

    auto thisptr = (CCondPlayerTypeInterf*)Memory::get().allocate(sizeof(CCondPlayerTypeInterf));

    static const char dialogName[]{"DLG_COND_PLAYER_TYPE"};

    CCondInterfApi::get().constructor(thisptr, dialogName, task);

    static bool initialized{false};
    if (!initialized) {
        initialized = true;

        onVisibilityChanged = thisptr->CInterface::vftable->onVisibilityChanged;

        std::memcpy(&condPlayerTypeInterfVftable, thisptr->CInterface::vftable,
                    sizeof(CInterfaceVftable));

        // Change methods that are specific for our custom class
        condPlayerTypeInterfVftable
            .destructor = (CInterfaceVftable::Destructor)&condPlayerTypeInterfDestructor;
        condPlayerTypeInterfVftable.onVisibilityChanged =
            (CInterfaceVftable::OnVisibilityChanged)&condPlayerTypeInterfOnVisibilityChanged;
        condPlayerTypeInterfVftable
            .setEventCondition = (CCondInterfVftable::
                                      SetEventCondition)&condPlayerTypeInterfSetEventCondition;
    }

    thisptr->CInterface::vftable = &condPlayerTypeInterfVftable;
    thisptr->eventId = *eventId;
    thisptr->unknown = a2;
    thisptr->condition = nullptr;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    if (dialogApi.findButton(dialog, "BTN_OK")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condPlayerTypeInterfOkButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_CANCEL")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condPlayerTypeInterfCancelButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    return thisptr;
}

struct CTestPlayerType : public game::ITestCondition
{
    CMidCondPlayerType* condition;
};

void __fastcall testPlayerTypeDestructor(CTestPlayerType* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall testPlayerTypeDoTest(const CTestPlayerType* thisptr,
                                     int /*%edx*/,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* playerId,
                                     const game::CMidgardID* eventId)
{
    using namespace game;

    if (!CMidEventApi::get().affectsPlayer(objectMap, playerId, eventId)) {
        return false;
    }

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, playerId);
    if (!obj) {
        return false;
    }

    auto player = static_cast<const CMidPlayer*>(obj);
    return thisptr->condition->playerTypeAi == !player->isHuman;
}

static game::ITestConditionVftable testPlayerTypeVftable{
    (game::ITestConditionVftable::Destructor)testPlayerTypeDestructor,
    (game::ITestConditionVftable::Test)testPlayerTypeDoTest,
};

game::ITestCondition* createTestPlayerType(game::CMidEvCondition* eventCondition)
{
    auto thisptr = (CTestPlayerType*)game::Memory::get().allocate(sizeof(CTestPlayerType));
    thisptr->condition = static_cast<CMidCondPlayerType*>(eventCondition);
    thisptr->vftable = &testPlayerTypeVftable;

    return thisptr;
}

bool checkPlayerTypeConditionValid(game::CDialogInterf* dialog,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* eventId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, eventId);
    if (!obj) {
        return false;
    }

    const auto& category = customEventConditions().playerType.category;
    auto event = static_cast<const CMidEvent*>(obj);

    std::vector<const CMidEvCondition*> conditions{};
    getConditionsOfType(event, &category, conditions);

    if (conditions.size() > 1) {
        auto message = getTranslatedText(textIds().events.conditions.playerType.tooMany.c_str());
        if (message.empty()) {
            message = "Only one condition of type \"Player type\" is allowed per event.";
        }

        showMessageBox(message);
        return false;
    }

    return true;
}

} // namespace hooks
