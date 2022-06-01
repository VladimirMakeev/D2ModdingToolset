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
#include "imageptrvector.h"
#include "mempool.h"
#include "midunitdescriptor.h"
#include "originalfunctions.h"
#include "settings.h"
#include "stringarray.h"
#include "textboxinterf.h"
#include "unittypedescriptor.h"
#include "unitutils.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

struct CEncLayoutUnitDataPatched : game::CEncLayoutUnitData
{
    game::Vector<game::SmartPtr<game::IMqImage2>> modifierIcons;
    game::Vector<game::String> modifierTexts;
};

game::IEncUnitDescriptor* createUnitDescriptor(const game::IMidgardObjectMap* objectMap,
                                               const game::CMidgardID* unitId,
                                               const game::CMidgardID* playerId)
{
    using namespace game;

    const auto& memAlloc = Memory::get().allocate;

    switch (CMidgardIDApi::get().getType(unitId)) {
    case IdType::Unit: {
        auto descriptor = (CMidUnitDescriptor*)memAlloc(sizeof(CMidUnitDescriptor));
        CMidUnitDescriptorApi::get().constructor(descriptor, objectMap, unitId, playerId);
        return descriptor;
    }
    case IdType::UnitGlobal:
    case IdType::UnitGenerated: {
        auto descriptor = (CUnitTypeDescriptor*)memAlloc(sizeof(CUnitTypeDescriptor));
        descriptor->vftable = CUnitTypeDescriptorApi::vftable();
        descriptor->unitImplId = *unitId;
        return descriptor;
    }
    }

    return nullptr;
}

game::CEncLayoutUnitData* createData(const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* unitId,
                                     game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& memAlloc = Memory::get().allocate;

    auto data = (CEncLayoutUnitDataPatched*)memAlloc(sizeof(CEncLayoutUnitDataPatched));
    CEncLayoutUnitApi::get().dataConstructor(data);

    data->unitId = *unitId;
    data->objectMap = objectMap;
    data->unknown5 = 0;
    data->unknown6 = 0;
    data->unitDescriptor = descriptor;

    return data;
}

game::CEncLayoutUnit* __fastcall encLayoutUnitCtorHooked(game::CEncLayoutUnit* thisptr,
                                                         int /*%edx*/,
                                                         const game::IMidgardObjectMap* objectMap,
                                                         game::CInterface* parent,
                                                         const game::CMqRect* rect,
                                                         const game::CMidgardID* unitId,
                                                         const game::CEncParamBase* encParam,
                                                         const game::CMidgardID* playerId)
{
    using namespace game;

    IEncLayoutApi::get().constructor(thisptr, parent, rect);

    auto descriptor = createUnitDescriptor(objectMap, unitId, playerId);

    thisptr->vftable = CEncLayoutUnitApi::vftable();
    thisptr->data = createData(objectMap, unitId, descriptor);

    parent->vftable->addChild(parent, thisptr);

    CEncLayoutUnitApi::get().initialize(thisptr, encParam);
    return thisptr;
}

game::CEncLayoutUnit* __fastcall encLayoutUnitCtor2Hooked(game::CEncLayoutUnit* thisptr,
                                                          int /*%edx*/,
                                                          game::IEncUnitDescriptor* descriptor,
                                                          game::CInterface* parent,
                                                          const game::CMqRect* rect,
                                                          const game::CEncParamBase* encParam)
{
    using namespace game;

    IEncLayoutApi::get().constructor(thisptr, parent, rect);

    thisptr->vftable = CEncLayoutUnitApi::vftable();
    thisptr->data = createData(nullptr, &emptyId, descriptor);

    parent->vftable->addChild(parent, thisptr);

    CEncLayoutUnitApi::get().initialize(thisptr, encParam);
    return thisptr;
}

game::CEncLayoutUnitData* __fastcall encLayoutUnitDataCtorHooked(game::CEncLayoutUnitData* thisptr,
                                                                 int /*%edx*/)
{
    using namespace game;

    thisptr->unitId = invalidId;

    thisptr->leaderAbilityIcons = {};
    ImagePtrVectorApi::get().reserve(&thisptr->leaderAbilityIcons, 1);

    thisptr->leaderAbilityTexts = {};
    StringArrayApi::get().reserve(&thisptr->leaderAbilityTexts, 1);

    thisptr->unitDescriptor = nullptr;

    auto patched = (CEncLayoutUnitDataPatched*)thisptr;
    patched->modifierIcons = {};
    ImagePtrVectorApi::get().reserve(&patched->modifierIcons, 1);

    patched->modifierTexts = {};
    StringArrayApi::get().reserve(&patched->modifierTexts, 1);

    return thisptr;
}

void __fastcall encLayoutUnitDataDtorHooked(game::CEncLayoutUnitData* thisptr, int /*%edx*/)
{
    using namespace game;

    auto descriptor = thisptr->unitDescriptor;
    if (descriptor) {
        descriptor->vftable->destructor(descriptor, true);
    }

    StringArrayApi::get().destructor(&thisptr->leaderAbilityTexts);
    ImagePtrVectorApi::get().destructor(&thisptr->leaderAbilityIcons);

    auto patched = (CEncLayoutUnitDataPatched*)thisptr;
    StringArrayApi::get().destructor(&patched->modifierTexts);
    ImagePtrVectorApi::get().destructor(&patched->modifierIcons);
}

void __fastcall encLayoutUnitInitializeHooked(game::CEncLayoutUnit* thisptr,
                                              const game::CEncParamBase* encParam)
{
    using namespace game;

    getOriginalFunctions().encLayoutUnitInitialize(thisptr, encParam);

    // TODO: implement listbox callback, create and assign functor

    // TODO: fill modifier icons and texts
}

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
