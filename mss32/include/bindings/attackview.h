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

#ifndef ATTACKVIEW_H
#define ATTACKVIEW_H

#include "idview.h"

namespace sol {
class state;
}

namespace game {
struct IAttack;
}

namespace bindings {

class ModifierView;

class AttackView
{
public:
    AttackView(const game::IAttack* attack);

    static void bind(sol::state& lua);

    IdView getId() const;
    std::string getName() const;
    std::string getDescription() const;
    /** Returns attack class id. */
    int getAttackClass() const;
    /** Returns attack source id. */
    int getAttackSource() const;
    /** Returns attack initiative value. */
    int getInitiative() const;
    /** Returns attack power (accuracy). */
    int getPower() const;
    /** Returns attack reach id. */
    int getReach() const;
    /** Returns damage the attack can inflict. */
    int getDamage() const;
    /** Returns healing the attack can apply. */
    int getHeal() const;
    /** Returns true if attack has long effect duration. */
    bool isInfinite() const;
    /** Returns true if attack can apply critical damage. */
    bool canCrit() const;
    /** Returns level for boost damage, lower damage and lower initiative attacks. */
    int getLevel() const;
    /** Returns list of modifiers applied by bestow wards attack. */
    std::vector<ModifierView> getWards() const;
    /** Returns which portion of the specified damage is returned as drain. */
    int getDrain(int damage) const;

    AttackView getGlobal() const;
    AttackView getGenerated() const;

    /** Custom attack data. */
    bool isMelee() const;
    int maxTargets() const;
    int critDamage() const;
    int critPower() const;
    int damageRatio() const;
    bool damageRatioPerTarget() const;
    bool damageSplit() const;

    const game::IAttack* getAttack() const;

private:
    const game::IAttack* attack;
};

} // namespace bindings

#endif // ATTACKVIEW_H
