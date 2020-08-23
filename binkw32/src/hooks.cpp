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
#include "d2string.h"
#include "game.h"
#include "log.h"
#include "settings.h"
#include <algorithm>
#include <cstring>
#include <fstream>
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

game::AutoDialogData* __fastcall LoadScriptFileHooked(game::AutoDialogData* thisptr,
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

    using ScriptLines = std::vector<std::string>;
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

    const std::string skirmishDialog{"DIALOG\tDLG_CHOOSE_SKIRMISH"};
    auto dialogLine = std::find_if(scriptLines.begin(), scriptLines.end(),
                                   [&skirmishDialog](const std::string& line) {
                                       return line.find(skirmishDialog) != std::string::npos;
                                   });

    if (dialogLine == scriptLines.end()) {
        logError("binkwProxy.log",
                 "Could not find 'DLG_CHOOSE_SKIRMISH' in AutoDialog script file");
        return thisptr;
    }

    const std::string endToken{"END"};
    auto endTokenLine = std::find_if(dialogLine, scriptLines.end(),
                                     [&endToken](const std::string& line) {
                                         return line.find(endToken) != std::string::npos;
                                     });

    if (endTokenLine == scriptLines.end()) {
        logError("binkwProxy.log",
                 "Could not find 'END' token for 'DLG_CHOOSE_SKIRMISH' in AutoDialog script file");
        return thisptr;
    }

    // As for now, it is easier to add new ui elements to AutoDialog script
    // and let the game do processing, than reverse-engineer all CDlgPrototype class hierarchy.
    // Use uncommon names to avoid collisions.
    ScriptLines newUiElements{
        "\tBUTTON\tBINKW_PROXY_BTN_GEN_MAP,212,122,422,142,,,,,\"\",0",
        "\tTEXT\tBINKW_PROXY_TXT_GEN_MAP,212,122,422,142,\\hC;\\vC;,\"Generate random map\",\"Open map generation menu\""};

    scriptLines.insert(endTokenLine, newUiElements.begin(), newUiElements.end());

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

} // namespace hooks
