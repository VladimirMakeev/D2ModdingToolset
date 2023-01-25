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
#include "midgardmsgbox.h"
#include "midgardobjectmap.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midscenvariables.h"
#include "smartptr.h"
#include "uimanager.h"
#include <Windows.h>
#include <fmt/format.h>
#include <fstream>
#include <random>
#include <wincrypt.h>

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

const std::filesystem::path& globalsFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Globals"};
    return folder;
}

const std::filesystem::path& scriptsFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Scripts"};
    return folder;
}

const std::filesystem::path& modifiersFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Scripts" / "Modifiers"};
    return folder;
}

const std::filesystem::path& templatesFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Templates"};
    return folder;
}

const std::filesystem::path& exportsFolder()
{
    static const std::filesystem::path folder{gameFolder() / "Exports"};
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

std::string idToString(const game::CMidgardID* id, bool lowercase)
{
    char idString[11] = {0};
    game::CMidgardIDApi::get().toString(id, idString);

    if (lowercase) {
        for (auto& c : idString) {
            c = (char)std::tolower((unsigned char)c);
        }
    }

    return {idString};
}

std::string getInterfaceText(const char* textIdString)
{
    using namespace game;

    if (textIdString == nullptr || strlen(textIdString) == 0)
        return "";

    CMidgardID textId{};
    CMidgardIDApi::get().fromString(&textId, textIdString);

    return {gameFunctions().getInterfaceText(&textId)};
}

const char* getGlobalText(const game::CMidgardID& textId)
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    const auto texts = (*globalApi.getGlobalData())->texts;
    return globalApi.findTextById(texts, &textId);
}

const char* getGlobalText(const std::string& textIdString)
{
    using namespace game;

    CMidgardID textId{};
    CMidgardIDApi::get().fromString(&textId, textIdString.c_str());
    if (textId == invalidId)
        return "";

    return getGlobalText(textId);
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
    // Do not resize resulting string to match file size because ifstream replaces \r\n with \n.
    // Otherwise the string is padded with 0 chars and its size() returns incorrect value.
    // Resulting string_view causes failures in Lua functions without explicit call to c_str.
    // Either use binary mode to get exact contents or use iterator approach.
    // While iterators are slower than binary read, Lua transforms \r\n to \n by itself anyway.
    std::ifstream stream(file);
    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
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

void showInterface(game::CInterface* interf)
{
    using namespace game;

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->showInterface(ptr.data, interf);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);
}

void hideInterface(game::CInterface* interf)
{
    using namespace game;

    InterfManagerImplPtr ptr;
    CInterfManagerImplApi::get().get(&ptr);

    ptr.data->CInterfManagerImpl::CInterfManager::vftable->hideInterface(ptr.data, interf);
    SmartPointerApi::get().createOrFree((SmartPointer*)&ptr, nullptr);
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
    CMidgardMsgBoxApi::get().constructor(msgBox, message.c_str(), showCancel, buttonHandler,
                                         nullptr, nullptr);

    showInterface(msgBox);
}

void showErrorMessageBox(const std::string& message)
{
    logError("mssProxyError.log", message);
    MessageBox(NULL, message.c_str(), "mss32.dll proxy", MB_OK);
}

void createTimerEvent(game::UiEvent* timerEvent,
                      void* userData,
                      void* callback,
                      std::uint32_t timeoutMs)
{
    using namespace game;

    const auto freeFunctor = SmartPointerApi::get().createOrFreeNoDtor;
    const auto& uiManagerApi = CUIManagerApi::get();

    using TimerCallback = CUIManagerApi::Api::TimerEventCallback;

    TimerCallback timerCallback{};
    timerCallback.callback = (TimerCallback::Callback)callback;

    SmartPointer functor;
    uiManagerApi.createTimerEventFunctor(&functor, 0, userData, &timerCallback);

    UIManagerPtr uiManager;
    uiManagerApi.get(&uiManager);

    uiManagerApi.createTimerEvent(uiManager.data, timerEvent, &functor, timeoutMs);

    freeFunctor(&functor, nullptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&uiManager, nullptr);
}

std::uint32_t createMessageEvent(game::UiEvent* messageEvent,
                                 void* userData,
                                 void* callback,
                                 const char* messageName)
{
    using namespace game;

    const auto freeFunctor = SmartPointerApi::get().createOrFreeNoDtor;
    const auto& uiManagerApi = CUIManagerApi::get();

    using MessageCallback = CUIManagerApi::Api::MessageEventCallback;

    MessageCallback messageCallback = (MessageCallback)callback;

    SmartPointer functor;
    uiManagerApi.createMessageEventFunctor(&functor, 0, userData, &messageCallback);

    UIManagerPtr uiManager;
    uiManagerApi.get(&uiManager);

    std::uint32_t messageId = uiManagerApi.registerMessage(uiManager.data, messageName);
    uiManagerApi.createMessageEvent(uiManager.data, messageEvent, &functor, messageId);

    freeFunctor(&functor, nullptr);
    SmartPointerApi::get().createOrFree((SmartPointer*)&uiManager, nullptr);

    return messageId;
}

bool computeHash(const std::filesystem::path& folder, std::string& hash)
{
    struct HashGuard
    {
        HashGuard() = default;
        ~HashGuard()
        {
            CryptDestroyHash(hash);
            CryptReleaseContext(provider, 0);
        }

        HCRYPTPROV provider{};
        HCRYPTHASH hash{};
    };

    std::vector<std::filesystem::path> filenames;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        filenames.push_back(entry.path());
    }

    std::sort(filenames.begin(), filenames.end());

    HashGuard guard;
    if (!CryptAcquireContext(&guard.provider, nullptr, nullptr, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        logError("mssProxyError.log",
                 fmt::format("Could not acquire context, reason {:d}", GetLastError()));
        return false;
    }

    if (!CryptCreateHash(guard.provider, CALG_MD5, 0, 0, &guard.hash)) {
        logError("mssProxyError.log",
                 fmt::format("Could not create hash, reason {:d}", GetLastError()));
        return false;
    }

    for (const auto& file : filenames) {
        std::ifstream stream{file, std::ios_base::binary};
        if (!stream) {
            logError("mssProxyError.log",
                     fmt::format("Could not open file '{:s}'", file.filename().string()));
            return false;
        }

        const auto size = static_cast<size_t>(std::filesystem::file_size(file));
        std::vector<unsigned char> contents(size);

        stream.read(reinterpret_cast<char*>(contents.data()), size);
        stream.close();

        if (!CryptHashData(guard.hash, contents.data(), size, 0)) {
            logError("mssProxyError.log",
                     fmt::format("Compute hash failed, reason {:d}", GetLastError()));
            return false;
        }
    }

    constexpr size_t md5Length{16};
    DWORD length{md5Length};
    unsigned char md5Hash[md5Length] = {0};

    if (!CryptGetHashParam(guard.hash, HP_HASHVAL, md5Hash, &length, 0)) {
        logError("mssProxyError.log",
                 fmt::format("Could not get hash value, reason {:d}", GetLastError()));
        return false;
    }

    hash.clear();

    static const char hexDigits[] = "0123456789abcdef";
    for (DWORD i = 0; i < length; ++i) {
        hash += hexDigits[md5Hash[i] >> 4];
        hash += hexDigits[md5Hash[i] & 0xf];
    }

    return true;
}

void forEachScenarioObject(game::IMidgardObjectMap* objectMap,
                           game::IdType idType,
                           const std::function<void(const game::IMidScenarioObject*)>& func)
{
    using namespace game;

    IteratorPtr current{};
    objectMap->vftable->begin(objectMap, &current);

    IteratorPtr end{};
    objectMap->vftable->end(objectMap, &end);

    const auto& getType{CMidgardIDApi::get().getType};

    while (!current.data->vftable->end(current.data, end.data)) {
        const auto* id{current.data->vftable->getObjectId(current.data)};

        if (getType(id) == idType) {
            func(objectMap->vftable->findScenarioObjectById(objectMap, id));
        }

        current.data->vftable->advance(current.data);
    }

    const auto& free{SmartPointerApi::get().createOrFree};

    free((SmartPointer*)&current, nullptr);
    free((SmartPointer*)&end, nullptr);
}

} // namespace hooks
