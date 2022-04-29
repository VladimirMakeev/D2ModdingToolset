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

namespace hooks {

struct CCustomModifier
{
    game::IUsUnit usUnit;
    game::IUsSoldier usSoldier;
    game::CUmModifier umModifier;
    game::IUsStackLeader usStackLeader;
    game::IAttack attack;
    game::IAttack attack2;
    std::string script;

    game::IAttack* getPrevAttack(const game::IAttack* current);
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

CCustomModifier* castAttackToCustomModifier(const game::IAttack* attack);

CCustomModifier* customModifierCtor(CCustomModifier* thisptr,
                                    const char* script,
                                    const game::CMidgardID* id,
                                    const game::GlobalData** globalData);

} // namespace hooks

#endif // CUSTOMMODIFIER_H
