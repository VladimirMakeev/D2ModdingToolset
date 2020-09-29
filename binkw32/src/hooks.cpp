/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "hooks.h"
#include "autodialog.h"
#include "buildingtype.h"
#include "button.h"
#include "categories.h"
#include "d2string.h"
#include "dbtable.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "functor.h"
#include "game.h"
#include "globaldata.h"
#include "idlist.h"
#include "interfmanager.h"
#include "listbox.h"
#include "log.h"
#include "mapgen.h"
#include "mempool.h"
#include "menunewskirmishsingle.h"
#include "middatacache.h"
#include "midgardid.h"
#include "midgardmsgbox.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midplayer.h"
#include "playerbuildings.h"
#include "racetype.h"
#include "scenariodata.h"
#include "scenariodataarray.h"
#include "settings.h"
#include "smartptr.h"
#include "unitbranchcat.h"
#include "unitsforhire.h"
#include <algorithm>
#include <cstring>
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace hooks {

void respopupInitHooked(void)
{
    logDebug("binkwProxy.log", "Resource popup hook start");

    auto& variables = game::gameVariables();

    *variables.minimapMode = userSettings().showLandConverted;
    *variables.respopup = userSettings().showResources;

    logDebug("binkwProxy.log", "Resource popup hook finished");
}

void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/)
{
    logDebug("binkwProxy.log", "Show banners hook start");

    char* ptr = (char*)thisptr;
    *ptr = userSettings().showBanners;
    // meaning unknown
    ptr[1] = 0;

    logDebug("binkwProxy.log", "Show banners hook finished");
    return thisptr;
}

bool __fastcall processUnitModifiersHooked(void* thisptr, int /*%edx*/, int* a2)
{
    if (!thisptr) {
        return false;
    }

    return game::gameFunctions().processUnitModifiers(thisptr, a2);
}

using ScriptLines = std::vector<std::string>;

ScriptLines::iterator addDialogUiElements(ScriptLines& script,
                                          const std::string& dialogName,
                                          const ScriptLines& newElements)
{
    const std::string dialogStr{std::string{"DIALOG\t"} + dialogName};

    auto line = std::find_if(script.begin(), script.end(), [&dialogStr](const std::string& line) {
        return line.find(dialogStr) != std::string::npos;
    });

    if (line == script.end()) {
        return line;
    }

    // Skip DIALOG and BEGIN lines
    std::advance(line, 2);
    return script.insert(line, newElements.begin(), newElements.end());
}

void addRandomMapGeneratorUi(ScriptLines& script)
{
    const ScriptLines uiElements{
        "\tBUTTON\tBINKW_PROXY_BTN_GEN_MAP,212,122,422,142,,,,,\"\",0",
        "\tTEXT\tBINKW_PROXY_TXT_GEN_MAP,212,122,422,142,\\hC;\\vC;,\"Generate random map\",\"Open map generation menu\""};

    addDialogUiElements(script, "DLG_CHOOSE_SKIRMISH", uiElements);
}

void changeHireDialogUi(ScriptLines& script)
{
    const std::string btnPgUpName{"BINKW_PROXY_BTN_PG_UP"};
    const std::string btnPgDownName{"BINKW_PROXY_BTN_PG_DN"};
    const std::string btnUpName{"BINKW_PROXY_BTN_LIST_UP"};
    const std::string btnDownName{"BINKW_PROXY_BTN_LIST_DN"};

    const std::string buttonImageName{"DLG_UPGRADE_LEADER_ARROW"};
    const std::string btnDownImageName{fmt::format("{:s}_DOWN_", buttonImageName)};
    const std::string btnUpImageName{fmt::format("{:s}_UP_", buttonImageName)};

    const int buttonSize{26};
    const int buttonX{415};
    const int buttonUpY{30};
    const int buttonDownY{480};

    const ScriptLines uiElements{
        // Page up and page down hotkeys
        fmt::format("\tBUTTON\t{0},0,0,10,10,,,,,\"\",0,34", btnPgDownName),
        fmt::format("\tBUTTON\t{0},10,0,20,10,,,,,\"\",0,33", btnPgUpName),
        // Up and down keys, this also enables list scrolling with mouse wheel
        fmt::format("\tBUTTON\t{0},{1},{2},{3},{4},{5}N,{5}H,{5}C,{5}N,\"\",1,38", btnUpName,
                    buttonX, buttonUpY, buttonX + buttonSize, buttonUpY + buttonSize,
                    btnUpImageName),
        fmt::format("\tBUTTON\t{0},{1},{2},{3},{4},{5}N,{5}H,{5}C,{5}N,\"\",1,40", btnDownName,
                    buttonX, buttonDownY, buttonX + buttonSize, buttonDownY + buttonSize,
                    btnDownImageName)};

    auto line = addDialogUiElements(script, "DLG_HIRE_LEADER_2", uiElements);
    auto lboxLine = std::find_if(line, script.end(), [](const std::string& scriptLine) {
        return scriptLine.find("\tLBOX\tLBOX_LEADER") != std::string::npos;
    });

    if (lboxLine != script.end()) {
        *lboxLine = fmt::format(
            "\tLBOX\tLBOX_LEADER,125,43,410,503,285,85,0,7,{0},{1},,,{2},{3},BTN_HIRE_LEADER,,,0,\"\",0",
            btnUpName, btnDownName, btnPgUpName, btnPgDownName);
    }
}

game::AutoDialogData* __fastcall loadScriptFileHooked(game::AutoDialogData* thisptr,
                                                      int /*%edx*/,
                                                      const char* filePath,
                                                      int /*unknown*/)
{
    const auto& stringApi = game::StringApi::get();

    thisptr->initialized = false;
    thisptr->unknown = 0;

    game::StringArray* array = &thisptr->lines;
    std::memset(array, 0, sizeof(game::StringArray));

    stringApi.initFromString(&thisptr->scriptPath, filePath);

    ScriptLines scriptLines;

    {
        std::ifstream stream(filePath);
        if (!stream) {
            logError("binkwProxy.log", "Failed to open AutoDialog script file");
            return thisptr;
        }

        for (std::string line; std::getline(stream, line);) {
            scriptLines.push_back(line);
        }
    }

    // As for now, it is easier to add new ui elements to AutoDialog script
    // and let the game do processing, than reverse-engineer all CDlgPrototype class hierarchy.
    // Use uncommon names to avoid collisions.
    // addRandomMapGeneratorUi(scriptLines);

    if (!unitsForHire().empty()) {
        changeHireDialogUi(scriptLines);
    }

    for (auto& line : scriptLines) {
        if (line.empty()) {
            continue;
        }

        const auto lastCharacter = line.length() - 1;
        if (line[lastCharacter] == '\n') {
            line.erase(lastCharacter);
        }

        game::String str;
        stringApi.initFromString(&str, line.c_str());
        game::StringArrayApi::get().pushBack(array, &str);
        stringApi.free(&str);
    }

    thisptr->initialized = true;
    return thisptr;
}

static int* sub_6804ae(void* thisptr)
{
    return *(int**)(((int*)thisptr)[2]);
}

/**
 * Returns ScenarioDataArrayWrapped, actual meaning and types are unknown.
 * @param[in] unknown parameter is a pointer returned from sub_6804ae().
 */
static game::ScenarioDataArrayWrapped* sub_573134(int* unknown)
{
    return *(game::ScenarioDataArrayWrapped**)(*(unknown + 2) + 20);
}

static size_t getScenariosTotal(const game::ScenarioDataArray& data)
{
    const auto bgn = reinterpret_cast<size_t>(data.bgn);
    const auto end = reinterpret_cast<size_t>(data.end);

    return (end - bgn) / sizeof(game::ScenarioData);
}

void showMessageBox(const std::string& message)
{
    using namespace game;

    auto memAlloc = Memory::get().allocate;
    auto* btnHandler = (CMidMsgBoxButtonHandlerStd*)memAlloc(sizeof(CMidMsgBoxButtonHandlerStd));
    btnHandler->vftable = CMidMsgBoxButtonHandlerStdApi::vftable();

    CMidgardMsgBox* msgBox = (CMidgardMsgBox*)memAlloc(sizeof(CMidgardMsgBox));
    CMidgardMsgBoxApi::get().constructor(msgBox, message.c_str(), 0, btnHandler, 0, nullptr);

    SmartPointer ptr;
    CInterfManagerImplApi::get().get(&ptr);
    CInterfManagerImpl* manager = (CInterfManagerImpl*)ptr.data;

    manager->CInterfManagerImpl::CInterfManager::vftable->showInterface(manager, msgBox);
    SmartPointerApi::get().createOrFree(&ptr, nullptr);
}

static void __fastcall buttonGenerateMapCallback(game::CMenuNewSkirmish* thisptr, int /*%edx*/)
{
    std::string errorMessage;
    if (!showMapGeneratorDialog(errorMessage)) {
        showMessageBox(errorMessage);
    }
}

static void menuNewSkirmishCtor(game::CMenuNewSkirmish* thisptr, int a1, const char* dialogName)
{
    using namespace game;

    const auto& menuBase = CMenuBaseApi::get();
    menuBase.constructor(thisptr, a1);
    thisptr->vftable = CMenuNewSkirmishApi::vftable();
    menuBase.createMenu(thisptr, dialogName);

    const auto dialog = menuBase.getDialogInterface(thisptr);
    const auto& menu = CMenuNewSkirmishApi::get();
    const auto& button = CButtonInterfApi::get();
    const auto freeFunctor = FunctorApi::get().createOrFree;
    Functor functor;

    menu.createButtonFunctor(&functor, 0, thisptr, &menu.buttonBackCallback);
    button.assignFunctor(dialog, "BTN_BACK", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    menu.createButtonFunctor(&functor, 0, thisptr, &menu.loadScenarioCallback);
    CButtonInterf* loadButton = button.assignFunctor(dialog, "BTN_LOAD", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    void* callback = buttonGenerateMapCallback;
    menu.createButtonFunctor(&functor, 0, thisptr,
                             (CMenuNewSkirmishApi::Api::ButtonCallback*)&callback);
    button.assignFunctor(dialog, "BINKW_PROXY_BTN_GEN_MAP", dialogName, &functor, 0);
    freeFunctor(&functor, nullptr);

    const auto& listBox = CListBoxInterfApi::get();
    menu.createListBoxDisplayTextFunctor(&functor, 0, thisptr, &menu.displayTextCallback);
    listBox.assignDisplayTextFunctor(dialog, "TLBOX_GAME_SLOT", dialogName, &functor, true);
    freeFunctor(&functor, nullptr);

    menu.createListBoxFunctor(&functor, 0, thisptr, &menu.listBoxCallback);
    listBox.assignFunctor(dialog, "TLBOX_GAME_SLOT", dialogName, &functor);
    freeFunctor(&functor, nullptr);

    int* unknown = sub_6804ae(thisptr);
    ScenarioDataArrayWrapped* scenarios = sub_573134(unknown);

    const size_t scenariosTotal = getScenariosTotal(scenarios->data);
    if (!scenariosTotal) {
        // Nothing to load
        CButtonInterfApi::vftable()->setEnabled(loadButton, false);
        return;
    }

    CListBoxInterf* lBox = CDialogInterfApi::get().findListBox(dialog, "TLBOX_GAME_SLOT");
    listBox.initContents(lBox, scenariosTotal);
    menu.updateScenarioUi(unknown, dialog, listBox.selectedIndex(lBox));
}

game::CMenuNewSkirmishSingle* __fastcall menuNewSkirmishSingleCtorHooked(
    game::CMenuNewSkirmishSingle* thisptr,
    int /*%edx*/,
    int a1)
{
    menuNewSkirmishCtor(thisptr, a1, "DLG_CHOOSE_SKIRMISH");
    thisptr->vftable = game::CMenuNewSkirmishSingleApi::vftable();

    return thisptr;
}

bool __stdcall addPlayerUnitsToHireListHooked(game::CMidDataCache2* dataCache,
                                              const game::CMidgardID* playerId,
                                              const game::CMidgardID* a3,
                                              game::IdList* hireList)
{
    using namespace game;

    const auto& list = IdListApi::get();
    list.setEmpty(hireList);

    const auto& id = CMidgardIDApi::get();
    if (id.getType(a3) != IdType::Fortification) {
        return false;
    }

    auto vftable = (const CMidDataCache2Api::Vftable*)dataCache->vftable;
    auto findScenarioObjectById = vftable->findScenarioObjectById;

    auto playerObject = findScenarioObjectById(dataCache, playerId);
    if (!playerObject) {
        logError("binkwProxyError.log",
                 fmt::format("Could not find player object with id {:x}", playerId->value));
        return false;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    CMidPlayer* player = (CMidPlayer*)dynamicCast(playerObject, 0, rtti.IMidScenarioObjectType,
                                                  rtti.CMidPlayerType, 0);
    if (!player) {
        logError("binkwProxyError.log",
                 fmt::format("Object with id {:x} is not player", playerId->value));
        return false;
    }

    auto buildingsObject = findScenarioObjectById(dataCache, &player->buildingsId);
    if (!buildingsObject) {
        logError("binkwProxyError.log", fmt::format("Could not find player buildings with id {:x}",
                                                    player->buildingsId.value));
        return false;
    }

    CPlayerBuildings* buildings = (CPlayerBuildings*)dynamicCast(buildingsObject, 0,
                                                                 rtti.IMidScenarioObjectType,
                                                                 rtti.CPlayerBuildingsType, 0);
    if (!buildings) {
        logError("binkwProxyError.log", fmt::format("Object with id {:x} is not player buildings",
                                                    player->buildingsId.value));
        return false;
    }

    const auto& global = GlobalDataApi::get();
    int* races = (*global.getGlobalData())->races;
    TRaceType* race = (TRaceType*)global.findById(races, &player->raceId);

    const auto& addUnitToHireList = gameFunctions().addUnitToHireList;
    const auto& unitBranch = UnitBranchCategories::get();

    addUnitToHireList(race, buildings, unitBranch.fighter, hireList);
    addUnitToHireList(race, buildings, unitBranch.archer, hireList);
    addUnitToHireList(race, buildings, unitBranch.mage, hireList);
    addUnitToHireList(race, buildings, unitBranch.special, hireList);

    gameFunctions().addSideshowUnitToHireList(race, buildings, hireList);

    const int raceIndex = id.getTypeIndex(&player->raceId);
    if (!unitsForHire().empty()) {
        const auto& units = unitsForHire()[raceIndex];
        for (const auto& unit : units) {
            list.add(hireList, &unit);
        }
    }

    return true;
}

void __stdcall createBuildingTypeHooked(const game::CDBTable* dbTable,
                                        void* a2,
                                        const game::GlobalData** globalData)
{
    using namespace game;

    auto* buildings = (*globalData)->buildingCategories;
    LBuildingCategory category;
    category.vftable = BuildingCategories::vftable();

    auto& db = CDBTableApi::get();
    db.findBuildingCategory(&category, dbTable, "CATEGORY", buildings);

    auto memAlloc = Memory::get().allocate;
    auto constructor = TBuildingTypeApi::get().constructor;
    TBuildingType* buildingType = nullptr;

    auto& buildingCategories = BuildingCategories::get();
    if (category.id == buildingCategories.unit->id || category.id == buildingCategories.heal->id) {
        // This is TBuildingUnitUpgType constructor
        // without TBuildingTypeData::category validity check
        TBuildingUnitUpgType* unitBuilding = (TBuildingUnitUpgType*)memAlloc(
            sizeof(TBuildingUnitUpgType));

        constructor(unitBuilding, dbTable, globalData);
        unitBuilding->branch.vftable = UnitBranchCategories::vftable();
        unitBuilding->vftable = TBuildingUnitUpgTypeApi::vftable();

        auto* branches = (*globalData)->unitBranches;
        db.findUnitBranchCategory(&unitBuilding->branch, dbTable, "BRANCH", branches);
        db.readUnitLevel(&unitBuilding->level, dbTable, "LEVEL");

        buildingType = unitBuilding;
    } else {
        buildingType = constructor((TBuildingType*)memAlloc(sizeof(TBuildingType)), dbTable,
                                   globalData);
    }

    if (!gameFunctions().addObjectAndCheckDuplicates(a2, buildingType)) {
        db.duplicateRecordException(dbTable, &buildingType->buildingId);
    }
}

} // namespace hooks
