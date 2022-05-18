/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#include "interftexthooks.h"
#include "attackdescriptor.h"
#include "attackimpl.h"
#include "attackutils.h"
#include "customattackutils.h"
#include "d2string.h"
#include "dialoginterf.h"
#include "encunitdescriptor.h"
#include "midunit.h"
#include "midunitdescriptor.h"
#include "settings.h"
#include "textboxinterf.h"
#include "textids.h"
#include "unitutils.h"
#include "ussoldierimpl.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

std::string getMaxText(const std::string& value)
{
    // "%DMG% \c000;000;000;(\c128;000;000;Max\c000;000;000;)"
    auto result = getInterfaceText("X005TA0811");
    replace(result, "%DMG%", value);
    return result;
}

std::string getNumberText(int value, bool percent)
{
    return fmt::format(percent ? "{:d}%" : "{:d}", value);
}

std::string getBonusNumberText(int bonus, bool percent)
{
    if (!bonus)
        return "";

    // "\c025;090;000; + %NUMBER%"
    // "\c100;000;000; - %NUMBER%"
    auto result = getInterfaceText(bonus > 0 ? "X005TA0486" : "X005TA0487");
    // "\c000;000;000;"
    result += getInterfaceText("X005TA0488");
    replace(result, "%NUMBER%", fmt::format(percent ? "{:d}%" : "{:d}", abs(bonus)));

    return result;
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

std::string getModifiedNumberText(int value, int base, bool percent, int max = INT_MAX)
{
    std::string result;

    int bonus = value - base;
    if (base && bonus) {
        result = getNumberText(base, percent);
        result += getBonusNumberText(bonus, percent);
    } else if (bonus) {
        result = getModifiedStringText(getNumberText(bonus, percent), true);
    } else {
        result = getNumberText(base, percent);
    }

    if (value >= max) {
        result = getMaxText(result);
    }

    return result;
}

std::string addAltAttackText(const std::string& base, const std::string& value)
{
    auto result = getInterfaceText("X005TA0829"); // "%ATTACK% or %BLANK%"
    replace(result, "%ATTACK%", value);
    replace(result, "%BLANK%", base);
    return result;
}

std::string addAttack2Text(const std::string& base, const std::string& value)
{
    auto result = getInterfaceText("X005TA0785"); // " / %ATTACK%"
    replace(result, "%ATTACK%", value);
    result.insert(0, base);
    return result;
}

std::string getCritHitText()
{
    auto text = getInterfaceText(textIds().interf.critHitAttack.c_str());
    if (text.length())
        return text;

    return getInterfaceText("X160TA0017"); // "Critical hit"
}

std::string addCritHitText(const std::string& base, const std::string& value, bool full)
{
    auto text = getInterfaceText(textIds().interf.critHitDamage.c_str());
    if (text.empty())
        text = "%DMG% (%CRIT%)";

    replace(text, "%DMG%", base);
    replace(text, "%CRIT%", full ? fmt::format("{:s} {:s}", getCritHitText(), value) : value);
    return text;
}

std::string getInfiniteText()
{
    auto text = getInterfaceText(textIds().interf.infiniteAttack.c_str());
    if (text.length())
        return text;

    return "Lasting";
}

std::string addInfiniteText(const std::string& base,
                            const utils::AttackDescriptor& actual,
                            const utils::AttackDescriptor& global)
{
    if (!actual.infinite())
        return base;

    auto text = getInterfaceText(textIds().interf.infiniteText.c_str());
    if (text.empty())
        text = "%ATTACK% (%INFINITE%)";

    replace(text, "%ATTACK%", base);
    replace(text, "%INFINITE%", getModifiedStringText(getInfiniteText(), !global.infinite()));
    return text;
}

std::string getRatedAttackDamageText(int damage, int critDamage, double ratio)
{
    auto result = getNumberText(applyAttackDamageRatio(damage, ratio), false);

    if (critDamage) {
        result = addCritHitText(result,
                                getNumberText(applyAttackDamageRatio(critDamage, ratio), false),
                                false);
    }

    return result;
}

std::string getRatedAttackDamageText(const std::string& base,
                                     const utils::AttackDescriptor& actual,
                                     int damageBoosted)
{
    auto ratios = computeAttackDamageRatio(actual.custom(), getAttackMaxTargets(actual.reachId()));
    if (ratios.size() < 2)
        return base;

    auto critDamage = damageBoosted * actual.critDamage() / 100;
    if (!actual.damageRatioPerTarget() && ratios.size() > 2) {
        auto result = getInterfaceText(textIds().interf.ratedDamageEqual.c_str());
        if (result.empty())
            result = "%DMG%, (%TARGETS%x) %RATED%";

        replace(result, "%DMG%", base);
        replace(result, "%TARGETS%", fmt::format("{:d}", ratios.size() - 1));
        replace(result, "%RATED%", getRatedAttackDamageText(damageBoosted, critDamage, ratios[1]));
        return result;
    } else {
        auto result = getInterfaceText(textIds().interf.ratedDamage.c_str());
        if (result.empty())
            result = "%DMG%, %RATED%";

        auto separator = getInterfaceText(textIds().interf.ratedDamageSeparator.c_str());
        if (separator.empty())
            separator = ", ";

        std::string rated;
        for (auto it = ratios.begin() + 1; it < ratios.end(); ++it) {
            if (!rated.empty())
                rated += separator;
            rated += getRatedAttackDamageText(damageBoosted, critDamage, *it);
        }

        replace(result, "%DMG%", base);
        replace(result, "%RATED%", rated);
        return result;
    }
}

std::string getSplitAttackDamageText(const std::string& base)
{
    auto result = getInterfaceText(textIds().interf.splitDamage.c_str());
    if (result.empty())
        result = "%DMG%, split between targets";

    replace(result, "%DMG%", base);
    return result;
}

std::string getAttackPowerText(const utils::AttackDescriptor& actual,
                               const utils::AttackDescriptor& global,
                               const game::IdList* editorModifiers)
{
    std::string result;
    if (actual.hasPower() && global.hasPower()) {
        result = getModifiedNumberText(actual.power(), global.power(editorModifiers), true);
    } else {
        result = getNumberText(actual.power(), true);
    }

    if (actual.critHit()) {
        result = addCritHitText(result,
                                getModifiedNumberText(actual.critPower(), global.critPower(), true),
                                true);
    }

    return result;
}

std::string getAttackInitiativeText(const utils::AttackDescriptor& actual,
                                    const utils::AttackDescriptor& global,
                                    const game::IdList* editorModifiers,
                                    int lowerInitiativeLevel)
{
    int boosted = actual.initiative() * (1 - getLowerInitiative(lowerInitiativeLevel) / 100);
    return getModifiedNumberText(boosted, global.initiative(editorModifiers), false);
}

std::string getDamageDrainAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global,
                                           const game::IdList* editorModifiers,
                                           int boostDamageLevel,
                                           int lowerDamageLevel,
                                           int damageMax)
{
    int multiplier = actual.damageSplit() ? userSettings().splitDamageMultiplier : 1;

    int boost = getBoostDamage(boostDamageLevel) - getLowerDamage(lowerDamageLevel);
    int boosted = actual.damage() * (1 + boost / 100);
    if (boosted > damageMax)
        boosted = damageMax;
    boosted *= multiplier;

    auto result = getModifiedNumberText(boosted, global.damage(editorModifiers) * multiplier, false,
                                        damageMax * multiplier);

    if (actual.critHit()) {
        result = addCritHitText(result,
                                getModifiedNumberText(boosted * actual.critDamage() / 100,
                                                      boosted * global.critDamage() / 100, false),
                                true);
    }

    if (getAttackMaxTargets(actual.reachId()) < 2)
        return result;
    else if (actual.damageSplit()) {
        return getSplitAttackDamageText(result);
    } else {
        return getRatedAttackDamageText(result, actual, boosted);
    }
}

std::string getBoostDamageAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global)
{
    int boost = global.boost() ? global.boost() : actual.boost();

    auto result = getInterfaceText("X005TA0535"); // "+%BOOST%%"
    replace(result, "%BOOST%", getNumberText(boost, false));

    result += getBonusNumberText(actual.boost() - boost, true);

    return result;
}

std::string getLowerDamageAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global)
{
    int lower = global.lower() ? global.lower() : actual.lower();

    auto result = getInterfaceText("X005TA0550"); // "-%LOWER%%"
    replace(result, "%LOWER%", getNumberText(lower, false));

    result += getBonusNumberText(actual.lower() - lower, true);

    return result;
}

std::string getLowerInitiativeAttackDamageText(const utils::AttackDescriptor& actual,
                                               const utils::AttackDescriptor& global)
{
    int lower = global.lowerIni() ? global.lowerIni() : actual.lowerIni();

    auto result = getInterfaceText("X005TA0550"); // "-%LOWER%%"
    replace(result, "%LOWER%", getNumberText(lower, false));

    result += getBonusNumberText(actual.lowerIni() - lower, true);

    return result;
}

std::string getAttackDamageText(const utils::AttackDescriptor& actual,
                                const utils::AttackDescriptor& global,
                                const game::IdList* editorModifiers,
                                int boostDamageLevel,
                                int lowerDamageLevel,
                                int damageMax)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    std::string result;
    if (actual.classId() == classes.boostDamage->id) {
        result = getBoostDamageAttackDamageText(actual, global);
    } else if (actual.classId() == classes.lowerDamage->id) {
        result = getLowerDamageAttackDamageText(actual, global);
    } else if (actual.classId() == classes.lowerInitiative->id) {
        result = getLowerInitiativeAttackDamageText(actual, global);
    } else if (actual.classId() == classes.damage->id || actual.classId() == classes.drain->id
               || actual.classId() == classes.drainOverflow->id) {
        result = getDamageDrainAttackDamageText(actual, global, editorModifiers, boostDamageLevel,
                                                lowerDamageLevel, damageMax);
    } else if (actual.classId() == classes.heal->id
               || actual.classId() == classes.bestowWards->id) {
        result = getModifiedNumberText(actual.heal(), global.heal(), false);
    } else if (actual.classId() == classes.poison->id || actual.classId() == classes.frostbite->id
               || actual.classId() == classes.blister->id) {
        result = getNumberText(global.damage(), false);
    } else {
        result = getModifiedNumberText(actual.damage(), global.damage(), false, damageMax);
    }

    return addInfiniteText(result, actual, global);
}

std::string getAttackSourceText(game::AttackSourceId id)
{
    using namespace game;

    const auto& sources = AttackSourceCategories::get();

    if (id == (AttackSourceId)emptyCategoryId)
        return getInterfaceText("X005TA0473"); // "None"
    else if (id == sources.weapon->id)
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

std::string getAttackSourceText(const utils::AttackDescriptor& actual,
                                const utils::AttackDescriptor& global)
{
    std::string result = getAttackSourceText(actual.sourceId());
    return getModifiedStringText(result, actual.sourceId() != global.sourceId());
}

std::string getAttackReachText(game::AttackReachId id)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    if (id == reaches.adjacent->id)
        return getInterfaceText("X005TA0201"); // "Adjacent units"
    else if (id == reaches.all->id || id == reaches.any->id)
        return getInterfaceText("X005TA0200"); // "Any unit"
    else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id) {
                return getInterfaceText(custom.reachTxt.c_str());
            }
        }
    }

    return "";
}

std::string getAttackTargetsText(game::AttackReachId id)
{
    using namespace game;

    const auto& reaches = AttackReachCategories::get();

    if (id == reaches.all->id)
        return getInterfaceText("X005TA0674"); // "6"
    else if (id == reaches.any->id || id == reaches.adjacent->id)
        return getInterfaceText("X005TA0675"); // "1"
    else {
        for (const auto& custom : getCustomAttacks().reaches) {
            if (id == custom.reach.id) {
                return getInterfaceText(custom.targetsTxt.c_str());
            }
        }
    }

    return "";
}

std::string getAttackNameText(const utils::AttackDescriptor& actual,
                              const utils::AttackDescriptor& global)
{
    return getModifiedStringText(actual.name(), actual.name() != global.name());
}

std::string getTwiceField(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    if (!descriptor->vftable->attacksTwice(descriptor))
        return "";

    auto result = getInterfaceText("X005TA0786"); // "(2x) %BLANK%"
    replace(result, "%BLANK%", "");

    bool modified = false;
    auto midUnitDescriptor = (const game::CMidUnitDescriptor*)
        dynamicCast(descriptor, 0, rtti.IEncUnitDescriptorType, rtti.CMidUnitDescriptorType, 0);
    if (midUnitDescriptor) {
        const auto unitImpl = midUnitDescriptor->unit->unitImpl;
        const auto soldierImpl = getSoldierImpl(unitImpl);
        modified = soldierImpl && !soldierImpl->vftable->getAttackTwice(soldierImpl);
    }

    return getModifiedStringText(result, modified);
}

std::string getAltAttackField(const utils::AttackDescriptor& actual,
                              const utils::AttackDescriptor& global)
{
    if (actual.empty())
        return "";

    return addAltAttackText("", getAttackNameText(actual, global));
}

std::string getAttackField(const utils::AttackDescriptor& actual,
                           const utils::AttackDescriptor& global)
{
    return getAttackNameText(actual, global);
}

std::string getSecondField(const utils::AttackDescriptor& actual,
                           const utils::AttackDescriptor& global)
{
    if (actual.empty())
        return "";

    return addAttack2Text("", getAttackNameText(actual, global));
}

std::string getHitField(const utils::AttackDescriptor& actual,
                        const utils::AttackDescriptor& global,
                        const game::IdList* editorModifiers)
{
    return getAttackPowerText(actual, global, editorModifiers);
}

std::string getHit2Field(const utils::AttackDescriptor& actual,
                         const utils::AttackDescriptor& global)
{
    if (actual.empty())
        return "";

    auto result = getInterfaceText("X005TA0881"); // " / %POWER%"
    replace(result, "%POWER%", getAttackPowerText(actual, global, nullptr));
    return result;
}

std::string getEffectField(const utils::AttackDescriptor& actual)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (actual.classId() == classes.heal->id || actual.classId() == classes.bestowWards->id)
        return getInterfaceText("X005TA0504"); // "Heal"
    else if (actual.classId() == classes.boostDamage->id)
        return getInterfaceText("X005TA0534"); // "Boost"
    else if (actual.classId() == classes.lowerDamage->id)
        return getInterfaceText("X005TA0547"); // "Lower"
    else if (actual.classId() == classes.lowerInitiative->id)
        return getInterfaceText("X005TA0551"); // "Lower initiative"
    else
        return getInterfaceText("X005TA0503"); // "Damage"
}

std::string getDamageField(const utils::AttackDescriptor& actual,
                           const utils::AttackDescriptor& global,
                           const utils::AttackDescriptor& actual2,
                           const utils::AttackDescriptor& global2,
                           const game::IdList* editorModifiers,
                           int boostDamageLevel,
                           int lowerDamageLevel,
                           int damageMax)
{
    auto result = getAttackDamageText(actual, global, editorModifiers, boostDamageLevel,
                                      lowerDamageLevel, damageMax);

    if (!actual2.empty()) {
        auto damage2 = getAttackDamageText(actual2, global2, nullptr, boostDamageLevel,
                                           lowerDamageLevel, damageMax);
        if (result == "0")
            result = damage2;
        else if (damage2 != "0")
            result = addAttack2Text(result, damage2);
    }

    // Fixes vertical tab in case of multiline
    return fmt::format("\\p110;{:s}\\p0;", result);
}

std::string getSourceField(const utils::AttackDescriptor& actual,
                           const utils::AttackDescriptor& global,
                           const utils::AttackDescriptor& actualAlt,
                           const utils::AttackDescriptor& globalAlt)
{
    std::string result = getAttackSourceText(actual, global);

    const auto& classes = game::AttackClassCategories::get();
    if (actualAlt.classId() == classes.doppelganger->id) {
        if (userSettings().doppelgangerRespectsEnemyImmunity
            || userSettings().doppelgangerRespectsAllyImmunity) {
            result = addAltAttackText(result, getAttackSourceText(actualAlt, globalAlt));
        }
    }

    return result;
}

std::string getSource2Field(const utils::AttackDescriptor& actual,
                            const utils::AttackDescriptor& global)
{
    if (actual.empty())
        return "";

    auto result = getInterfaceText("X005TA0816"); // " / %SOURCE%"
    replace(result, "%SOURCE%", getAttackSourceText(actual, global));
    return result;
}

std::string getInitField(const utils::AttackDescriptor& actual,
                         const utils::AttackDescriptor& global,
                         const game::IdList* editorModifiers,
                         int lowerInitiativeLevel)
{
    return getAttackInitiativeText(actual, global, editorModifiers, lowerInitiativeLevel);
}

std::string getReachField(const utils::AttackDescriptor& actual,
                          const utils::AttackDescriptor& global)
{
    std::string result = getAttackReachText(actual.reachId());
    return getModifiedStringText(result, actual.reachId() != global.reachId());
}

std::string getTargetsField(const utils::AttackDescriptor& actual,
                            const utils::AttackDescriptor& global)
{
    std::string result = getAttackTargetsText(actual.reachId());
    return getModifiedStringText(result, actual.reachId() != global.reachId());
}

void __stdcall generateAttackDescriptionHooked(game::IEncUnitDescriptor* descriptor,
                                               game::CDialogInterf* dialog,
                                               int boostDamageLevel,
                                               int lowerDamageLevel,
                                               int lowerInitiativeLevel,
                                               const game::IdList* editorModifiers,
                                               int damageMax)
{
    using namespace utils;

    AttackDescriptor actual(descriptor, AttackType::Primary, false);
    AttackDescriptor global(descriptor, AttackType::Primary, true);
    AttackDescriptor actual2(descriptor, AttackType::Secondary, false);
    AttackDescriptor global2(descriptor, AttackType::Secondary, true);
    AttackDescriptor actualAlt(descriptor, AttackType::Alternative, false);
    AttackDescriptor globalAlt(descriptor, AttackType::Alternative, true);

    using namespace game;

    auto description = getInterfaceText("X005TA0424"); // "%PART1%%PART2%"

    // \s110;
    // \fMedBold;Attack:\t\p110;\fNormal;%TWICE%%ALTATTACK%%ATTACK%%SECOND%\p0;\n
    // \fMedBold;Chances to hit:\t\fNormal;%HIT%%HIT2%\n
    replace(description, "%PART1%", getInterfaceText("X005TA0787"));

    // \fMedBold;%EFFECT%:\t\fNormal;%DAMAGE%\n
    // \fMedBold;Source:\t\fNormal;%SOURCE%%SOURCE2%\n
    // \fMedBold;Initiative:\t\fNormal;%INIT%\n
    // \fMedBold;Reach:\t\fNormal;%REACH%\n
    // \fMedBold;Targets:\t\fNormal;%TARGETS%
    replace(description, "%PART2%", getInterfaceText("X005TA0788"));

    replace(description, "%TWICE%", getTwiceField(descriptor));

    replace(description, "%ALTATTACK%", getAltAttackField(actualAlt, globalAlt));

    replace(description, "%ATTACK%", getAttackField(actual, global));

    replace(description, "%SECOND%", getSecondField(actual2, global2));

    replace(description, "%HIT%", getHitField(actual, global, editorModifiers));

    replace(description, "%HIT2%", getHit2Field(actual2, global2));

    replace(description, "%EFFECT%", getEffectField(actual));

    replace(description, "%DAMAGE%",
            getDamageField(actual, global, actual2, global2, editorModifiers, boostDamageLevel,
                           lowerDamageLevel, damageMax));

    replace(description, "%SOURCE%", getSourceField(actual, global, actualAlt, globalAlt));

    replace(description, "%SOURCE2%", getSource2Field(actual2, global2));

    replace(description, "%INIT%",
            getInitField(actual, global, editorModifiers, lowerInitiativeLevel));

    replace(description, "%REACH%", getReachField(actual, global));

    replace(description, "%TARGETS%", getTargetsField(actual, global));

    auto textBox = CDialogInterfApi::get().findTextBox(dialog, "TXT_ATTACK_INFO");
    CTextBoxInterfApi::get().setString(textBox, description.c_str());
}

game::String* __stdcall getAttackSourceTextHooked(game::String* value,
                                                  const game::LAttackSource* attackSource)
{
    const auto& stringApi = game::StringApi::get();
    stringApi.initFromString(value, getAttackSourceText(attackSource->id).c_str());
    return value;
}

void __stdcall appendAttackSourceTextHooked(const game::LAttackSource* attackSource,
                                            game::String* value,
                                            bool* valueIsNotEmpty)
{
    const auto& stringApi = game::StringApi::get();

    auto text = getAttackSourceText(attackSource->id);

    if (*valueIsNotEmpty)
        stringApi.append(value, ", ", strlen(", "));
    stringApi.append(value, text.c_str(), text.length());
    *valueIsNotEmpty = true;
}

} // namespace hooks
