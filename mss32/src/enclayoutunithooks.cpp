/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "enclayoutunithooks.h"
#include "dialoginterf.h"
#include "enclayoutunit.h"
#include "encunitdescriptor.h"
#include "originalfunctions.h"
#include "settings.h"
#include "textboxinterf.h"
#include "unitutils.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

void __fastcall encLayoutUnitUpdateHooked(game::CEncLayoutUnit* thisptr, int /*%edx*/)
{
    using namespace game;

    getOriginalFunctions().encLayoutUnitUpdate(thisptr);

    static const char controlName[]{"TXT_EFFECTIVE_HP"};
    const auto& dialogApi{CDialogInterfApi::get()};

    if (!dialogApi.findControl(thisptr->dialog, controlName)) {
        return;
    }

    auto textBox{dialogApi.findTextBox(thisptr->dialog, controlName)};
    if (!textBox) {
        return;
    }

    std::string text{textBox->data->text.string};

    auto data{thisptr->data};
    auto objectMap{data->objectMap};

    auto unitDescriptor{data->unitDescriptor};
    auto unitArmor{unitDescriptor->vftable->getUnitArmor(unitDescriptor)};

    if (!unitDescriptor->vftable->method1(unitDescriptor)) {
        unitArmor += getCityProtection(objectMap, &data->unitId);
    }

    unitArmor = std::clamp(unitArmor, 0, userSettings().unitMaxArmor);

    auto findObject{objectMap->vftable->findScenarioObjectById};

    auto unit{(const CMidUnit*)findObject(objectMap, &data->unitId)};
    const auto effectiveHp{(std::uint32_t)computeUnitEffectiveHp(unit, unitArmor)};

    if (replace(text, "%HP%", fmt::format("{:d}", effectiveHp))) {
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }
}

} // namespace hooks
