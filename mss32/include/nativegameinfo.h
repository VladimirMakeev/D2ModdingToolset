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

#ifndef NATIVEGAMEINFO_H
#define NATIVEGAMEINFO_H

#include "gameinfo.h"

namespace hooks {

class NativeGameInfo final : public rsg::GameInfo
{
public:
    NativeGameInfo(const std::filesystem::path& gameFolderPath);

    ~NativeGameInfo() override = default;

    const rsg::UnitsInfo& getUnits() const override;
    const rsg::UnitInfoArray& getLeaders() const override;
    const rsg::UnitInfoArray& getSoldiers() const override;
    int getMinLeaderValue() const override;
    int getMaxLeaderValue() const override;
    int getMinSoldierValue() const override;
    int getMaxSoldierValue() const override;

    const rsg::ItemsInfo& getItemsInfo() const override;
    const rsg::ItemInfoArray& getItems() const override;
    const rsg::ItemInfoArray& getItems(rsg::ItemType itemType) const override;

    const rsg::SpellsInfo& getSpellsInfo() const override;
    const rsg::SpellInfoArray& getSpells() const override;
    const rsg::SpellInfoArray& getSpells(rsg::SpellType spellType) const override;

    const rsg::LandmarksInfo& getLandmarksInfo() const override;

    const rsg::LandmarkInfoArray& getLandmarks(rsg::LandmarkType landmarkType) const override;
    const rsg::LandmarkInfoArray& getLandmarks(rsg::RaceType raceType) const override;
    const rsg::LandmarkInfoArray& getMountainLandmarks() const override;

    const rsg::RacesInfo& getRacesInfo() const override;
    const rsg::RaceInfo& getRaceInfo(rsg::RaceType raceType) const override;

    const char* getGlobalText(const rsg::CMidgardID& textId) const override;
    const char* getEditorInterfaceText(const rsg::CMidgardID& textId) const override;

    const rsg::CityNames& getCityNames() const override;

    const rsg::SiteTexts& getMercenaryTexts() const override;
    const rsg::SiteTexts& getMageTexts() const override;
    const rsg::SiteTexts& getMerchantTexts() const override;
    const rsg::SiteTexts& getRuinTexts() const override;
    const rsg::SiteTexts& getTrainerTexts() const override;

private:
    bool readGameInfo(const std::filesystem::path& gameFolderPath);

    bool readUnitsInfo();
    bool readItemsInfo();
    bool readSpellsInfo();
    bool readLandmarksInfo();
    bool readRacesInfo();

    bool readEditorInterfaceTexts(const std::filesystem::path& interfFolderPath);

    bool readCityNames(const std::filesystem::path& scenDataFolderPath);
    bool readSiteTexts(const std::filesystem::path& scenDataFolderPath);

    rsg::UnitsInfo unitsInfo{};
    rsg::UnitInfoArray leaders{};
    rsg::UnitInfoArray soldiers{};

    int minLeaderValue{};
    int maxLeaderValue{};

    int minSoldierValue{};
    int maxSoldierValue{};

    rsg::ItemsInfo itemsInfo;
    rsg::ItemInfoArray allItems;
    std::map<rsg::ItemType, rsg::ItemInfoArray> itemsByType;

    rsg::SpellsInfo spellsInfo;
    rsg::SpellInfoArray allSpells;
    std::map<rsg::SpellType, rsg::SpellInfoArray> spellsByType;

    rsg::LandmarksInfo landmarksInfo;
    std::map<rsg::LandmarkType, rsg::LandmarkInfoArray> landmarksByType;
    std::map<rsg::RaceType, rsg::LandmarkInfoArray> landmarksByRace;
    rsg::LandmarkInfoArray mountainLandmarks;

    rsg::RacesInfo racesInfo;

    rsg::TextsInfo editorInterfaceTexts;

    rsg::CityNames cityNames;

    rsg::SiteTexts mercenaryTexts;
    rsg::SiteTexts mageTexts;
    rsg::SiteTexts merchantTexts;
    rsg::SiteTexts ruinTexts;
    rsg::SiteTexts trainerTexts;
};

} // namespace hooks

#endif // NATIVEGAMEINFO_H
