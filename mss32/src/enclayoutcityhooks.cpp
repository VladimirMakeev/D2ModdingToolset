/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#include "enclayoutcityhooks.h"
#include "dialoginterf.h"
#include "fortification.h"
#include "gameutils.h"
#include "interfaceutils.h"
#include "midstack.h"
#include "originalfunctions.h"
#include "textboxinterf.h"
#include "utils.h"
#include <string>

namespace hooks {

static std::string getXpKilledField(const game::IMidgardObjectMap* objectMap,
                                    const game::CFortification* fort)
{
    return getNumberText(getGroupXpKilled(objectMap, &fort->group), false);
}

static std::string getXpKilledStackField(const game::IMidgardObjectMap* objectMap,
                                         const game::CFortification* fort)
{
    using namespace game;

    if (fort->stackId == emptyId) {
        return getNumberText(0, false);
    }

    auto stack = getStack(objectMap, &fort->stackId);

    return getNumberText(getGroupXpKilled(objectMap, &stack->group), false);
}

static void setTxtXpKilled(game::CDialogInterf* dialog,
                           const game::IMidgardObjectMap* objectMap,
                           const game::CFortification* fort)
{
    if (!fort) {
        return;
    }

    using namespace game;

    static const char controlName[]{"TXT_XP_KILLED"};
    const auto& dialogApi{CDialogInterfApi::get()};

    if (!dialogApi.findControl(dialog, controlName)) {
        return;
    }

    auto textBox{dialogApi.findTextBox(dialog, controlName)};
    if (!textBox) {
        return;
    }

    std::string text{textBox->data->text.string};

    if (replace(text, "%XPKILL%", getXpKilledField(objectMap, fort))) {
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }
}

static void setTxtXpKilledStack(game::CDialogInterf* dialog,
                                const game::IMidgardObjectMap* objectMap,
                                const game::CFortification* fort)
{
    if (!fort) {
        return;
    }

    using namespace game;

    static const char controlName[]{"TXT_XP_KILLED_STACK"};
    const auto& dialogApi{CDialogInterfApi::get()};

    if (!dialogApi.findControl(dialog, controlName)) {
        return;
    }

    auto textBox{dialogApi.findTextBox(dialog, controlName)};
    if (!textBox) {
        return;
    }

    std::string text{textBox->data->text.string};

    if (replace(text, "%XPKILL%", getXpKilledStackField(objectMap, fort))) {
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }
}

void __fastcall encLayoutCityUpdateHooked(game::CEncLayoutCity* thisptr,
                                          int /*%edx*/,
                                          const game::IMidgardObjectMap* objectMap,
                                          const game::CFortification* fort,
                                          game::CDialogInterf* dialog)
{
    getOriginalFunctions().encLayoutCityUpdate(thisptr, objectMap, fort, dialog);

    setTxtXpKilled(dialog, objectMap, fort);
    setTxtXpKilledStack(dialog, objectMap, fort);
}

} // namespace hooks
