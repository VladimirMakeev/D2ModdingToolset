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

#include "attackview.h"
#include "attack.h"
#include "attackimpl.h"
#include "attackutils.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "modifierutils.h"
#include "modifierview.h"
#include "unitmodifier.h"
#include <sol/sol.hpp>

namespace bindings {

AttackView::AttackView(const game::IAttack* attack)
    : attack{attack}
{ }

void AttackView::bind(sol::state& lua)
{
    auto attackView = lua.new_usertype<AttackView>("AttackView");
    attackView["id"] = sol::property(&AttackView::getId);
    attackView["name"] = sol::property(&AttackView::getName);
    attackView["description"] = sol::property(&AttackView::getDescription);
    attackView["type"] = sol::property(&AttackView::getAttackClass);
    attackView["source"] = sol::property(&AttackView::getAttackSource);
    attackView["initiative"] = sol::property(&AttackView::getInitiative);
    attackView["power"] = sol::property(&AttackView::getPower);
    attackView["reach"] = sol::property(&AttackView::getReach);
    attackView["damage"] = sol::property(&AttackView::getDamage);
    attackView["heal"] = sol::property(&AttackView::getHeal);
    attackView["infinite"] = sol::property(&AttackView::isInfinite);
    attackView["crit"] = sol::property(&AttackView::canCrit);
    attackView["level"] = sol::property(&AttackView::getLevel);
    attackView["wards"] = sol::property(&AttackView::getWards);
    attackView["getDrain"] = &AttackView::getDrain;

    attackView["global"] = sol::property(&AttackView::getGlobal);
    attackView["generated"] = sol::property(&AttackView::getGenerated);

    attackView["melee"] = sol::property(&AttackView::isMelee);
    attackView["maxTargets"] = sol::property(&AttackView::maxTargets);
    attackView["critDamage"] = sol::property(&AttackView::critDamage);
    attackView["critPower"] = sol::property(&AttackView::critPower);
    attackView["damageRatio"] = sol::property(&AttackView::damageRatio);
    attackView["damageRatioPerTarget"] = sol::property(&AttackView::damageRatioPerTarget);
    attackView["damageSplit"] = sol::property(&AttackView::damageSplit);
}

IdView AttackView::getId() const
{
    return attack->id;
}

std::string AttackView::getName() const
{
    return attack->vftable->getName(attack);
}

std::string AttackView::getDescription() const
{
    return attack->vftable->getDescription(attack);
}

int AttackView::getAttackClass() const
{
    auto attackClass{attack->vftable->getAttackClass(attack)};
    if (!attackClass) {
        return 0;
    }

    return static_cast<int>(attackClass->id);
}

int AttackView::getAttackSource() const
{
    auto attackSource{attack->vftable->getAttackSource(attack)};
    if (!attackSource) {
        return 0;
    }

    return static_cast<int>(attackSource->id);
}

int AttackView::getInitiative() const
{
    return attack->vftable->getInitiative(attack);
}

int AttackView::getPower() const
{
    int power{};
    attack->vftable->getPower(attack, &power);

    return power;
}

int AttackView::getReach() const
{
    auto reach{attack->vftable->getAttackReach(attack)};
    if (!reach) {
        return 0;
    }

    return static_cast<int>(reach->id);
}

int AttackView::getDamage() const
{
    return attack->vftable->getQtyDamage(attack);
}

int AttackView::getHeal() const
{
    return attack->vftable->getQtyHeal(attack);
}

bool AttackView::isInfinite() const
{
    return attack->vftable->getInfinite(attack);
}

bool AttackView::canCrit() const
{
    return attack->vftable->getCritHit(attack);
}

int AttackView::getLevel() const
{
    return attack->vftable->getLevel(attack);
}

std::vector<ModifierView> AttackView::getWards() const
{
    std::vector<ModifierView> result;

    const auto wards = attack->vftable->getWards(attack);
    for (const game::CMidgardID* modifierId = wards->bgn; modifierId != wards->end; modifierId++) {
        auto modifier = hooks::getUnitModifier(modifierId);
        result.push_back(ModifierView{modifier->data->modifier});
    }

    return result;
}

int AttackView::getDrain(int damage) const
{
    return attack->vftable->getDrain(attack, damage);
}

AttackView AttackView::getGlobal() const
{
    return hooks::getGlobalAttackImpl(&attack->id);
}

AttackView AttackView::getGenerated() const
{
    return hooks::getAttackImpl(&attack->id);
}

bool AttackView::isMelee() const
{
    return hooks::isMeleeAttack(attack);
}

int AttackView::maxTargets() const
{
    auto reach{attack->vftable->getAttackReach(attack)};
    return hooks::getAttackMaxTargets(reach->id);
}

int AttackView::critDamage() const
{
    return hooks::getCustomAttackData(attack).critDamage;
}

int AttackView::critPower() const
{
    return hooks::getCustomAttackData(attack).critPower;
}

int AttackView::damageRatio() const
{
    return hooks::getCustomAttackData(attack).damageRatio;
}

bool AttackView::damageRatioPerTarget() const
{
    return hooks::getCustomAttackData(attack).damageRatioPerTarget;
}

bool AttackView::damageSplit() const
{
    return hooks::getCustomAttackData(attack).damageSplit;
}

const game::IAttack* AttackView::getAttack() const
{
    return attack;
}

} // namespace bindings
