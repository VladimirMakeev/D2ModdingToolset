/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Stanislav Egorov.
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

#ifndef AIATTITUDES_H
#define AIATTITUDES_H

#include "aiattitudescat.h"
#include "d2assert.h"
#include "textandid.h"

namespace game {

struct CAiAttitudesVftable;
struct CAiAttitudesData;

struct CAiAttitudes
{
    CAiAttitudesVftable* vftable;
    CAiAttitudesData* data;
};

assert_size(CAiAttitudes, 8);

struct CAiAttitudesVftable
{
    using Destructor = void(__thiscall*)(CAiAttitudes* thisptr, char flags);
    Destructor destructor;
};

assert_vftable_size(CAiAttitudesVftable, 1);

struct CAiAttitudesData
{
    LAttitudesCategory attitudeCategory;
    TextAndId name;
    TextAndId description;
    int bUnits;
    int bLeaders;
    int bCap;
    int bCities;
    int bSpellR;
    int bSpellC;
    int bIso;
    int lMin;
    int lMax;
    int pctFighter;
    int tMin;
    int tMax;
    int pctThief;
    int rMin;
    int rMax;
    int pctRod;
    int iCitiesN;
    int iCitiesR;
    int iCitiesP;
    int iStacksN;
    int iStacksR;
    int iStacksP;
    int iBags;
    int iRuins;
    int iTowers;
    int iMerchant;
    int iHUnits;
    int iHMercs;
    int iHeal;
    int iNCities;
    int iNStack;
    int iPRod;
    int iARod;
    int iTrainer;
    int iObjM;
    int dEasyM;
    int dAvgM;
    int dHardM;
    int dVHardM;
    int dEasyXp;
    int dAvgXp;
    int dHardXp;
    int dVHardXp;
    int hpStayMin;
    int hpHealMin;
    int castChance;
    int castChance2;
    int castDblMin;
    int spell1Chance;
    int spell2Chance;
    int spell3Chance;
    int splHW;
    int splHC;
    int splMW;
    int splMC;
    int splLW;
    int splLC;
    int splL5Pow;
    int splL4Pow;
    int splL3Pow;
    int splL2Pow;
    int sritSplW1;
    int sritSplC1;
    int sritSplW2;
    int sritSplC2;
    int sritSplW3;
    int sritSplC3;
    int sritSplW4;
    int sritSplC4;
};

assert_size(CAiAttitudesData, 304);

} // namespace game

#endif // AIATTITUDES_H
