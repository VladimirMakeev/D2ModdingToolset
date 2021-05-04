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

#include "menuracehooks.h"
#include "dialoginterf.h"
#include "game.h"
#include "log.h"
#include "menurace.h"
#include "pictureinterf.h"
#include "racecategory.h"
#include "racehooks.h"
#include "textboxinterf.h"
#include "utils.h"
#include <fmt/format.h>
#include <list>

namespace hooks {

static void menuRaceScroll(game::CMenuRace* menuRace, bool forward)
{
    using namespace game;

    const auto& menuApi = CMenuRaceApi::get();
    menuApi.updateButtons(menuRace);

    auto data = menuRace->menuRaceData;
    data->unknown4 = 1;

    const int racesTotal{5 + static_cast<int>(newRaces().size())};

    data->screenIndex += forward ? 1 : -1;
    data->screenIndex %= racesTotal;
    if (data->screenIndex < 0) {
        data->screenIndex = racesTotal - 1;
    }

    LRaceCategory raceCategory;
    getRaceCategoryHooked(&raceCategory, data->screenIndex);
    data->scrollBack = forward;

    const auto& findPicture = CDialogInterfApi::get().findPicture;

    auto dialog = CMenuBaseApi::get().getDialogInterface(menuRace);
    auto waterPicture = findPicture(dialog, "IMG_WATER");

    const auto& setImage = CPictureInterfApi::get().setImage;

    CMqPoint position{};
    setImage(waterPicture, nullptr, &position);

    auto fireflyPicture = findPicture(dialog, "IMG_FIREFLY");
    setImage(fireflyPicture, nullptr, &position);

    const auto& smartPtr = SmartPointerApi::get();
    SmartPtr<IMqImage2> animation{};
    gameFunctions().createMenuAnimation(&animation,
                                        menuApi.getTransitionAnimationName(&raceCategory,
                                                                           data->scrollBack));

    SmartPointer* animationPtr{reinterpret_cast<SmartPointer*>(&animation)};

    smartPtr.copy(reinterpret_cast<SmartPointer*>(&data->backgroundImage), animationPtr);
    smartPtr.createOrFree(animationPtr, nullptr);
}

void __fastcall menuRaceBtnNextCallbackHooked(game::CMenuRace* thisptr, int /*%edx*/)
{
    menuRaceScroll(thisptr, true);
}

void __fastcall menuRaceBtnPrevCallbackHooked(game::CMenuRace* thisptr, int /*%edx*/)
{
    menuRaceScroll(thisptr, false);
}

void __stdcall updateRaceDescriptionHooked(game::CDialogInterf* dialogInterf,
                                           const game::LRaceCategory* raceCategory)
{
    using namespace game;

    const auto& id = CMidgardIDApi::get();

    CMidgardID raceDescId{};
    if (!raceCategory) {
        id.fromString(&raceDescId, "x005ta0479");
    } else {
        const auto& races = RaceCategories::get();
        const auto raceId = raceCategory->id;
        const char* idString{};

        if (raceId == races.human->id) {
            idString = "x005ta0480";
        } else if (raceId == races.undead->id) {
            idString = "x005ta0481";
        } else if (raceId == races.dwarf->id) {
            idString = "x005ta0482";
        } else if (raceId == races.heretic->id) {
            idString = "x005ta0483";
        } else if (raceId == races.elf->id) {
            idString = "x160ta0003";
        } else {
            // TODO: read text id string from lua
            idString = "x160ta0515";
        }

        id.fromString(&raceDescId, idString);
    }

    CMidgardID formatStringId{};
    // \fMediuml%RACE%
    id.fromString(&formatStringId, "x005ta0484");

    std::string description{gameFunctions().getInterfaceText(&formatStringId)};
    replace(description, "%RACE%", {gameFunctions().getInterfaceText(&raceDescId)});

    auto textBox = CDialogInterfApi::get().findTextBox(dialogInterf, "TXT_RACE_DESC");
    CTextBoxInterfApi::get().setString(textBox, description.c_str());
}

game::LRaceCategory* __stdcall getRaceCategoryHooked(game::LRaceCategory* raceCategory,
                                                     int screenIndex)
{
    using namespace game;

    const auto& races = RaceCategories::get();

    raceCategory->vftable = RaceCategories::vftable();

    switch (screenIndex) {
    case 0:
        raceCategory->table = races.human->table;
        raceCategory->id = races.human->id;
        break;
    case 1:
        raceCategory->table = races.heretic->table;
        raceCategory->id = races.heretic->id;
        break;
    case 2:
        raceCategory->table = races.dwarf->table;
        raceCategory->id = races.dwarf->id;
        break;
    case 3:
        raceCategory->table = races.elf->table;
        raceCategory->id = races.elf->id;
        break;
    case 4:
        raceCategory->table = races.undead->table;
        raceCategory->id = races.undead->id;
        break;
    default: {
        // check screen index is not greater than number of new races
        const auto& racesNew = newRaces();
        if (screenIndex - 5 >= racesNew.size()) {
            raceCategory->table = races.human->table;
            raceCategory->id = races.human->id;
            break;
        }

        const auto& newRaceCategory = racesNew[screenIndex - 5].category;
        raceCategory->table = newRaceCategory.table;
        raceCategory->id = newRaceCategory.id;
        break;
    }
    }

    return raceCategory;
}

const char* __stdcall getRaceBgndImageNameHooked(const game::LRaceCategory* raceCategory)
{
    using namespace game;

    const auto& races = RaceCategories::get();
    const auto raceId{raceCategory->id};

    if (raceId == races.human->id) {
        return "DLG_CHOOSE_RACE_GODHU_BG";
    }

    if (raceId == races.dwarf->id) {
        return "DLG_CHOOSE_RACE_GODDW_BG";
    }

    if (raceId == races.heretic->id) {
        return "DLG_CHOOSE_RACE_GODHE_BG";
    }

    if (raceId == races.undead->id) {
        return "DLG_CHOOSE_RACE_GODUN_BG";
    }

    if (raceId == races.elf->id) {
        return "DLG_CHOOSE_RACE_GODEL_BG";
    }

    static std::vector<std::string> backgroundNames{};

    for (const auto& race : newRaces()) {

        if (raceId == race.category.id) {
            auto name{fmt::format("DLG_CHOOSE_RACE_GOD{:s}_BG", race.abbreviation)};

            auto it = std::find(backgroundNames.begin(), backgroundNames.end(), name);
            if (it == backgroundNames.end()) {
                backgroundNames.push_back(name);

                return backgroundNames.back().c_str();
            }

            return it->c_str();
        }
    }

    return nullptr;
}

const char* __stdcall getTransitionAnimationNameHooked(const game::LRaceCategory* raceCategory,
                                                       bool scrollBack)
{
    return nullptr;
}

game::RaceCategoryList* __stdcall getPlayableRacesHooked(game::RaceCategoryList* racesList)
{
    using namespace game;

    const auto& list = RaceCategoryListApi::get();
    const auto& races = RaceCategories::get();

    RaceCategoryList tmpList{};
    list.constructor(&tmpList);

    list.add(&tmpList, races.human);
    list.add(&tmpList, races.heretic);
    list.add(&tmpList, races.dwarf);
    list.add(&tmpList, races.undead);
    list.add(&tmpList, races.elf);

    for (auto& race : newRaces()) {
        list.add(&tmpList, &race.category);
    }

    list.copyConstructor(racesList, &tmpList);

    list.freeNodes(&tmpList);
    list.freeNode(&tmpList, tmpList.head);

    return racesList;
}

void __stdcall setRacesToSkipHooked(game::SortedRaceList* racesToSkip,
                                    const game::RaceCategoryList* scenarioRaces)
{
    using namespace game;

    const auto& list = SortedRaceListApi::get();
    list.clear(racesToSkip);

    auto& raceCategories = RaceCategories::get();

    std::list<LRaceCategory*> races;
    races.push_back(raceCategories.human);
    races.push_back(raceCategories.dwarf);
    races.push_back(raceCategories.heretic);
    races.push_back(raceCategories.undead);
    races.push_back(raceCategories.elf);

    for (auto& race : newRaces()) {
        races.push_back(&race.category);
    }

    for (auto node = scenarioRaces->head->next; node != scenarioRaces->head; node = node->next) {
        auto it = std::find_if(races.begin(), races.end(),
                               [id = node->data.id](const LRaceCategory* category) {
                                   return category->id == id;
                               });

        if (it != races.end()) {
            races.erase(it);
        }
    }

    SortedRaceListIterator iterator{};
    for (auto& race : races) {
        list.add(racesToSkip, &iterator, race);
    }
}

} // namespace hooks
