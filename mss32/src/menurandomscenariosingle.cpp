/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#include "menurandomscenariosingle.h"
#include "autodialog.h"
#include "button.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "enums.h"
#include "image2outline.h"
#include "listbox.h"
#include "log.h"
#include "mempool.h"
#include "multilayerimg.h"
#include "scenariotemplates.h"
#include "spinbuttoninterf.h"
#include "stringarray.h"
#include "textboxinterf.h"
#include "utils.h"
#include <algorithm>
#include <cstring>
#include <set>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace hooks {

static const char dialogName[] = "DLG_RANDOM_SCENARIO_SINGLE";
static const char templatesListName[] = "TLBOX_TEMPLATES";
static const char sizeSpinName[] = "SPIN_SIZE";
static const char forestSpinName[] = "SPIN_FOREST";
static const char roadsSpinName[] = "SPIN_ROADS";
static const char goldSpinName[] = "SPIN_GOLD";
static const char manaSpinName[] = "SPIN_MANA";
static const char descTextName[] = "TXT_DESC";
static const char btnRaceName[] = "BTN_RACE_";

static const int forestSpinStep{5};
static const int roadsSpinStep{5};
static const int goldSpinStep{100};
static const int manaSpinStep{50};

static game::RttiInfo<game::CInterfaceVftable> menuRttiInfo;
static game::CInterfaceVftable::Destructor menuBaseDtor{nullptr};

static const char* getRaceImage(rsg::RaceType race)
{
    switch (race) {
    case rsg::RaceType::Human:
        return "GOD_EMPIRE";
    case rsg::RaceType::Dwarf:
        return "GOD_CLANS";
    case rsg::RaceType::Undead:
        return "GOD_UNDEAD";
    case rsg::RaceType::Heretic:
        return "GOD_LEGIONS";
    case rsg::RaceType::Elf:
        return "GOD_ELVES";
    default:
    case rsg::RaceType::Neutral:
    case rsg::RaceType::Random:
        return "GOD_RANDOM";
    }
}

static int raceToImageIndex(rsg::RaceType race)
{
    switch (race) {
    case rsg::RaceType::Human:
        return 1;
    case rsg::RaceType::Dwarf:
        return 2;
    case rsg::RaceType::Undead:
        return 3;
    case rsg::RaceType::Heretic:
        return 4;
    case rsg::RaceType::Elf:
        return 5;
    default:
    case rsg::RaceType::Neutral:
    case rsg::RaceType::Random:
        return 0;
    }
}

static rsg::RaceType imageIndexToRace(int index)
{
    switch (index) {
    case 1:
        return rsg::RaceType::Human;
    case 2:
        return rsg::RaceType::Dwarf;
    case 3:
        return rsg::RaceType::Undead;
    case 4:
        return rsg::RaceType::Heretic;
    case 5:
        return rsg::RaceType::Elf;
    default:
    case 0:
        return rsg::RaceType::Random;
    }
}

static void setupSpinButtonOptions(game::CSpinButtonInterf* spinButton,
                                   int first,
                                   int last,
                                   int step,
                                   const char* suffix = nullptr)
{
    if (first > last) {
        return;
    }

    const int count = (last - first) / step;
    if (count <= 0) {
        return;
    }

    using namespace game;

    const auto& string{StringApi::get()};
    const auto& stringArray{StringArrayApi::get()};

    StringArray options{};
    stringArray.reserve(&options, static_cast<const unsigned int>(count));

    for (int i = first; i <= last; i += step) {
        char buffer[50] = {0};
        std::snprintf(buffer, sizeof(buffer) - 1, "%d%s", i, (suffix ? suffix : ""));

        String str;
        string.initFromString(&str, buffer);

        stringArray.pushBack(&options, &str);
        string.free(&str);
    }

    CSpinButtonInterfApi::get().setOptions(spinButton, &options);
    stringArray.destructor(&options);
}

static void setupSizeSpinOptions(game::CSpinButtonInterf* spinButton, int sizeMin, int sizeMax)
{
    if (sizeMin > sizeMax) {
        return;
    }

    constexpr int scenarioSizeStep{24};
    const int count = 1 + (sizeMax - sizeMin) / scenarioSizeStep;

    using namespace game;

    const auto& string{StringApi::get()};
    const auto& stringArray{StringArrayApi::get()};

    StringArray options{};
    stringArray.reserve(&options, static_cast<const unsigned int>(count));

    for (int i = 0; i < count; ++i) {
        const int size = sizeMin + 24 * i;

        char buffer[50] = {0};
        std::snprintf(buffer, sizeof(buffer) - 1, "%d x %d", size, size);

        String str;
        string.initFromString(&str, buffer);

        stringArray.pushBack(&options, &str);
        string.free(&str);
    }

    CSpinButtonInterfApi::get().setOptions(spinButton, &options);
    stringArray.destructor(&options);
}

static game::IMqImage2* createRaceButtonSelectedImage(game::CButtonInterf* button,
                                                      rsg::RaceType race)
{
    using namespace game;

    const auto& allocateMem{Memory::get().allocate};
    const auto& loadImage{AutoDialogApi::get().loadImage};

    const CMqRect* area{button->vftable->getArea((CInterface*)button)};
    const CMqPoint size{area->p2 - area->p1};
    Color color{0x00b2d9f5u};

    auto outline{(CImage2Outline*)allocateMem(sizeof(CImage2Outline))};
    CImage2OutlineApi::get().constructor(outline, &size, &color, 0xff);

    auto multilayerImage{(CMultiLayerImg*)allocateMem(sizeof(CMultiLayerImg))};

    const auto& multilayerApi{CMultiLayerImgApi::get()};
    multilayerApi.constructor(multilayerImage);

    multilayerApi.addImage(multilayerImage, loadImage(getRaceImage(race)), -999, -999);
    multilayerApi.addImage(multilayerImage, outline, -999, -999);

    return multilayerImage;
}

static void raceButtonSetImages(game::CButtonInterf* button, rsg::RaceType race)
{
    using namespace game;

    const auto& loadImage{AutoDialogApi::get().loadImage};

    button->vftable->setImage(button, loadImage(getRaceImage(race)), ButtonState::Normal);

    auto hoveredImage{createRaceButtonSelectedImage(button, race)};
    button->vftable->setImage(button, hoveredImage, ButtonState::Hovered);

    auto clickedImage{createRaceButtonSelectedImage(button, race)};
    button->vftable->setImage(button, clickedImage, ButtonState::Clicked);

    // Disabled race buttons are invisible
    button->vftable->setImage(button, nullptr, ButtonState::Disabled);
}

/** Updates menu UI according to selected index in templates list box. */
static void updateMenuUi(CMenuRandomScenarioSingle* menu, int selectedIndex)
{
    const auto& templates{getScenarioTemplates()};
    if (selectedIndex < 0 || selectedIndex >= (int)templates.size()) {
        return;
    }

    using namespace game;

    const auto& menuBase{CMenuBaseApi::get()};
    const auto& dialogApi{CDialogInterfApi::get()};
    const auto& spinBox{CSpinButtonInterfApi::get()};
    const auto& textBox{CTextBoxInterfApi::get()};
    const auto& button{CButtonInterfApi::get()};

    const auto& settings{templates[selectedIndex].settings};

    CDialogInterf* dialog{menuBase.getDialogInterface(menu)};

    CSpinButtonInterf* forestSpin{dialogApi.findSpinButton(dialog, forestSpinName)};
    if (forestSpin) {
        spinBox.setSelectedOption(forestSpin, settings.forest / forestSpinStep);
    }

    CSpinButtonInterf* roadsSpin{dialogApi.findSpinButton(dialog, roadsSpinName)};
    if (roadsSpin) {
        spinBox.setSelectedOption(roadsSpin, settings.roads / roadsSpinStep);
    }

    CSpinButtonInterf* goldSpin{dialogApi.findSpinButton(dialog, goldSpinName)};
    if (goldSpin) {
        spinBox.setSelectedOption(goldSpin, settings.startingGold / goldSpinStep);
    }

    CTextBoxInterf* descText{dialogApi.findTextBox(dialog, descTextName)};
    if (descText) {
        textBox.setString(descText, settings.description.c_str());
    }

    CSpinButtonInterf* sizeSpin{dialogApi.findSpinButton(dialog, sizeSpinName)};
    if (sizeSpin) {
        setupSizeSpinOptions(sizeSpin, settings.sizeMin, settings.sizeMax);
        // Select smallest scenario size by default
        CSpinButtonInterfApi::get().setSelectedOption(sizeSpin, 0);
    }

    // Setup initial race buttons state: random race image and index
    const rsg::RaceType race{rsg::RaceType::Random};

    for (int i = 0; i < 4; ++i) {
        auto& pair{menu->raceIndices[i]};
        CButtonInterf* raceButton{pair.first};

        raceButtonSetImages(raceButton, race);
        // Disable buttons of unused race slots
        raceButton->vftable->setEnabled(raceButton, i < settings.maxPlayers);

        int& index{pair.second};
        index = raceToImageIndex(race);
    }
}

static void __fastcall menuRandomScenarioSingleDtor(CMenuRandomScenarioSingle* menu,
                                                    int /*%edx*/,
                                                    char flags)
{
    logDebug("mss32Proxy.log", "CMenuRandomScenarioSingle d-tor called");
    menu->~CMenuRandomScenarioSingle();

    if (menuBaseDtor) {
        logDebug("mss32Proxy.log", "CMenuRandomScenarioSingle d-tor calls CMenuBase d-tor");
        // This will properly destroy base class and free memory
        menuBaseDtor(menu, flags);
    }
}

static void __fastcall listBoxDisplayHandler(CMenuRandomScenarioSingle* thisptr,
                                             int /*%edx*/,
                                             game::String* string,
                                             bool,
                                             int selectedIndex)
{
    const auto& templates{getScenarioTemplates()};
    if (selectedIndex < 0 || selectedIndex >= (int)templates.size()) {
        return;
    }

    const char* templateName{templates[selectedIndex].settings.name.c_str()};
    game::StringApi::get().initFromString(string, templateName);
}

static void __fastcall listBoxUpdateHandler(CMenuRandomScenarioSingle* thisptr,
                                            int /*%edx*/,
                                            int selectedIndex)
{
    updateMenuUi(thisptr, selectedIndex);
}

/**
 * Changes index associated with race button depending on other race button indices.
 * Don't allow to select duplicates of playable races,
 * only random race can be selected multiple times.
 */
static void raceButtonHandler(CMenuRandomScenarioSingle* thisptr, game::CButtonInterf* button)
{
    if (!button) {
        return;
    }

    auto& raceIndices{thisptr->raceIndices};

    auto it{std::find_if(raceIndices.begin(), raceIndices.end(),
                         [button](const auto& pair) { return pair.first == button; })};
    if (it == raceIndices.end()) {
        return;
    }

    std::set<int> possibleIndices{raceToImageIndex(rsg::RaceType::Random),
                                  raceToImageIndex(rsg::RaceType::Human),
                                  raceToImageIndex(rsg::RaceType::Dwarf),
                                  raceToImageIndex(rsg::RaceType::Undead),
                                  raceToImageIndex(rsg::RaceType::Heretic),
                                  raceToImageIndex(rsg::RaceType::Elf)};

    for (const auto& [raceButton, index] : raceIndices) {
        possibleIndices.erase(index);
    }

    const int currentIndex{it->second};

    int nextIndex{raceToImageIndex(rsg::RaceType::Random)};
    // Find possible index that is greater than current one
    auto greaterIt{possibleIndices.upper_bound(currentIndex)};
    if (greaterIt != possibleIndices.end()) {
        // Found possible index that belongs to playable race, use it
        nextIndex = *greaterIt;
    }

    // Update button images and index
    const rsg::RaceType nextRace{imageIndexToRace(nextIndex)};

    raceButtonSetImages(button, nextRace);
    it->second = nextIndex;
}

static void __fastcall button1Handler(CMenuRandomScenarioSingle* thisptr, int /*%edx*/)
{
    raceButtonHandler(thisptr, thisptr->raceIndices[0].first);
}

static void __fastcall button2Handler(CMenuRandomScenarioSingle* thisptr, int /*%edx*/)
{
    raceButtonHandler(thisptr, thisptr->raceIndices[1].first);
}

static void __fastcall button3Handler(CMenuRandomScenarioSingle* thisptr, int /*%edx*/)
{
    raceButtonHandler(thisptr, thisptr->raceIndices[2].first);
}

static void __fastcall button4Handler(CMenuRandomScenarioSingle* thisptr, int /*%edx*/)
{
    raceButtonHandler(thisptr, thisptr->raceIndices[3].first);
}

static void setupMenuUi(CMenuRandomScenarioSingle* menu)
{
    using namespace game;

    const auto& menuBase{CMenuBaseApi::get()};
    const auto& dialogApi{CDialogInterfApi::get()};
    const auto freeFunctor{SmartPointerApi::get().createOrFreeNoDtor};

    CDialogInterf* dialog{menuBase.getDialogInterface(menu)};
    SmartPointer functor;

    menuBase.createButtonFunctor(&functor, 0, menu, &menuBase.buttonBackCallback);

    const auto& button{CButtonInterfApi::get()};
    button.assignFunctor(dialog, "BTN_BACK", dialogName, &functor, 0);

    freeFunctor(&functor, nullptr);

    // Create spin button options, template selection will only change currently selected option
    CSpinButtonInterf* forestSpin{dialogApi.findSpinButton(dialog, forestSpinName)};
    if (forestSpin) {
        setupSpinButtonOptions(forestSpin, 0, 100, forestSpinStep, "%");
    }

    CSpinButtonInterf* roadsSpin{dialogApi.findSpinButton(dialog, roadsSpinName)};
    if (roadsSpin) {
        setupSpinButtonOptions(roadsSpin, 0, 100, roadsSpinStep, "%");
    }

    CSpinButtonInterf* goldSpin{dialogApi.findSpinButton(dialog, goldSpinName)};
    if (goldSpin) {
        setupSpinButtonOptions(goldSpin, 0, 9999, goldSpinStep);
    }

    CSpinButtonInterf* manaSpin{dialogApi.findSpinButton(dialog, manaSpinName)};
    if (manaSpin) {
        setupSpinButtonOptions(manaSpin, 0, 9999, manaSpinStep);
    }

    // Cache race buttons, assign initial indices and setup callbacks
    using ButtonCallback = CMenuBaseApi::Api::ButtonCallback;
    // clang-format off
    std::array<ButtonCallback, 4> buttonCallbacks{{
        (ButtonCallback)button1Handler, (ButtonCallback)button2Handler,
        (ButtonCallback)button3Handler, (ButtonCallback)button4Handler
    }};
    // clang-format on

    for (int i = 0; i < 4; ++i) {
        char name[50] = {0};
        std::snprintf(name, sizeof(name) - 1, "%s%d", btnRaceName, i + 1);

        CButtonInterf* raceButton{dialogApi.findButton(dialog, name)};
        if (!raceButton) {
            continue;
        }

        // Cache button and make sure we start with random race index
        menu->raceIndices[i] = {raceButton, raceToImageIndex(rsg::RaceType::Random)};

        menuBase.createButtonFunctor(&functor, 0, menu, &buttonCallbacks[i]);
        button.assignFunctor(dialog, name, dialogName, &functor, 0);
        freeFunctor(&functor, nullptr);
    }

    const auto& listBox{CListBoxInterfApi::get()};

    CListBoxInterf* templatesList{dialogApi.findListBox(dialog, templatesListName)};
    if (templatesList) {
        using ListBoxDisplayCallback = CMenuBaseApi::Api::ListBoxDisplayTextCallback;
        ListBoxDisplayCallback displayCallback{};

        // Show template names as list box contents
        displayCallback.callback = (ListBoxDisplayCallback::Callback)listBoxDisplayHandler;
        menuBase.createListBoxDisplayTextFunctor(&functor, 0, menu, &displayCallback);

        listBox.assignDisplayTextFunctor(dialog, templatesListName, dialogName, &functor, false);
        freeFunctor(&functor, nullptr);

        using Callback = CMenuBaseApi::Api::ListBoxCallback;

        // Update UI each time template changes
        Callback callback = (Callback)listBoxUpdateHandler;
        menuBase.createListBoxFunctor(&functor, 0, menu, &callback);

        listBox.assignFunctor(dialog, templatesListName, dialogName, &functor);
        freeFunctor(&functor, nullptr);

        listBox.setElementsTotal(templatesList, (int)getScenarioTemplates().size());

        updateMenuUi(menu, listBox.selectedIndex(templatesList));
    }
}

static void menuRandomScenarioSingleCtor(CMenuRandomScenarioSingle* menu,
                                         game::CMenuPhase* menuPhase)
{
    using namespace game;

    logDebug("mss32Proxy.log", "CMenuRandomScenarioSingle c-tor called");

    const auto& menuBase{CMenuBaseApi::get()};
    menuBase.constructor(menu, menuPhase);

    CInterfaceVftable* vftable = &menuRttiInfo.vftable;

    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;

        replaceRttiInfo(menuRttiInfo, menu->vftable);
        // Remember base class destructor
        menuBaseDtor = menu->vftable->destructor;
        // Make sure our vftable uses our own destructor
        vftable->destructor = (CInterfaceVftable::Destructor)&menuRandomScenarioSingleDtor;
    }

    // Use our vftable
    menu->vftable = vftable;

    menuBase.createMenu(menu, dialogName);

    setupMenuUi(menu);
    // TODO: show additional debug buttons when debug mode is enabled
}

CMenuRandomScenarioSingle::CMenuRandomScenarioSingle(game::CMenuPhase* menuPhase)
{
    menuRandomScenarioSingleCtor(this, menuPhase);
}

game::CMenuBase* __stdcall createMenuRandomScenarioSingle(game::CMenuPhase* menuPhase)
{
    auto menu = (CMenuRandomScenarioSingle*)game::Memory::get().allocate(
        sizeof(CMenuRandomScenarioSingle));

    new (menu) CMenuRandomScenarioSingle(menuPhase);

    return menu;
}

} // namespace hooks
