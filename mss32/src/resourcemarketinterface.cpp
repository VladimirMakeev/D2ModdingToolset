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

#include "resourcemarketinterface.h"
#include "button.h"
#include "dialoginterf.h"
#include "draganddropinterf.h"
#include "editboxinterf.h"
#include "editor.h"
#include "intvector.h"
#include "mapinterf.h"
#include "mempool.h"
#include "midgardscenariomap.h"
#include "midsiteresourcemarket.h"
#include "mqpresentationmanager.h"
#include "pictureinterf.h"
#include "rendererimpl.h"
#include "scenedit.h"
#include "sitecategoryhooks.h"
#include "spinbuttoninterf.h"
#include "stringarray.h"
#include "taskobjprop.h"
#include "togglebutton.h"
#include "trainingcampinterf.h"
#include "utils.h"
#include "visitors.h"
#include <charconv>
#include <cstring>
#include <fmt/format.h>

namespace hooks {

static const char dialogName[] = "DLG_RESOURCE_MARKET";

struct CResourceMarketInterf : public game::CDragAndDropInterf
{
    std::string customRates;
    game::IntVector imageIndices;
    game::editor::CTaskObjProp* taskObjProp;
    CMidSiteResourceMarket* market;
    int selectedIndex;
};

static game::CInterfaceVftable resMarketInterfVftable{};

static game::CInterfaceVftable::Destructor dragDropDtor = nullptr;

static void __fastcall resMarketInterfDtor(CResourceMarketInterf* thisptr, int /*%edx*/, char flags)
{
    using namespace game;

    thisptr->customRates.~basic_string();
    IntVectorApi::get().destructor(&thisptr->imageIndices);

    if (dragDropDtor) {
        dragDropDtor(thisptr, flags);
    }
}

static void setEditBoxManaValue(game::CDialogInterf* dialog,
                                const char* editBoxName,
                                std::int16_t mana)
{
    using namespace game;

    char buffer[5] = {};
    fmt::format_to_n(buffer, sizeof(buffer) - 1u, "{:d}", mana);

    CEditBoxInterf* editBox{CDialogInterfApi::get().findEditBox(dialog, editBoxName)};
    CEditBoxInterfApi::get().setString(editBox, buffer);
}

static std::int16_t getEditBoxManaValue(game::CDialogInterf* dialog, const char* editBoxName)
{
    using namespace game;

    CEditBoxInterf* editBox{CDialogInterfApi::get().findEditBox(dialog, editBoxName)};
    const String& string{editBox->data->editBoxData.inputString};

    std::int16_t value{};
    auto [ptr, error] = std::from_chars(string.string, string.string + string.length, value);
    if (error != std::errc()) {
        return 0;
    }

    return value;
}

static void setInfiniteManaToggle(game::CDialogInterf* dialog,
                                  const char* buttonName,
                                  bool infinite)
{
    using namespace game;

    CToggleButton* button{CDialogInterfApi::get().findToggleButton(dialog, buttonName)};
    CToggleButtonApi::get().setChecked(button, infinite);
}

static bool getInfiniteManaToggle(game::CDialogInterf* dialog, const char* buttonName)
{
    using namespace game;

    const CToggleButton* button{CDialogInterfApi::get().findToggleButton(dialog, buttonName)};
    return button->data->checked;
}

static void __fastcall resMarketInterfOkButtonHandler(CResourceMarketInterf* thisptr, int /*%edx*/)
{
    using namespace game;
    using namespace editor;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    CEditBoxInterf* editName{dialogApi.findEditBox(dialog, "EDIT_NAME")};
    CEditBoxInterf* editDesc{dialogApi.findEditBox(dialog, "EDIT_DESCRIPTION")};

    const auto& getCString{StringApi::get().cStr};

    const char* name{getCString(&editName->data->editBoxData.inputString)};
    const char* description{getCString(&editDesc->data->editBoxData.inputString)};

    CScenEdit* scenEdit{CScenEditApi::get().instance()};
    IMidgardObjectMap* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    VisitorApi::get().changeSiteInfo(&thisptr->market->id, name, description, objectMap, 1);

    // Returned object is the same as thisptr->market, the call is needed to mark it for change
    objectMap->vftable->findScenarioObjectByIdForChange(objectMap, &thisptr->market->id);

    Bank* stock{&thisptr->market->stock};
    const auto& setCurrency{BankApi::get().set};
    setCurrency(stock, CurrencyType::LifeMana, getEditBoxManaValue(dialog, "EDIT_LIFE"));
    setCurrency(stock, CurrencyType::InfernalMana, getEditBoxManaValue(dialog, "EDIT_INFERNAL"));
    setCurrency(stock, CurrencyType::RunicMana, getEditBoxManaValue(dialog, "EDIT_RUNIC"));
    setCurrency(stock, CurrencyType::DeathMana, getEditBoxManaValue(dialog, "EDIT_DEATH"));
    setCurrency(stock, CurrencyType::GroveMana, getEditBoxManaValue(dialog, "EDIT_GROVE"));
    setCurrency(stock, CurrencyType::Gold, getEditBoxManaValue(dialog, "EDIT_GOLD"));

    InfiniteStock& infiniteStock{thisptr->market->infiniteStock};
    infiniteStock.parts.lifeMana = getInfiniteManaToggle(dialog, "TOG_INF_LIFE");
    infiniteStock.parts.infernalMana = getInfiniteManaToggle(dialog, "TOG_INF_INFERNAL");
    infiniteStock.parts.runicMana = getInfiniteManaToggle(dialog, "TOG_INF_RUNIC");
    infiniteStock.parts.deathMana = getInfiniteManaToggle(dialog, "TOG_INF_DEATH");
    infiniteStock.parts.groveMana = getInfiniteManaToggle(dialog, "TOG_INF_GROVE");
    infiniteStock.parts.gold = getInfiniteManaToggle(dialog, "TOG_INF_GOLD");

    const CToggleButton* customRates{dialogApi.findToggleButton(dialog, "TOG_CUSTOM_RATES")};
    thisptr->market->customExchangeRates = customRates->data->checked;

    if (customRates->data->checked) {
        thisptr->market->exchangeRatesScript = thisptr->customRates;
    }

    const CSpinButtonInterf* spin{dialogApi.findSpinButton(dialog, "SPIN_AI_PRIORITY")};
    const int aiPriority{spin->data->selectedOption};

    VisitorApi::get().changeSiteAiPriority(&thisptr->market->id, aiPriority, objectMap, 1);

    CTaskObjPropVftable* vftable{(CTaskObjPropVftable*)thisptr->taskObjProp->vftable};
    vftable->closePropertiesInterface(thisptr->taskObjProp);
}

static void updateMarketImage(CResourceMarketInterf* thisptr, int imageIndex)
{
    using namespace game;

    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};
    CPictureInterf* picture{CDialogInterfApi::get().findPicture(dialog, "IMG_SITE")};

    const LSiteCategory* category{&customSiteCategories().resourceMarket};
    IMqImage2* image{editorFunctions.getSiteImage(category, imageIndex, false)};

    CPictureInterfApi::get().setImageWithAnchor(picture, image, 9);

    CScenEdit* scenEdit{CScenEditApi::get().instance()};
    IMidgardObjectMap* objectMap{scenEdit->data->unknown2->data->scenarioMap};

    if (!VisitorApi::get().changeSiteImage(&thisptr->market->id, imageIndex, objectMap, 1)) {
        return;
    }

    editorFunctions.showOrHideSiteOnStrategicMap(thisptr->market, objectMap, nullptr, 1);
}

static void __fastcall resMarketInterfImgUpButtonHandler(CResourceMarketInterf* thisptr,
                                                         int /*%edx*/)
{
    using namespace game;

    const auto& indices{thisptr->imageIndices};

    if (thisptr->selectedIndex == 0) {
        thisptr->selectedIndex = indices.end - indices.bgn;
    }

    --thisptr->selectedIndex;
    const int imageIndex{indices.bgn[thisptr->selectedIndex]};

    updateMarketImage(thisptr, imageIndex);
}

static void __fastcall resMarketInterfImgDownButtonHandler(CResourceMarketInterf* thisptr,
                                                           int /*%edx*/)
{
    using namespace game;

    ++thisptr->selectedIndex;

    const auto& indices{thisptr->imageIndices};
    if (thisptr->selectedIndex == indices.end - indices.bgn) {
        thisptr->selectedIndex = 0;
    }

    const int imageIndex{indices.bgn[thisptr->selectedIndex]};
    updateMarketImage(thisptr, imageIndex);
}

static void __fastcall resMarketInterfLoadButtonHandler(CResourceMarketInterf* thisptr,
                                                        int /*%edx*/)
{
    using namespace game;

    const auto& managerApi{CMqPresentationManagerApi::get()};

    PresentationMgrPtr manager;
    managerApi.getPresentationManager(&manager);

    auto* renderer{manager.data->data->renderer};
    IMqDisplay2* display = (IMqDisplay2*)renderer;

    display->vftable->flipToGDISurface(display);

    managerApi.presentationMgrPtrSetData(&manager, nullptr);

    std::string script;
    if (readUserSelectedFile(script, "Lua scripts (*.lua)\0*.lua\0\0",
                             scriptsFolder().string().c_str())) {
        CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};
        CEditBoxInterf* editRates{CDialogInterfApi::get().findEditBox(dialog, "EDIT_EXC_RATES")};

        CEditBoxInterfApi::get().setString(editRates, script.c_str());
        std::swap(thisptr->customRates, script);
    }
}

static void setupAiPriority(game::CSpinButtonInterf* spin, int priority)
{
    using namespace game;

    const auto& stringArrayApi{StringArrayApi::get()};
    const auto& stringApi{StringApi::get()};
    const auto& spinApi{CSpinButtonInterfApi::get()};

    StringArray options{};
    stringArrayApi.reserve(&options, 7);

    char buffer[2];
    buffer[1] = 0;

    for (int i = 0; i < 7; ++i) {
        buffer[0] = static_cast<char>(i + '0');

        String option{};
        stringApi.initFromString(&option, buffer);

        stringArrayApi.pushBack(&options, &option);
        stringApi.free(&option);
    }

    spinApi.setOptions(spin, &options);
    spinApi.setSelectedOption(spin, priority);

    stringArrayApi.destructor(&options);
}

static void __fastcall resMarketInterfOnCustomRatesToggleHandler(CResourceMarketInterf* thisptr,
                                                                 int /*%edx*/,
                                                                 bool pressed,
                                                                 game::CToggleButton* button)
{
    using namespace game;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    CButtonInterf* loadButton{dialogApi.findButton(dialog, "BTN_LOAD")};
    loadButton->vftable->setEnabled(loadButton, pressed);
}

static void setupUi(const CResourceMarketInterf* thisptr)
{
    using namespace game;
    using namespace editor;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    const auto& editBox{CEditBoxInterfApi::get()};

    editBox.setFilterAndLength(dialog, "EDIT_NAME", dialogName, EditFilter::NoFilter, 40);
    editBox.setFilterAndLength(dialog, "EDIT_DESCRIPTION", dialogName, EditFilter::NoFilter, 100);

    CEditBoxInterf* nameEdit{dialogApi.findEditBox(dialog, "EDIT_NAME")};
    CEditBoxInterf* descriptionEdit{dialogApi.findEditBox(dialog, "EDIT_DESCRIPTION")};

    const CMidSiteResourceMarket* market{thisptr->market};

    const char* title{market->title.string ? market->title.string : ""};
    editBox.setString(nameEdit, title);

    const char* description{market->description.string ? market->description.string : ""};
    editBox.setString(descriptionEdit, description);

    // Make sure resource edit boxes allow entering only valid amounts
    editBox.setFilterAndLength(dialog, "EDIT_LIFE", dialogName, EditFilter::DigitsOnly, 4);
    editBox.setFilterAndLength(dialog, "EDIT_INFERNAL", dialogName, EditFilter::DigitsOnly, 4);
    editBox.setFilterAndLength(dialog, "EDIT_RUNIC", dialogName, EditFilter::DigitsOnly, 4);
    editBox.setFilterAndLength(dialog, "EDIT_DEATH", dialogName, EditFilter::DigitsOnly, 4);
    editBox.setFilterAndLength(dialog, "EDIT_GROVE", dialogName, EditFilter::DigitsOnly, 4);
    editBox.setFilterAndLength(dialog, "EDIT_GOLD", dialogName, EditFilter::DigitsOnly, 4);

    // Resource amount settings
    const Bank& stock{market->stock};
    setEditBoxManaValue(dialog, "EDIT_LIFE", stock.lifeMana);
    setEditBoxManaValue(dialog, "EDIT_INFERNAL", stock.infernalMana);
    setEditBoxManaValue(dialog, "EDIT_RUNIC", stock.runicMana);
    setEditBoxManaValue(dialog, "EDIT_DEATH", stock.deathMana);
    setEditBoxManaValue(dialog, "EDIT_GROVE", stock.groveMana);
    setEditBoxManaValue(dialog, "EDIT_GOLD", stock.gold);

    // Infinite resource amounts
    const InfiniteStock& infiniteStock{market->infiniteStock};
    setInfiniteManaToggle(dialog, "TOG_INF_LIFE", infiniteStock.parts.lifeMana);
    setInfiniteManaToggle(dialog, "TOG_INF_INFERNAL", infiniteStock.parts.infernalMana);
    setInfiniteManaToggle(dialog, "TOG_INF_RUNIC", infiniteStock.parts.runicMana);
    setInfiniteManaToggle(dialog, "TOG_INF_DEATH", infiniteStock.parts.deathMana);
    setInfiniteManaToggle(dialog, "TOG_INF_GROVE", infiniteStock.parts.groveMana);
    setInfiniteManaToggle(dialog, "TOG_INF_GOLD", infiniteStock.parts.gold);

    CToggleButton* customRates{dialogApi.findToggleButton(dialog, "TOG_CUSTOM_RATES")};
    CToggleButtonApi::get().setChecked(customRates, market->customExchangeRates);

    const auto freeFunctor{SmartPointerApi::get().createOrFreeNoDtor};
    SmartPointer functor;

    {
        using ButtonCallback = editor::CMapInterfApi::Api::ToggleButtonCallback;
        ButtonCallback callback{};
        callback.callback = (ButtonCallback::Callback)resMarketInterfOnCustomRatesToggleHandler;

        editor::CMapInterfApi::get().createToggleButtonFunctor(&functor, 0,
                                                               (editor::CMapInterf*)thisptr,
                                                               &callback);
        CToggleButtonApi::get().assignFunctor(dialog, "TOG_CUSTOM_RATES", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    CEditBoxInterf* editRates{dialogApi.findEditBox(dialog, "EDIT_EXC_RATES")};
    CEditBoxInterfApi::get().setFilterAndLength(dialog, "EDIT_EXC_RATES", dialogName,
                                                EditFilter::NoFilter, 2048);

    if (market->customExchangeRates) {
        CEditBoxInterfApi::get().setString(editRates, market->exchangeRatesScript.c_str());
    }

    CSpinButtonInterf* spin{dialogApi.findSpinButton(dialog, "SPIN_AI_PRIORITY")};
    setupAiPriority(spin, market->aiPriority.priority);

    const auto& createButtonFunctor{CTrainingCampInterfApi::get().createButtonFunctor};
    const auto assignFunctor{CButtonInterfApi::get().assignFunctor};

    using ButtonCallback = CTrainingCampInterfApi::Api::ButtonCallback;
    ButtonCallback callback{};

    {
        callback.callback = (ButtonCallback::Callback)resMarketInterfOkButtonHandler;
        createButtonFunctor(&functor, 0, (CTrainingCampInterf*)thisptr, &callback);
        assignFunctor(dialog, "BTN_OK", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    {
        callback.callback = (ButtonCallback::Callback)resMarketInterfImgUpButtonHandler;
        createButtonFunctor(&functor, 0, (CTrainingCampInterf*)thisptr, &callback);
        assignFunctor(dialog, "BTN_UP", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    {
        callback.callback = (ButtonCallback::Callback)resMarketInterfImgDownButtonHandler;
        createButtonFunctor(&functor, 0, (CTrainingCampInterf*)thisptr, &callback);
        assignFunctor(dialog, "BTN_DOWN", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    {
        callback.callback = (ButtonCallback::Callback)resMarketInterfLoadButtonHandler;
        createButtonFunctor(&functor, 0, (CTrainingCampInterf*)thisptr, &callback);
        assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    CButtonInterf* loadButton{dialogApi.findButton(dialog, "BTN_LOAD")};
    loadButton->vftable->setEnabled(loadButton, market->customExchangeRates);
}

game::CInterface* createResourceMarketInterface(game::editor::CTaskObjProp* task,
                                                CMidSiteResourceMarket* market)
{
    using namespace game;
    using namespace editor;

    auto* interf = (CResourceMarketInterf*)Memory::get().allocate(sizeof(CResourceMarketInterf));
    CDragAndDropInterfApi::get().constructor(interf, dialogName, task, 0);

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;
        // Remember base class d-tor for proper cleanup
        dragDropDtor = interf->vftable->destructor;

        std::memcpy(&resMarketInterfVftable, interf->vftable, sizeof(CInterfaceVftable));
    }

    // Use our own destructor and vftable
    resMarketInterfVftable.destructor = (CInterfaceVftable::Destructor)resMarketInterfDtor;
    interf->vftable = &resMarketInterfVftable;

    new (&interf->customRates) std::string();

    if (market->customExchangeRates) {
        interf->customRates = market->exchangeRatesScript;
    }

    auto& indices = interf->imageIndices;

    indices.bgn = nullptr;
    indices.end = nullptr;
    indices.allocatedMemEnd = nullptr;
    indices.allocator = nullptr;
    IntVectorApi::get().reserve(&indices, 1u);

    interf->taskObjProp = task;
    interf->market = market;
    interf->selectedIndex = 0;

    editorFunctions.getSiteImageIndices(&indices, &customSiteCategories().resourceMarket, 0);

    const int imgIso = market->imgIso;
    int index = 0;
    for (int* i = indices.bgn; i != indices.end; ++i, ++index) {
        if (imgIso == *i) {
            interf->selectedIndex = index;
            break;
        }
    }

    const int imageIndex{indices.bgn[interf->selectedIndex]};
    IMqImage2* siteImage{
        editorFunctions.getSiteImage(&customSiteCategories().resourceMarket, imageIndex, false)};

    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};
    CPictureInterf* picture{CDialogInterfApi::get().findPicture(dialog, "IMG_SITE")};
    CPictureInterfApi::get().setImageWithAnchor(picture, siteImage, 9);

    setupUi(interf);

    return interf;
}

} // namespace hooks
