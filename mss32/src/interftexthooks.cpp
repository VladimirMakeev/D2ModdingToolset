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
#include "attackimpl.h"
#include "attackutils.h"
#include "dialoginterf.h"
#include "encunitdescriptor.h"
#include "game.h"
#include "hooks.h"
#include "settings.h"
#include "textboxinterf.h"
#include "textids.h"
#include "ummodifier.h"
#include "unitutils.h"
#include "utils.h"
#include <fmt/format.h>

namespace hooks {

game::IAttack* getAttack(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    CMidgardID attackId{};
    descriptor->vftable->getAttackId(descriptor, &attackId);

    auto attack = getAttack(&attackId);
    if (attack == nullptr) {
        generateUnitImplByAttackId(&attackId);
        attack = getAttack(&attackId);
    }

    return attack;
}

game::IAttack* getAttack2(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    CMidgardID attack2Id{};
    descriptor->vftable->getAttack2Id(descriptor, &attack2Id);
    return getAttack(&attack2Id);
}

game::IAttack* getAltAttack(game::IEncUnitDescriptor* descriptor)
{
    using namespace game;

    CMidgardID altAttackId{};
    descriptor->vftable->getAltAttackId(descriptor, &altAttackId);
    return getAttack(&altAttackId);
}

std::string appendColoredBonus(const std::string& base, int diff)
{
    if (diff == 0)
        return base;

    auto text = base;
    // "\c025;090;000; + %NUMBER%"
    // "\c100;000;000; - %NUMBER%"
    text += getTranslatedText(diff > 0 ? "X005TA0486" : "X005TA0487");
    // "\c000;000;000;"
    text += getTranslatedText("X005TA0488");
    replace(text, "%NUMBER%", fmt::format("{:d}", abs(diff)));

    return text;
}

std::string getAttackPowerText(game::IAttack* attack,
                               const game::IdList* modifiers,
                               int attackPowerTotal)
{
    using namespace game;

    std::string result;
    if (isAttackClassUsesAccuracy(attack->vftable->getAttackClass(attack))) {
        int attackPower;
        attack->vftable->getPower(attack, &attackPower);

        if (modifiers != nullptr) {
            const auto& fn = gameFunctions();
            int attackPowerModified = std::clamp(
                fn.applyPercentModifiers(attackPower, modifiers, ModifierElementTypeFlag::Power),
                attackPowerLimits.min, attackPowerLimits.max);

            result = appendColoredBonus(fmt::format("{:d}%", attackPowerModified),
                                        attackPowerTotal - attackPowerModified);
        } else
            result = fmt::format("{:d}%", attackPower);
    } else
        result = "100%";

    if (userSettings().criticalHitChance != 100 && attack->vftable->getCritHit(attack)) {
        return fmt::format("{:s} ({:d}%)", result.c_str(), userSettings().criticalHitChance);
    } else
        return result;
}

std::string getAttackDamageMaxText(const std::string& damage)
{
    // "%DMG% \c000;000;000;(\c128;000;000;Max\c000;000;000;)"
    auto max = getTranslatedText("X005TA0811");
    replace(max, "%DMG%", damage);
    return max;
}

std::string getAttackDamageModifiableText(game::IAttack* attack,
                                          const game::IdList* modifiers,
                                          int damageTotal,
                                          int boostDamageLevel,
                                          int lowerDamageLevel,
                                          int damageMax)
{
    using namespace game;

    int damage = getAttackQtyDamageOrHealHooked(attack, damageMax);

    const auto& fn = gameFunctions();
    int damageModified = fn.applyPercentModifiers(damage, modifiers,
                                                  ModifierElementTypeFlag::QtyDamage);
    if (damageModified > damageMax)
        damageModified = damageMax;

    int damageTotalBoosted = damageTotal;
    damageTotalBoosted += damageTotal * getBoostDamage(boostDamageLevel) / 100;
    damageTotalBoosted -= damageTotal * getLowerDamage(lowerDamageLevel) / 100;
    if (damageTotalBoosted > damageMax)
        damageTotalBoosted = damageMax;

    auto damagePlusBonus = appendColoredBonus(fmt::format("{:d}", damageModified),
                                              damageTotalBoosted - damageModified);

    auto damagePlusBonusPlusMax = damageTotalBoosted < damageMax
                                      ? damagePlusBonus
                                      : getAttackDamageMaxText(damagePlusBonus);

    if (attack->vftable->getCritHit(attack)) {
        int critDamage = damageTotalBoosted * userSettings().criticalHitDamage / 100;
        return fmt::format("{:s} ({:d})", damagePlusBonusPlusMax.c_str(), critDamage);
    }

    return damagePlusBonusPlusMax;
}

std::string getAttackDamageText(game::IAttack* attack,
                                const game::IdList* modifiers,
                                int damageTotal,
                                int boostDamageLevel,
                                int lowerDamageLevel,
                                int damageMax)
{
    using namespace game;

    std::string text;
    auto id = attack->vftable->getAttackClass(attack)->id;
    const auto& attackClasses = AttackClassCategories::get();
    if (id == attackClasses.boostDamage->id) {
        int level = attack->vftable->getLevel(attack);
        text = getTranslatedText("X005TA0535"); // "+%BOOST%%"
        replace(text, "%BOOST%", fmt::format("{:d}", getBoostDamage(level)));
    } else if (id == attackClasses.lowerDamage->id) {
        int level = attack->vftable->getLevel(attack);
        text = getTranslatedText("X005TA0550"); // "-%LOWER%%"
        replace(text, "%LOWER%", fmt::format("{:d}", getLowerDamage(level)));
    } else if (id == attackClasses.lowerInitiative->id) {
        int level = attack->vftable->getLevel(attack);
        text = getTranslatedText("X005TA0550"); // "-%LOWER%%"
        replace(text, "%LOWER%", fmt::format("{:d}", getLowerInitiative(level)));
    } else if (id == attackClasses.damage->id || id == attackClasses.drain->id
               || id == attackClasses.drainOverflow->id) {
        text = getAttackDamageModifiableText(attack, modifiers, damageTotal, boostDamageLevel,
                                             lowerDamageLevel, damageMax);
    } else {
        int damage = getAttackQtyDamageOrHealHooked(attack, damageMax);
        auto dmg = fmt::format("{:d}", damage);
        text = damage < damageMax ? dmg : getAttackDamageMaxText(dmg);
    }

    return text;
}

std::string getAttackInitiativeText(game::IAttack* attack,
                                    const game::IdList* modifiers,
                                    int initiativeTotal,
                                    int lowerInitiativeLevel)
{
    using namespace game;

    int initiative = attack->vftable->getInitiative(attack);

    const auto& fn = gameFunctions();
    int initiativeModified = std::clamp(
        fn.applyPercentModifiers(initiative, modifiers, ModifierElementTypeFlag::Initiative),
        attackInitiativeLimits.min, attackInitiativeLimits.max);

    int initiativeTotalBoosted = initiativeTotal;
    initiativeTotalBoosted -= initiativeTotal * getLowerInitiative(lowerInitiativeLevel) / 100;

    return appendColoredBonus(fmt::format("{:d}", initiativeModified),
                              initiativeTotalBoosted - initiativeModified);
}

std::string getAttackSourceText(game::IAttack* attack)
{
    using namespace game;

    const LAttackSource* attackSource = attack->vftable->getAttackSource(attack);
    if (attackSource == nullptr)
        return getTranslatedText("X005TA0473"); // "None"

    std::string text;
    AttackSourceId id = attackSource->id;
    const auto& attackSources = AttackSourceCategories::get();
    if (id == attackSources.weapon->id)
        return getTranslatedText("X005TA0145"); // "Weapon"
    else if (id == attackSources.mind->id)
        return getTranslatedText("X005TA0146"); // "Mind"
    else if (id == attackSources.life->id)
        return getTranslatedText("X005TA0147"); // "Life"
    else if (id == attackSources.death->id)
        return getTranslatedText("X005TA0148"); // "Death"
    else if (id == attackSources.fire->id)
        return getTranslatedText("X005TA0149"); // "Fire"
    else if (id == attackSources.water->id)
        return getTranslatedText("X005TA0150"); // "Water"
    else if (id == attackSources.air->id)
        return getTranslatedText("X005TA0151"); // "Air"
    else if (id == attackSources.earth->id)
        return getTranslatedText("X005TA0152"); // "Earth"
    else
        return "";
}

std::string getAttackReachText(game::IAttack* attack)
{
    using namespace game;

    auto reach = attack->vftable->getAttackReach(attack);
    if (reach->id == AttackReachCategories::get().adjacent->id)
        return getTranslatedText("X005TA0201"); // "Adjacent units"
    else
        return getTranslatedText("X005TA0200"); // "Any unit"
}

std::string getAttackTargetsText(game::IAttack* attack)
{
    using namespace game;

    auto reach = attack->vftable->getAttackReach(attack);
    if (reach->id == AttackReachCategories::get().all->id)
        return getTranslatedText("X005TA0674"); // "6"
    else
        return getTranslatedText("X005TA0675"); // "1"
}

std::string getInfiniteText()
{
    using namespace game;

    auto text = getTranslatedText(textIds().interf.infiniteAttack.c_str());
    if (text.length())
        return text;

    return "Lasting";
}

std::string getCritHitText()
{
    using namespace game;

    auto text = getTranslatedText(textIds().interf.critHitAttack.c_str());
    if (text.length())
        return text;

    return getTranslatedText("X160TA0017"); // "Critical hit"
}

std::string getAttackName(game::IAttack* attack)
{
    using namespace game;

    auto name = attack->vftable->getName(attack);
    if (attack->vftable->getInfinite(attack))
        return fmt::format("{:s} ({:s})", name, getInfiniteText());
    else if (attack->vftable->getCritHit(attack))
        return fmt::format("{:s} ({:s})", name, getCritHitText());
    else
        return name;
}

std::string addAltAttackTextValue(const std::string& value,
                                  const std::string& altValue,
                                  bool omitDuplicate = true)
{
    if (omitDuplicate && value == altValue)
        return value;

    auto result = getTranslatedText("X005TA0829"); // "%ATTACK% or %BLANK%"
    replace(result, "%ATTACK%", altValue);
    replace(result, "%BLANK%", value);
    return result;
}

std::string addAttack2TextValue(const std::string& value,
                                const std::string& value2,
                                bool omitDuplicate = true)
{
    if (omitDuplicate && value == value2)
        return value;

    auto result = getTranslatedText("X005TA0785"); // " / %ATTACK%"
    replace(result, "%ATTACK%", value2);
    result.insert(0, value);
    return result;
}

std::string getEffectText(game::IAttack* attack)
{
    using namespace game;

    auto id = attack->vftable->getAttackClass(attack)->id;
    const auto& attackClasses = AttackClassCategories::get();
    if (id == attackClasses.heal->id)
        return getTranslatedText("X005TA0504"); // "Heal"
    else if (id == attackClasses.bestowWards->id)
        return getTranslatedText("X005TA0504"); // "Heal"
    else if (id == attackClasses.boostDamage->id)
        return getTranslatedText("X005TA0534"); // "Boost"
    else if (id == attackClasses.lowerDamage->id)
        return getTranslatedText("X005TA0547"); // "Lower"
    else if (id == attackClasses.lowerInitiative->id)
        return getTranslatedText("X005TA0551"); // "Lower initiative"
    else
        return getTranslatedText("X005TA0503"); // "Damage"
}

std::string getTwiceText(game::IEncUnitDescriptor* descriptor)
{
    if (!descriptor->vftable->attacksTwice(descriptor))
        return "";

    auto result = getTranslatedText("X005TA0786"); // "(2x) %BLANK%"
    replace(result, "%BLANK%", "");
    return result;
}

std::string getAltAttackText(game::IAttack* altAttack)
{
    if (altAttack == nullptr)
        return "";

    auto result = getTranslatedText("X005TA0829"); // "%ATTACK% or %BLANK%"
    replace(result, "%ATTACK%", getAttackName(altAttack));
    replace(result, "%BLANK%", "");
    return result;
}

std::string getHitText(game::IEncUnitDescriptor* descriptor,
                       game::IAttack* attack,
                       game::IAttack* altAttack,
                       const game::IdList* modifiers)
{
    auto result = getAttackPowerText(attack, modifiers,
                                     descriptor->vftable->getAttackPower(descriptor));

    if (altAttack != nullptr
        && isAttackClassUsesAccuracy(altAttack->vftable->getAttackClass(altAttack))) {
        auto value = getAttackPowerText(altAttack, nullptr,
                                        descriptor->vftable->getAltAttackPower(descriptor));
        result = addAltAttackTextValue(result, value);
    }

    return result;
}

std::string getSourceText(game::IAttack* attack, game::IAttack* altAttack)
{
    auto result = getAttackSourceText(attack);

    if (altAttack != nullptr) {
        auto value = addAltAttackTextValue(result, getAttackSourceText(altAttack));
        result = fmt::format("\\p110;{:s}\\p0;", value); // Fixes vertical tab in case of multiline
    }

    return result;
}

std::string getReachText(game::IAttack* attack, game::IAttack* altAttack)
{
    auto result = getAttackReachText(attack);

    if (altAttack != nullptr) {
        auto value = addAltAttackTextValue(result, getAttackReachText(altAttack));
        result = fmt::format("\\p110;{:s}\\p0;", value); // Fixes vertical tab in case of multiline
    }

    return result;
}

std::string getTargetsText(game::IAttack* attack, game::IAttack* altAttack)
{
    auto result = getAttackTargetsText(attack);

    if (altAttack != nullptr)
        result = addAltAttackTextValue(result, getAttackTargetsText(altAttack));

    return result;
}

std::string getSecondText(game::IAttack* attack2)
{
    if (attack2 == nullptr)
        return "";

    auto result = getTranslatedText("X005TA0785"); // " / %ATTACK%"
    replace(result, "%ATTACK%", getAttackName(attack2));
    return result;
}

std::string getHit2Text(game::IEncUnitDescriptor* descriptor, game::IAttack* attack2)
{
    if (attack2 == nullptr)
        return "";

    auto power = getAttackPowerText(attack2, nullptr,
                                    descriptor->vftable->getAttack2Power(descriptor));

    auto result = getTranslatedText("X005TA0881"); // " / %POWER%"
    replace(result, "%POWER%", power);
    return result;
}

std::string getSource2Text(game::IAttack* attack2)
{
    if (attack2 == nullptr)
        return "";

    auto result = getTranslatedText("X005TA0816"); // " / %SOURCE%"
    replace(result, "%SOURCE%", getAttackSourceText(attack2));
    return result;
}

std::string getDamageText(game::IEncUnitDescriptor* descriptor,
                          game::IAttack* attack,
                          game::IAttack* attack2,
                          game::IAttack* altAttack,
                          const game::IdList* modifiers,
                          int boostDamageLevel,
                          int lowerDamageLevel,
                          int damageMax)
{
    auto result = getAttackDamageText(attack, modifiers,
                                      descriptor->vftable->getAttackDamageOrHeal(descriptor),
                                      boostDamageLevel, lowerDamageLevel, damageMax);

    if (attack2 != nullptr) {
        auto damage2 = getAttackDamageText(attack2, modifiers,
                                           getAttackQtyDamageOrHealHooked(attack2, damageMax),
                                           boostDamageLevel, lowerDamageLevel, damageMax);
        if (result == "0")
            result = damage2;
        else if (damage2 != "0")
            result = addAttack2TextValue(result, damage2, false);
    }

    if (altAttack != nullptr) {
        auto altDamage = getAttackDamageText(altAttack, modifiers,
                                             getAttackQtyDamageOrHealHooked(altAttack, damageMax),
                                             boostDamageLevel, lowerDamageLevel, damageMax);
        if (altDamage != "0")
            result = addAltAttackTextValue(result, altDamage, false);
    }

    return result;
}

std::string getInitText(game::IEncUnitDescriptor* descriptor,
                        game::IAttack* attack,
                        const game::IdList* modifiers,
                        int lowerInitiativeLevel)
{
    return getAttackInitiativeText(attack, modifiers,
                                   descriptor->vftable->getAttackInitiative(descriptor),
                                   lowerInitiativeLevel);
}

void __stdcall generateAttackDescriptionHooked(game::IEncUnitDescriptor* descriptor,
                                               game::CDialogInterf* dialog,
                                               int boostDamageLevel,
                                               int lowerDamageLevel,
                                               int lowerInitiativeLevel,
                                               const game::IdList* modifiers,
                                               int damageMax)
{
    using namespace game;

    auto attack = getAttack(descriptor);
    auto attack2 = getAttack2(descriptor);
    auto altAttack = getAltAttack(descriptor);

    auto description = getTranslatedText("X005TA0424"); // "%PART1%%PART2%"

    // \s110;
    // \fMedBold;Attack:\t\p110;\fNormal;%TWICE%%ALTATTACK%%ATTACK%%SECOND%\p0;\n
    // \fMedBold;Chances to hit:\t\fNormal;%HIT%%HIT2%\n
    replace(description, "%PART1%", getTranslatedText("X005TA0787"));

    // \fMedBold;%EFFECT%:\t\fNormal;%DAMAGE%\n
    // \fMedBold;Source:\t\fNormal;%SOURCE%%SOURCE2%\n
    // \fMedBold;Initiative:\t\fNormal;%INIT%\n
    // \fMedBold;Reach:\t\fNormal;%REACH%\n
    // \fMedBold;Targets:\t\fNormal;%TARGETS%
    replace(description, "%PART2%", getTranslatedText("X005TA0788"));

    replace(description, "%TWICE%", getTwiceText(descriptor));

    replace(description, "%ALTATTACK%", getAltAttackText(altAttack));

    replace(description, "%ATTACK%", getAttackName(attack));

    replace(description, "%SECOND%", getSecondText(attack2));

    replace(description, "%HIT%", getHitText(descriptor, attack, altAttack, modifiers));

    replace(description, "%HIT2%", getHit2Text(descriptor, attack2));

    replace(description, "%EFFECT%", getEffectText(attack));

    replace(description, "%DAMAGE%",
            getDamageText(descriptor, attack, attack2, altAttack, modifiers, boostDamageLevel,
                          lowerDamageLevel, damageMax));

    replace(description, "%SOURCE%", getSourceText(attack, altAttack));

    replace(description, "%SOURCE2%", getSource2Text(attack2));

    replace(description, "%INIT%",
            getInitText(descriptor, attack, modifiers, lowerInitiativeLevel));

    replace(description, "%REACH%", getReachText(attack, altAttack));

    replace(description, "%TARGETS%", getTargetsText(attack, altAttack));

    auto textBox = CDialogInterfApi::get().findTextBox(dialog, "TXT_ATTACK_INFO");
    CTextBoxInterfApi::get().setString(textBox, description.c_str());
}

} // namespace hooks
