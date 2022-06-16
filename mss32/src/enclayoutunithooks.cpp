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
#include "custommodifier.h"
#include "custommodifiers.h"
#include "dialoginterf.h"
#include "enclayoutunit.h"
#include "encunitdescriptor.h"
#include "gameimages.h"
#include "imageptrvector.h"
#include "listbox.h"
#include "mempool.h"
#include "midunitdescriptor.h"
#include "modifierutils.h"
#include "originalfunctions.h"
#include "settings.h"
#include "stringarray.h"
#include "textboxinterf.h"
#include "textids.h"
#include "unitmodifier.h"
#include "unittypedescriptor.h"
#include "unitutils.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

static const char modifiersTextBoxName[] = "TXT_MODIFIERS";
static const char modifiersListBoxName[] = "LBOX_MODIFIERS";

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

void addModifierInfo(game::CEncLayoutUnit* layout, hooks::CCustomModifier* modifier, bool native)
{
    using namespace game;

    const auto& stringApi = game::StringApi::get();
    const auto& smartPtrApi = SmartPointerApi::get();

    auto data = (CEncLayoutUnitDataPatched*)layout->data;

    IMqImage2* icon = nullptr;
    GameImagesApi::get().getIconImageByName(&icon, modifier->getIconName().c_str());

    ImagePtr iconPtr{};
    smartPtrApi.createOrFree((SmartPointer*)&iconPtr, icon);
    ImagePtrVectorApi::get().pushBack(&data->modifierIcons, &iconPtr);
    smartPtrApi.createOrFree((SmartPointer*)&iconPtr, nullptr);

    std::string format;
    if (native) {
        format = getInterfaceText(textIds().interf.nativeModifierDescription.c_str());
        if (format.empty())
            format = "\\vC;\\fMedBold;%DESC%\\fNormal;";
    } else {
        format = getInterfaceText(textIds().interf.modifierDescription.c_str());
        if (format.empty())
            format = "\\vC;%DESC%";
    }
    replace(format, "%DESC%", getGlobalText(modifier->getDescTxt()));

    String text{};
    stringApi.initFromString(&text, format.c_str());
    StringArrayApi::get().pushBack(&data->modifierTexts, &text);
    stringApi.free(&text);
}

void addModifiersInfo(game::CEncLayoutUnit* layout)
{
    using namespace game;

    int count = 0;
    if (CMidgardIDApi::get().getType(&layout->data->unitId) == IdType::Unit) {
        const auto unit = gameFunctions().findUnitById(layout->data->objectMap,
                                                       &layout->data->unitId);
        if (!unit)
            return;

        auto nativeModifiers = getNativeModifiers(unit->unitImpl->id);

        for (auto curr = getFirstUmModifier(unit->unitImpl); curr; curr = curr->data->next) {
            auto customModifier = castModifierToCustomModifier(curr);
            if (!customModifier || !customModifier->getDisplay()) {
                continue;
            }

            auto it = std::find(nativeModifiers.begin(), nativeModifiers.end(),
                                curr->data->modifierId);

            addModifierInfo(layout, customModifier, it != nativeModifiers.end());
            ++count;
        }
    } else {
        for (const auto& modifierId : getNativeModifiers(layout->data->unitId)) {
            const auto unitModifier = getUnitModifier(&modifierId);
            if (!unitModifier) {
                continue;
            }

            auto customModifier = castModifierToCustomModifier(unitModifier->data->modifier);
            if (!customModifier || !customModifier->getDisplay()) {
                continue;
            }

            addModifierInfo(layout, customModifier, true);
            ++count;
        }
    }

    if (count == 0)
        count = 1; // To display "None" text

    auto listBox = CDialogInterfApi::get().findListBox(layout->dialog, modifiersListBoxName);
    CListBoxInterfApi::get().setElementsTotal(listBox, count);
}

void __fastcall modifiersListBoxDisplayCallback(const game::CEncLayoutUnit* thisptr,
                                                int /*%edx*/,
                                                game::ImagePointList* contents,
                                                const game::CMqRect* lineArea,
                                                unsigned int index,
                                                bool selected)
{
    using namespace game;

    const auto& imagePointListApi = ImagePointListApi::get();

    if (index < 0)
        return;

    auto data = (CEncLayoutUnitDataPatched*)thisptr->data;

    CMqRect textClientArea = *lineArea;
    textClientArea.p2.x -= textClientArea.p1.x;
    textClientArea.p2.y -= textClientArea.p1.y;
    textClientArea.p1.x = 31; // Width of modifier icon
    textClientArea.p1.y = 0;

    auto length = data->modifierTexts.end - data->modifierTexts.bgn;
    if (length == 0 && index == 0) {
        auto text = getInterfaceText(textIds().interf.modifiersEmpty.c_str());
        if (text.empty())
            text = getInterfaceText("X005TA0676");

        textClientArea.p1.x = 0;
        imagePointListApi.addText(contents, lineArea, text.c_str(), &textClientArea, false, 0);
        return;
    }

    if ((int)index >= length)
        return;

    std::string text = data->modifierTexts.bgn[index].string;
    auto& icon = data->modifierIcons.bgn[index];
    if (icon.data) {
        CMqPoint iconClientPos{0, 0};
        imagePointListApi.addImageWithText(contents, lineArea, &icon, &iconClientPos, text.c_str(),
                                           &textClientArea, false, 0);
    } else {
        imagePointListApi.addText(contents, lineArea, text.c_str(), &textClientArea, false, 0);
    }
}

game::CEncLayoutUnit* __fastcall encLayoutUnitCtorHooked(game::CEncLayoutUnit* thisptr,
                                                         int /*%edx*/,
                                                         const game::IMidgardObjectMap* objectMap,
                                                         game::CInterface* parent,
                                                         const game::CMqRect* area,
                                                         const game::CMidgardID* unitId,
                                                         const game::CEncParamBase* encParam,
                                                         const game::CMidgardID* playerId)
{
    using namespace game;

    IEncLayoutApi::get().constructor(thisptr, parent, area);

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
                                                          const game::CMqRect* area,
                                                          const game::CEncParamBase* encParam)
{
    using namespace game;

    IEncLayoutApi::get().constructor(thisptr, parent, area);

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

    const auto& imagePtrVectorApi = ImagePtrVectorApi::get();
    const auto& stringArrayApi = StringArrayApi::get();

    thisptr->unitId = invalidId;

    thisptr->leaderAbilityIcons = {};
    imagePtrVectorApi.reserve(&thisptr->leaderAbilityIcons, 1);

    thisptr->leaderAbilityTexts = {};
    stringArrayApi.reserve(&thisptr->leaderAbilityTexts, 1);

    thisptr->unitDescriptor = nullptr;

    auto patched = (CEncLayoutUnitDataPatched*)thisptr;
    patched->modifierIcons = {};
    imagePtrVectorApi.reserve(&patched->modifierIcons, 1);

    patched->modifierTexts = {};
    stringArrayApi.reserve(&patched->modifierTexts, 1);

    return thisptr;
}

void __fastcall encLayoutUnitDataDtorHooked(game::CEncLayoutUnitData* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto& imagePtrVectorApi = ImagePtrVectorApi::get();
    const auto& stringArrayApi = StringArrayApi::get();

    auto descriptor = thisptr->unitDescriptor;
    if (descriptor) {
        descriptor->vftable->destructor(descriptor, true);
    }

    stringArrayApi.destructor(&thisptr->leaderAbilityTexts);
    imagePtrVectorApi.destructor(&thisptr->leaderAbilityIcons);

    auto patched = (CEncLayoutUnitDataPatched*)thisptr;
    stringArrayApi.destructor(&patched->modifierTexts);
    imagePtrVectorApi.destructor(&patched->modifierIcons);
}

void __fastcall encLayoutUnitInitializeHooked(game::CEncLayoutUnit* thisptr,
                                              int /*%edx*/,
                                              const game::CEncParamBase* encParam)
{
    using namespace game;

    const auto& dialogApi = CDialogInterfApi::get();

    getOriginalFunctions().encLayoutUnitInitialize(thisptr, encParam);

    if (!dialogApi.findControl(thisptr->dialog, modifiersListBoxName)) {
        return;
    }

    if (dialogApi.findControl(thisptr->dialog, modifiersTextBoxName)) {
        auto text = getInterfaceText(textIds().interf.modifiersCaption.c_str());
        if (text.empty())
            text = "\\fMedBold;Effects:\\fNormal;";

        auto textBox = dialogApi.findTextBox(thisptr->dialog, modifiersTextBoxName);
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }

    SmartPointer functor{};
    auto callback = (CEncLayoutUnitApi::Api::ListBoxDisplayCallback)modifiersListBoxDisplayCallback;
    CEncLayoutUnitApi::get().createListBoxDisplayFunctor(&functor, 0, thisptr, &callback);
    CListBoxInterfApi::get().assignDisplaySurfaceFunctor(thisptr->dialog, modifiersListBoxName,
                                                         "DLG_R_C_UNIT", &functor);
    SmartPointerApi::get().createOrFreeNoDtor(&functor, nullptr);

    addModifiersInfo(thisptr);
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
