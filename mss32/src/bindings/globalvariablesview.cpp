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

#include "globalvariablesview.h"
#include "globalvariables.h"
#include <sol/sol.hpp>

namespace bindings {

GlobalVariablesView::GlobalVariablesView(const game::GlobalVariables* variables)
    : variables{variables}
{ }

void GlobalVariablesView::bind(sol::state& lua)
{
    auto view = lua.new_usertype<GlobalVariablesView>("GlobalVariablesView");

    view["morale"] = &GlobalVariablesView::getMorale;
    view["weapnMstr"] = sol::property(&GlobalVariablesView::getMorale);

    view["batInit"] = sol::property(&GlobalVariablesView::getBatInit);
    view["batDamage"] = sol::property(&GlobalVariablesView::getBatDamage);
    view["batRound"] = sol::property(&GlobalVariablesView::getBatRound);
    view["batBreak"] = sol::property(&GlobalVariablesView::getBatBreak);
    view["batBModif"] = sol::property(&GlobalVariablesView::getBatBModif);
    view["batBoostd"] = &GlobalVariablesView::getBatBoostd;
    view["batLowerd"] = &GlobalVariablesView::getBatLowerd;
    view["batLoweri"] = sol::property(&GlobalVariablesView::getBatLoweri);

    view["ldrMaxAbil"] = sol::property(&GlobalVariablesView::getLdrMaxAbil);
    view["spyDiscov"] = sol::property(&GlobalVariablesView::getSpyDiscov);
    view["poisonC"] = sol::property(&GlobalVariablesView::getPoisonC);
    view["poisonS"] = sol::property(&GlobalVariablesView::getPoisonS);
    view["bribe"] = sol::property(&GlobalVariablesView::getBribe);

    view["stealRace"] = sol::property(&GlobalVariablesView::getStealRace);
    view["stealNeut"] = sol::property(&GlobalVariablesView::getStealNeut);

    view["riotMin"] = sol::property(&GlobalVariablesView::getRiotMin);
    view["riotMax"] = sol::property(&GlobalVariablesView::getRiotMax);
    view["riotDmg"] = sol::property(&GlobalVariablesView::getRiotDmg);

    view["sellRatio"] = sol::property(&GlobalVariablesView::getSellRatio);

    view["tCapture"] = sol::property(&GlobalVariablesView::getTCapture);
    view["tCapital"] = sol::property(&GlobalVariablesView::getTCapital);
    view["tCity"] = &GlobalVariablesView::getTCity;

    view["rodCost"] = sol::property(&GlobalVariablesView::getRodCost);
    view["rodRange"] = sol::property(&GlobalVariablesView::getRodRange);

    view["crystalP"] = sol::property(&GlobalVariablesView::getCrystalP);
    view["constUrg"] = sol::property(&GlobalVariablesView::getConstUrg);

    view["regenLwar"] = sol::property(&GlobalVariablesView::getRegenLWar);
    view["regenRuin"] = sol::property(&GlobalVariablesView::getRegenRuin);

    view["dPeace"] = sol::property(&GlobalVariablesView::getDPeace);
    view["dWar"] = sol::property(&GlobalVariablesView::getDWar);
    view["dNeutral"] = sol::property(&GlobalVariablesView::getDNeutral);
    view["dGold"] = sol::property(&GlobalVariablesView::getDGold);
    view["dMkAlly"] = sol::property(&GlobalVariablesView::getDMkAlly);
    view["dAttakSc"] = sol::property(&GlobalVariablesView::getDAttackSc);
    view["dAttakFo"] = sol::property(&GlobalVariablesView::getDAttackFo);
    view["dAttakPc"] = sol::property(&GlobalVariablesView::getDAttackPc);
    view["dRod"] = sol::property(&GlobalVariablesView::getDRod);
    view["dRefAlly"] = sol::property(&GlobalVariablesView::getDRefAlly);
    view["dBkAlly"] = sol::property(&GlobalVariablesView::getDBkAlly);
    view["dNoble"] = sol::property(&GlobalVariablesView::getDNoble);
    view["dBkaChnc"] = sol::property(&GlobalVariablesView::getDBkaChnc);
    view["dBkaTurn"] = sol::property(&GlobalVariablesView::getDBkaTurn);

    view["prot"] = &GlobalVariablesView::getProt;
    view["protCap"] = sol::property(&GlobalVariablesView::getProtCap);

    view["bonusE"] = sol::property(&GlobalVariablesView::getBonusE);
    view["bonusA"] = sol::property(&GlobalVariablesView::getBonusA);
    view["bonusH"] = sol::property(&GlobalVariablesView::getBonusH);
    view["bonusV"] = sol::property(&GlobalVariablesView::getBonusV);

    view["incomeE"] = sol::property(&GlobalVariablesView::getIncomeE);
    view["incomeA"] = sol::property(&GlobalVariablesView::getIncomeA);
    view["incomeH"] = sol::property(&GlobalVariablesView::getIncomeH);
    view["incomeV"] = sol::property(&GlobalVariablesView::getIncomeV);

    view["guRange"] = sol::property(&GlobalVariablesView::getGuRange);
    view["paRange"] = sol::property(&GlobalVariablesView::getPaRange);
    view["loRange"] = sol::property(&GlobalVariablesView::getLoRange);

    view["defendBonus"] = sol::property(&GlobalVariablesView::getDefendBonus);
    view["talisChrg"] = sol::property(&GlobalVariablesView::getTalisChrg);

    view["splPwr"] = &GlobalVariablesView::getSplpwr;
    view["gainSpell"] = sol::property(&GlobalVariablesView::getGainSpell);
}

int GlobalVariablesView::getMorale(int tier) const
{
    if (tier < 1 || tier > 6) {
        return 0;
    }

    return variables->morale[tier - 1];
}

int GlobalVariablesView::getWeapnMstr() const
{
    return variables->weaponMaster;
}

int GlobalVariablesView::getBatInit() const
{
    return variables->additionalBattleIni;
}

int GlobalVariablesView::getBatDamage() const
{
    return variables->additionalBattleDmg;
}

int GlobalVariablesView::getBatRound() const
{
    return variables->batRound;
}

int GlobalVariablesView::getBatBreak() const
{
    return variables->batBreak;
}

int GlobalVariablesView::getBatBModif() const
{
    return variables->batBmodif;
}

int GlobalVariablesView::getBatBoostd(int level) const
{
    if (level < 1 || level > 4) {
        return 0;
    }

    return variables->battleBoostDamage[level - 1];
}

int GlobalVariablesView::getBatLowerd(int level) const
{
    if (level < 1 || level > 2) {
        return 0;
    }

    return variables->battleLowerDamage[level - 1];
}

int GlobalVariablesView::getBatLoweri() const
{
    return variables->battleLowerIni;
}

int GlobalVariablesView::getLdrMaxAbil() const
{
    return variables->maxLeaderAbilities;
}

int GlobalVariablesView::getSpyDiscov() const
{
    return variables->spyDiscoveryChance;
}

int GlobalVariablesView::getPoisonS() const
{
    return variables->poisonStackDamage;
}

int GlobalVariablesView::getPoisonC() const
{
    return variables->poisonCityDamage;
}

int GlobalVariablesView::getBribe() const
{
    return variables->bribeMultiplier;
}

int GlobalVariablesView::getStealRace() const
{
    return variables->stealRace;
}

int GlobalVariablesView::getStealNeut() const
{
    return variables->stealNeut;
}

int GlobalVariablesView::getRiotMin() const
{
    return variables->riotDaysMin;
}

int GlobalVariablesView::getRiotMax() const
{
    return variables->riotDaysMax;
}

int GlobalVariablesView::getRiotDmg() const
{
    return variables->riotDamage;
}

int GlobalVariablesView::getSellRatio() const
{
    return variables->sellRatio;
}

int GlobalVariablesView::getTCapture() const
{
    return variables->landTransformCapture;
}

int GlobalVariablesView::getTCapital() const
{
    return variables->landTransformCapital;
}

int GlobalVariablesView::getTCity(int tier) const
{
    if (tier < 1 || tier > 5) {
        return 0;
    }

    return variables->landTransformCity[tier - 1];
}

CurrencyView GlobalVariablesView::getRodCost() const
{
    return CurrencyView{variables->rodPlacementCost};
}

int GlobalVariablesView::getRodRange() const
{
    return variables->rodRange;
}

int GlobalVariablesView::getCrystalP() const
{
    return variables->crystalProfit;
}

int GlobalVariablesView::getConstUrg() const
{
    return variables->constUrg;
}

int GlobalVariablesView::getRegenLWar() const
{
    return variables->fighterLeaderRegen;
}

int GlobalVariablesView::getRegenRuin() const
{
    return variables->regenRuin;
}

int GlobalVariablesView::getDPeace() const
{
    return variables->diplomacyPeace;
}

int GlobalVariablesView::getDWar() const
{
    return variables->diplomacyWar;
}

int GlobalVariablesView::getDNeutral() const
{
    return variables->diplomacyNeutral;
}

int GlobalVariablesView::getDGold() const
{
    return variables->dGold;
}

int GlobalVariablesView::getDMkAlly() const
{
    return variables->dMkAlly;
}

int GlobalVariablesView::getDAttackSc() const
{
    return variables->dAttackSc;
}

int GlobalVariablesView::getDAttackFo() const
{
    return variables->dAttackFo;
}

int GlobalVariablesView::getDAttackPc() const
{
    return variables->dAttackPc;
}

int GlobalVariablesView::getDRod() const
{
    return variables->dRod;
}

int GlobalVariablesView::getDRefAlly() const
{
    return variables->dRefAlly;
}

int GlobalVariablesView::getDBkAlly() const
{
    return variables->dBkAlly;
}

int GlobalVariablesView::getDNoble() const
{
    return variables->dNoble;
}

int GlobalVariablesView::getDBkaChnc() const
{
    return variables->dBkaChance;
}

int GlobalVariablesView::getDBkaTurn() const
{
    return variables->dBkaTurn;
}

int GlobalVariablesView::getProt(int tier) const
{
    if (tier < 1 || tier > 5) {
        return 0;
    }

    return variables->cityProtection[tier - 1];
}

int GlobalVariablesView::getProtCap() const
{
    return variables->capitalProtection;
}

int GlobalVariablesView::getBonusE() const
{
    return variables->bonusGoldEasy;
}

int GlobalVariablesView::getBonusA() const
{
    return variables->bonusGoldAverage;
}

int GlobalVariablesView::getBonusH() const
{
    return variables->bonusGoldHard;
}

int GlobalVariablesView::getBonusV() const
{
    return variables->bonusGoldVeryHard;
}

int GlobalVariablesView::getIncomeE() const
{
    return variables->incomeEasy;
}

int GlobalVariablesView::getIncomeA() const
{
    return variables->incomeAverage;
}

int GlobalVariablesView::getIncomeH() const
{
    return variables->incomeHard;
}

int GlobalVariablesView::getIncomeV() const
{
    return variables->incomeVeryHard;
}

int GlobalVariablesView::getGuRange() const
{
    return variables->guRange;
}

int GlobalVariablesView::getPaRange() const
{
    return variables->paRange;
}

int GlobalVariablesView::getLoRange() const
{
    return variables->loRange;
}

int GlobalVariablesView::getDfendBonus() const
{
    return variables->defendBonus;
}

int GlobalVariablesView::getTalisChrg() const
{
    return variables->talismanCharges;
}

int GlobalVariablesView::getSplpwr(int level) const
{
    if (level < 1 || level > 5) {
        return 0;
    }

    return variables->spellPower[level - 1];
}

int GlobalVariablesView::getGainSpell() const
{
    return variables->gainSpellChance;
}

int GlobalVariablesView::getDefendBonus() const
{
    return variables->defendBonus;
}

} // namespace bindings
