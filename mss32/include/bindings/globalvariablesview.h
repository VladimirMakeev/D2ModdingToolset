/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef GLOBALVARIABLESVIEW_H
#define GLOBALVARIABLESVIEW_H

#include "currencyview.h"

namespace sol {
class state;
}

namespace game {
struct GlobalVariables;
}

namespace bindings {

class GlobalVariablesView
{
public:
    GlobalVariablesView(const game::GlobalVariables* variables);

    static void bind(sol::state& lua);

    int getMorale(int tier) const;
    int getWeapnMstr() const;

    int getBatInit() const;
    int getBatDamage() const;
    int getBatRound() const;
    int getBatBreak() const;
    int getBatBModif() const;

    int getBatBoostd(int level) const;
    int getBatLowerd(int level) const;
    int getBatLoweri() const;

    int getLdrMaxAbil() const;
    int getSpyDiscov() const;

    int getPoisonS() const;
    int getPoisonC() const;

    int getBribe() const;

    int getStealRace() const;
    int getStealNeut() const;

    int getRiotMin() const;
    int getRiotMax() const;
    int getRiotDmg() const;

    int getSellRatio() const;

    int getTCapture() const;
    int getTCapital() const;
    int getTCity(int tier) const;

    CurrencyView getRodCost() const;
    int getRodRange() const;

    int getCrystalP() const;

    int getConstUrg() const;

    int getRegenLWar() const;
    int getRegenRuin() const;

    int getDPeace() const;
    int getDWar() const;
    int getDNeutral() const;
    int getDGold() const;
    int getDMkAlly() const;
    int getDAttackSc() const;
    int getDAttackFo() const;
    int getDAttackPc() const;
    int getDRod() const;
    int getDRefAlly() const;
    int getDBkAlly() const;
    int getDNoble() const;
    int getDBkaChnc() const;
    int getDBkaTurn() const;

    int getProt(int tier) const;
    int getProtCap() const;

    int getBonusE() const;
    int getBonusA() const;
    int getBonusH() const;
    int getBonusV() const;

    int getIncomeE() const;
    int getIncomeA() const;
    int getIncomeH() const;
    int getIncomeV() const;

    int getGuRange() const;
    int getPaRange() const;
    int getLoRange() const;

    int getDfendBonus() const;
    int getTalisChrg() const;

    int getSplpwr(int level) const;

    int getGainSpell() const;

    int getDefendBonus() const;

private:
    const game::GlobalVariables* variables;
};

} // namespace bindings

#endif // GLOBALVARIABLESVIEW_H
