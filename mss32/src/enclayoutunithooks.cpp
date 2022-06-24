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
#include "buildingtype.h"
#include "categorylist.h"
#include "custommodifier.h"
#include "custommodifiers.h"
#include "dialoginterf.h"
#include "dynupgrade.h"
#include "enclayoutunit.h"
#include "encunitdescriptor.h"
#include "gameimages.h"
#include "gameutils.h"
#include "imageptrvector.h"
#include "interfaceutils.h"
#include "intvector.h"
#include "listbox.h"
#include "mempool.h"
#include "midplayer.h"
#include "midunitdescriptor.h"
#include "modifierutils.h"
#include "mqimage2.h"
#include "originalfunctions.h"
#include "pictureinterf.h"
#include "racetype.h"
#include "restrictions.h"
#include "settings.h"
#include "stringarray.h"
#include "textboxinterf.h"
#include "textids.h"
#include "unitmodifier.h"
#include "unittypedescriptor.h"
#include "unitutils.h"
#include "usunitimpl.h"
#include "utils.h"

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
    data->shatteredArmor = 0;
    data->fortificationArmor = 0;
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

    parent->vftable->addChild(parent, (CInterface*)thisptr);

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

    parent->vftable->addChild(parent, (CInterface*)thisptr);

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

static std::string getImmuOrWardField(game::IEncUnitDescriptor* descriptor,
                                      const game::IUsSoldier* soldierImpl,
                                      game::IEncUnitDescriptorVftable::GetAttackSources getSources,
                                      game::IEncUnitDescriptorVftable::GetAttackClasses getClasses,
                                      game::ImmuneId immuneId,
                                      const game::IdList& editorModifiers)
{
    using namespace game;

    const auto& categoryListApi = CategoryListApi::get();

    List<LAttackSource> sources{};
    categoryListApi.constructor((CategoryList*)&sources);
    getSources(descriptor, &sources);

    List<LAttackClass> classes{};
    categoryListApi.constructor((CategoryList*)&classes);
    getClasses(descriptor, &classes);

    std::string result;
    for (const auto& source : sources) {
        if (!result.empty()) {
            result += ", ";
        }

        auto immune = soldierImpl->vftable->getImmuneByAttackSource(soldierImpl, &source);

        bool modified = immune->id != immuneId;
        if (modified) {
            for (auto modifierId : editorModifiers) {
                if (isImmunityModifier(&modifierId, &source, immuneId)) {
                    modified = false;
                    break;
                }
            }
        }

        auto text = getAttackSourceText(&source);
        result += getModifiedStringText(text, modified);
    }

    for (const auto& class_ : classes) {
        if (!result.empty()) {
            result += ", ";
        }

        auto immune = soldierImpl->vftable->getImmuneByAttackClass(soldierImpl, &class_);

        bool modified = immune->id != immuneId;
        if (modified) {
            for (auto modifierId : editorModifiers) {
                if (isImmunityclassModifier(&modifierId, &class_, immuneId)) {
                    modified = false;
                    break;
                }
            }
        }

        auto text = getAttackClassText(class_.id);
        result += getModifiedStringText(text, modified);
    }

    if (result.empty()) {
        // "None"
        result = getInterfaceText("X005TA0469");
    }

    categoryListApi.clear((CategoryList*)&sources);
    categoryListApi.freeNode((CategoryList*)&sources, (CategoryListNode*)sources.head);
    categoryListApi.clear((CategoryList*)&classes);
    categoryListApi.freeNode((CategoryList*)&classes, (CategoryListNode*)classes.head);
    return result;
}

static std::string getImmuField(game::IEncUnitDescriptor* descriptor,
                                const game::IUsSoldier* soldierImpl,
                                const game::IdList& editorModifiers)
{
    return getImmuOrWardField(descriptor, soldierImpl,
                              descriptor->vftable->getAttackSourcesUnitIsImmuneTo,
                              descriptor->vftable->getAttackClassesUnitIsImmuneTo,
                              game::ImmuneId::Always, editorModifiers);
}

static std::string getWardField(game::IEncUnitDescriptor* descriptor,
                                const game::IUsSoldier* soldierImpl,
                                const game::IdList& editorModifiers)
{
    return getImmuOrWardField(descriptor, soldierImpl,
                              descriptor->vftable->getAttackSourcesUnitIsResistantTo,
                              descriptor->vftable->getAttackClassesUnitIsResistantTo,
                              game::ImmuneId::Once, editorModifiers);
}

static int getUnitArmor(game::CEncLayoutUnit* layout)
{
    using namespace game;

    const auto& restrictions = gameRestrictions();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;
    auto objectMap = data->objectMap;

    int result = descriptor->vftable->getUnitArmor(descriptor);
    if (!data->unknown4 && objectMap && !descriptor->vftable->isUnitType(descriptor)) {
        result += getCityProtection(objectMap, &data->unitId);
    }

    result -= data->shatteredArmor;
    if (result < data->fortificationArmor)
        result = data->fortificationArmor;

    return std::clamp(result, restrictions.unitArmor->min, restrictions.unitArmor->max);
}

static std::string getHp2Field(game::IEncUnitDescriptor* descriptor,
                               const game::IUsSoldier* soldierImpl,
                               const game::IdList& editorModifiers)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& restrictions = gameRestrictions();

    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
    if (!midUnitDescriptor || !userSettings().unitEncyclopedia.displayBonusHp) {
        return getNumberText(descriptor->vftable->getHpMax(descriptor), false);
    }

    auto unit = midUnitDescriptor->unit;
    if (unit->transformed && unit->keepHp) {
        return getNumberText(unit->hpBefMax, false);
    }

    int implHpMax = soldierImpl->vftable->getHitPoints(soldierImpl);
    implHpMax = applyModifiers(implHpMax, editorModifiers, ModifierElementTypeFlag::Hp);
    implHpMax = std::clamp(implHpMax, restrictions.unitHp->min, restrictions.unitHp->max);

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    auto actualHpMax = soldier->vftable->getHitPoints(soldier);

    return getModifiedNumberTextTotal(actualHpMax, implHpMax, false);
}

static std::string getXp2Field(game::IEncUnitDescriptor* descriptor,
                               const game::IUsSoldier* soldierImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
    if (!midUnitDescriptor || !userSettings().unitEncyclopedia.displayBonusXp) {
        return getNumberText(descriptor->vftable->getUnitXpNext(descriptor), false);
    }

    int implXpNext = soldierImpl->vftable->getXpNext(soldierImpl);

    auto soldier = fn.castUnitImplToSoldier(midUnitDescriptor->unit->unitImpl);
    auto actualXpNext = soldier->vftable->getXpNext(soldier);

    return getModifiedNumberTextTotalReverseBonus(actualXpNext, implXpNext, false);
}

static std::string getArmorField(game::CEncLayoutUnit* layout,
                                 const game::IUsSoldier* soldierImpl,
                                 const game::IdList& editorModifiers)
{
    using namespace game;

    const auto& restrictions = gameRestrictions();

    int implArmor;
    soldierImpl->vftable->getArmor(soldierImpl, &implArmor);
    implArmor = applyModifiers(implArmor, editorModifiers, ModifierElementTypeFlag::Armor);
    implArmor = std::clamp(implArmor, restrictions.unitArmor->min, restrictions.unitArmor->max);

    int actualArmor = getUnitArmor(layout);

    return getModifiedNumberTextFull(actualArmor, implArmor, false);
}

static std::string getXpField(game::IEncUnitDescriptor* descriptor,
                              const game::IUsSoldier* soldierImpl)
{
    if (descriptor->vftable->isUnitAtMaxLevel(descriptor)) {
        // \c128;000;000;Max\c000;000;000;
        return getInterfaceText("X005TA0648");
    }

    // %XP1% / %XP2%
    auto result = getInterfaceText("X005TA0649");
    replace(result, "%XP1%",
            getNumberText(descriptor->vftable->getUnitCurrentXp(descriptor), false));
    replace(result, "%XP2%", getXp2Field(descriptor, soldierImpl));

    return result;
}

static std::string getEffhpField(game::CEncLayoutUnit* layout)
{
    using namespace game;

    auto data{layout->data};
    auto descriptor{data->unitDescriptor};

    auto unitArmor{getUnitArmor(layout)};
    auto unitHp{descriptor->vftable->getHp(descriptor)};

    return getNumberText(computeUnitEffectiveHp(unitHp, unitArmor), false);
}

static std::string getRegenField(game::CEncLayoutUnit* layout, const game::IUsSoldier* soldierImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;
    auto objectMap = data->objectMap;

    auto implRegen = *soldierImpl->vftable->getRegen(soldierImpl);

    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
    if (!midUnitDescriptor) {
        return getNumberText(implRegen, true);
    }

    auto actualRegen = getUnitRegen(objectMap, &data->unitId);

    return getModifiedNumberText(actualRegen, implRegen, true);
}

static std::string getXpKillField(game::IEncUnitDescriptor* descriptor,
                                  const game::IUsSoldier* soldierImpl)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto implXpKilled = soldierImpl->vftable->getXpKilled(soldierImpl);

    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
    if (!midUnitDescriptor) {
        return getNumberText(implXpKilled, false);
    }

    auto soldier = fn.castUnitImplToSoldier(midUnitDescriptor->unit->unitImpl);
    auto actualXpKilled = soldier->vftable->getXpKilled(soldier);

    return getModifiedNumberTextReverseBonus(actualXpKilled, implXpKilled, false);
}

static std::string getApField(game::IEncUnitDescriptor* descriptor)
{
    if (!descriptor->vftable->isUnitType(descriptor)) {
        return "";
    }

    // \fmedbold;Move points:\t\fnormal;%AP%\n
    auto result = getInterfaceText("X005TA0650");
    replace(result, "%AP%", getNumberText(descriptor->vftable->getMovement(descriptor), false));

    return result;
}

static std::string getLdrshpField(game::IEncUnitDescriptor* descriptor)
{
    // \s110;\fmedbold;Leadership:\t\fnormal;%MAX%\n
    auto result = getInterfaceText("X005TA0583");
    replace(result, "%MAX%",
            getNumberText(descriptor->vftable->getLeadership(descriptor) - 1, false));

    return result;
}

static std::string getNbbatField(game::IEncUnitDescriptor* descriptor)
{
    if (descriptor->vftable->isUnitType(descriptor)) {
        return "";
    }

    // \s110;\fMedBold;Battles won:\t\fNormal;%VALUE%\n
    auto result = getInterfaceText("X005TA0737");
    replace(result, "%VALUE%", getNumberText(descriptor->vftable->getNbBattle(descriptor), false));

    return result;
}

static std::string getBuildingField(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    Vector<TBuildingType*> buildings{};
    IntVectorApi::get().reserve((IntVector*)&buildings, 1);
    descriptor->vftable->getUnitRequiredBuildings(descriptor, &buildings);

    std::string result;
    for (auto it = buildings.bgn; it != buildings.end; ++it) {
        auto building = *it;
        auto namesAndId = building->data->name;
        auto name = GlobalDataApi::get().findTextById(namesAndId.texts, &namesAndId.id);

        if (!result.empty()) {
            //  \fNormal;\c000;000;000; or \fNormal;
            result += getInterfaceText("X005TA0459");
        }
        result += name;
    }

    return result;
}

static std::string getRaceField(game::IEncUnitDescriptor* descriptor,
                                const game::IUsSoldier* soldier)
{
    using namespace game;

    const auto& globalApi = GlobalDataApi::get();

    auto raceId = soldier->vftable->getRaceId(soldier);
    auto races = (*globalApi.getGlobalData())->races;
    auto race = (TRaceType*)globalApi.findById(races, raceId);

    auto namesAndId = race->data->name;
    return globalApi.findTextById(namesAndId.texts, &namesAndId.id);
}

static void setImgUnitIcon(game::CEncLayoutUnit* layout)
{
    using namespace game;

    const auto& pictureApi = CPictureInterfApi::get();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    auto picture = CDialogInterfApi::get().findPicture(layout->dialog, "IMG_UNIT_ICON");

    CMidgardID globalUnitImplId;
    descriptor->vftable->getGlobalUnitImplId(descriptor, &globalUnitImplId);

    auto image = GameImagesApi::get().getUnitIcon(&globalUnitImplId);
    if (!image) {
        CMqPoint offset{};
        pictureApi.setImage(picture, nullptr, &offset);
    } else {
        auto pictureArea = picture->vftable->getArea(picture);
        CMqPoint pictureSize{pictureArea->p2.x - pictureArea->p1.x,
                             pictureArea->p2.y - pictureArea->p1.y};

        CMqPoint imageSize{};
        image->vftable->getSize(image, &imageSize);

        CMqPoint offset{(pictureSize.x - imageSize.x) / 2, (pictureSize.y - imageSize.y) / 2};
        pictureApi.setImage(picture, image, &offset);
    }
}

static void addStatsDynUpgradeText(std::string& text,
                                   game::IEncUnitDescriptor* descriptor,
                                   const game::IUsSoldier* soldierImpl)
{
    using namespace game;

    const CDynUpgrade* upgrade1 = nullptr;
    const CDynUpgrade* upgrade2 = nullptr;
    if (!getDynUpgradesToDisplay(descriptor, &upgrade1, &upgrade2)) {
        return;
    }

    addDynUpgradeLevelToField(text, "%LEVEL%", soldierImpl->vftable->getDynUpgLvl(soldierImpl));
    addDynUpgradeTextToField(text, "%HP2%", upgrade1->hp, upgrade2->hp);
    addDynUpgradeTextToField(text, "%ARMOR%", upgrade1->armor, upgrade2->armor);
    addDynUpgradeTextToField(text, "%XP%", upgrade1->xpNext, upgrade2->xpNext);
}

static void setTxtStats(game::CEncLayoutUnit* layout,
                        const game::IUsSoldier* soldierImpl,
                        const game::IdList& editorModifiers)
{
    using namespace game;

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    // \s110;
    // \fmedbold;Level:\t\fnormal;%LEVEL%\n
    // \fMedbold;XP:\t\fNormal;%XP%\n
    // \fMedbold;HP:\t\fnormal;%HP1% / %HP2%\n
    // \fMedbold;Armor:\t\fNormal;%ARMOR%\n
    // \fMedbold;Immunities:\t\fNormal;\p110;%IMMU%\mL0;
    // \fMedbold;Wards:\t\fNormal;\p110;%WARD%
    auto text = getInterfaceText("X005TA0423");
    addStatsDynUpgradeText(text, descriptor, soldierImpl);
    replace(text, "%LEVEL%", getNumberText(descriptor->vftable->getUnitLevel(descriptor), false));
    replace(text, "%HP1%", getNumberText(descriptor->vftable->getHp(descriptor), false));
    replace(text, "%HP2%", getHp2Field(descriptor, soldierImpl, editorModifiers));
    replace(text, "%ARMOR%", getArmorField(layout, soldierImpl, editorModifiers));
    replace(text, "%XP%", getXpField(descriptor, soldierImpl));
    replace(text, "%IMMU%", getImmuField(descriptor, soldierImpl, editorModifiers));
    replace(text, "%WARD%", getWardField(descriptor, soldierImpl, editorModifiers));

    auto textBox = CDialogInterfApi::get().findTextBox(layout->dialog, "TXT_STATS");
    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void addStats2DynUpgradeText(std::string& text, game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const CDynUpgrade* upgrade1 = nullptr;
    const CDynUpgrade* upgrade2 = nullptr;
    if (!getDynUpgradesToDisplay(descriptor, &upgrade1, &upgrade2)) {
        return;
    }

    addDynUpgradeTextToField(text, "%REGEN%", upgrade1->regen, upgrade2->regen);
    addDynUpgradeTextToField(text, "%XPKILL%", upgrade1->xpKilled, upgrade2->xpKilled);
}

static void setTxtStats2(game::CEncLayoutUnit* layout,
                         const game::IUsSoldier* soldierImpl,
                         const game::IdList& editorModifiers)
{
    using namespace game;

    static const char controlName[]{"TXT_STATS_2"};
    const auto& dialogApi{CDialogInterfApi::get()};

    auto data{layout->data};
    auto descriptor{data->unitDescriptor};

    if (!dialogApi.findControl(layout->dialog, controlName)) {
        return;
    }

    auto textBox{dialogApi.findTextBox(layout->dialog, controlName)};
    if (!textBox) {
        return;
    }

    std::string text{textBox->data->text.string};
    addStats2DynUpgradeText(text, descriptor);
    replace(text, "%EFFHP%", getEffhpField(layout));
    replace(text, "%REGEN%", getRegenField(layout, soldierImpl));
    replace(text, "%XPKILL%", getXpKillField(descriptor, soldierImpl));

    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void setTxtUnitName(game::CEncLayoutUnit* layout)
{
    using namespace game;

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    // \fLarge;%NAME%
    auto text = getInterfaceText("X005TA0567");
    replace(text, "%NAME%", descriptor->vftable->getUnitName(descriptor));

    auto textBox = CDialogInterfApi::get().findTextBox(layout->dialog, "TXT_UNIT_NAME");
    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void setTxtUnitInfo(game::CEncLayoutUnit* layout)
{
    using namespace game;

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    // \fnormal;\hL;\vT;%DESC%
    auto text = getInterfaceText("X005TA0422");
    replace(text, "%DESC%", descriptor->vftable->getUnitDescription(descriptor));

    auto textBox = CDialogInterfApi::get().findTextBox(layout->dialog, "TXT_UNIT_INFO");
    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void setTxtLeaderInfo(game::CEncLayoutUnit* layout)
{
    using namespace game;

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    if (!descriptor->vftable->isUnitLeader(descriptor)) {
        return;
    }

    // \c000;000;000;%LDRSHP%%AP%%NBBAT%
    auto text = getInterfaceText("X005TA0809");
    replace(text, "%AP%", getApField(descriptor));
    replace(text, "%LDRSHP%", getLdrshpField(descriptor));
    replace(text, "%NBBAT%", getNbbatField(descriptor));

    auto textBox = CDialogInterfApi::get().findTextBox(layout->dialog, "TXT_LEADER_INFO");
    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void setTxtNeedUpgrade(game::CEncLayoutUnit* layout, bool* needUpgrade)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;
    auto objectMap = data->objectMap;

    *needUpgrade = false;

    std::string text;
    if (descriptor->vftable->canUnitLevelUp(descriptor)) {
        auto unit = fn.findUnitById(objectMap, &data->unitId);
        auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
        auto player = getPlayerByUnitId(objectMap, &data->unitId);

        if (player->raceId == *soldier->vftable->getRaceId(soldier)) {
            // You must build the %BUILDING% in your capital to upgrade this unit.
            text = getInterfaceText("X005TA0513");
            replace(text, "%BUILDING%", getBuildingField(descriptor));

            *needUpgrade = true;
        } else {
            // This unit is ready to upgrade but can not because it needs a building that is only
            // accessible to the %RACE%
            text = getInterfaceText("X005TA0822");
            replace(text, "%RACE%", getRaceField(descriptor, soldier));
        }
    } else if (descriptor->vftable->getUnitDynLevel(descriptor)) {
        // This unit's type has been locked. It does not need a new building to upgrade.
        text = getInterfaceText("X005TA0941");
    } else {
        return;
    }

    auto textBox = CDialogInterfApi::get().findTextBox(layout->dialog, "TXT_NEED_UPGRADE");
    CTextBoxInterfApi::get().setString(textBox, text.c_str());
}

static void setImgUpgrade(game::CEncLayoutUnit* layout, bool needUpgrade)
{
    using namespace game;

    if (!needUpgrade) {
        return;
    }

    const auto& imagesApi = GameImagesApi::get();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    auto picture = CDialogInterfApi::get().findPicture(layout->dialog, "IMG_UPGRADE");

    GameImagesPtr imagesPtr;
    imagesApi.getGameImages(&imagesPtr);
    auto images = *imagesPtr.data;
    auto image = imagesApi.getImage(images->battle, "FIUPGRADE", 0, false, images->log);

    CMqPoint offset{};
    CPictureInterfApi::get().setImage(picture, image, &offset);

    imagesApi.createOrFreeGameImages(&imagesPtr, nullptr);
}

static void setTxtAttackInfo(game::CEncLayoutUnit* layout, const game::IdList& editorModifiers)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto data = layout->data;
    auto descriptor = data->unitDescriptor;

    CMidgardID globalUnitImplId;
    descriptor->vftable->getGlobalUnitImplId(descriptor, &globalUnitImplId);

    int damageMax = fn.getUnitImplDamageMax(&globalUnitImplId);
    fn.generateAttackDescription(descriptor, layout->dialog, data->boostDamageLevel,
                                 data->lowerDamageLevel, data->lowerInitiativeLevel,
                                 &editorModifiers, damageMax);
}

static void setTxtEffectiveHp(game::CEncLayoutUnit* layout)
{
    using namespace game;

    static const char controlName[]{"TXT_EFFECTIVE_HP"};
    const auto& dialogApi{CDialogInterfApi::get()};

    if (!dialogApi.findControl(layout->dialog, controlName)) {
        return;
    }

    auto textBox{dialogApi.findTextBox(layout->dialog, controlName)};
    if (!textBox) {
        return;
    }

    std::string text{textBox->data->text.string};

    if (replace(text, "%HP%", getEffhpField(layout))) {
        CTextBoxInterfApi::get().setString(textBox, text.c_str());
    }
}

void __fastcall encLayoutUnitUpdateHooked(game::CEncLayoutUnit* thisptr, int /*%edx*/)
{
    if (!userSettings().unitEncyclopedia.detailedUnitDescription) {
        getOriginalFunctions().encLayoutUnitUpdate(thisptr);
        setTxtEffectiveHp(thisptr);
        return;
    }

    using namespace game;

    const auto& fn = gameFunctions();
    const auto& idListApi = IdListApi::get();

    auto data = thisptr->data;
    auto descriptor = data->unitDescriptor;
    auto objectMap = data->objectMap;

    auto unitImpl = getUnitImpl(descriptor);
    auto soldierImpl = gameFunctions().castUnitImplToSoldier(unitImpl);

    IdList editorModifiers = {};
    idListApi.constructor(&editorModifiers);
    if (objectMap && !descriptor->vftable->isUnitType(descriptor)) {
        auto unit = fn.findUnitById(objectMap, &data->unitId);
        getEditorModifiers(unit, &editorModifiers);
    }

    bool needUpgrade;
    setImgUnitIcon(thisptr);
    setTxtUnitName(thisptr);
    setTxtUnitInfo(thisptr);
    setTxtStats(thisptr, soldierImpl, editorModifiers);
    setTxtStats2(thisptr, soldierImpl, editorModifiers);
    setTxtLeaderInfo(thisptr);
    setTxtNeedUpgrade(thisptr, &needUpgrade);
    setImgUpgrade(thisptr, needUpgrade);
    setTxtAttackInfo(thisptr, editorModifiers);
    setTxtEffectiveHp(thisptr);

    idListApi.destructor(&editorModifiers);
}

} // namespace hooks
