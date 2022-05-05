/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
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

#ifndef CUSTOMMODIFIER_H
#define CUSTOMMODIFIER_H

#include "attack.h"
#include "umunit.h"
#include "usstackleader.h"
#include <string>

namespace game {

struct CMidUnit;

} // namespace game

namespace hooks {

struct CustomAttackData;

struct CCustomModifier
{
    game::IUsUnit usUnit;
    game::IUsSoldier usSoldier;
    game::CUmModifier umModifier;
    game::IUsStackLeader usStackLeader;
    game::IAttack attack;
    game::IAttack attack2;
    const game::CMidUnit* unit;
    game::ModifierElementTypeFlag lastElementQuery;
    std::string script;
    int regen;

    game::IUsUnit* getPrev();
    game::IUsSoldier* getPrevSoldier();
    game::IUsStackLeader* getPrevStackLeader();
    game::IAttack* getPrevAttack(const game::IAttack* current);
    CCustomModifier* getPrevCustomModifier();
    game::IAttack* getAttack(bool primary);
    CustomAttackData getCustomAttackData(const game::IAttack* current);
    void setUnit(const game::CMidUnit* value);
    const char* getFormattedGlobalText(const std::string& formatId, const std::string& valueId);

    std::string getNameTxt();
    std::string getPrevNameTxt();
    std::string getBaseNameTxt();
    std::string getDescTxt();
    std::string getPrevDescTxt();
    std::string getBaseDescTxt();

    std::string getString(const char* functionName, const std::string& prev);
    int getInteger(const char* functionName, int prev);
    int getIntegerIntParam(const char* functionName, int param, int prev);
};

static_assert(
    offsetof(CCustomModifier, usUnit) == offsetof(game::CUmUnit, usUnit),
    "CustomModifier::usUnit offset should be the same as of CUmUnit as it reuses its rtti info");

static_assert(
    offsetof(CCustomModifier, usSoldier) == offsetof(game::CUmUnit, usSoldier),
    "CustomModifier::usSoldier offset should be the same as of CUmUnit as it reuses its rtti info");

static_assert(
    offsetof(CCustomModifier, umModifier) == offsetof(game::CUmUnit, umModifier),
    "CustomModifier::umModifier offset should be the same as of CUmUnit as it reuses its rtti info");

CCustomModifier* castModifierToCustomModifier(const game::CUmModifier* modifier);

CCustomModifier* castAttackToCustomModifier(const game::IAttack* attack);

game::CUmModifier* createCustomModifier(const char* script,
                                        const game::CMidgardID* id,
                                        const game::GlobalData** globalData);

} // namespace hooks

#endif // CUSTOMMODIFIER_H
