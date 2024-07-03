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
    view["weapnMstr"] = sol::property(&GlobalVariablesView::getWeapnMstr);

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

    return variables->data->morale[tier - 1];
}

int GlobalVariablesView::getWeapnMstr() const
{
    return variables->data->weaponMaster;
}

int GlobalVariablesView::getBatInit() const
{
    return variables->data->additionalBattleIni;
}

int GlobalVariablesView::getBatDamage() const
{
    return variables->data->additionalBattleDmg;
}

int GlobalVariablesView::getBatRound() const
{
    return variables->data->batRound;
}

int GlobalVariablesView::getBatBreak() const
{
    return variables->data->batBreak;
}

int GlobalVariablesView::getBatBModif() const
{
    return variables->data->batBmodif;
}

int GlobalVariablesView::getBatBoostd(int level) const
{
    if (level < 1 || level > 4) {
        return 0;
    }

    return variables->data->battleBoostDamage[level - 1];
}

int GlobalVariablesView::getBatLowerd(int level) const
{
    if (level < 1 || level > 2) {
        return 0;
    }

    return variables->data->battleLowerDamage[level - 1];
}

int GlobalVariablesView::getBatLoweri() const
{
    return variables->data->battleLowerIni;
}

int GlobalVariablesView::getLdrMaxAbil() const
{
    return variables->data->maxLeaderAbilities;
}

int GlobalVariablesView::getSpyDiscov() const
{
    return variables->data->spyDiscoveryChance;
}

int GlobalVariablesView::getPoisonS() const
{
    return variables->data->poisonStackDamage;
}

int GlobalVariablesView::getPoisonC() const
{
    return variables->data->poisonCityDamage;
}

int GlobalVariablesView::getBribe() const
{
    return variables->data->bribeMultiplier;
}

int GlobalVariablesView::getStealRace() const
{
    return variables->data->stealRace;
}

int GlobalVariablesView::getStealNeut() const
{
    return variables->data->stealNeut;
}

int GlobalVariablesView::getRiotMin() const
{
    return variables->data->riotDaysMin;
}

int GlobalVariablesView::getRiotMax() const
{
    return variables->data->riotDaysMax;
}

int GlobalVariablesView::getRiotDmg() const
{
    return variables->data->riotDamage;
}

int GlobalVariablesView::getSellRatio() const
{
    return variables->data->sellRatio;
}

int GlobalVariablesView::getTCapture() const
{
    return variables->data->landTransformCapture;
}

int GlobalVariablesView::getTCapital() const
{
    return variables->data->landTransformCapital;
}

int GlobalVariablesView::getTCity(int tier) const
{
    if (tier < 1 || tier > 5) {
        return 0;
    }

    return variables->data->landTransformCity[tier - 1];
}

CurrencyView GlobalVariablesView::getRodCost() const
{
    return CurrencyView{variables->data->rodPlacementCost};
}

int GlobalVariablesView::getRodRange() const
{
    return variables->data->rodRange;
}

int GlobalVariablesView::getCrystalP() const
{
    return variables->data->crystalProfit;
}

int GlobalVariablesView::getConstUrg() const
{
    return variables->data->constUrg;
}

int GlobalVariablesView::getRegenLWar() const
{
    return variables->data->fighterLeaderRegen;
}

int GlobalVariablesView::getRegenRuin() const
{
    return variables->data->regenRuin;
}

int GlobalVariablesView::getDPeace() const
{
    return variables->data->diplomacyPeace;
}

int GlobalVariablesView::getDWar() const
{
    return variables->data->diplomacyWar;
}

int GlobalVariablesView::getDNeutral() const
{
    return variables->data->diplomacyNeutral;
}

int GlobalVariablesView::getDGold() const
{
    return variables->data->dGold;
}

int GlobalVariablesView::getDMkAlly() const
{
    return variables->data->dMkAlly;
}

int GlobalVariablesView::getDAttackSc() const
{
    return variables->data->dAttackSc;
}

int GlobalVariablesView::getDAttackFo() const
{
    return variables->data->dAttackFo;
}

int GlobalVariablesView::getDAttackPc() const
{
    return variables->data->dAttackPc;
}

int GlobalVariablesView::getDRod() const
{
    return variables->data->dRod;
}

int GlobalVariablesView::getDRefAlly() const
{
    return variables->data->dRefAlly;
}

int GlobalVariablesView::getDBkAlly() const
{
    return variables->data->dBkAlly;
}

int GlobalVariablesView::getDNoble() const
{
    return variables->data->dNoble;
}

int GlobalVariablesView::getDBkaChnc() const
{
    return variables->data->dBkaChance;
}

int GlobalVariablesView::getDBkaTurn() const
{
    return variables->data->dBkaTurn;
}

int GlobalVariablesView::getProt(int tier) const
{
    if (tier < 1 || tier > 6) {
        return 0;
    }

    if (tier == 6) {
        return getProtCap();
    }

    return variables->data->cityProtection[tier - 1];
}

int GlobalVariablesView::getProtCap() const
{
    return variables->data->capitalProtection;
}

int GlobalVariablesView::getBonusE() const
{
    return variables->data->bonusGoldEasy;
}

int GlobalVariablesView::getBonusA() const
{
    return variables->data->bonusGoldAverage;
}

int GlobalVariablesView::getBonusH() const
{
    return variables->data->bonusGoldHard;
}

int GlobalVariablesView::getBonusV() const
{
    return variables->data->bonusGoldVeryHard;
}

int GlobalVariablesView::getIncomeE() const
{
    return variables->data->incomeEasy;
}

int GlobalVariablesView::getIncomeA() const
{
    return variables->data->incomeAverage;
}

int GlobalVariablesView::getIncomeH() const
{
    return variables->data->incomeHard;
}

int GlobalVariablesView::getIncomeV() const
{
    return variables->data->incomeVeryHard;
}

int GlobalVariablesView::getGuRange() const
{
    return variables->data->guRange;
}

int GlobalVariablesView::getPaRange() const
{
    return variables->data->paRange;
}

int GlobalVariablesView::getLoRange() const
{
    return variables->data->loRange;
}

int GlobalVariablesView::getDfendBonus() const
{
    return variables->data->defendBonus;
}

int GlobalVariablesView::getTalisChrg() const
{
    return variables->data->talismanCharges;
}

int GlobalVariablesView::getSplpwr(int level) const
{
    if (level < 1 || level > 5) {
        return 0;
    }

    return variables->data->spellPower[level - 1];
}

int GlobalVariablesView::getGainSpell() const
{
    return variables->data->gainSpellChance;
}

int GlobalVariablesView::getDefendBonus() const
{
    return variables->data->defendBonus;
}

} // namespace bindings
