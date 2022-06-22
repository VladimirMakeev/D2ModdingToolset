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

#include "interfaceutils.h"
#include "attackclasscat.h"
#include "attacksourcecat.h"
#include "categorylist.h"
#include "customattacks.h"
#include "encunitdescriptor.h"
#include "leaderunitdescriptor.h"
#include "midunit.h"
#include "midunitdescriptor.h"
#include "settings.h"
#include "textids.h"
#include "unittypedescriptor.h"
#include "unitutils.h"
#include "usunitimpl.h"
#include "utils.h"
#include <editor.h>
#include <fmt/format.h>

namespace hooks {

const game::CMidUnitDescriptor* castToMidUnitDescriptor(const game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    return (const game::CMidUnitDescriptor*)dynamicCast(descriptor, 0, rtti.IEncUnitDescriptorType,
                                                        rtti.CMidUnitDescriptorType, 0);
}

const game::CUnitTypeDescriptor* castToUnitTypeDescriptor(
    const game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    return (const game::CUnitTypeDescriptor*)dynamicCast(descriptor, 0, rtti.IEncUnitDescriptorType,
                                                         rtti.CUnitTypeDescriptorType, 0);
}

const game::CLeaderUnitDescriptor* castToLeaderUnitDescriptor(
    const game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    return (const game::CLeaderUnitDescriptor*)dynamicCast(descriptor, 0,
                                                           rtti.IEncUnitDescriptorType,
                                                           rtti.CLeaderUnitDescriptorType, 0);
}

const game::TUsUnitImpl* getUnitImpl(const game::IEncUnitDescriptor* descriptor)
{
    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
    if (midUnitDescriptor) {
        return getUnitImpl(midUnitDescriptor->unit->unitImpl);
    }

    auto unitTypeDescriptor = castToUnitTypeDescriptor(descriptor);
    if (unitTypeDescriptor) {
        return getUnitImpl(&unitTypeDescriptor->unitImplId);
    }

    auto leaderUnitDescriptor = castToLeaderUnitDescriptor(descriptor);
    if (leaderUnitDescriptor) {
        const auto& data = leaderUnitDescriptor->data;
        return generateUnitImpl(&data.globalUnitImplId, data.level);
    }

    // Should not happen as base game only have 3 descendants of IEncUnitDescriptor
    return nullptr;
}

bool hasCriticalHitLeaderAbility(const game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& categoryListApi = CategoryListApi::get();

    if (!descriptor->vftable->isUnitLeader(descriptor)) {
        return false;
    }

    List<LLeaderAbility> abilities{};
    categoryListApi.constructor((CategoryList*)&abilities);
    descriptor->vftable->getLeaderAbilities(descriptor, &abilities);

    bool result = false;
    for (const auto& ability : abilities) {
        if (ability.id == LeaderAbilityCategories::get().criticalHit->id) {
            result = true;
            break;
        }
    }

    categoryListApi.clear((CategoryList*)&abilities);
    categoryListApi.freeNode((CategoryList*)&abilities, (CategoryListNode*)abilities.head);
    return result;
}

std::string getNumberText(int value, bool percent)
{
    return fmt::format(percent ? "{:d}%" : "{:d}", value);
}

std::string getBonusNumberText(int bonus, bool percent, bool trim, bool reverse)
{
    if (!bonus) {
        return "";
    }
    bool positive = bonus > 0;

    // "\c025;090;000; + %NUMBER%"
    // "\c100;000;000; - %NUMBER%"
    auto result = getInterfaceText(positive ? "X005TA0486" : "X005TA0487");
    // "\c000;000;000;"
    result += getInterfaceText("X005TA0488");

    if (trim || reverse) {
        const char* bonus = "+ %NUMBER%";
        const char* loss = "- %NUMBER%";
        std::string keyword = positive ? bonus : loss;
        if (trim) {
            keyword.insert(0, 1, ' ');
        }
        replace(result, keyword, positive != reverse ? bonus : loss);
    }

    replace(result, "%NUMBER%", fmt::format(percent ? "{:d}%" : "{:d}", abs(bonus)));
    return result;
}

std::string getModifiedNumberText(int value, int base, bool percent, bool reverse)
{
    int bonus = value - base;
    if (base && bonus) {
        auto result = getNumberText(base, percent);
        result += getBonusNumberText(bonus, percent, reverse);
        return result;
    } else if (bonus) {
        return getBonusNumberText(bonus, percent, true, reverse);
    } else {
        return getNumberText(base, percent);
    }
}

std::string getModifiedStringText(const std::string& value, bool modified)
{
    if (!modified)
        return value;

    auto result = getInterfaceText(textIds().interf.modifiedValue.c_str());
    if (result.empty())
        result = "\\c025;090;000;%VALUE%\\c000;000;000;";

    replace(result, "%VALUE%", value);
    return result;
}

std::string getAttackSourceText(const game::LAttackSource* source)
{
    // Spell can have no source
    if (!source)
        return getInterfaceText("X005TA0473"); // "None"

    return getAttackSourceText(source->id);
}

std::string getAttackSourceText(game::AttackSourceId id)
{
    using namespace game;

    const auto& sources = AttackSourceCategories::get();

    if (id == sources.weapon->id)
        return getInterfaceText("X005TA0145"); // "Weapon"
    else if (id == sources.mind->id)
        return getInterfaceText("X005TA0146"); // "Mind"
    else if (id == sources.life->id)
        return getInterfaceText("X005TA0147"); // "Life"
    else if (id == sources.death->id)
        return getInterfaceText("X005TA0148"); // "Death"
    else if (id == sources.fire->id)
        return getInterfaceText("X005TA0149"); // "Fire"
    else if (id == sources.water->id)
        return getInterfaceText("X005TA0150"); // "Water"
    else if (id == sources.air->id)
        return getInterfaceText("X005TA0151"); // "Air"
    else if (id == sources.earth->id)
        return getInterfaceText("X005TA0152"); // "Earth"
    else {
        for (const auto& custom : getCustomAttacks().sources) {
            if (id == custom.source.id)
                return getInterfaceText(custom.nameId.c_str());
        }
    }

    return "";
}

std::string getAttackClassText(game::AttackClassId id)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (id == classes.paralyze->id)
        return getInterfaceText("X005TA0789");
    else if (id == classes.petrify->id)
        return getInterfaceText("X005TA0790");
    else if (id == classes.damage->id)
        return getInterfaceText("X005TA0791");
    else if (id == classes.drain->id)
        return getInterfaceText("X005TA0792");
    else if (id == classes.heal->id)
        return getInterfaceText("X005TA0793");
    else if (id == classes.fear->id)
        return getInterfaceText("X005TA0794");
    else if (id == classes.boostDamage->id)
        return getInterfaceText("X005TA0795");
    else if (id == classes.lowerDamage->id)
        return getInterfaceText("X005TA0796");
    else if (id == classes.lowerInitiative->id)
        return getInterfaceText("X005TA0797");
    else if (id == classes.poison->id)
        return getInterfaceText("X005TA0798");
    else if (id == classes.frostbite->id)
        return getInterfaceText("X005TA0799");
    else if (id == classes.blister->id)
        return getInterfaceText("X160TA0012");
    else if (id == classes.bestowWards->id)
        return getInterfaceText("X160TA0014");
    else if (id == classes.shatter->id)
        return getInterfaceText("X160TA0020");
    else if (id == classes.revive->id)
        return getInterfaceText("X005TA0800");
    else if (id == classes.drainOverflow->id)
        return getInterfaceText("X005TA0801");
    else if (id == classes.cure->id)
        return getInterfaceText("X005TA0802");
    else if (id == classes.summon->id)
        return getInterfaceText("X005TA0803");
    else if (id == classes.drainLevel->id)
        return getInterfaceText("X005TA0804");
    else if (id == classes.giveAttack->id)
        return getInterfaceText("X005TA0805");
    else if (id == classes.doppelganger->id)
        return getInterfaceText("X005TA0806");
    else if (id == classes.transformSelf->id)
        return getInterfaceText("X005TA0807");
    else if (id == classes.transformOther->id)
        return getInterfaceText("X005TA0808");

    return "";
}

bool getDynUpgradesToDisplay(game::IEncUnitDescriptor* descriptor,
                             const game::CDynUpgrade** upgrade1,
                             const game::CDynUpgrade** upgrade2)
{
    using namespace game;

    if (!userSettings().unitEncyclopedia.displayDynamicUpgradeValues) {
        return false;
    }

    if (!descriptor->vftable->isUnitType(descriptor)) {
        return false;
    }

    CMidgardID globalUnitImplId;
    descriptor->vftable->getGlobalUnitImplId(descriptor, &globalUnitImplId);
    auto globalUnitImpl = getGlobalUnitImpl(&globalUnitImplId);

    *upgrade1 = getDynUpgrade(globalUnitImpl, 1);
    *upgrade2 = getDynUpgrade(globalUnitImpl, 2);
    return *upgrade1 && *upgrade2;
}

void addDynUpgradeLevelToField(std::string& text, const char* field, int level)
{
    auto result = getInterfaceText(textIds().interf.dynamicUpgradeLevel.c_str());
    if (result.empty()) {
        result = "%STAT% (level-ups weaken at %UPGLV%)";
    }

    replace(result, "%STAT%", field);
    replace(result, "%UPGLV%", fmt::format("{:d}", level));

    replace(text, field, result);
}

void addDynUpgradeTextToField(std::string& text, const char* field, int upgrade1, int upgrade2)
{
    if (!upgrade1 || !upgrade2) {
        return;
    }

    auto result = getInterfaceText(textIds().interf.dynamicUpgradeValues.c_str());
    if (result.empty()) {
        result = "%STAT% (%UPG1% | %UPG2% per level)";
    }

    replace(result, "%STAT%", field);
    replace(result, "%UPG1%", fmt::format("{:+d}", upgrade1));
    replace(result, "%UPG2%", fmt::format("{:+d}", upgrade2));

    replace(text, field, result);
}

} // namespace hooks
