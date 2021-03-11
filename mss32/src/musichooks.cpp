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

#include "musichooks.h"
#include "gamesettings.h"
#include "midgard.h"
#include "midmusic.h"
#include "racecategory.h"
#include "utils.h"
#include <algorithm>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

namespace hooks {

enum class CapitalsTracksIndex
{
    Empire,
    Hordes,
    Clans,
    Legions,
    Elves
};

using MusicTracksArray = std::vector<std::string>;

static MusicTracksArray findBattleTrackNames()
{
    namespace fs = std::filesystem;

    const auto settings = *game::CMidgardApi::get().instance()->data->settings;
    const fs::path dir{settings->musicFolder.string};

    MusicTracksArray trackNames;
    for (const fs::directory_entry& file : fs::directory_iterator(dir)) {
        if (!file.is_regular_file()) {
            continue;
        }

        auto filename{file.path().stem().string()};
        std::transform(filename.begin(), filename.end(), filename.begin(),
                       [](unsigned char ch) { return std::tolower(ch); });

        if (filename.find("battle") != filename.npos) {
            trackNames.push_back(filename);
        }
    }

    return trackNames;
}

static std::vector<MusicTracksArray> findCapitalsTracksNames()
{
    namespace fs = std::filesystem;

    const auto settings = *game::CMidgardApi::get().instance()->data->settings;
    const fs::path dir{settings->musicFolder.string};

    std::vector<MusicTracksArray> capitalsTracks(5);
    for (const fs::directory_entry& file : fs::directory_iterator(dir)) {
        if (!file.is_regular_file()) {
            continue;
        }

        auto filename{file.path().stem().string()};
        std::transform(filename.begin(), filename.end(), filename.begin(),
                       [](unsigned char ch) { return std::tolower(ch); });

        if (filename.find("humntrk") != filename.npos) {
            capitalsTracks[(int)CapitalsTracksIndex::Empire].push_back(filename);
        } else if (filename.find("undetrk") != filename.npos) {
            capitalsTracks[(int)CapitalsTracksIndex::Hordes].push_back(filename);
        } else if (filename.find("dwrftrk") != filename.npos) {
            capitalsTracks[(int)CapitalsTracksIndex::Clans].push_back(filename);
        } else if (filename.find("heretrk") != filename.npos) {
            capitalsTracks[(int)CapitalsTracksIndex::Legions].push_back(filename);
        } else if (filename.find("elftrk") != filename.npos) {
            capitalsTracks[(int)CapitalsTracksIndex::Elves].push_back(filename);
        }
    }

    return capitalsTracks;
}

void __fastcall playBattleTrackHooked(game::CMidMusic* thisptr, int /*%edx*/)
{
    using namespace game;

    static const MusicTracksArray battleTracks{findBattleTrackNames()};
    if (battleTracks.empty()) {
        return;
    }

    const auto tracksTotal = battleTracks.size();
    static size_t trackIndex{(size_t)getRandomNumber(0, tracksTotal)};
    trackIndex++;

    if (trackIndex >= tracksTotal) {
        trackIndex = 0;
    }

    CMidMusicApi::get().playFile(thisptr, thisptr->data->musicFolder,
                                 battleTracks[trackIndex].c_str(), 0, false);
}

void __fastcall playCapitalTrackHooked(game::CMidMusic* thisptr,
                                       int /*%edx*/,
                                       const game::LRaceCategory* raceCategory)
{
    using namespace game;

    static const std::vector<MusicTracksArray> capitalsTracks{findCapitalsTracksNames()};

    const auto& music = CMidMusicApi::get();
    music.freeStrings(thisptr);

    auto category = &thisptr->data->raceCategory;
    if (category != raceCategory) {
        category->table = raceCategory->table;
        category->id = raceCategory->id;
    }

    if (capitalsTracks.empty()) {
        return;
    }

    const auto& categories = RaceCategories::get();
    const auto id = raceCategory->id;

    CapitalsTracksIndex capitalsIndex{CapitalsTracksIndex::Empire};
    if (id == categories.human->id) {
        capitalsIndex = CapitalsTracksIndex::Empire;
    } else if (id == categories.dwarf->id) {
        capitalsIndex = CapitalsTracksIndex::Clans;
    } else if (id == categories.undead->id) {
        capitalsIndex = CapitalsTracksIndex::Hordes;
    } else if (id == categories.heretic->id) {
        capitalsIndex = CapitalsTracksIndex::Legions;
    } else if (id == categories.elf->id) {
        capitalsIndex = CapitalsTracksIndex::Elves;
    }

    const auto& tracks = capitalsTracks[(int)capitalsIndex];
    if (tracks.empty()) {
        return;
    }

    static size_t trackIndex{0};
    if (++trackIndex >= tracks.size()) {
        trackIndex = 0;
    }

    music.playFile(thisptr, thisptr->data->musicFolder, tracks[trackIndex].c_str(), 1, false);
}

} // namespace hooks
