/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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
#include "attackimpl.h"
#include "attackreachcat.h"
#include "autodialog.h"
#include "batattackgiveattack.h"
#include "batattackshatter.h"
#include "battlemsgdata.h"
#include "buildingbranch.h"
#include "buildingtype.h"
#include "button.h"
#include "d2string.h"
#include "dbf/dbffile.h"
#include "dbtable.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "editor.h"
#include "functor.h"
#include "game.h"
#include "globaldata.h"
#include "idlist.h"
#include "interfmanager.h"
#include "listbox.h"
#include "log.h"
#include "lordtype.h"
#include "mapgen.h"
#include "mempool.h"
#include "menunewskirmishsingle.h"
#include "middatacache.h"
#include "midgardid.h"
#include "midgardmsgbox.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midplayer.h"
#include "midunit.h"
#include "playerbuildings.h"
#include "racecategory.h"
#include "racetype.h"
#include "scenariodata.h"
#include "scenariodataarray.h"
#include "settings.h"
#include "smartptr.h"
#include "unitbranchcat.h"
#include "unitsforhire.h"
#include "ussoldier.h"
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
    logDebug("mss32Proxy.log", "Resource popup hook start");

    auto& variables = game::gameVariables();

    *variables.minimapMode = userSettings().showLandConverted;
    *variables.respopup = userSettings().showResources;

    logDebug("mss32Proxy.log", "Resource popup hook finished");
}

void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/)
{
    logDebug("mss32Proxy.log", "Show banners hook start");

    char* ptr = (char*)thisptr;
    *ptr = userSettings().showBanners;
    // meaning unknown
    ptr[1] = 0;

    logDebug("mss32Proxy.log", "Show banners hook finished");
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
            logError("mssProxyError.log", "Failed to open AutoDialog script file");
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

    auto findScenarioObjectById = dataCache->vftable->findScenarioObjectById;
    auto playerObject = findScenarioObjectById(dataCache, playerId);
    if (!playerObject) {
        logError("mssProxyError.log",
                 fmt::format("Could not find player object with id {:x}", playerId->value));
        return false;
    }

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    CMidPlayer* player = (CMidPlayer*)dynamicCast(playerObject, 0, rtti.IMidScenarioObjectType,
                                                  rtti.CMidPlayerType, 0);
    if (!player) {
        logError("mssProxyError.log",
                 fmt::format("Object with id {:x} is not player", playerId->value));
        return false;
    }

    auto buildingsObject = findScenarioObjectById(dataCache, &player->buildingsId);
    if (!buildingsObject) {
        logError("mssProxyError.log", fmt::format("Could not find player buildings with id {:x}",
                                                  player->buildingsId.value));
        return false;
    }

    CPlayerBuildings* buildings = (CPlayerBuildings*)dynamicCast(buildingsObject, 0,
                                                                 rtti.IMidScenarioObjectType,
                                                                 rtti.CPlayerBuildingsType, 0);
    if (!buildings) {
        logError("mssProxyError.log", fmt::format("Object with id {:x} is not player buildings",
                                                  player->buildingsId.value));
        return false;
    }

    const auto& global = GlobalDataApi::get();
    auto races = (*global.getGlobalData())->races;
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

static game::LBuildingCategory custom;
static bool customCategoryExists{false};

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
    if (category.id == buildingCategories.unit->id
        || (customCategoryExists && (category.id == custom.id))) {
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

static std::string trimSpaces(const std::string& str)
{
    const auto begin = str.find_first_not_of(" ");
    if (begin == std::string::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" ");
    return str.substr(begin, end - begin + 1);
}

game::LBuildingCategoryTable* __fastcall buildingCategoryTableCtorHooked(
    game::LBuildingCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    static const char dbfFileName[] = "LBuild.dbf";

    logDebug("newBuildingType.log", "Hook started");

    {
        utils::DbfFile dbf;
        std::filesystem::path globals{globalsFolderPath};
        if (!dbf.open(globals / dbfFileName)) {
            logError("mssProxyError.log", fmt::format("Could not open {:s}", dbfFileName));
        } else {
            utils::DbfRecord record;
            if (dbf.recordsTotal() > 4 && dbf.record(record, 4)) {
                std::string categoryName;
                if (record.value(categoryName, "TEXT") && trimSpaces(categoryName) == "L_CUSTOM") {
                    customCategoryExists = true;
                    logDebug("newBuildingType.log", "Found custom building category");
                }
            }
        }
    }

    using namespace game;
    auto& table = LBuildingCategoryTableApi::get();
    auto& categories = BuildingCategories::get();

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LBuildingCategoryTableApi::vftable();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(categories.guild, thisptr, "L_GUILD", dbfFileName);
    table.readCategory(categories.heal, thisptr, "L_HEAL", dbfFileName);
    table.readCategory(categories.magic, thisptr, "L_MAGIC", dbfFileName);
    table.readCategory(categories.unit, thisptr, "L_UNIT", dbfFileName);

    if (customCategoryExists) {
        table.readCategory(&custom, thisptr, "L_CUSTOM", dbfFileName);
    }

    table.initDone(thisptr);

    logDebug("newBuildingType.log", "Hook finished");
    return thisptr;
}

game::CBuildingBranch* __fastcall buildingBranchCtorHooked(game::CBuildingBranch* thisptr,
                                                           int /*%edx*/,
                                                           int phaseGame,
                                                           int* branchNumber)
{
    using namespace game;

    logDebug("newBuildingType.log", "CBuildingBranchCtor hook started");

    auto memAlloc = Memory::get().allocate;
    CBuildingBranchData* data = (CBuildingBranchData*)memAlloc(sizeof(CBuildingBranchData));

    const auto& buildingBranch = CBuildingBranchApi::get();
    buildingBranch.initData(data);

    thisptr->data = data;
    thisptr->vftable = CBuildingBranchApi::vftable();

    buildingBranch.initData2(&thisptr->data->unk1);
    thisptr->data->branchNumber = *branchNumber;
    buildingBranch.initData3(&thisptr->data->unk12);

    const auto& fn = gameFunctions();
    const CMidgardID* playerId = fn.getPlayerIdFromPhase(phaseGame + 8);
    auto objectMap = fn.getObjectMapFromPhase(phaseGame + 8);
    auto findScenarioObjectById = objectMap->vftable->findScenarioObjectById;

    auto playerObject = findScenarioObjectById(objectMap, playerId);

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    const CMidPlayer* player = (const CMidPlayer*)dynamicCast(playerObject, 0,
                                                              rtti.IMidScenarioObjectType,
                                                              rtti.CMidPlayerType, 0);

    const LRaceCategory* playerRace = &player->raceType->data->raceType;
    thisptr->data->raceCategory.table = playerRace->table;
    thisptr->data->raceCategory.id = playerRace->id;

    auto lord = fn.getLordByPlayer(player);
    auto buildList = lord->data->buildList;

    auto lordTypeApi = TLordTypeApi::get();
    BuildListIterator iterator;
    lordTypeApi.getIterator(buildList, &iterator);

    const auto globalData = GlobalDataApi::get().getGlobalData();
    auto buildings = (*globalData)->buildings;

    while (true) {
        BuildListIterator endIterator;
        lordTypeApi.getEndIterator(buildList, &endIterator);

        if (iterator.node == endIterator.node && iterator.node2 == endIterator.node2) {
            break;
        }

        const auto findById = GlobalDataApi::get().findById;
        const TBuildingType* buildingType = (const TBuildingType*)findById(buildings,
                                                                           &iterator.node->value);

        const LBuildingCategory* buildingCategory = &buildingType->data->category;
        const auto buildingCategories = BuildingCategories::get();

        if (buildingCategory->id == buildingCategories.unit->id) {
            const TBuildingUnitUpgType* unitUpg = (const TBuildingUnitUpgType*)
                dynamicCast(buildingType, 0, rtti.TBuildingTypeType, rtti.TBuildingUnitUpgTypeType,
                            0);

            LUnitBranch unitBranch;
            unitBranch.table = unitUpg->branch.table;
            unitBranch.id = unitUpg->branch.id;
            unitBranch.vftable = UnitBranchCategories::vftable();

            const auto& unitBranchCategories = UnitBranchCategories::get();
            const int num = *branchNumber;

            if (unitBranch.id == unitBranchCategories.sideshow->id) {
                buildingBranch.addSideshowUnitBuilding(&thisptr->data->unk1, unitUpg);
            } else if (unitBranch.id == unitBranchCategories.fighter->id && num == 0
                       || unitBranch.id == unitBranchCategories.mage->id && num == 1
                       || unitBranch.id == unitBranchCategories.archer->id && num == 2
                       || unitBranch.id == unitBranchCategories.special->id && num == 4) {
                buildingBranch.addUnitBuilding(phaseGame, thisptr->data, unitUpg);
            }

        } else if ((buildingCategory->id == buildingCategories.guild->id
                    || buildingCategory->id == buildingCategories.heal->id
                    || buildingCategory->id == buildingCategories.magic->id
                    || (customCategoryExists && (buildingCategory->id == custom.id)))
                   && *branchNumber == 3) {
            buildingBranch.addBuilding(phaseGame, thisptr->data, buildingType);
        }

        lordTypeApi.advanceIterator(&iterator.node, &iterator.node2->unknown);
    }

    logDebug("newBuildingType.log", "Ctor finished");
    return thisptr;
}

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    auto soldierVftable = (const IUsSoldierVftable*)soldier->vftable;
    IAttack* attack = soldierVftable->getAttackById(soldier);
    auto attackVftable = (const IAttackVftable*)attack->vftable;
    const LAttackReach* reach = attackVftable->getAttackReach(attack);

    // Place units with adjacent attack reach at the front lane, despite of their attack class
    if (reach->id == AttackReachCategories::get().adjacentAttackReach->id) {
        return 1;
    }

    return 0;
}

bool __stdcall isTurnValidHooked(int turn)
{
    return turn >= 0 && turn <= 9999 || turn == -1;
}

game::CMidgardID* __stdcall radioButtonIndexToPlayerIdHooked(game::CMidgardID* playerId,
                                                             game::IMidgardObjectMap* objectMap,
                                                             int index)
{
    using namespace game;
    const auto& races = RaceCategories::get();
    const auto& fn = editorFunctions;

    const CMidPlayer* player{nullptr};
    switch (index) {
    case 0:
        player = fn.findPlayerByRaceCategory(races.human, objectMap);
        break;
    case 1:
        player = fn.findPlayerByRaceCategory(races.heretic, objectMap);
        break;
    case 2:
        player = fn.findPlayerByRaceCategory(races.undead, objectMap);
        break;
    case 3:
        player = fn.findPlayerByRaceCategory(races.dwarf, objectMap);
        break;
    case 4:
        player = fn.findPlayerByRaceCategory(races.elf, objectMap);
        break;
    }

    *playerId = player ? player->playerId : emptyId;

    return playerId;
}

bool __fastcall giveAttackCanPerformHooked(game::CBatAttackGiveAttack* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetStackId{};
    thisptr->vftable->getTargetStackId(thisptr, &targetStackId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID alliedStackId{};
    fn.getAllyOrEnemyStackId(&alliedStackId, battleMsgData, unitId, true);

    if (targetStackId != alliedStackId) {
        // Do not allow to give additional attacks to enemies
        return false;
    }

    if (*unitId == thisptr->unitId1) {
        // Do not allow to give additional attacks to self
        return false;
    }

    CMidUnit* unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    auto soldierVftable = static_cast<const IUsSoldierVftable*>(soldier->vftable);

    auto attack = soldierVftable->getAttackById(soldier);
    auto attackVftable = static_cast<const IAttackVftable*>(attack->vftable);
    const auto attackClass = attackVftable->getAttackClass(attack);

    const auto& attackCategories = AttackClassCategories::get();

    if (attackClass->id == attackCategories.giveAttack->id) {
        // Do not allow to buff other units with this attack type
        return false;
    }

    auto secondAttack = soldierVftable->getSecondAttackById(soldier);
    if (!secondAttack) {
        return true;
    }

    auto secondAttackVftable = static_cast<const IAttackVftable*>(secondAttack->vftable);
    const auto secondAttackClass = secondAttackVftable->getAttackClass(secondAttack);
    // Do not allow to buff other units with this attack type as their second attack
    return secondAttackClass->id != attackCategories.giveAttack->id;
}

bool __fastcall shatterCanPerformHooked(game::CBatAttackShatter* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetStackId{};
    thisptr->vftable->getTargetStackId(thisptr, &targetStackId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID alliedStackId{};
    fn.getAllyOrEnemyStackId(&alliedStackId, battleMsgData, unitId, true);

    if (targetStackId != alliedStackId) {
        // Can't target allies
        return false;
    }

    auto& battle = BattleMsgDataApi::get();
    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::Retreat)) {
        // Can't target retreating units
        return false;
    }

    const int shatteredArmor = battle.getUnitShatteredArmor(battleMsgData, unitId);
    if (shatteredArmor >= userSettings().shatteredArmorMax) {
        return false;
    }

    CMidUnit* unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    auto soldierVftable = (const IUsSoldierVftable*)soldier->vftable;

    int unitArmor{};
    soldierVftable->getArmor(soldier, &unitArmor);

    const int fortArmor = battle.getUnitFortificationArmor(battleMsgData, unitId);
    const int reducedArmor = unitArmor - shatteredArmor;

    if (reducedArmor > fortArmor) {
        return reducedArmor > 0;
    }

    return false;
}

static game::UnitInfo* getUnitInfoById(game::BattleMsgData* battleMsgData,
                                       const game::CMidgardID* unitId)
{
    for (auto& info : battleMsgData->unitsInfo) {
        if (info.unitId1 == *unitId) {
            return &info;
        }
    }

    return nullptr;
}

void __fastcall setUnitShatteredArmorHooked(game::BattleMsgData* thisptr,
                                            int /*%edx*/,
                                            const game::CMidgardID* unitId,
                                            int armor)
{
    auto info = getUnitInfoById(thisptr, unitId);
    if (!info) {
        return;
    }

    info->shatteredArmor = std::clamp(info->shatteredArmor + armor, 0,
                                      userSettings().shatteredArmorMax);
}

} // namespace hooks
