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

#include "siteresourcemarketinterf.h"
#include "autodialog.h"
#include "button.h"
#include "ddstackgroup.h"
#include "ddstackinventorydisplay.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "encparamidplayer.h"
#include "exchangeresourcesmsg.h"
#include "game.h"
#include "gameutils.h"
#include "image2outline.h"
#include "image2text.h"
#include "listbox.h"
#include "log.h"
#include "mainview2.h"
#include "mempool.h"
#include "menubase.h"
#include "middatacache.h"
#include "middragdropinterf.h"
#include "midgard.h"
#include "midplayer.h"
#include "midsiteresourcemarket.h"
#include "midstack.h"
#include "midunit.h"
#include "midunitgroupadapter.h"
#include "multilayerimg.h"
#include "paperdollchildinterf.h"
#include "phasegame.h"
#include "pictureinterf.h"
#include "radiobuttoninterf.h"
#include "resetstackext.h"
#include "scripts.h"
#include "sitecategoryhooks.h"
#include "sounds.h"
#include "stackview.h"
#include "task.h"
#include "taskmanager.h"
#include "textboxinterf.h"
#include "textids.h"
#include "togglebutton.h"
#include "unitutils.h"
#include "usunitimpl.h"
#include "utils.h"
#include <algorithm>
#include <cstring>
#include <fmt/format.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace hooks {

static const char dialogName[]{"DLG_RESOURCE_MARKET"};

struct CSiteResourceMarketInterf
    : public game::CMidDataCache2::INotify
    , public game::IResetStackExt
    , public game::CMidDragDropInterf
{
    MarketExchangeRates exchangeRates;
    game::CMqRect paperdollArea;
    game::CMqRect groupArea;
    game::CMidgardID visitorStackId;
    game::CMidgardID marketId;
    int exchangeStep;
    game::CInterface* paperdoll;
    game::CDDStackGroup* stackGroup;
    game::IMqImage2* groupBackground;
    game::CDDStackInventoryDisplay* inventory;
};

static CSiteResourceMarketInterf* resetStackExtToSiteInterf(game::IResetStackExt* resetStack)
{
    using namespace game;

    constexpr auto offset{offsetof(CSiteResourceMarketInterf, IResetStackExt::vftable)};

    auto ptr{reinterpret_cast<std::uintptr_t>(resetStack) - offset};
    return reinterpret_cast<CSiteResourceMarketInterf*>(ptr);
}

static CSiteResourceMarketInterf* dragDropToSiteInterf(game::CMidDragDropInterf* dragDrop)
{
    using namespace game;

    constexpr auto offset{offsetof(CSiteResourceMarketInterf, CMidDragDropInterf::vftable)};

    auto ptr{reinterpret_cast<std::uintptr_t>(dragDrop) - offset};
    return reinterpret_cast<CSiteResourceMarketInterf*>(ptr);
}

// Do the same way as the game: d-tor is implemented in IResetStackExt vftable
// CMidDragDropInterf d-tor adjusts this pointer and calls it
static void __fastcall siteResMarketInterfResetStackExtDtor(game::IResetStackExt* thisptr,
                                                            int /*%edx*/,
                                                            char flags)
{
    using namespace game;

    CSiteResourceMarketInterf* interf{resetStackExtToSiteInterf(thisptr)};

    if (interf->paperdoll) {
        CInterface* marketInterface = interf;
        int index = -1;
        marketInterface->vftable->getChildIndex(marketInterface, &index, interf->paperdoll);
        marketInterface->vftable->deleteChildAt(marketInterface, &index);
    }

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&interf->phaseGame->phase)};
    CMidDataCache2Api::get().removeNotify(dataCache, interf);

    if (interf->inventory) {
        interf->inventory->IMidDropSource::vftable->destructor(interf->inventory, 1);
    }

    if (interf->groupBackground) {
        interf->groupBackground->vftable->destructor(interf->groupBackground, 1);
    }

    if (interf->stackGroup) {
        interf->stackGroup->IMidDropSource::vftable->destructor(interf->stackGroup, 1);
    }

    interf->exchangeRates.~vector();

    CMidDragDropInterfApi::get().destructor(interf);

    if (flags & 1) {
        Memory::get().freeNonZero(interf);
    }
}

static bool __fastcall siteResMarketResetStackExtHireLeader(game::IResetStackExt*,
                                                            int /*%edx*/,
                                                            const game::CMidgardID*,
                                                            int,
                                                            int,
                                                            int)
{
    return true;
}

static bool __fastcall siteResMarketResetStackExtMethod2(game::IResetStackExt*, int /*%edx*/)
{
    return true;
}

static bool __fastcall siteResMarketResetStackExtMethod3(game::IResetStackExt*, int /*%edx*/)
{
    return false;
}

static void __fastcall siteResMarketResetStackExtGetUnitIdsForHire(game::IResetStackExt*,
                                                                   int /*%edx*/,
                                                                   game::IdVector*,
                                                                   int)
{
    return;
}

static game::CMidgardID* __fastcall siteResMarketResetStackExtGetStackId(
    game::IResetStackExt* thisptr,
    int /*%edx*/,
    game::CMidgardID* value)
{
    auto interf{resetStackExtToSiteInterf(thisptr)};

    *value = interf->visitorStackId;
    return value;
}

static game::IResetStackExtVftable siteResMarketStackExtVftable{
    (game::IResetStackExtVftable::Destructor)siteResMarketInterfResetStackExtDtor,
    (game::IResetStackExtVftable::HireLeader)siteResMarketResetStackExtHireLeader,
    (void*)siteResMarketResetStackExtMethod2,
    (void*)siteResMarketResetStackExtMethod3,
    (game::IResetStackExtVftable::GetUnitIdsForHire)siteResMarketResetStackExtGetUnitIdsForHire,
    (game::IResetStackExtVftable::GetStackId)siteResMarketResetStackExtGetStackId};

static void __fastcall siteResMarketInterfDragDropDtor(game::CMidDragDropInterf* thisptr,
                                                       int /*%edx*/,
                                                       char flags)
{
    using namespace game;

    constexpr auto dragDropOffset{offsetof(CSiteResourceMarketInterf, CMidDragDropInterf::vftable)};

    auto ptr{reinterpret_cast<std::uintptr_t>(thisptr) - dragDropOffset};
    auto interf{reinterpret_cast<CSiteResourceMarketInterf*>(ptr)};

    siteResMarketInterfResetStackExtDtor(interf, 0, flags);
}

static void __fastcall siteResMarketInterfBtnCloseHandler(CSiteResourceMarketInterf* thisptr,
                                                          int /*%edx*/)
{
    using namespace game;

    auto currentTask{thisptr->dragAndDropInterfData->task};
    auto taskManager{currentTask->vftable->getTaskManager(currentTask)};

    CTaskManagerApi::get().setCurrentTask(taskManager, nullptr);
}

static const char* getCurrencyImage(game::CurrencyType currency)
{
    using namespace game;

    switch (currency) {
    default:
    case CurrencyType::Gold:
        return "DLG_BEGIN_TURN_GOLD";
    case CurrencyType::InfernalMana:
        return "DLG_BEGIN_TURN_REDM_P";
    case CurrencyType::LifeMana:
        return "DLG_BEGIN_TURN_BLUEM_P";
    case CurrencyType::DeathMana:
        return "DLG_BEGIN_TURN_BLACKM_P";
    case CurrencyType::RunicMana:
        return "DLG_BEGIN_TURN_WHITEM_P";
    case CurrencyType::GroveMana:
        return "_GREEN_MANA__GREEN_MANA";
    }
}

static game::CMultiLayerImg* createResourceToggleImage(const game::CMqPoint& size,
                                                       game::CurrencyType currency,
                                                       const char* amount,
                                                       bool selected = false)
{
    using namespace game;

    const auto& allocateMem{Memory::get().allocate};

    auto text{(CImage2Text*)allocateMem(sizeof(CImage2Text))};
    CImage2TextApi::get().constructor(text, size.x, size.y);
    CImage2TextApi::get().setText(text, amount);

    const auto& multilayerApi{CMultiLayerImgApi::get()};

    auto multilayerImage{(CMultiLayerImg*)allocateMem(sizeof(CMultiLayerImg))};
    multilayerApi.constructor(multilayerImage);

    const Color color{0x00b2d9f5u};
    auto outline{(CImage2Outline*)allocateMem(sizeof(CImage2Outline))};
    // We always use outline to get consistent images whether button is selected or not.
    // When button is not selected, outline is completely transparent
    CImage2OutlineApi::get().constructor(outline, &size, &color, selected ? 0xff : 0);

    constexpr int offset{10};

    multilayerApi.addImage(multilayerImage, outline, -999, -999);

    IMqImage2* currencyImage{AutoDialogApi::get().loadImage(getCurrencyImage(currency))};
    multilayerApi.addImage(multilayerImage, currencyImage, offset, -999);

    multilayerApi.addImage(multilayerImage, text, -10, -999);

    return multilayerImage;
}

static std::string getResourceAmountString(std::uint16_t amount, bool infinite)
{
    if (infinite) {
        return std::string{
            "\\hR;\\vC;"
            + getInterfaceText(textIds().resourceMarket.infiniteAmount.c_str(), "Inf.")};
    }

    return fmt::format("\\hR;\\vC;{:d}", amount);
}

static void updateExchangeRatesUi(CSiteResourceMarketInterf* interf)
{
    using namespace game;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};

    const auto& radio{CRadioButtonInterfApi::get()};

    CRadioButtonInterf* radioP{dialogApi.findRadioButton(dialog, "RAD_RESOURCE_P")};
    if (radioP->data->selectedButton < 0) {
        return;
    }

    const CurrencyType selectedResource{(CurrencyType)radioP->data->selectedButton};

    const auto& exchangeRates{interf->exchangeRates};

    const auto it{std::find_if(exchangeRates.cbegin(), exchangeRates.cend(),
                               [selectedResource](const ResourceExchange& exchange) {
                                   return (int)exchange.resource1 == (int)selectedResource;
                               })};

    // clang-format off
    const std::array<CTextBoxInterf*, 6u> rates{
        dialogApi.findTextBox(dialog, "TXT_RATE_GOLD"),
        dialogApi.findTextBox(dialog, "TXT_RATE_INFERNAL"),
        dialogApi.findTextBox(dialog, "TXT_RATE_LIFE"),
        dialogApi.findTextBox(dialog, "TXT_RATE_DEATH"),
        dialogApi.findTextBox(dialog, "TXT_RATE_RUNIC"),
        dialogApi.findTextBox(dialog, "TXT_RATE_GROVE")
    };
    // clang-format on

    bool exchangePossible[6]{};

    if (it != exchangeRates.cend()) {
        // Player can exchange selected resource to something from the market
        char text[32];
        for (const auto& rate : it->rates) {
            const int index{(int)rate.resource2};
            exchangePossible[index] = true;

            const auto result{
                fmt::format_to_n(text, sizeof(text) - 1u, "{:d}/{:d}", rate.amount1, rate.amount2)};
            text[result.size] = 0;

            CTextBoxInterf* textBox{rates[index]};
            CTextBoxInterfApi::get().setString(textBox, text);
        }
    }

    const auto notAvailableText{
        getInterfaceText(textIds().resourceMarket.exchangeNotAvailable.c_str(), "N/A")};
    // Mark unavailable exchanges
    for (std::size_t i = 0u; i < std::size(exchangePossible); ++i) {
        if (exchangePossible[i]) {
            continue;
        }

        CTextBoxInterf* textBox{rates[i]};
        CTextBoxInterfApi::get().setString(textBox, notAvailableText.c_str());
    }
}

static game::CurrencyType getPlayerCurrencyType(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    const auto* button{dialogApi.findRadioButton(dialog, "RAD_RESOURCE_P")};
    return static_cast<CurrencyType>(button->data->selectedButton);
}

static game::CurrencyType getMarketCurrencyType(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    const auto* button{dialogApi.findRadioButton(dialog, "RAD_RESOURCE_M")};
    return static_cast<CurrencyType>(button->data->selectedButton);
}

static bool isExchangePossible(CSiteResourceMarketInterf* thisptr, int exchangeStep)
{
    using namespace game;

    const auto playerCurrency{getPlayerCurrencyType(thisptr)};
    const auto marketCurrency{getMarketCurrencyType(thisptr)};

    auto rates{findExchangeRates(thisptr->exchangeRates, playerCurrency, marketCurrency)};
    if (!rates) {
        return false;
    }

    // Check if player can exchange
    const std::uint16_t playerExchangeAmount = exchangeStep * rates->amount1;

    const CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&thisptr->phaseGame->phase)};
    const auto* stack{getStack(dataCache, &thisptr->visitorStackId)};
    const auto* player{getPlayer(dataCache, &stack->ownerId)};

    const auto& bankApi{BankApi::get()};
    const auto playerResource{bankApi.get(&player->bank, playerCurrency)};
    if (playerResource < playerExchangeAmount) {
        return false;
    }

    auto site{dataCache->vftable->findScenarioObjectById(dataCache, &thisptr->marketId)};
    auto market{static_cast<const CMidSiteResourceMarket*>(site)};

    if (isMarketStockInfinite(market->infiniteStock, marketCurrency)) {
        return true;
    }

    // Check if market can exchange
    const std::uint16_t marketExchangeAmount = exchangeStep * rates->amount2;

    const auto marketResource{bankApi.get(&market->stock, marketCurrency)};
    return marketResource >= marketExchangeAmount;
}

static void updateMarketExchangeAmount(CSiteResourceMarketInterf* interf,
                                       game::CurrencyType currency)
{
    using namespace game;

    const CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&interf->phaseGame->phase)};
    auto obj{dataCache->vftable->findScenarioObjectById(dataCache, &interf->marketId)};
    auto market{static_cast<const CMidSiteResourceMarket*>(obj)};

    const CurrencyType playerCurrency{getPlayerCurrencyType(interf)};
    auto rates{findExchangeRates(interf->exchangeRates, playerCurrency, currency)};

    std::uint16_t amount = 0u;
    if (rates) {
        amount = interf->exchangeStep * rates->amount2;
    }

    const auto string{getResourceAmountString(amount, false)};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};

    CRadioButtonInterf* radio{dialogApi.findRadioButton(dialog, "RAD_RESOURCE_M")};
    const auto& buttons{radio->data->buttons};

    const CToggleButton* button{buttons.bgn[0].first};
    const CMqRect* area{button->vftable->getArea((CInterface*)button)};
    const CMqPoint size{area->right - area->left, area->bottom - area->top};

    auto image{createResourceToggleImage(size, currency, string.c_str())};

    CPictureInterf* picture{dialogApi.findPicture(dialog, "IMG_RESOURCE_M")};
    CPictureInterfApi::get().setImageWithAnchor(picture, image, 9);
}

static void updatePlayerExchangeAmount(CSiteResourceMarketInterf* interf,
                                       game::CurrencyType currency)
{
    using namespace game;

    const CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&interf->phaseGame->phase)};

    const CMidStack* visitorStack{getStack(dataCache, &interf->visitorStackId)};
    const CMidPlayer* visitorPlayer{getPlayer(dataCache, &visitorStack->ownerId)};

    std::uint16_t amount = 0u;

    const CurrencyType marketCurrency{getMarketCurrencyType(interf)};
    auto rates{findExchangeRates(interf->exchangeRates, currency, marketCurrency)};
    if (rates) {
        amount = interf->exchangeStep * rates->amount1;
    }

    const auto string{getResourceAmountString(amount, false)};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};

    CRadioButtonInterf* radio{dialogApi.findRadioButton(dialog, "RAD_RESOURCE_P")};
    const auto& buttons{radio->data->buttons};

    const CToggleButton* button{buttons.bgn[0].first};
    const CMqRect* area{button->vftable->getArea((CInterface*)button)};
    const CMqPoint size{area->right - area->left, area->bottom - area->top};

    auto image{createResourceToggleImage(size, currency, string.c_str())};

    CPictureInterf* picture{dialogApi.findPicture(dialog, "IMG_RESOURCE_P")};
    CPictureInterfApi::get().setImageWithAnchor(picture, image, 9);
}

static void updateExchangeButtons(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    updateMarketExchangeAmount(thisptr, getMarketCurrencyType(thisptr));
    updatePlayerExchangeAmount(thisptr, getPlayerCurrencyType(thisptr));

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    auto* stepDownButton{dialogApi.findButton(dialog, "BTN_EXCHANGE_DOWN")};
    // Allow to decrease exchange step until 1.
    stepDownButton->vftable->setEnabled(stepDownButton, thisptr->exchangeStep > 1);

    const bool currentPossible{isExchangePossible(thisptr, thisptr->exchangeStep)};

    auto* exchangeButton{dialogApi.findButton(dialog, "BTN_EXCHANGE")};
    // Update exchange button depending on exchange possibility at current step
    exchangeButton->vftable->setEnabled(exchangeButton, currentPossible);

    if (!currentPossible) {
        // Disable up button if exchange at current step is not possible
        auto* stepUpButton{dialogApi.findButton(dialog, "BTN_EXCHANGE_UP")};
        stepUpButton->vftable->setEnabled(stepUpButton, false);
        return;
    }

    // Disable up button if it is not possible to perform exchange at the next exchange step
    const bool nextPossible{isExchangePossible(thisptr, thisptr->exchangeStep + 1)};

    auto* stepUpButton{dialogApi.findButton(dialog, "BTN_EXCHANGE_UP")};
    stepUpButton->vftable->setEnabled(stepUpButton, nextPossible);
}

static std::string getResourceName(game::CurrencyType resource)
{
    using namespace game;

    const char* textId{};
    switch (resource) {
    case CurrencyType::Gold:
        textId = "X100TA0082";
        break;
    case CurrencyType::LifeMana:
        textId = "X100TA0098";
        break;
    case CurrencyType::DeathMana:
        textId = "X100TA0096";
        break;
    case CurrencyType::RunicMana:
        textId = "X100TA0097";
        break;
    case CurrencyType::InfernalMana:
        textId = "X100TA0099";
        break;
    case CurrencyType::GroveMana:
        textId = "X160TA0038";
        break;
    }

    return getInterfaceText(textId);
}

static void updateExchangeDescription(CSiteResourceMarketInterf* interf)
{
    using namespace game;

    std::string desc{getInterfaceText(textIds().resourceMarket.exchangeDesc.c_str(),
                                      "You offer %RES1% to get %RES2% in return.")};

    const auto playerCurrency{getPlayerCurrencyType(interf)};
    const auto marketCurrency{getMarketCurrencyType(interf)};

    replace(desc, "%RES1%", getResourceName(playerCurrency));
    replace(desc, "%RES2%", getResourceName(marketCurrency));

    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};
    CTextBoxInterf* exchangeDesc{CDialogInterfApi::get().findTextBox(dialog, "TXT_EXCHANGE_DESC")};

    CTextBoxInterfApi::get().setString(exchangeDesc, desc.c_str());
}

static void __fastcall onMarketResourceTogglePressed(CSiteResourceMarketInterf* interf,
                                                     int /*%edx*/,
                                                     int index)
{
    using namespace game;

    // Resource selection changed, reset exchange step
    interf->exchangeStep = 1;

    updateExchangeRatesUi(interf);
    updateExchangeButtons(interf);
    updateExchangeDescription(interf);
}

static void __fastcall onPlayerResourceTogglePressed(CSiteResourceMarketInterf* interf,
                                                     int /*%edx*/,
                                                     int index)
{
    using namespace game;

    // Resource selection changed, reset exchange step
    interf->exchangeStep = 1;

    updateExchangeRatesUi(interf);
    updateExchangeButtons(interf);
    updateExchangeDescription(interf);
}

static void __fastcall exchangeButtonHandler(CSiteResourceMarketInterf* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto playerCurrency{getPlayerCurrencyType(thisptr)};
    const auto marketCurrency{getMarketCurrencyType(thisptr)};

    sendExchangeResourcesMsg(thisptr->phaseGame, thisptr->marketId, thisptr->visitorStackId,
                             playerCurrency, marketCurrency, thisptr->exchangeStep);

    // Clear exchange step
    thisptr->exchangeStep = 1;
    updateExchangeButtons(thisptr);

    playSoundEffect(SoundEffect::Buyitem);
}

static void __fastcall exchangeStepDownButtonHandler(CSiteResourceMarketInterf* thisptr,
                                                     int /*%edx*/)
{
    if (thisptr->exchangeStep == 1) {
        return;
    }

    --thisptr->exchangeStep;
    updateExchangeButtons(thisptr);
}

static void __fastcall exchangeStepUpButtonHandler(CSiteResourceMarketInterf* thisptr, int /*%edx*/)
{
    ++thisptr->exchangeStep;
    updateExchangeButtons(thisptr);
}

static void setupResourceToggleButton(game::CDialogInterf* dialog,
                                      const char* buttonName,
                                      game::CurrencyType currency,
                                      std::uint16_t amount,
                                      bool infinite = false)
{
    using namespace game;

    const auto string{getResourceAmountString(amount, infinite)};
    const char* ptr{string.c_str()};

    CToggleButton* button{CDialogInterfApi::get().findToggleButton(dialog, buttonName)};
    const CMqRect* area{button->vftable->getArea((CInterface*)button)};
    const CMqPoint size{area->right - area->left, area->bottom - area->top};

    auto normal{createResourceToggleImage(size, currency, ptr)};
    button->vftable->setImage(button, normal, ToggleButtonState::Normal);

    auto normalChecked{createResourceToggleImage(size, currency, ptr, true)};
    button->vftable->setImage(button, normalChecked, ToggleButtonState::NormalChecked);

    auto hovered{createResourceToggleImage(size, currency, ptr, true)};
    button->vftable->setImage(button, hovered, ToggleButtonState::Hovered);

    auto hoveredChecked{createResourceToggleImage(size, currency, ptr, true)};
    button->vftable->setImage(button, hoveredChecked, ToggleButtonState::HoveredChecked);

    auto clicked{createResourceToggleImage(size, currency, ptr, true)};
    button->vftable->setImage(button, clicked, ToggleButtonState::Clicked);

    auto clickedChecked{createResourceToggleImage(size, currency, ptr, true)};
    button->vftable->setImage(button, clickedChecked, ToggleButtonState::ClickedChecked);
}

static void updatePlayerResources(CSiteResourceMarketInterf* interf)
{
    using namespace game;

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&interf->phaseGame->phase)};

    const CMidStack* visitorStack{getStack(dataCache, &interf->visitorStackId)};
    const CMidPlayer* visitorPlayer{getPlayer(dataCache, &visitorStack->ownerId)};
    const Bank& playerBank{visitorPlayer->bank};

    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};
    setupResourceToggleButton(dialog, "TOG_GOLD_P", CurrencyType::Gold, playerBank.gold);
    setupResourceToggleButton(dialog, "TOG_LIFE_P", CurrencyType::LifeMana, playerBank.lifeMana);
    setupResourceToggleButton(dialog, "TOG_DEATH_P", CurrencyType::DeathMana, playerBank.deathMana);
    setupResourceToggleButton(dialog, "TOG_RUNIC_P", CurrencyType::RunicMana, playerBank.runicMana);
    setupResourceToggleButton(dialog, "TOG_INFERNAL_P", CurrencyType::InfernalMana,
                              playerBank.infernalMana);
    setupResourceToggleButton(dialog, "TOG_GROVE_P", CurrencyType::GroveMana, playerBank.groveMana);
}

static void updateMarketResources(CSiteResourceMarketInterf* interf)
{
    using namespace game;

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&interf->phaseGame->phase)};

    auto obj{dataCache->vftable->findScenarioObjectById(dataCache, &interf->marketId)};
    auto market{(const CMidSiteResourceMarket*)obj};

    const Bank& stock{market->stock};
    const auto& inf{market->infiniteStock.parts};

    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};
    setupResourceToggleButton(dialog, "TOG_GOLD_M", CurrencyType::Gold, stock.gold, inf.gold);
    setupResourceToggleButton(dialog, "TOG_LIFE_M", CurrencyType::LifeMana, stock.lifeMana,
                              inf.lifeMana);
    setupResourceToggleButton(dialog, "TOG_DEATH_M", CurrencyType::DeathMana, stock.deathMana,
                              inf.deathMana);
    setupResourceToggleButton(dialog, "TOG_RUNIC_M", CurrencyType::RunicMana, stock.runicMana,
                              inf.runicMana);
    setupResourceToggleButton(dialog, "TOG_INFERNAL_M", CurrencyType::InfernalMana,
                              stock.infernalMana, inf.infernalMana);
    setupResourceToggleButton(dialog, "TOG_GROVE_M", CurrencyType::GroveMana, stock.groveMana,
                              inf.groveMana);
}

static void setupUi(CSiteResourceMarketInterf* interf)
{
    using namespace game;

    // Reuse button functors from CMenuBase
    const auto& menuBase{CMenuBaseApi::get()};
    const auto& button{CButtonInterfApi::get()};
    const auto& freeFunctor{SmartPointerApi::get().createOrFreeNoDtor};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};
    SmartPointer functor;

    using ButtonCallback = CMenuBaseApi::Api::ButtonCallback;
    auto buttonCallback = (ButtonCallback)siteResMarketInterfBtnCloseHandler;

    menuBase.createButtonFunctor(&functor, 0, (CMenuBase*)interf, &buttonCallback);
    button.assignFunctor(dialog, "BTN_CLOSE", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    buttonCallback = (ButtonCallback)exchangeButtonHandler;
    menuBase.createButtonFunctor(&functor, 0, (CMenuBase*)interf, &buttonCallback);
    button.assignFunctor(dialog, "BTN_EXCHANGE", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    buttonCallback = (ButtonCallback)exchangeStepDownButtonHandler;
    menuBase.createButtonFunctor(&functor, 0, (CMenuBase*)interf, &buttonCallback);
    button.assignFunctor(dialog, "BTN_EXCHANGE_DOWN", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    buttonCallback = (ButtonCallback)exchangeStepUpButtonHandler;
    menuBase.createButtonFunctor(&functor, 0, (CMenuBase*)interf, &buttonCallback);
    button.assignFunctor(dialog, "BTN_EXCHANGE_UP", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    updateMarketResources(interf);
    updatePlayerResources(interf);

    updateExchangeButtons(interf);
    updateExchangeRatesUi(interf);

    const auto& radio{CRadioButtonInterfApi::get()};

    using Callback = CMenuBaseApi::Api::RadioButtonCallback;

    Callback callback = (Callback)onMarketResourceTogglePressed;
    menuBase.createRadioButtonFunctor(&functor, 0, (CMenuBase*)interf, &callback);

    radio.setOnButtonPressed(dialog, "RAD_RESOURCE_M", dialogName, &functor);
    freeFunctor(&functor, nullptr);

    callback = (Callback)onPlayerResourceTogglePressed;
    menuBase.createRadioButtonFunctor(&functor, 0, (CMenuBase*)interf, &callback);

    radio.setOnButtonPressed(dialog, "RAD_RESOURCE_P", dialogName, &functor);
    freeFunctor(&functor, nullptr);

    onMarketResourceTogglePressed(interf, 0, static_cast<int>(CurrencyType::Gold));
    onPlayerResourceTogglePressed(interf, 0, static_cast<int>(CurrencyType::Gold));
}

static void removeStackGroupDropSourceTarget(CSiteResourceMarketInterf* thisptr)
{
    if (!thisptr->stackGroup) {
        return;
    }

    using namespace game;
    IMidDropManager* dropManager = &thisptr->dropManager;
    if (dropManager->vftable->getDropSource(dropManager) == thisptr->stackGroup) {
        dropManager->vftable->resetDropSource(dropManager);
    }

    CMidDragDropInterfApi::get().removeDropTarget(thisptr, thisptr->stackGroup);
    CMidDragDropInterfApi::get().removeDropSource(thisptr, thisptr->stackGroup);

    thisptr->stackGroup->IMidDropSource::vftable->destructor(thisptr->stackGroup, 1);
    thisptr->stackGroup = nullptr;
}

static void createPaperdollInterface(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    auto paperdoll = (CPaperdollChildInterf*)Memory::get().allocate(sizeof(CPaperdollChildInterf));

    CMidDragDropInterf* dragDrop = thisptr;
    CPaperdollChildInterfApi::get().constructor(paperdoll, dragDrop, thisptr->phaseGame,
                                                &thisptr->visitorStackId, dragDrop,
                                                &thisptr->paperdollArea);

    if (thisptr->paperdoll) {
        CInterface* interf = thisptr;
        int index = -1;
        interf->vftable->getChildIndex(interf, &index, thisptr->paperdoll);
        interf->vftable->deleteChildAt(interf, &index);
    }

    thisptr->paperdoll = paperdoll;

    if (thisptr->groupBackground) {
        thisptr->groupBackground->vftable->destructor(thisptr->groupBackground, 1);
        thisptr->groupBackground = nullptr;
    }

    removeStackGroupDropSourceTarget(thisptr);
}

static void hidePaperdoll(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    if (thisptr->paperdoll) {
        CInterface* interf = thisptr;
        int index = -1;
        interf->vftable->getChildIndex(interf, &index, thisptr->paperdoll);
        interf->vftable->deleteChildAt(interf, &index);
        thisptr->paperdoll = nullptr;
    }

    IMqImage2* backgroundImage{AutoDialogApi::get().loadImage("DLG_MERCHANT_GROUP_BG")};
    if (thisptr->groupBackground) {
        thisptr->groupBackground->vftable->destructor(thisptr->groupBackground, 1);
    }

    thisptr->groupBackground = backgroundImage;
}

static void createStackGroupInterface(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&thisptr->phaseGame->phase)};

    const auto* stack{getStack(dataCache, &thisptr->visitorStackId)};
    auto leader{
        (const CMidUnit*)dataCache->vftable->findScenarioObjectById(dataCache, &stack->leaderId)};

    auto leaderName{getUnitName(leader)};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    auto nameTextBox{dialogApi.findTextBox(dialog, "TXT_LEADER_NAME")};
    CTextBoxInterfApi::get().setString(nameTextBox, leaderName);

    // 'Items of party %NAME%'
    auto description{getInterfaceText("X005TA0106")};
    replace(description, "%NAME%", leaderName);

    auto itemsTextBox{dialogApi.findTextBox(dialog, "TXT_LEADER")};
    CTextBoxInterfApi::get().setString(itemsTextBox, description.c_str());

    auto faceImage{gameFunctions().createUnitFaceImage(&leader->unitImpl->id, true)};
    if (faceImage) {
        faceImage->vftable->setUnknown68(faceImage, 1);
        faceImage->vftable->setLeftSide(faceImage, 1);
    }

    auto leaderImage{dialogApi.findPicture(dialog, "IMG_LEADER")};
    CPictureInterfApi::get().setImageWithAnchor(leaderImage, (IMqImage2*)faceImage, 18);

    const auto& dragDrop{CMidDragDropInterfApi::get()};
    dragDrop.removeDropSource(thisptr, thisptr->stackGroup);
    if (thisptr->stackGroup) {
        dragDrop.removeDropTarget(thisptr, thisptr->stackGroup);
    }

    const CMidgard* midgard{CMidgardApi::get().instance()};
    const CMidgardID* currentPlayerId{&midgard->data->netPlayerClientPtr->second};

    auto adapter{(CMidUnitGroupAdapter*)Memory::get().allocate(sizeof(CMidUnitGroupAdapter))};
    CMidUnitGroupAdapterApi::get().constructor(adapter, dataCache, &thisptr->visitorStackId,
                                               currentPlayerId, 1);

    IMidDropManager* dropManager{&thisptr->dropManager};

    auto stackGroup{(CDDStackGroup*)Memory::get().allocate(sizeof(CDDStackGroup))};
    CDDStackGroupApi::get().constructor(stackGroup, adapter, &thisptr->visitorStackId, dataCache,
                                        dropManager, 1, &thisptr->groupArea,
                                        thisptr->dragAndDropInterfData->task, thisptr->phaseGame,
                                        thisptr, nullptr);

    auto currentSource{dropManager->vftable->getDropSource(dropManager)};
    if (currentSource == thisptr->stackGroup && currentSource != nullptr) {
        dropManager->vftable->setDropSource(dropManager, stackGroup, true);
    }

    if (thisptr->stackGroup) {
        thisptr->stackGroup->IMidDropSource::vftable->destructor(thisptr->stackGroup, 1);
    }

    thisptr->stackGroup = stackGroup;
    dropManager->vftable->addDropSource(dropManager, thisptr->stackGroup);
    dropManager->vftable->addDropTarget(dropManager, thisptr->stackGroup);
    hidePaperdoll(thisptr);
}

static void createPaperdollOrStackGroup(CSiteResourceMarketInterf* thisptr, bool toggleOn)
{
    if (toggleOn) {
        createPaperdollInterface(thisptr);
    } else {
        createStackGroupInterface(thisptr);
    }
}

static void createStackInventory(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&thisptr->phaseGame->phase)};

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    auto listBox{dialogApi.findListBox(dialog, "MLBOX_RINVENTORY")};

    auto inventory{
        (CDDStackInventoryDisplay*)Memory::get().allocate(sizeof(CDDStackInventoryDisplay))};
    CDDStackInventoryDisplayApi::get().constructor(inventory, thisptr, listBox,
                                                   &thisptr->visitorStackId, dataCache);

    auto currentInv{thisptr->inventory};

    IMidDropManager* dropManager{&thisptr->dropManager};
    auto current{dropManager->vftable->getDropSource(dropManager)};

    if (current == currentInv && current != nullptr) {
        dropManager->vftable->setDropSource(dropManager, inventory, 1);
    }

    CMidDragDropInterfApi::get().removeDropSource(thisptr, thisptr->inventory);

    int elementCount = 0;
    if (thisptr->inventory) {
        CMidDragDropInterfApi::get().removeDropTarget(thisptr, thisptr->inventory);

        elementCount = thisptr->inventory->foldedInvDisplayData->elementCount;

        thisptr->inventory->IMidDropSource::vftable->destructor(thisptr->inventory, 1);
    }

    thisptr->inventory = inventory;
    CDDStackInventoryDisplayApi::get().setElementCount(inventory, elementCount);
    dropManager->vftable->addDropSource(dropManager, inventory);
    dropManager->vftable->addDropTarget(dropManager, inventory);
}

static void onVisitorStackChanged(CSiteResourceMarketInterf* thisptr)
{
    using namespace game;

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{thisptr->dragAndDropInterfData->dialogInterf};

    const auto* toggle{dialogApi.findToggleButton(dialog, "TOG_PAPERDOLL")};
    createPaperdollOrStackGroup(thisptr, toggle->data->checked);
}

static void __fastcall onPaperdollTogglePressed(CSiteResourceMarketInterf* thisptr,
                                                int /*%edx*/,
                                                bool toggleOn,
                                                game::CToggleButton*)
{
    using namespace game;

    IMidDropManager* dropManager = &thisptr->dropManager;
    if (dropManager->vftable->getDropSource(dropManager)) {
        dropManager->vftable->resetDropSource(dropManager);
    }

    createPaperdollOrStackGroup(thisptr, toggleOn);
}

static void __fastcall siteResMarketInterfOnObjectChanged(game::CMidDataCache2::INotify* thisptr,
                                                          int /*%edx*/,
                                                          game::IMidScenarioObject* scenarioObject)
{
    if (!scenarioObject) {
        return;
    }

    using namespace game;

    const CMidgardID* objectId = &scenarioObject->id;
    const auto type = CMidgardIDApi::get().getType(objectId);

    auto interf = (CSiteResourceMarketInterf*)thisptr;

    switch (type) {
    case IdType::Stack: {
        if (*objectId == interf->visitorStackId) {
            onVisitorStackChanged(interf);

            if (interf->inventory) {
                createStackInventory(interf);
            }
        }

        return;
    }
    case IdType::Item: {
        if (!interf->inventory) {
            return;
        }

        const CDDFoldedInvData* invData{interf->inventory->foldedInvData};
        const CMidStack* stack{getStack(invData->objectMap, &invData->objectId)};
        const CMidInventory* inventory{&stack->inventory};

        if (inventory->vftable->getItemIndex(inventory, objectId) > -1) {
            createStackInventory(interf);
        }

        return;
    }
    case IdType::Player: {
        CPhaseGame* phaseGame{interf->phaseGame};
        const CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&phaseGame->phase)};
        const CMidStack* visitorStack{getStack(dataCache, &interf->visitorStackId)};

        if (visitorStack->ownerId == *objectId) {
            updatePlayerResources(interf);
            updateExchangeButtons(interf);
        }
        return;
    }
    case IdType::Site: {
        if (interf->marketId == *objectId) {
            updateMarketResources(interf);
            updateExchangeButtons(interf);
        }
        return;
    }
    }
}

static game::CInterfaceVftable::Draw drawInterface = nullptr;

static void __stdcall siteResMarketDraw(game::CMidDragDropInterf* thisptr,
                                        game::IMqRenderer2* renderer)
{
    using namespace game;

    CSiteResourceMarketInterf* interf{dragDropToSiteInterf(thisptr)};

    IMqImage2* background{interf->groupBackground};
    if (background) {
        const CMqPoint start{interf->paperdollArea.left, interf->paperdollArea.top};
        background->vftable->render(background, renderer, &start, nullptr, nullptr, nullptr);
    }

    if (drawInterface) {
        drawInterface(thisptr, renderer);
    }
}

static game::CInterfaceVftable::HandleMouse handleMouse = nullptr;

static int __fastcall siteResMarketHandleMouse(game::CMidDragDropInterf* thisptr,
                                               int /*%edx*/,
                                               std::uint32_t mouseButton,
                                               const game::CMqPoint* mousePosition)
{
    using namespace game;

    const CSiteResourceMarketInterf* interf{dragDropToSiteInterf(thisptr)};

    if (mouseButton != WM_RBUTTONDOWN) {
        return handleMouse(thisptr, mouseButton, mousePosition);
    }

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};

    const CPictureInterf* picture{dialogApi.findPicture(dialog, "IMG_LEADER")};
    const CMqRect* area{picture->vftable->getArea(picture)};

    if (!MqRectApi::get().ptInRect(area, mousePosition)) {
        return handleMouse(thisptr, mouseButton, mousePosition);
    }

    const auto* paperdoll{dialogApi.findToggleButton(dialog, "TOG_PAPERDOLL")};
    if (paperdoll->data->checked) {
        return handleMouse(thisptr, mouseButton, mousePosition);
    }

    if (interf->visitorStackId == emptyId) {
        return handleMouse(thisptr, mouseButton, mousePosition);
    }

    const auto& phaseApi{CPhaseApi::get()};
    CPhase* phase{&interf->phaseGame->phase};

    CMidDataCache2* dataCache{phaseApi.getDataCache(phase)};
    const CMidStack* stack{getStack(dataCache, &interf->visitorStackId)};
    if (!stack) {
        return handleMouse(thisptr, mouseButton, mousePosition);
    }

    CEncParamIDPlayer encParam;
    CEncParamIDPlayerApi::get().constructor(&encParam, &stack->leaderId,
                                            phaseApi.getCurrentPlayerId(phase), nullptr);

    phaseApi.showEncyclopediaPopup(phase, &encParam);
    CEncParamIDPlayerApi::get().destructor(&encParam);

    return 1;
}

static game::CMidDataCache2::INotifyVftable siteResMarketNotifyVftable{
    (game::CMidDataCache2::INotifyVftable::OnObjectChanged)siteResMarketInterfOnObjectChanged};

static game::CInterfaceVftable::Destructor dragDropInterfDtor = nullptr;

static game::RttiInfo<game::CInterfaceVftable> siteResMarketRttiInfo;

game::CInterface* createSiteResourceMarketInterf(game::ITask* task,
                                                 game::CPhaseGame* phaseGame,
                                                 const game::CMidgardID& visitorStackId,
                                                 const game::CMidgardID& marketId)
{
    using namespace game;

    auto interf = (CSiteResourceMarketInterf*)Memory::get().allocate(
        sizeof(CSiteResourceMarketInterf));

    CMidDragDropInterfApi::get().constructor(interf, dialogName, task, phaseGame);
    static bool firstTime{true};
    if (firstTime) {
        firstTime = false;

        const auto* vftable{interf->CMidDragDropInterf::vftable};
        // Remember base class destructor for proper cleanup
        dragDropInterfDtor = vftable->destructor;
        drawInterface = vftable->draw;
        handleMouse = vftable->handleMouse;

        replaceRttiInfo(siteResMarketRttiInfo, vftable, true);

        siteResMarketRttiInfo.vftable.destructor = (CInterfaceVftable::Destructor)
            siteResMarketInterfDragDropDtor;
        siteResMarketRttiInfo.vftable.draw = (CInterfaceVftable::Draw)siteResMarketDraw;
        siteResMarketRttiInfo.vftable.handleMouse = (CInterfaceVftable::HandleMouse)
            siteResMarketHandleMouse;
    }

    // Use our own vftables
    interf->CInterface::vftable = &siteResMarketRttiInfo.vftable;
    interf->CMidDataCache2::INotify::vftable = &siteResMarketNotifyVftable;
    interf->IResetStackExt::vftable = &siteResMarketStackExtVftable;

    new (&interf->exchangeRates) MarketExchangeRates();
    interf->visitorStackId = visitorStackId;
    interf->marketId = marketId;
    interf->exchangeStep = 1;
    interf->paperdoll = nullptr;
    interf->stackGroup = nullptr;
    interf->groupBackground = nullptr;
    interf->inventory = nullptr;

    CMidDataCache2* dataCache{CPhaseApi::get().getDataCache(&phaseGame->phase)};
    CMidDataCache2Api::get().addNotify(dataCache, interf);

    const auto& dialogApi{CDialogInterfApi::get()};
    CDialogInterf* dialog{interf->dragAndDropInterfData->dialogInterf};

    if (getExchangeRates(dataCache, marketId, visitorStackId, interf->exchangeRates)) {
        setupUi(interf);
    } else {
        // Exchange rates script contain errors, disable exchange button
        CButtonInterf* button{dialogApi.findButton(dialog, "BTN_EXCHANGE")};
        button->vftable->setEnabled(button, false);
    }

    // Setup leader image, etc
    const CPictureInterf* paperdollBg{dialogApi.findPicture(dialog, "IMG_PAPERDOLL_BG")};
    interf->paperdollArea = *paperdollBg->vftable->getArea(paperdollBg);

    const CPictureInterf* slotImage{dialogApi.findPicture(dialog, "IMG_SLOT1")};
    interf->groupArea = *slotImage->vftable->getArea(slotImage);

    dialogApi.hideControl(dialog, "IMG_PAPERDOLL_BG");
    dialogApi.hideControl(dialog, "IMG_SLOT1");

    auto obj{dataCache->vftable->findScenarioObjectById(dataCache, &marketId)};
    auto market{static_cast<const CMidSiteResourceMarket*>(obj)};
    const char* title{market->title.string ? market->title.string : ""};

    CTextBoxInterf* marketText{dialogApi.findTextBox(dialog, "TXT_LABEL1")};
    CTextBoxInterfApi::get().setString(marketText, title);

    createStackGroupInterface(interf);
    createStackInventory(interf);

    using ButtonCallback = CMainView2Api::Api::ToggleButtonCallback;
    ButtonCallback callback{};
    callback.callback = (ButtonCallback::Callback)&onPaperdollTogglePressed;

    SmartPointer functor{};
    CMainView2Api::get().createToggleButtonFunctor(&functor, 0, (CMainView2*)interf, &callback);

    CToggleButtonApi::get().assignFunctor(dialog, "TOG_PAPERDOLL", dialogName, &functor, 0);
    SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);

    playSoundEffect(SoundEffect::Entrsite);

    return interf;
}

} // namespace hooks
