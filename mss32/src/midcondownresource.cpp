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

#include "midcondownresource.h"
#include "button.h"
#include "condinterf.h"
#include "condinterfhandler.h"
#include "dialoginterf.h"
#include "editboxinterf.h"
#include "eventconditioncathooks.h"
#include "functor.h"
#include "game.h"
#include "interfmanager.h"
#include "log.h"
#include "mempool.h"
#include "midevcondition.h"
#include "midevent.h"
#include "mideventhooks.h"
#include "midgard.h"
#include "midgardobjectmap.h"
#include "midgardstream.h"
#include "midplayer.h"
#include "testcondition.h"
#include "textboxinterf.h"
#include "textids.h"
#include "togglebutton.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

/**
 * Custom event condition that checks if player have
 * less or equal / greater or equal amount of resources.
 */
struct CMidCondOwnResource : public game::CMidEvCondition
{
    game::Bank resource;
    /** If true, player must have more than specified amount to met the condition. */
    bool greaterOrEqual;
};

void __fastcall condOwnResourceDestructor(CMidCondOwnResource* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall condOwnResourceIsIdsEqual(const CMidCondOwnResource*,
                                          int /*%edx*/,
                                          const game::CMidgardID*)
{
    return false;
}

void __fastcall condOwnResourceAddToList(const CMidCondOwnResource*,
                                         int /*%edx*/,
                                         game::Set<game::CMidgardID>*)
{ }

bool __fastcall condOwnResourceIsValid(const CMidCondOwnResource* thisptr,
                                       int /*%edx*/,
                                       const game::IMidgardObjectMap* objectMap)
{
    return game::BankApi::get().isValid(&thisptr->resource);
}

bool __fastcall condOwnResourceMethod4(const CMidCondOwnResource*, int /*%edx*/, int)
{
    return false;
}

void __fastcall condOwnResourceStream(CMidCondOwnResource* thisptr,
                                      int /*%edx*/,
                                      game::IMidgardStream** stream)
{
    auto streamPtr = *stream;
    auto vftable = streamPtr->vftable;

    vftable->streamCurrency(streamPtr, "BANK", &thisptr->resource);
    vftable->streamBool(streamPtr, "GRE", &thisptr->greaterOrEqual);
}

// clang-format off
static game::CMidEvConditionVftable condOwnResourceVftable{
    (game::CMidEvConditionVftable::Destructor)condOwnResourceDestructor,
    (game::CMidEvConditionVftable::IsIdsEqual)condOwnResourceIsIdsEqual,
    (game::CMidEvConditionVftable::AddToList)condOwnResourceAddToList,
    (game::CMidEvConditionVftable::IsValid)condOwnResourceIsValid,
    (game::CMidEvConditionVftable::Method4)condOwnResourceMethod4,
    (game::CMidEvConditionVftable::Stream)condOwnResourceStream,
};
// clang-format on

game::CMidEvCondition* createMidCondOwnResource()
{
    using namespace game;

    auto ownResource = (CMidCondOwnResource*)Memory::get().allocate(sizeof(CMidCondOwnResource));

    ownResource->category.vftable = EventCondCategories::vftable();
    ownResource->category.id = customEventConditions().ownResource.category.id;
    ownResource->category.table = customEventConditions().ownResource.category.table;
    ownResource->greaterOrEqual = true;
    ownResource->resource = Bank{};
    ownResource->vftable = &condOwnResourceVftable;

    return ownResource;
}

void __stdcall midCondOwnResourceGetInfoString(game::String* info,
                                               const game::IMidgardObjectMap* objectMap,
                                               const game::CMidEvCondition* eventCondition)
{
    const auto ownResource = static_cast<const CMidCondOwnResource*>(eventCondition);
    const auto& res = ownResource->resource;

    const auto textInfoId = &customEventConditions().ownResource.infoText;
    std::string str{game::gameFunctions().getInterfaceText(textInfoId)};

    replace(str, "%COND%", (ownResource->greaterOrEqual ? ">=" : "<="));
    replace(str, "%GOLD%", fmt::format("{:d}", res.gold));
    replace(str, "%INFERNAL%", fmt::format("{:d}", res.infernalMana));
    replace(str, "%LIFE%", fmt::format("{:d}", res.lifeMana));
    replace(str, "%DEATH%", fmt::format("{:d}", res.deathMana));
    replace(str, "%RUNIC%", fmt::format("{:d}", res.runicMana));
    replace(str, "%GROVE%", fmt::format("{:d}", res.groveMana));

    game::StringApi::get().initFromStringN(info, str.c_str(), str.length());
}

struct CCondOwnResourceInterf : public game::editor::CCondInterf
{
    void* unknown;
    CMidCondOwnResource* ownResourceCondition;
    game::CMidgardID eventId;
};

void __fastcall condOwnResourceInterfDestructor(CCondOwnResourceInterf* thisptr,
                                                int /*%edx*/,
                                                char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

static game::CInterfaceVftable::OnVisibilityChanged onVisibilityChanged{};

void __fastcall condOwnResourceInterfOnVisibilityChanged(CCondOwnResourceInterf* thisptr,
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

    auto ownResource = thisptr->ownResourceCondition;
    if (!ownResource) {
        return;
    }

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();
    const auto& setString = CEditBoxInterfApi::get().setString;
    const auto& resources = ownResource->resource;

    auto goldEdit = dialogApi.findEditBox(dialog, "EDIT_GOLD");
    if (goldEdit) {
        setString(goldEdit, fmt::format("{:d}", resources.gold).c_str());
    }

    auto infernalEdit = dialogApi.findEditBox(dialog, "EDIT_INFERNAL");
    if (infernalEdit) {
        setString(infernalEdit, fmt::format("{:d}", resources.infernalMana).c_str());
    }

    auto lifeEdit = dialogApi.findEditBox(dialog, "EDIT_LIFE");
    if (lifeEdit) {
        setString(lifeEdit, fmt::format("{:d}", resources.lifeMana).c_str());
    }

    auto deathEdit = dialogApi.findEditBox(dialog, "EDIT_DEATH");
    if (deathEdit) {
        setString(deathEdit, fmt::format("{:d}", resources.deathMana).c_str());
    }

    auto runicEdit = dialogApi.findEditBox(dialog, "EDIT_RUNIC");
    if (runicEdit) {
        setString(runicEdit, fmt::format("{:d}", resources.runicMana).c_str());
    }

    auto groveEdit = dialogApi.findEditBox(dialog, "EDIT_GROVE");
    if (groveEdit) {
        setString(groveEdit, fmt::format("{:d}", resources.groveMana).c_str());
    }

    auto toggle = dialogApi.findToggleButton(dialog, "TOG_GRE");
    if (toggle) {
        CToggleButtonApi::get().setChecked(toggle, ownResource->greaterOrEqual);
    }

    thisptr->ownResourceCondition = nullptr;
}

bool __fastcall condOwnResourceInterfSetEventCondition(CCondOwnResourceInterf* thisptr,
                                                       int /*%edx*/,
                                                       game::CMidEvCondition* eventCondition)
{
    if (eventCondition->category.id == customEventConditions().ownResource.category.id) {
        thisptr->ownResourceCondition = reinterpret_cast<CMidCondOwnResource*>(eventCondition);
        return true;
    }

    return false;
}

void __fastcall condOwnResourcesInterfCancelButtonHandler(CCondOwnResourceInterf* thisptr,
                                                          int /*%edx*/)
{
    using namespace game;
    using namespace editor;

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

void __fastcall condOwnResourcesInterfOkButtonHandler(CCondOwnResourceInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    Bank resources{};

    auto goldEdit = dialogApi.findEditBox(dialog, "EDIT_GOLD");
    if (goldEdit) {
        resources.gold = std::atoi(goldEdit->data->editBoxData.inputString.string);
    }

    auto infernalEdit = dialogApi.findEditBox(dialog, "EDIT_INFERNAL");
    if (infernalEdit) {
        resources.infernalMana = std::atoi(infernalEdit->data->editBoxData.inputString.string);
    }

    auto lifeEdit = dialogApi.findEditBox(dialog, "EDIT_LIFE");
    if (lifeEdit) {
        resources.lifeMana = std::atoi(lifeEdit->data->editBoxData.inputString.string);
    }

    auto deathEdit = dialogApi.findEditBox(dialog, "EDIT_DEATH");
    if (deathEdit) {
        resources.deathMana = std::atoi(deathEdit->data->editBoxData.inputString.string);
    }

    auto runicEdit = dialogApi.findEditBox(dialog, "EDIT_RUNIC");
    if (runicEdit) {
        resources.runicMana = std::atoi(runicEdit->data->editBoxData.inputString.string);
    }

    auto groveEdit = dialogApi.findEditBox(dialog, "EDIT_GROVE");
    if (groveEdit) {
        resources.groveMana = std::atoi(groveEdit->data->editBoxData.inputString.string);
    }

    // This should never happen since we don't allow to enter resource values outside [0 : 9999].
    if (!BankApi::get().isValid(&resources)) {
        // Could not create new condition
        showMessageBox(getTranslatedText("X100TA0631"));
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

    auto midEvent = (CMidEvent*)objectMap->vftable
                        ->findScenarioObjectByIdForChange(objectMap, &thisptr->eventId);

    auto ownResource = static_cast<CMidCondOwnResource*>(createMidCondOwnResource());

    BankApi::get().copy(&ownResource->resource, &resources);

    auto greToggle = dialogApi.findToggleButton(dialog, "TOG_GRE");
    if (greToggle) {
        ownResource->greaterOrEqual = greToggle->data->checked;
    }

    CMidEventApi::get().addCondition(midEvent, nullptr, ownResource);

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->hideInterface(ptr.data, thisptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);

    if (thisptr) {
        thisptr->CInterface::vftable->destructor(thisptr, 1);
    }
}

static game::editor::CCondInterfVftable condOwnResourceInterfVftable{};

game::editor::CCondInterf* createCondOwnResourceInterf(game::ITask* task,
                                                       void* a2,
                                                       const game::CMidgardID* eventId)
{
    using namespace game;
    using namespace editor;

    auto thisptr = (CCondOwnResourceInterf*)Memory::get().allocate(sizeof(CCondOwnResourceInterf));

    static const char dialogName[]{"DLG_COND_OWN_RESOURCE"};

    CCondInterfApi::get().constructor(thisptr, dialogName, task);

    // After CCondInterf c-tor we have its vftable.
    // Use its address to populate our handmade vftable
    // since its easier than defining all these methods by hand in code.
    static bool initialized{false};
    if (!initialized) {
        // Remember parent's OnVisibilityChanged address so we can use it later
        onVisibilityChanged = thisptr->CInterface::vftable->onVisibilityChanged;

        initialized = true;
        std::memcpy(&condOwnResourceInterfVftable, thisptr->CInterface::vftable,
                    sizeof(CInterfaceVftable));

        // Change methods that are specific for our custom class
        condOwnResourceInterfVftable
            .destructor = (CInterfaceVftable::Destructor)&condOwnResourceInterfDestructor;
        condOwnResourceInterfVftable.onVisibilityChanged =
            (CInterfaceVftable::OnVisibilityChanged)&condOwnResourceInterfOnVisibilityChanged;
        condOwnResourceInterfVftable
            .setEventCondition = (CCondInterfVftable::
                                      SetEventCondition)&condOwnResourceInterfSetEventCondition;
    }

    thisptr->CInterface::vftable = &condOwnResourceInterfVftable;
    thisptr->eventId = *eventId;
    thisptr->unknown = a2;
    thisptr->ownResourceCondition = nullptr;

    auto dialog = thisptr->popupData->dialog;
    const auto& dialogApi = CDialogInterfApi::get();

    if (dialogApi.findButton(dialog, "BTN_OK")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condOwnResourcesInterfOkButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    if (dialogApi.findButton(dialog, "BTN_CANCEL")) {
        using ButtonCallback = CCondInterfApi::Api::ButtonCallback;
        ButtonCallback callback{};
        Functor functor{};

        callback.callback = (ButtonCallback::Callback)condOwnResourcesInterfCancelButtonHandler;
        CCondInterfApi::get().createButtonFunctor(&functor, 0, thisptr, &callback);

        const auto& button = CButtonInterfApi::get();
        button.assignFunctor(dialog, "BTN_CANCEL", dialogName, &functor, 0);
        FunctorApi::get().createOrFree(&functor, nullptr);
    }

    const auto& setFilterAndLength = CEditBoxInterfApi::get().setFilterAndLength;
    setFilterAndLength(dialog, "EDIT_GOLD", dialogName, EditFilter::DigitsOnly, 4);
    setFilterAndLength(dialog, "EDIT_INFERNAL", dialogName, EditFilter::DigitsOnly, 4);
    setFilterAndLength(dialog, "EDIT_LIFE", dialogName, EditFilter::DigitsOnly, 4);
    setFilterAndLength(dialog, "EDIT_DEATH", dialogName, EditFilter::DigitsOnly, 4);
    setFilterAndLength(dialog, "EDIT_RUNIC", dialogName, EditFilter::DigitsOnly, 4);
    setFilterAndLength(dialog, "EDIT_GROVE", dialogName, EditFilter::DigitsOnly, 4);
    return thisptr;
}

struct CTestOwnResource : public game::ITestCondition
{
    CMidCondOwnResource* ownResource;
};

void __fastcall testOwnResourceDestructor(CTestOwnResource* thisptr, int /*%edx*/, char flags)
{
    if (flags & 1) {
        game::Memory::get().freeNonZero(thisptr);
    }
}

bool __fastcall testOwnResourceDoTest(const CTestOwnResource* thisptr,
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
    auto player = reinterpret_cast<const CMidPlayer*>(obj);
    const auto& playerBank = player->bank;
    const auto condition = thisptr->ownResource;
    const auto& resource = condition->resource;

    return condition->greaterOrEqual ? playerBank >= resource : playerBank <= resource;
}

game::ITestConditionVftable testOwnResourceVftable{
    (game::ITestConditionVftable::Destructor)testOwnResourceDestructor,
    (game::ITestConditionVftable::Test)testOwnResourceDoTest,
};

game::ITestCondition* createTestOwnResource(game::CMidEvCondition* eventCondition)
{
    auto thisptr = (CTestOwnResource*)game::Memory::get().allocate(sizeof(CTestOwnResource));
    thisptr->ownResource = reinterpret_cast<CMidCondOwnResource*>(eventCondition);
    thisptr->vftable = &testOwnResourceVftable;

    return thisptr;
}

bool checkOwnResourceConditionsValid(game::CDialogInterf*,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* eventId)
{
    using namespace game;

    auto obj = objectMap->vftable->findScenarioObjectById(objectMap, eventId);
    if (!obj) {
        return false;
    }

    const auto& category = customEventConditions().ownResource.category;
    auto event = static_cast<const CMidEvent*>(obj);

    std::vector<const CMidEvCondition*> conditions{};
    getConditionsOfType(event, &category, conditions);

    const auto& ownResourceTextIds = textIds().events.conditions.ownResource;

    // At most 2 own resource conditions allowed
    if (conditions.size() > 2) {
        auto message = getTranslatedText(ownResourceTextIds.tooMany.c_str());
        if (message.empty()) {
            message = "At most two conditions of type \"Own resource\" is allowed per event.";
        }

        showMessageBox(message);
        return false;
    }

    // Single condition is always valid
    if (conditions.size() != 2) {
        return true;
    }

    auto first = static_cast<const CMidCondOwnResource*>(conditions[0]);
    auto second = static_cast<const CMidCondOwnResource*>(conditions[1]);

    // Allow meaningless conditions
    if (first->greaterOrEqual == second->greaterOrEqual) {
        return true;
    }

    bool mutuallyExclusive{false};

    // Valid range should be [first : second]
    if (first->greaterOrEqual) {
        mutuallyExclusive = !(first->resource <= second->resource);
    }

    // Valid range should be [second : first]
    if (second->greaterOrEqual) {
        mutuallyExclusive = !(second->resource <= first->resource);
    }

    if (mutuallyExclusive) {
        auto message = getTranslatedText(ownResourceTextIds.mutuallyExclusive.c_str());
        if (message.empty()) {
            message = "Conditions of type \"Own resource\" are mutually exclusive.";
        }

        showMessageBox(message);
    }

    return !mutuallyExclusive;
}

} // namespace hooks
