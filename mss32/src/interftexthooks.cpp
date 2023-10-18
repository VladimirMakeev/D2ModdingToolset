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
#include "dynupgrade.h"
#include "encunitdescriptor.h"
#include "interfaceutils.h"
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

std::string getMaxDamageText(const std::string& value)
{
    // "%DMG% \c000;000;000;(\c128;000;000;Max\c000;000;000;)"
    auto result = getInterfaceText("X005TA0811");
    replace(result, "%DMG%", value);
    return result;
}

std::string getDamageText(int value, int base, int max)
{
    auto result = getModifiedNumberText(value, base, false);
    return value >= max ? getMaxDamageText(result) : result;
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

std::string getDrainEffectText()
{
    auto text = getInterfaceText(textIds().interf.drainEffect.c_str());
    if (text.length())
        return text;

    return getInterfaceText("X005TA0792"); // "Drain"
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

std::string getDurationText()
{
    auto text = getInterfaceText(textIds().interf.durationText.c_str());
    if (text.length())
        return text;

    return "Duration";
}

std::string getInstantDurationText()
{
    auto text = getInterfaceText(textIds().interf.instantDurationText.c_str());
    if (text.length())
        return text;

    return "Instant";
}

std::string getRandomDurationText()
{
    auto text = getInterfaceText(textIds().interf.randomDurationText.c_str());
    if (text.length())
        return text;

    return "Random";
}

std::string getSingleTurnDurationText()
{
    auto text = getInterfaceText(textIds().interf.singleTurnDurationText.c_str());
    if (text.length())
        return text;

    return "Single turn";
}

std::string getWholeBattleDurationText()
{
    auto text = getInterfaceText(textIds().interf.wholeBattleDurationText.c_str());
    if (text.length())
        return text;

    return "Whole battle";
}

std::string getOverflowText()
{
    auto text = getInterfaceText(textIds().interf.overflowAttack.c_str());
    if (text.length())
        return text;

    return "Overflow";
}

std::string getInfiniteText()
{
    auto text = getInterfaceText(textIds().interf.infiniteAttack.c_str());
    if (text.length())
        return text;

    return "Lasting";
}

std::string getAttackDurationText(const utils::AttackDescriptor& actual,
                                  const utils::AttackDescriptor& global)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (actual.classId() == classes.paralyze->id || actual.classId() == classes.petrify->id
        || actual.classId() == classes.poison->id || actual.classId() == classes.frostbite->id
        || actual.classId() == classes.blister->id) {
        return getModifiedStringText(actual.infinite() ? getRandomDurationText()
                                                       : getSingleTurnDurationText(),
                                     actual.infinite() != global.infinite());
    } else if (actual.classId() == classes.boostDamage->id
               || actual.classId() == classes.lowerDamage->id
               || actual.classId() == classes.lowerInitiative->id) {
        return getModifiedStringText(actual.infinite() ? getWholeBattleDurationText()
                                                       : getSingleTurnDurationText(),
                                     actual.infinite() != global.infinite());
    } else if (actual.classId() == classes.transformOther->id) {
        return getModifiedStringText(actual.infinite() ? getRandomDurationText()
                                                       : getWholeBattleDurationText(),
                                     actual.infinite() != global.infinite());
    } else {
        return getInstantDurationText();
    }
}

std::string addOverflowText(const std::string& base,
                            const utils::AttackDescriptor& actual,
                            const utils::AttackDescriptor& global)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (actual.classId() != classes.drainOverflow->id)
        return base;

    auto text = getInterfaceText(textIds().interf.overflowText.c_str());
    if (text.empty())
        text = "%ATTACK% (%OVERFLOW%)";

    replace(text, "%ATTACK%", base);
    replace(text, "%OVERFLOW%",
            getModifiedStringText(getOverflowText(),
                                  global.classId() != classes.drainOverflow->id));
    return text;
}

std::string getRatedAttackDamageText(int damage, int critDamage, double ratio)
{
    auto result = getNumberText(applyAttackDamageRatio(damage, ratio), false);

    if (critDamage) {
        result = addCritHitText(result,
                                getNumberText(applyAttackDamageRatio(critDamage, ratio), false),
                                !userSettings().unitEncyclopedia.displayCriticalHitTextInAttackName);
    }

    return result;
}

std::string getRatedAttackDamageText(const std::string& base,
                                     const utils::AttackDescriptor& actual,
                                     int maxTargets)
{
    auto ratios = computeAttackDamageRatio(actual.custom(), maxTargets);
    if (ratios.size() < 2)
        return base;

    auto critDamage = actual.damage() * actual.critDamage() / 100;
    if (!actual.damageRatioPerTarget() && ratios.size() > 2) {
        auto result = getInterfaceText(textIds().interf.ratedDamageEqual.c_str());
        if (result.empty())
            result = "%DMG%, (%TARGETS%x) %RATED%";

        replace(result, "%DMG%", base);
        replace(result, "%TARGETS%", fmt::format("{:d}", ratios.size() - 1));
        replace(result, "%RATED%",
                getRatedAttackDamageText(actual.damage(), critDamage, ratios[1]));
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
            rated += getRatedAttackDamageText(actual.damage(), critDamage, *it);
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
                               const utils::AttackDescriptor& global)
{
    std::string result;
    if (actual.hasPower() && global.hasPower()) {
        result = getModifiedNumberText(actual.power(), global.power(), true);
    } else {
        result = getNumberText(actual.power(), true);
    }

    if (actual.critHit()) {
        result = addCritHitText(result,
                                getModifiedNumberText(actual.critPower(), global.critPower(), true),
                                !userSettings().unitEncyclopedia.displayCriticalHitTextInAttackName);
    }

    return result;
}

std::string getDamageDrainAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global,
                                           int maxTargets,
                                           int damageMax)
{
    int multiplier = actual.damageSplit() ? userSettings().splitDamageMultiplier : 1;
    auto result = getDamageText(actual.damage(), global.damage(), damageMax * multiplier);

    if (actual.critHit()) {
        result = addCritHitText(result,
                                getModifiedNumberText(actual.damage() * actual.critDamage() / 100,
                                                      actual.damage() * global.critDamage() / 100,
                                                      false),
                                !userSettings().unitEncyclopedia.displayCriticalHitTextInAttackName);
    }

    if (maxTargets < 2)
        return result;
    else if (actual.damageSplit()) {
        return getSplitAttackDamageText(result);
    } else {
        return getRatedAttackDamageText(result, actual, maxTargets);
    }
}

std::string getBoostDamageAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global)
{
    int boost = global.boost() ? global.boost() : actual.boost();

    auto result = getInterfaceText("X005TA0535"); // "+%BOOST%%"
    replace(result, "%BOOST%", getNumberText(boost, false));

    return addBonusNumberText(result, actual.boost() - boost, true, false);
}

std::string getLowerDamageAttackDamageText(const utils::AttackDescriptor& actual,
                                           const utils::AttackDescriptor& global)
{
    int lower = global.lower() ? global.lower() : actual.lower();

    auto result = getInterfaceText("X005TA0550"); // "-%LOWER%%"
    replace(result, "%LOWER%", getNumberText(lower, false));

    return addBonusNumberText(result, lower - actual.lower(), true, true);
}

std::string getLowerInitiativeAttackDamageText(const utils::AttackDescriptor& actual,
                                               const utils::AttackDescriptor& global)
{
    int lower = global.lowerIni() ? global.lowerIni() : actual.lowerIni();

    auto result = getInterfaceText("X005TA0550"); // "-%LOWER%%"
    replace(result, "%LOWER%", getNumberText(lower, false));

    return addBonusNumberText(result, lower - actual.lowerIni(), true, true);
}

std::string getAttackDamageText(const utils::AttackDescriptor& actual,
                                const utils::AttackDescriptor& global,
                                int maxTargets,
                                int damageMax)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (actual.classId() == classes.boostDamage->id) {
        return getBoostDamageAttackDamageText(actual, global);
    } else if (actual.classId() == classes.lowerDamage->id) {
        return getLowerDamageAttackDamageText(actual, global);
    } else if (actual.classId() == classes.lowerInitiative->id) {
        return getLowerInitiativeAttackDamageText(actual, global);
    } else if (actual.classId() == classes.damage->id || actual.classId() == classes.drain->id
               || actual.classId() == classes.drainOverflow->id) {
        return getDamageDrainAttackDamageText(actual, global, maxTargets, damageMax);
    } else if (actual.classId() == classes.heal->id
               || actual.classId() == classes.bestowWards->id) {
        return getModifiedNumberText(actual.heal(), global.heal(), false);
    } else if (actual.classId() == classes.poison->id || actual.classId() == classes.frostbite->id
               || actual.classId() == classes.blister->id) {
        return getNumberText(global.damage(), false);
    } else {
        return getDamageText(actual.damage(), global.damage(), damageMax);
    }
}

std::string getAttackDrainText(const utils::AttackDescriptor& actual,
                               const utils::AttackDescriptor& global)
{
    auto result = getModifiedNumberText(actual.drain(), global.drain(), false);

    return addOverflowText(result, actual, global);
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
    auto name = getModifiedStringText(actual.name(), actual.name() != global.name());
    if (actual.critHit() && userSettings().unitEncyclopedia.displayCriticalHitTextInAttackName) {
        return addCritHitText(name,
                              getModifiedStringText(getCritHitText(), !global.critHit()),
                              false);
    }
    if (!actual.infinite() || !userSettings().unitEncyclopedia.displayInfiniteAttackIndicator) {
        return name;
    }

    auto result = getInterfaceText(textIds().interf.infiniteText.c_str());
    if (result.empty())
        result = "%ATTACK% (%INFINITE%)";

    replace(result, "%ATTACK%", name);
    replace(result, "%INFINITE%", getModifiedStringText(getInfiniteText(), !global.infinite()));
    return result;
}

void addDynUpgradeText(std::string& description,
                       game::IEncUnitDescriptor* descriptor,
                       const utils::AttackDescriptor& actual,
                       const utils::AttackDescriptor& actual2)
{
    using namespace game;

    const CDynUpgrade* upgrade1 = nullptr;
    const CDynUpgrade* upgrade2 = nullptr;
    if (!getDynUpgradesToDisplay(descriptor, &upgrade1, &upgrade2)) {
        return;
    }

    if (actual.damage() || actual2.damage()) {
        addDynUpgradeTextToField(description, "%DAMAGE%", upgrade1->damage, upgrade2->damage);
    } else if (actual.heal() || actual2.heal()) {
        addDynUpgradeTextToField(description, "%DAMAGE%", upgrade1->heal, upgrade2->heal);
    }

    if (actual.hasPower() || actual2.hasPower()) {
        addDynUpgradeTextToField(description, "%HIT2%", upgrade1->power, upgrade2->power);
    }

    addDynUpgradeTextToField(description, "%INIT%", upgrade1->initiative, upgrade2->initiative);
}

std::string getTwiceField(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    if (!descriptor->vftable->attacksTwice(descriptor))
        return "";

    auto result = getInterfaceText("X005TA0786"); // "(2x) %BLANK%"
    replace(result, "%BLANK%", "");

    bool modified = false;
    auto midUnitDescriptor = castToMidUnitDescriptor(descriptor);
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
                        const utils::AttackDescriptor& global)
{
    return getAttackPowerText(actual, global);
}

std::string getHit2Field(const utils::AttackDescriptor& actual,
                         const utils::AttackDescriptor& global)
{
    if (actual.empty())
        return "";

    auto result = getInterfaceText("X005TA0881"); // " / %POWER%"
    replace(result, "%POWER%", getAttackPowerText(actual, global));
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
                           int damageMax)
{
    auto maxTargets = getAttackMaxTargets(actual.reachId());
    auto result = getAttackDamageText(actual, global, maxTargets, damageMax);

    if (!actual2.empty()) {
        auto damage2 = getAttackDamageText(actual2, global2, maxTargets, damageMax);
        if (result == "0")
            result = damage2;
        else if (damage2 != "0")
            result = addAttack2Text(result, damage2);
    }

    return result;
}

std::string getDrainField(const utils::AttackDescriptor& actual,
                          const utils::AttackDescriptor& global,
                          const utils::AttackDescriptor& actual2,
                          const utils::AttackDescriptor& global2)
{
    if (!actual.drain() && !actual2.drain())
        return "";

    auto result = getInterfaceText(textIds().interf.drainDescription.c_str());
    if (result.empty())
        result = "\\fMedBold;%DRAINEFFECT%:\\t\\fNormal;%DRAIN%\\n";

    auto drain = getAttackDrainText(actual, global);
    if (!actual2.empty()) {
        auto drain2 = getAttackDrainText(actual2, global2);
        if (drain == "0")
            drain = drain2;
        else if (drain2 != "0")
            drain = addAttack2Text(drain, drain2);
    }

    replace(result, "%DRAINEFFECT%", getDrainEffectText());
    replace(result, "%DRAIN%", drain);

    return result;
}

std::string getDurationField(const utils::AttackDescriptor& actual,
                             const utils::AttackDescriptor& global,
                             const utils::AttackDescriptor& actual2,
                             const utils::AttackDescriptor& global2)
{
    using namespace game;

    const auto& classes = AttackClassCategories::get();

    if (!actual.hasInfinite() && !actual2.hasInfinite()) {
        return "";
    }

    auto result = getInterfaceText(textIds().interf.durationDescription.c_str());
    if (result.empty()) {
        result = "\\fMedBold;%DURATION%:\\t\\fNormal;%DURATIONVALUE%\\n";
    }

    auto duration = getAttackDurationText(actual, global);
    if (!actual2.empty()) {
        duration = addAttack2Text(duration, getAttackDurationText(actual2, global2));
    }

    replace(result, "%DURATION%", getDurationText());
    replace(result, "%DURATIONVALUE%", duration);

    return result;
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
                         const utils::AttackDescriptor& global)
{
    return getModifiedNumberText(actual.initiative(), global.initiative(), false);
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

    AttackDescriptor actual(descriptor, AttackType::Primary, false, boostDamageLevel,
                            lowerDamageLevel, lowerInitiativeLevel, nullptr, damageMax);
    AttackDescriptor global(descriptor, AttackType::Primary, true, 0, 0, 0, editorModifiers,
                            damageMax);
    AttackDescriptor actual2(descriptor, AttackType::Secondary, false, boostDamageLevel,
                             lowerDamageLevel, 0, nullptr, damageMax);
    AttackDescriptor global2(descriptor, AttackType::Secondary, true);
    AttackDescriptor actualAlt(descriptor, AttackType::Alternative, false);
    AttackDescriptor globalAlt(descriptor, AttackType::Alternative, true);

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
    addDynUpgradeText(description, descriptor, actual, actual2);
    replace(description, "%TWICE%", getTwiceField(descriptor));
    replace(description, "%ALTATTACK%", getAltAttackField(actualAlt, globalAlt));
    replace(description, "%ATTACK%", getAttackField(actual, global));
    replace(description, "%SECOND%", getSecondField(actual2, global2));
    replace(description, "%HIT%", getHitField(actual, global));
    replace(description, "%HIT2%", getHit2Field(actual2, global2));
    replace(description, "%EFFECT%", getEffectField(actual));
    replace(description, "%DURATION%", getDurationField(actual, global, actual2, global2));
    replace(description, "%DAMAGE%", getDamageField(actual, global, actual2, global2, damageMax));
    replace(description, "%DRAIN%", getDrainField(actual, global, actual2, global2));
    replace(description, "%SOURCE%", getSourceField(actual, global, actualAlt, globalAlt));
    replace(description, "%SOURCE2%", getSource2Field(actual2, global2));
    replace(description, "%INIT%", getInitField(actual, global));
    replace(description, "%REACH%", getReachField(actual, global));
    replace(description, "%TARGETS%", getTargetsField(actual, global));

    const game::CDynUpgrade* upgrade1 = nullptr;
    const game::CDynUpgrade* upgrade2 = nullptr;
    if (getDynamicUpgrades(descriptor, &upgrade1, &upgrade2)) {
        writeDynUpgradeText(description, "%UPGINIT%", upgrade1->initiative, upgrade2->initiative);
        if (actual.damage() || actual2.damage()) {
            writeDynUpgradeText(description, "%UPGDAMAGE%", upgrade1->damage, upgrade2->damage);
        } else if (actual.heal() || actual2.heal()) {
            writeDynUpgradeText(description, "%UPGDAMAGE%", upgrade1->heal, upgrade2->heal);
        } else {
            writeDynUpgradeText(description, "%UPGDAMAGE%", 0, 0);
        }
        if (actual.hasPower() || actual2.hasPower()) {
            writeDynUpgradeText(description, "%UPGHIT%", upgrade1->power, upgrade2->power);
        } else {
            writeDynUpgradeText(description, "%UPGHIT%", 0, 0);
        }
    }

    auto textBox = game::CDialogInterfApi::get().findTextBox(dialog, "TXT_ATTACK_INFO");
    game::CTextBoxInterfApi::get().setString(textBox, description.c_str());
}

game::String* __stdcall getAttackSourceTextHooked(game::String* value,
                                                  const game::LAttackSource* attackSource)
{
    const auto& stringApi = game::StringApi::get();
    stringApi.initFromString(value, getAttackSourceText(attackSource).c_str());
    return value;
}

void __stdcall appendAttackSourceTextHooked(const game::LAttackSource* attackSource,
                                            game::String* value,
                                            bool* valueIsNotEmpty)
{
    const auto& stringApi = game::StringApi::get();

    auto text = getAttackSourceText(attackSource);

    if (*valueIsNotEmpty)
        stringApi.append(value, ", ", strlen(", "));
    stringApi.append(value, text.c_str(), text.length());
    *valueIsNotEmpty = true;
}

} // namespace hooks
