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

#include "nativegameinfo.h"
#include "attack.h"
#include "customattacks.h"
#include "dbfaccess.h"
#include "dbffile.h"
#include "game.h"
#include "generatorsettings.h"
#include "globaldata.h"
#include "itembase.h"
#include "itemcategory.h"
#include "itemtypelist.h"
#include "landmark.h"
#include "log.h"
#include "nativeiteminfo.h"
#include "nativelandmarkinfo.h"
#include "nativeraceinfo.h"
#include "nativespellinfo.h"
#include "nativeunitinfo.h"
#include "racetype.h"
#include "strategicspell.h"
#include "ussoldierimpl.h"
#include "usstackleader.h"
#include "usunitimpl.h"
#include "utils.h"
#include <cassert>
#include <fmt/format.h>

namespace hooks {

/** Converts game id to scenario generator id. */
static const rsg::CMidgardID& idToRsgId(const game::CMidgardID& id)
{
    // We can do this because their internal representations match
    return reinterpret_cast<const rsg::CMidgardID&>(id);
}

/** Converts scenario generator id to game id. */
static const game::CMidgardID& rsgIdToId(const rsg::CMidgardID& id)
{
    // We can do this because their internal representations match
    return reinterpret_cast<const game::CMidgardID&>(id);
}

static bool isVanillaReachId(game::AttackReachId id)
{
    return id == game::AttackReachId::All || id == game::AttackReachId::Any
           || id == game::AttackReachId::Adjacent;
}

/** Represents sum of currencies in a bank as scenario generator value */
static int bankToValue(const game::Bank& bank)
{
    return bank.gold + bank.deathMana + bank.lifeMana + bank.infernalMana + bank.runicMana
           + bank.groveMana;
}

/**
 * Translates specified string according to current game locale settings.
 * Only 'length' characters of a specified string will be translated.
 */
static std::string translate(const std::string_view& string, std::uint8_t length)
{
    // Translated string can be no longer than 'length' characters, add 1 for null terminator
    char buffer[std::numeric_limits<decltype(length)>::max() + 1] = {0};

    std::memcpy(buffer, string.data(), std::min(string.length(), std::size_t{length}));
    buffer[sizeof(buffer) - 1] = '\0';

    auto& oemToCharA{*game::gameFunctions().oemToCharA};

    oemToCharA(buffer, buffer);

    return trimSpaces(buffer);
}

static bool readSiteText(rsg::SiteTexts& texts,
                         const std::filesystem::path& dbFilename,
                         bool readDescriptions = true)
{
    texts.clear();

    utils::DbfFile db;
    if (!db.open(dbFilename)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbFilename.filename().string()));
        return false;
    }

    const utils::DbfColumn* nameColumn{db.column("NAME")};
    if (!nameColumn) {
        return false;
    }

    const utils::DbfColumn* descColumn{db.column("DESC")};
    if (!descColumn) {
        return false;
    }

    const std::uint8_t nameLength{nameColumn->length};
    const std::uint8_t descriptionLength{descColumn->length};
    const std::uint32_t recordsTotal{db.recordsTotal()};

    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!db.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbFilename.filename().string()));
            continue;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string name;
        if (!record.value(name, "NAME")) {
            continue;
        }

        rsg::SiteText text;
        text.name = translate(name, nameLength);

        if (readDescriptions) {
            std::string description;
            if (record.value(description, "DESC")) {
                text.description = translate(description, descriptionLength);
            }
        }

        texts.emplace_back(std::move(text));
    }

    return true;
}

NativeGameInfo::NativeGameInfo(const std::filesystem::path& gameFolderPath)
{
    if (!readGameInfo(gameFolderPath)) {
        throw std::runtime_error("Could not read game info");
    }
}

const rsg::UnitsInfo& NativeGameInfo::getUnits() const
{
    return unitsInfo;
}

const rsg::UnitInfoArray& NativeGameInfo::getLeaders() const
{
    return leaders;
}

const rsg::UnitInfoArray& NativeGameInfo::getSoldiers() const
{
    return soldiers;
}

int NativeGameInfo::getMinLeaderValue() const
{
    return minLeaderValue;
}

int NativeGameInfo::getMaxLeaderValue() const
{
    return maxLeaderValue;
}

int NativeGameInfo::getMinSoldierValue() const
{
    return minSoldierValue;
}

int NativeGameInfo::getMaxSoldierValue() const
{
    return maxSoldierValue;
}

const rsg::ItemsInfo& NativeGameInfo::getItemsInfo() const
{
    return itemsInfo;
}

const rsg::ItemInfoArray& NativeGameInfo::getItems() const
{
    return allItems;
}

const rsg::ItemInfoArray& NativeGameInfo::getItems(rsg::ItemType itemType) const
{
    const auto it{itemsByType.find(itemType)};
    if (it == itemsByType.end()) {
        throw std::runtime_error("Could not find items by type");
    }

    return it->second;
}

const rsg::SpellsInfo& NativeGameInfo::getSpellsInfo() const
{
    return spellsInfo;
}

const rsg::SpellInfoArray& NativeGameInfo::getSpells() const
{
    return allSpells;
}

const rsg::SpellInfoArray& NativeGameInfo::getSpells(rsg::SpellType spellType) const
{
    const auto it{spellsByType.find(spellType)};
    if (it == spellsByType.end()) {
        throw std::runtime_error("Could not find spells by type");
    }

    return it->second;
}

const rsg::LandmarksInfo& NativeGameInfo::getLandmarksInfo() const
{
    return landmarksInfo;
}

const rsg::LandmarkInfoArray& NativeGameInfo::getLandmarks(rsg::LandmarkType landmarkType) const
{
    const auto it{landmarksByType.find(landmarkType)};
    if (it == landmarksByType.end()) {
        throw std::runtime_error("Could not find landmarks by type");
    }

    return it->second;
}

const rsg::LandmarkInfoArray& NativeGameInfo::getLandmarks(rsg::RaceType raceType) const
{
    const auto it{landmarksByRace.find(raceType)};
    if (it == landmarksByRace.end()) {
        throw std::runtime_error("Could not find landmarks by race");
    }

    return it->second;
}

const rsg::LandmarkInfoArray& NativeGameInfo::getMountainLandmarks() const
{
    return mountainLandmarks;
}

const rsg::RacesInfo& NativeGameInfo::getRacesInfo() const
{
    return racesInfo;
}

const rsg::RaceInfo& NativeGameInfo::getRaceInfo(rsg::RaceType raceType) const
{
    for (const auto& pair : racesInfo) {
        if (pair.second->getRaceType() == raceType) {
            return *pair.second.get();
        }
    }

    assert(false);
    throw std::runtime_error("Could not find race by type");
}

const char* NativeGameInfo::getGlobalText(const rsg::CMidgardID& textId) const
{
    return hooks::getGlobalText(rsgIdToId(textId));
}

const char* NativeGameInfo::getEditorInterfaceText(const rsg::CMidgardID& textId) const
{
    const auto it{editorInterfaceTexts.find(textId)};
    if (it == editorInterfaceTexts.end()) {
        // Return a string that is easy to spot in the game/editor.
        // This should help tracking potential problem
        return "NOT FOUND";
    }

    // This is fine because we don't change texts after loading
    // and GameInfo lives longer than scenario generator
    return it->second.c_str();
}

const rsg::CityNames& NativeGameInfo::getCityNames() const
{
    return cityNames;
}

const rsg::SiteTexts& NativeGameInfo::getMercenaryTexts() const
{
    return mercenaryTexts;
}

const rsg::SiteTexts& NativeGameInfo::getMageTexts() const
{
    return mageTexts;
}

const rsg::SiteTexts& NativeGameInfo::getMerchantTexts() const
{
    return merchantTexts;
}

const rsg::SiteTexts& NativeGameInfo::getRuinTexts() const
{
    return ruinTexts;
}

const rsg::SiteTexts& NativeGameInfo::getTrainerTexts() const
{
    return trainerTexts;
}

bool NativeGameInfo::readGameInfo(const std::filesystem::path& gameFolderPath)
{
    // Some parts of the data nedded by scenario generator is not loaded by game
    // Load and process it manually
    const std::filesystem::path scenDataFolder{gameFolderPath / "ScenData"};
    const std::filesystem::path interfDataFolder{gameFolderPath / "Interf"};

    return rsg::readGeneratorSettings(gameFolderPath) && readRacesInfo() && readUnitsInfo()
           && readItemsInfo() && readSpellsInfo() && readLandmarksInfo()
           && readEditorInterfaceTexts(interfDataFolder) && readCityNames(scenDataFolder)
           && readSiteTexts(scenDataFolder);
}

bool NativeGameInfo::readUnitsInfo()
{
    unitsInfo.clear();
    leaders.clear();
    soldiers.clear();

    minLeaderValue = std::numeric_limits<int>::max();
    maxLeaderValue = std::numeric_limits<int>::min();

    minSoldierValue = std::numeric_limits<int>::max();
    maxSoldierValue = std::numeric_limits<int>::min();

    using namespace game;

    const auto& fn{gameFunctions()};
    const auto& idApi{CMidgardIDApi::get()};

    const auto& globalApi{GlobalDataApi::get()};
    const GlobalData* global{*globalApi.getGlobalData()};
    const auto& units{global->units->map->data};

    for (const auto* i = units.bgn; i != units.end; ++i) {
        const TUsUnitImpl* unitImpl{i->second};

        // Ignore generated units
        if (idApi.getType(&unitImpl->id) == IdType::UnitGenerated) {
            continue;
        }

        // Check unit is a soldier
        const TUsSoldierImpl* soldier{(const TUsSoldierImpl*)fn.castUnitImplToSoldier(unitImpl)};
        if (!soldier) {
            continue;
        }

        // Skip water-only units until generator supports water zones
        if (soldier->vftable->getWaterOnly(soldier)) {
            continue;
        }

        auto& unitId{idToRsgId(unitImpl->id)};
        auto& raceId{idToRsgId(*soldier->vftable->getRaceId(soldier))};
        auto& nameId{idToRsgId(soldier->data->name.id)};

        int level{soldier->vftable->getLevel(soldier)};
        int value{soldier->vftable->getXpKilled(soldier)};

        rsg::SubRaceType subrace{(rsg::SubRaceType)soldier->vftable->getSubrace(soldier)->id};

        const IAttack* attack{soldier->vftable->getAttackById(soldier)};
        const AttackReachId reachId{attack->vftable->getAttackReach(attack)->id};

        rsg::ReachType reach;

        if (isVanillaReachId(reachId)) {
            reach = (rsg::ReachType)reachId;
        } else {
            // Map custom reaches to vanilla ones (Any, All or Adjacent)
            // depending on 'melee' hint and max targets count.
            // We don't care about their actual logic
            const auto& customReaches{getCustomAttacks().reaches};

            auto it{std::find_if(customReaches.begin(), customReaches.end(),
                                 [reachId](const CustomAttackReach& customReach) {
                                     return customReach.reach.id == reachId;
                                 })};
            if (it == customReaches.end()) {
                // This should never happen
                continue;
            }

            if (it->melee) {
                // Melle custom reaches become 'Adjacent'
                reach = rsg::ReachType::Adjacent;
            } else {
                // Non-melee custom reaches with 6 max targets becomes 'All',
                // others are 'Any'
                reach = it->maxTargets == 6 ? rsg::ReachType::All : rsg::ReachType::Any;
            }
        }

        rsg::AttackType attackType{(rsg::AttackType)attack->vftable->getAttackClass(attack)->id};

        int hp{soldier->vftable->getHitPoints(soldier)};

        rsg::UnitType unitType{(rsg::UnitType)unitImpl->category.id};
        int move{0};
        int leadership{0};

        // Get leader specifics
        if (unitType == rsg::UnitType::Leader) {
            const IUsStackLeader* stackLeader{fn.castUnitImplToStackLeader(unitImpl)};

            move = stackLeader->vftable->getMovement(stackLeader);
            leadership = stackLeader->vftable->getLeadership(stackLeader);
        }

        bool big{!soldier->vftable->getSizeSmall(soldier)};
        bool male{soldier->vftable->getSexM(soldier)};

        auto unitInfo{std::make_unique<NativeUnitInfo>(unitId, raceId, nameId, level, value,
                                                       unitType, subrace, reach, attackType, hp,
                                                       move, leadership, big, male)};

        if (unitType == rsg::UnitType::Leader) {
            leaders.push_back(unitInfo.get());

            if (value < minLeaderValue) {
                minLeaderValue = value;
            }

            if (value > maxLeaderValue) {
                maxLeaderValue = value;
            }
        } else if (unitType == rsg::UnitType::Soldier) {
            soldiers.push_back(unitInfo.get());

            if (value < minSoldierValue) {
                minSoldierValue = value;
            }

            if (value > maxSoldierValue) {
                maxSoldierValue = value;
            }
        }

        unitsInfo[unitId] = std::move(unitInfo);
    }

    return true;
}

bool NativeGameInfo::readItemsInfo()
{
    itemsInfo.clear();
    allItems.clear();
    itemsByType.clear();

    using namespace game;

    const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
    const auto& items{global->itemTypes->data->data};

    for (const auto* i = items.bgn; i != items.end; ++i) {
        const CItemBase* item{i->second};

        auto& itemId{idToRsgId(item->itemId)};
        rsg::ItemType itemType{(rsg::ItemType)item->vftable->getCategory(item)->id};

        const Bank* bank{item->vftable->getValue(item)};
        int value{bankToValue(*bank)};

        if (itemType == rsg::ItemType::Talisman) {
            // Talisman value is specified for a single charge, adjust it
            value *= 5;
        }

        auto itemInfo{std::make_unique<NativeItemInfo>(itemId, value, itemType)};

        allItems.push_back(itemInfo.get());
        itemsByType[itemType].push_back(itemInfo.get());
        itemsInfo[itemId] = std::move(itemInfo);
    }

    return true;
}

bool NativeGameInfo::readSpellsInfo()
{
    spellsInfo.clear();
    allSpells.clear();
    spellsByType.clear();

    using namespace game;

    const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
    const auto& spells{(*global->spells)->data};

    for (const auto* i = spells.bgn; i != spells.end; ++i) {
        const TStrategicSpell* spell{i->second};

        auto& spellId{idToRsgId(spell->id)};

        // Use buy cost as a spell value
        int value{bankToValue(spell->data->buyCost)};

        int level{spell->data->level};
        rsg::SpellType spellType{(rsg::SpellType)spell->data->spellCategory.id};

        auto spellInfo{std::make_unique<NativeSpellInfo>(spellId, value, level, spellType)};

        allSpells.push_back(spellInfo.get());
        spellsByType[spellType].push_back(spellInfo.get());
        spellsInfo[spellId] = std::move(spellInfo);
    }

    return true;
}

bool NativeGameInfo::readLandmarksInfo()
{
    landmarksInfo.clear();
    landmarksByType.clear();
    landmarksByRace.clear();
    mountainLandmarks.clear();

    using namespace game;

    const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
    const auto& landmarks{(*global->landmarks)->data};

    for (const auto* i = landmarks.bgn; i != landmarks.end; ++i) {
        const TLandmark* landmark{i->second};

        auto& landmarkId{idToRsgId(landmark->id)};
        rsg::Position size{landmark->data->size.x, landmark->data->size.y};
        rsg::LandmarkType landmarkType{(rsg::LandmarkType)landmark->data->category.id};
        bool mountain{landmark->data->mountain};

        auto info{std::make_unique<NativeLandmarkInfo>(landmarkId, size, landmarkType, mountain)};

        if (isEmpireLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Human].push_back(info.get());
        }

        if (isClansLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Dwarf].push_back(info.get());
        }

        if (isUndeadLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Undead].push_back(info.get());
        }

        if (isLegionsLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Heretic].push_back(info.get());
        }

        if (isElvesLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Elf].push_back(info.get());
        }

        if (isNeutralLandmark(landmarkId)) {
            landmarksByRace[rsg::RaceType::Neutral].push_back(info.get());
        }

        if (isMountainLandmark(landmarkId)) {
            mountainLandmarks.push_back(info.get());
        }

        landmarksByType[landmarkType].push_back(info.get());
        landmarksInfo[landmarkId] = std::move(info);
    }

    return true;
}

bool NativeGameInfo::readRacesInfo()
{
    racesInfo.clear();

    using namespace game;

    const GlobalData* global{*GlobalDataApi::get().getGlobalData()};
    const auto& races{(*global->races)->data};

    for (decltype(races.bgn) i = races.bgn; i != races.end; ++i) {
        const TRaceType* race{i->second};
        const auto data{race->data};

        auto& raceId{idToRsgId(race->id)};
        auto& guardianId{idToRsgId(data->guardianUnitId)};
        auto& nobleId{idToRsgId(data->nobleUnitId)};
        auto raceType{static_cast<rsg::RaceType>(data->raceType.id)};

        const auto& leaderNames{data->leaderNames->names};
        rsg::LeaderNames names;

        for (const LeaderName* j = leaderNames.bgn; j != leaderNames.end; ++j) {
            if (j->male) {
                names.maleNames.emplace_back(std::string(j->name));
            } else {
                names.femaleNames.emplace_back(std::string(j->name));
            }
        }

        std::vector<rsg::CMidgardID> leaderIds;
        for (const CMidgardID* id = data->leaders.bgn; id != data->leaders.end; ++id) {
            leaderIds.emplace_back(idToRsgId(*id));
        }

        racesInfo[raceId] = std::make_unique<NativeRaceInfo>(raceId, guardianId, nobleId, raceType,
                                                             std::move(names),
                                                             std::move(leaderIds));
    }

    return true;
}

bool NativeGameInfo::readEditorInterfaceTexts(const std::filesystem::path& interfFolderPath)
{
    editorInterfaceTexts.clear();

    const auto dbFilePath{interfFolderPath / "TAppEdit.dbf"};

    utils::DbfFile db;
    if (!db.open(dbFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbFilePath.filename().string()));
        return false;
    }

    const utils::DbfColumn* textColumn{db.column("TEXT")};
    if (!textColumn) {
        logError("mssProxyError.log",
                 fmt::format("Missing 'TEXT' column in {:s}", dbFilePath.filename().string()));
        return false;
    }

    const std::uint8_t textLength{textColumn->length};
    const std::uint32_t recordsTotal{db.recordsTotal()};

    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!db.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbFilePath.filename().string()));
            continue;
        }

        if (record.isDeleted()) {
            continue;
        }

        game::CMidgardID textId;
        if (!utils::dbRead(textId, db, i, "TXT_ID")) {
            continue;
        }

        std::string text;
        if (!record.value(text, "TEXT")) {
            continue;
        }

        editorInterfaceTexts[idToRsgId(textId)] = translate(text, textLength);
    }

    return true;
}

bool NativeGameInfo::readCityNames(const std::filesystem::path& scenDataFolderPath)
{
    cityNames.clear();

    const auto dbFilePath{scenDataFolderPath / "Cityname.dbf"};

    utils::DbfFile db;
    if (!db.open(dbFilePath)) {
        logError("mssProxyError.log",
                 fmt::format("Could not open {:s}", dbFilePath.filename().string()));
        return false;
    }

    const utils::DbfColumn* nameColumn{db.column("NAME")};
    if (!nameColumn) {
        logError("mssProxyError.log",
                 fmt::format("Missing 'NAME' column in {:s}", dbFilePath.filename().string()));
        return false;
    }

    const std::uint8_t textLength{nameColumn->length};
    const std::uint32_t recordsTotal{db.recordsTotal()};

    for (std::uint32_t i = 0; i < recordsTotal; ++i) {
        utils::DbfRecord record;
        if (!db.record(record, i)) {
            logError("mssProxyError.log", fmt::format("Could not read record {:d} from {:s}", i,
                                                      dbFilePath.filename().string()));
            continue;
        }

        if (record.isDeleted()) {
            continue;
        }

        std::string name{};
        if (!record.value(name, "NAME")) {
            continue;
        }

        cityNames.push_back(translate(name, textLength));
    }

    return true;
}

bool NativeGameInfo::readSiteTexts(const std::filesystem::path& scenDataFolderPath)
{
    return readSiteText(mercenaryTexts, scenDataFolderPath / "Campname.dbf")
           && readSiteText(mageTexts, scenDataFolderPath / "Magename.dbf")
           && readSiteText(merchantTexts, scenDataFolderPath / "Mercname.dbf")
           && readSiteText(ruinTexts, scenDataFolderPath / "Ruinname.dbf", false)
           && readSiteText(trainerTexts, scenDataFolderPath / "Trainame.dbf");
}

} // namespace hooks
