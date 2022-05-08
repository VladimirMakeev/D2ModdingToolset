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
#include "currency.h"
#include "idvector.h"
#include "scripts.h"
#include "umunit.h"
#include "unitview.h"
#include "usstackleader.h"

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
    std::filesystem::path script;
    int regen;
    game::Bank enrollCost;
    game::Bank reviveCost;
    game::Bank healCost;
    game::Bank trainingCost;
    game::CMidgardID altAttackId;
    game::IdVector wards;

    game::IAttack* getAttack(bool primary);
    void setUnit(const game::CMidUnit* value);

    game::IUsUnit* getPrev() const;
    game::IUsSoldier* getPrevSoldier() const;
    game::IUsStackLeader* getPrevStackLeader() const;
    game::IAttack* getPrevAttack(const game::IAttack* thisptr) const;
    game::IAttack* getPrevAttack(bool primary) const;
    CCustomModifier* getPrevCustomModifier() const;
    CustomAttackData getCustomAttackData(const game::IAttack* thisptr) const;
    const char* getFormattedGlobalText(const game::CMidgardID& formatId,
                                       const game::CMidgardID& valueId) const;

    game::CMidgardID getNameTxt() const;
    game::CMidgardID getBaseNameTxt() const;
    game::CMidgardID getDescTxt() const;
    game::CMidgardID getBaseDescTxt() const;

    game::CMidgardID getAttackNameTxt(const game::IAttack* thisptr) const;
    game::CMidgardID getAttackBaseNameTxt(const game::IAttack* thisptr) const;
    game::CMidgardID getAttackDescTxt(const game::IAttack* thisptr) const;
    game::CMidgardID getAttackBaseDescTxt(const game::IAttack* thisptr) const;

    template <typename F, typename T>
    T getValue(const char* functionName, const T& prev) const
    {
        std::optional<sol::environment> env;
        auto f = getScriptFunction<F>(script, functionName, env);
        try {
            if (f) {
                bindings::UnitView unitView{unit, getPrev()};
                return (*f)(unitView, prev);
            }
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                            "Reason: '{:s}'",
                                            functionName, e.what()));
        }

        return prev;
    }

    template <typename F, typename T>
    T getValueAs(const char* functionName, const T& prev) const
    {
        std::optional<sol::environment> env;
        auto f = getScriptFunction<F>(script, functionName, env);
        try {
            if (f) {
                bindings::UnitView unitView{unit, getPrev()};
                return (*f)(unitView, prev).as<T>();
            }
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                            "Reason: '{:s}'",
                                            functionName, e.what()));
        }

        return prev;
    }

    template <typename F, typename T, typename P>
    T getValueParam(const char* functionName, const P& param, const T& prev) const
    {
        std::optional<sol::environment> env;
        auto f = getScriptFunction<F>(script, functionName, env);
        try {
            if (f) {
                bindings::UnitView unitView{unit, getPrev()};
                return (*f)(unitView, param, prev);
            }
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                            "Reason: '{:s}'",
                                            functionName, e.what()));
        }

        return prev;
    }

    template <typename F, typename T>
    T getValueNoParam(const char* functionName, T def) const
    {
        std::optional<sol::environment> env;
        auto f = getScriptFunction<F>(script, functionName, env);
        try {
            if (f) {
                return (*f)();
            }
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                            "Reason: '{:s}'",
                                            functionName, e.what()));
        }

        return def;
    }
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
