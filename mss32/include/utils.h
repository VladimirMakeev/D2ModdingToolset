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

#ifndef UTILS_H
#define UTILS_H

#include "dynamiccast.h"
#include "midgardid.h"
#include "midscenvariables.h"
#include <filesystem>
#include <functional>
#include <string>

namespace game {
struct CMidMsgBoxButtonHandler;
struct IMidgardObjectMap;
struct UiEvent;
struct CInterface;
} // namespace game

namespace hooks {

std::string trimSpaces(const std::string& str);

/** Returns full path to the game folder. */
const std::filesystem::path& gameFolder();

/** Returns full path to the globals folder. */
const std::filesystem::path& globalsFolder();

/** Returns full path to the scripts folder. */
const std::filesystem::path& scriptsFolder();

/** Returns full path to the modifier scripts folder. */
const std::filesystem::path& modifiersFolder();

/** Returns full path to the scenario generator templates folder. */
const std::filesystem::path& templatesFolder();

/** Returns full path to the executable that is currently running. */
const std::filesystem::path& exePath();

/** Returns id as string. */
std::string idToString(const game::CMidgardID* id, bool lowercase = false);

/** Returns interface text by specified id string. */
std::string getInterfaceText(const char* textIdString);

/** Returns global text by specified id. */
const char* getGlobalText(const game::CMidgardID& textId);
const char* getGlobalText(const std::string& textIdString);

/** Tries to replace first occurence of 'keyword' in 'str' with 'replacement'. */
bool replace(std::string& str, const std::string& keyword, const std::string& replacement);

/** Returns random number from range [min : max]. */
int getRandomNumber(int min, int max);

std::string readFile(const std::filesystem::path& file);

/**
 * Shows file open dialog in specified directory and reads selected file.
 * @returns false in case of error or when selection is canceled by user.
 */
bool readUserSelectedFile(std::string& contents,
                          const char* filter,
                          const char* directory = nullptr);

void showInterface(game::CInterface* interf);
void hideInterface(game::CInterface* interf);

/** Shows native in-game message box with specified text. */
void showMessageBox(const std::string& message,
                    game::CMidMsgBoxButtonHandler* buttonHandler = nullptr,
                    bool showCancel = false);

/** Shows windows style message box that does not depend on game rendering and resources. */
void showErrorMessageBox(const std::string& message);

void createTimerEvent(game::UiEvent* timerEvent,
                      void* userData,
                      void* callback,
                      std::uint32_t timeoutMs);

/** Computes MD5 hash of files in specified folder. */
bool computeHash(const std::filesystem::path& folder, std::string& hash);

/** Executes function for each scenario object with specified id type. */
void forEachScenarioObject(game::IMidgardObjectMap* objectMap,
                           game::IdType idType,
                           const std::function<void(const game::IMidScenarioObject*)>& func);

template <typename T>
static inline void replaceRttiInfo(game::RttiInfo<T>& dst, const T* src, bool copyVftable = true)
{
    dst.locator = *reinterpret_cast<const game::CompleteObjectLocator**>(
        reinterpret_cast<std::uintptr_t>(src) - sizeof(game::CompleteObjectLocator*));

    if (copyVftable) {
        std::memcpy(&dst.vftable, src, sizeof(T));
    }
}

} // namespace hooks

#endif // UTILS_H
