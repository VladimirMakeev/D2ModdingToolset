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

#include "enclayoutruinhooks.h"
#include "dialoginterf.h"
#include "enclayoutruin.h"
#include "gameutils.h"
#include "interfaceutils.h"
#include "midruin.h"
#include "originalfunctions.h"
#include "textboxinterf.h"
#include "utils.h"
#include <string>

namespace hooks {

static std::string getXpKilledField(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidRuin* ruin)
{
    return getNumberText(getGroupXpKilled(objectMap, &ruin->group), false);
}

static void setTxtXpKilled(game::CDialogInterf* dialog,
                           const game::IMidgardObjectMap* objectMap,
                           const game::CMidRuin* ruin)
{
    if (!ruin) {
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

    if (replace(text, "%XPKILL%", getXpKilledField(objectMap, ruin))) {
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }
}

void __stdcall encLayoutRuinUpdateHooked(const game::CEncLayoutRuinData* data,
                                         const game::CMidRuin* ruin,
                                         game::CDialogInterf* dialog)
{
    getOriginalFunctions().encLayoutRuinUpdate(data, ruin, dialog);

    setTxtXpKilled(dialog, data->objectMap, ruin);
}

} // namespace hooks
