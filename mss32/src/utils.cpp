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

#include "utils.h"
#include "game.h"
#include "interfmanager.h"
#include "log.h"
#include "mempool.h"
#include "midgardid.h"
#include "midgardmsgbox.h"
#include "midmsgboxbuttonhandlerstd.h"
#include <Windows.h>
#include <fstream>
#include <random>

namespace hooks {

std::string trimSpaces(const std::string& str)
{
    const auto begin = str.find_first_not_of(" ");
    if (begin == std::string::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" ");
    return str.substr(begin, end - begin + 1);
}

const std::filesystem::path& gameFolder()
{
    static std::filesystem::path folder{};

    if (folder.empty()) {
        folder = exePath();
        folder.remove_filename();
    }

    return folder;
}

const std::filesystem::path& scriptsFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Scripts"};
    return folder;
}

const std::filesystem::path& exePath()
{
    static std::filesystem::path exe{};

    if (exe.empty()) {
        HMODULE module = GetModuleHandle(NULL);
        std::string moduleName(MAX_PATH, '\0');
        GetModuleFileName(module, &moduleName[0], MAX_PATH - 1);

        exe = std::filesystem::path(moduleName);
    }

    return exe;
}

std::string idToString(const game::CMidgardID* id)
{
    char idString[11] = {0};
    game::CMidgardIDApi::get().toString(id, idString);

    return {idString};
}

std::string getTranslatedText(const char* textIdString)
{
    using namespace game;

    if (textIdString == nullptr || strlen(textIdString) == 0)
        return "";

    CMidgardID textId{};
    CMidgardIDApi::get().fromString(&textId, textIdString);

    return {gameFunctions().getInterfaceText(&textId)};
}

bool replace(std::string& str, const std::string& keyword, const std::string& replacement)
{
    const auto pos = str.find(keyword);
    if (pos == std::string::npos) {
        return false;
    }

    str.replace(pos, keyword.length(), replacement);
    return true;
}

int getRandomNumber(int min, int max)
{
    static std::random_device dev;
    static std::mt19937 rng{dev()};

    std::uniform_int_distribution<int> disribution(min, max);
    return disribution(rng);
}

std::string readFile(const std::filesystem::path& file)
{
    std::ifstream stream(file);
    if (!stream) {
        return {};
    }

    const auto size = static_cast<size_t>(std::filesystem::file_size(file));
    std::string contents;
    contents.resize(size);

    stream.read(&contents[0], size);
    return contents;
}

bool readUserSelectedFile(std::string& contents, const char* filter, const char* directory)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    char file[MAX_PATH];
    ZeroMemory(file, sizeof(file));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = file;
    ofn.lpstrFile[0] = 0;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = directory;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    ofn.hwndOwner = FindWindowEx(nullptr, nullptr, "MQ_UIManager", nullptr);

    if (!GetOpenFileName(&ofn)) {
        return false;
    }

    contents = readFile(std::filesystem::path{file});
    return true;
}

void showMessageBox(const std::string& message,
                    game::CMidMsgBoxButtonHandler* buttonHandler,
                    bool showCancel)
{
    using namespace game;

    auto memAlloc = Memory::get().allocate;
    if (!buttonHandler) {
        buttonHandler = (CMidMsgBoxButtonHandlerStd*)memAlloc(sizeof(CMidMsgBoxButtonHandlerStd));
        buttonHandler->vftable = CMidMsgBoxButtonHandlerStdApi::vftable();
    }

    CMidgardMsgBox* msgBox = (CMidgardMsgBox*)memAlloc(sizeof(CMidgardMsgBox));
    CMidgardMsgBoxApi::get().constructor(msgBox, message.c_str(), showCancel, buttonHandler, 0,
                                         nullptr);

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->showInterface(ptr.data, msgBox);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);
}

void showErrorMessageBox(const std::string& message)
{
    logError("mssProxyError.log", message);
    MessageBox(NULL, message.c_str(), "mss32.dll proxy", MB_OK);
}

} // namespace hooks
