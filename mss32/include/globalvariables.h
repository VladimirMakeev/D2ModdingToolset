/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "currency.h"
#include "d2string.h"
#include <cstddef>

namespace game {

/** Holds global game settings read from GVars.dbf. */
struct GlobalVariables
{
    /** 'MORALE_n' */
    int morale[6];
    /** Instructor skill bonus experience. 'WEAPN_MSTR' */
    int weaponMaster;
    /** Max additional initiative points that randomly added to unit ini in battle. 'BAT_INIT' */
    int additionalBattleIni;
    /** Max additional damage points that randomly added to unit damage in battle. 'BAT_DAMAGE' */
    int additionalBattleDmg;
    /** 'BAT_ROUND' */
    int batRound;
    /** Damage boost values. 'BATBOOSTDn' */
    int battleBoostDamage[4];
    /** Damage debuff values. 'BATLOWERDn' */
    int battleLowerDamage[2];
    /** Initiative debuff. 'BATLOWERI' */
    int battleLowerIni;
    /** 'BAT_BREAK' */
    int batBreak;
    /** 'BAT_BMODIF' */
    int batBmodif;
    /** Max abilities leader can learn. 'LDRMAXABIL' */
    int maxLeaderAbilities;
    /** Spy discovery change per turn. 'SPY_DISCOV' */
    int spyDiscoveryChance;
    /** 'POISON_S' */
    int poisonStackDamage;
    /** 'POISON_C' */
    int poisonCityDamage;
    /** 'BRIBE' */
    int bribeMultiplier;
    /** 'STEAL_RACE' */
    int stealRace;
    /** 'STEAL_NEUT' */
    int stealNeut;
    /** Minimal riot duration in days. 'RIOT_MIN' */
    int riotDaysMin;
    /** Maximal riot duration in days. 'RIOT_MAX' */
    int riotDaysMax;
    /** Percentage of riot damage. 'RIOT_DMG' */
    int riotDamage;
    /** Percentage of the original price of the items at sale. 'SELL_RATIO' */
    int sellRatio;
    /** Land transformation per turn by capital. 'T_CAPITAL' */
    int landTransformCapital;
    /** Land transformation per turn by cities of different tiers. 'T_CITYn' */
    int landTransformCity[5];
    /** Land transformation after city capture. 'T_CAPTURE' */
    int landTransformCapture;
    /** 'ROD_COST' */
    Bank rodPlacementCost;
    /** Range of land transformation by rod per turn. 'ROD_RANGE' */
    int rodRange;
    /** Profit per mana crystal or gold mine per turn. 'CRYSTAL_P' */
    int crystalProfit;
    /** 'CONST_UPG' */
    int constUpg;
    /** Change to get spells with capture of a capital. 'GAIN_SPELL' */
    int gainSpellChance;
    /** Bonus per day regeneration for units in ruins. 'REGEN_RUIN' */
    int regenRuin;
    /** Bonus per day regeneration for fighter leader. 'REGEN_LWAR' */
    int fighterLeaderRegen;
    /** Diplomacy level representing peace. 'D_PEACE' */
    std::uint32_t diplomacyPeace;
    /** Diplomacy level representing war. 'D_WAR' */
    std::uint32_t diplomacyWar;
    /** Diplomacy level representing neutrality. 'D_NEUTRAL' */
    std::uint32_t diplomacyNeutral;
    /** 'D_GOLD' */
    int dGold;
    /** 'D_MK_ALLY' */
    int dMkAlly;
    /** 'D_ATTACK_SC' */
    int dAttackSc;
    /** 'D_ATTACK_FO' */
    int dAttackFo;
    /** 'D_ATTACK_PC' */
    int dAttackPc;
    /** 'D_ROD' */
    int dRod;
    /** 'D_BK_ALLY' */
    int dBkAlly;
    /** 'D_REF_ALLY' */
    int dRefAlly;
    /** 'D_NOBLE' */
    int dNoble;
    /** 'D_BKA_CHANCE' */
    int dBkaChance;
    /** 'D_BKA_TURN' */
    int dBkaTurn;
    /** City protection values. 'PROT_n' */
    int cityProtection[5];
    /** Capital protection. 'PROT_CAP' */
    int capitalProtection;
    /** Additional gold on easy difficulty. 'BONUS_E' */
    int bonusGoldEasy;
    /** Additional gold on average difficulty. 'BONUS_A' */
    int bonusGoldAverage;
    /** Additional gold on hard difficulty. 'BONUS_H' */
    int bonusGoldHard;
    /** Additional gold on very hard difficulty. 'BONUS_V' */
    int bonusGoldVeryHard;
    /** 'PA_RANGE' */
    int paRange;
    /** 'LO_RANGE' */
    int loRange;
    /** 'GU_RANGE' */
    int guRange;
    /** Armor bonus when unit uses defend in battle. 'DFENDBONUS' */
    int defendBonus;
    /** 'TALIS_CHRG' */
    int talismanCharges;
    /** Income increase on easy difficulty. 'INCOME_E' */
    int incomeEasy;
    /** Income increase on average difficulty. 'INCOME_A' */
    int incomeAverage;
    /** Income increase on hard difficulty. 'INCOME_H' */
    int incomeHard;
    /** Income increase on very hard difficulty. 'INCOME_V' */
    int incomeVeryHard;
    /** 'SPLPWR_n' */
    int spellPower[5];
    /** Tutorial scenario file name. 'TUTORIAL' */
    String tutorialName;
};

assert_size(GlobalVariables, 352);
assert_offset(GlobalVariables, rodPlacementCost, 148);
assert_offset(GlobalVariables, talismanCharges, 296);

} // namespace game

#endif // GLOBALVARIABLES_H
