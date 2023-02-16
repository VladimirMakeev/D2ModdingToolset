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
#include "borderedimg.h"
#include "dialoginterf.h"
#include "dynamiccast.h"
#include "faceimg.h"
#include "fortification.h"
#include "gameutils.h"
#include "interfaceutils.h"
#include "midstack.h"
#include "mqimage2.h"
#include "originalfunctions.h"
#include "pictureinterf.h"
#include "textboxinterf.h"
#include "unitutils.h"
#include "usunit.h"
#include "utils.h"
#include <fmt/format.h>
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

void __fastcall encLayoutCityOnObjectChangedHooked(game::CEncLayoutCity* thisptr,
                                                   int /*%edx*/,
                                                   const game::IMidObject* obj)
{
    using namespace game;

    if (!obj) {
        return;
    }

    const auto& api = CEncLayoutCityApi::get();
    const auto& idApi = CMidgardIDApi::get();
    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    auto idType = idApi.getType(&obj->id);
    if (idType == IdType::Fortification) {
        auto fort = static_cast<const CFortification*>(
            dynamicCast(obj, 0, rtti.IMidObjectType, rtti.CFortificationType, 0));
        if (fort && fort->id == thisptr->data->fortificationId) {
            api.update(thisptr, thisptr->data->objectMap, fort, thisptr->dialog);
        }
    } else if (idType == IdType::Stack) {
        auto stack = static_cast<const CMidStack*>(
            dynamicCast(obj, 0, rtti.IMidObjectType, rtti.CMidStackType, 0));
        if (stack && stack->insideId == thisptr->data->fortificationId) {
            auto objectMap = thisptr->data->objectMap;
            auto fort = static_cast<const CFortification*>(
                objectMap->vftable->findScenarioObjectById(objectMap,
                                                           &thisptr->data->fortificationId));
            api.update(thisptr, objectMap, fort, thisptr->dialog);
        }
    }
}

std::string getUnitUiControlName(int unitPosition, const char* nameFormat)
{
    std::string result = nameFormat;
    replace(result, "%d", fmt::format("{:d}", unitPosition));
    return result;
}

void resizeUnitUiControl(game::CInterface* control,
                         const game::CInterface* relative,
                         bool isUnitSmall)
{
    using namespace game;

    auto relativeArea = relative->vftable->getArea(relative);
    CMqRect area = *control->vftable->getArea(control);
    if (isUnitSmall) {
        if (relativeArea->left > area.left) {
            area.right = area.left + (relativeArea->right - relativeArea->left);
        } else {
            area.left = area.right - (relativeArea->right - relativeArea->left);
        }
    } else {
        if (relativeArea->left > area.left) {
            area.right = relativeArea->right;
        } else {
            area.left = relativeArea->left;
        }
    }
    control->vftable->setArea(control, &area);
}

void alignUnitUiControls(game::CDialogInterf* dialog,
                         game::CInterface* control,
                         int unitPosition,
                         const char* controlNameFormat,
                         bool isUnitSmall)
{
    using namespace game;

    const auto& dialogApi = CDialogInterfApi::get();

    if (unitPosition % 2 == 0) {
        auto control2Name = getUnitUiControlName(unitPosition + 1, controlNameFormat);
        dialogApi.showControl(dialog, dialog->data->dialogName, control2Name.c_str());
        auto control2 = dialogApi.findControl(dialog, control2Name.c_str());

        resizeUnitUiControl(control, control2, isUnitSmall);

        if (!isUnitSmall) {
            dialogApi.hideControl(dialog, control2Name.c_str());
        }
    }
}

game::IMqImage2* createUnitFaceImage(const game::CMidUnit* unit, bool isStackGroup)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto faceImage = fn.createUnitFaceImage(&unit->unitImpl->id, false);
    if (unit->currentHp > 0) {
        auto percentHp = 100 * unit->currentHp / getUnitHpMax(unit);
        faceImage->vftable->setPercentHp(faceImage, percentHp);
        faceImage->vftable->setUnknown68(faceImage, 0);
    } else {
        faceImage->vftable->setUnknown68(faceImage, 10);
    }
    faceImage->vftable->setLeftSide(faceImage, isStackGroup);

    return (IMqImage2*)faceImage;
}

void updateTxtStack(game::CDialogInterf* dialog,
                    const game::CMidUnit* unit,
                    int unitPosition,
                    const char* txtStackNameFormat,
                    bool isUnitSmall)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& dialogApi = CDialogInterfApi::get();

    auto txtStackName = getUnitUiControlName(unitPosition, txtStackNameFormat);
    auto txtStack = dialogApi.findTextBox(dialog, txtStackName.c_str());

    alignUnitUiControls(dialog, txtStack, unitPosition, txtStackNameFormat, isUnitSmall);

    std::string unitName;
    if (unit) {
        if (fn.castUnitImplToStackLeader(unit->unitImpl)) {
            unitName = getInterfaceText("X005TA0784");
        } else {
            unitName = getInterfaceText("X005TA0783");
        }
        replace(unitName, "%NAME%", getUnitName(unit));
    }

    CTextBoxInterfApi::get().setString(txtStack, unitName.c_str());
}

void updateImgStack(game::CDialogInterf* dialog,
                    const game::CMidUnit* unit,
                    int unitPosition,
                    const char* imgStackNameFormat,
                    bool isUnitSmall,
                    bool isStackGroup)
{
    using namespace game;

    const auto& dialogApi = CDialogInterfApi::get();

    auto imgStackName = getUnitUiControlName(unitPosition, imgStackNameFormat);
    auto imgStack = dialogApi.findPicture(dialog, imgStackName.c_str());

    alignUnitUiControls(dialog, imgStack, unitPosition, imgStackNameFormat, isUnitSmall);

    auto faceImage = unit ? createUnitFaceImage(unit, isStackGroup) : nullptr;
    auto borderedFaceImage = createBorderedImage(faceImage, isUnitSmall ? BorderType::UnitSmall
                                                                        : BorderType::UnitLarge);

    setCenteredImage(imgStack, borderedFaceImage);
}

void __stdcall encLayoutCityUpdateGroupUiHooked(const game::IMidgardObjectMap* objectMap,
                                                const game::CMidUnitGroup* group,
                                                game::CDialogInterf* dialog,
                                                const char* txtStackNameFormat,
                                                const char* imgStackNameFormat,
                                                bool isStackGroup)
{
    using namespace game;

    const auto& groupApi = CMidUnitGroupApi::get();

    for (int unitPosition = 0; unitPosition < 6; ++unitPosition) {
        auto unitId = group ? *groupApi.getUnitIdByPosition(group, unitPosition) : emptyId;
        if (unitId != emptyId) {
            auto unit = static_cast<const CMidUnit*>(
                objectMap->vftable->findScenarioObjectById(objectMap, &unitId));

            bool isUnitSmall = hooks::isUnitSmall(unit);
            updateTxtStack(dialog, unit, unitPosition, txtStackNameFormat, isUnitSmall);
            updateImgStack(dialog, unit, unitPosition, imgStackNameFormat, isUnitSmall,
                           isStackGroup);
            if (!isUnitSmall) {
                ++unitPosition;
            }
        } else {
            updateTxtStack(dialog, nullptr, unitPosition, txtStackNameFormat, true);
            updateImgStack(dialog, nullptr, unitPosition, imgStackNameFormat, true, isStackGroup);
        }
    }
}

} // namespace hooks
