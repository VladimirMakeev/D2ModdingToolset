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
#include <map>
#include <mutex>

namespace game {

struct CMidUnit;
struct TUnitModifier;

} // namespace game

namespace hooks {

struct CustomAttackData;

struct CAttackModifiedCustom : public game::IAttack
{
    const game::IAttack* prev;
};

struct CustomModifierData
{
    game::ModifierElementTypeFlag lastElementQuery;
    game::Bank enrollCost;
    game::Bank reviveCost;
    game::Bank healCost;
    game::Bank trainingCost;
    game::IdVector wards;
    int regen;
};

using CustomModifierDataMap = std::map<std::thread::id, CustomModifierData>;

struct CCustomModifier
{
    game::IUsUnit usUnit;
    game::IUsSoldier usSoldier;
    game::CUmModifier umModifier;
    game::IUsStackLeader usStackLeader;
    CAttackModifiedCustom attack;
    CAttackModifiedCustom attack2;
    CAttackModifiedCustom altAttack;
    const game::CMidUnit* unit;
    const game::TUnitModifier* unitModifier;
    const std::string scriptFileName;
    const game::CMidgardID descTxt;
    const bool display;
    // Thread-sensitive data
    CustomModifierDataMap data;
    std::mutex dataMutex;

    CustomModifierData& getData();
    void setUnit(const game::CMidUnit* value);
    game::IAttack* getAttack(bool primary);
    game::IAttack* wrapAltAttack(const game::IAttack* value);

    bool getDisplay() const;
    game::CMidgardID getDescTxt() const;
    std::string getIconName() const;

    const game::IUsUnit* getPrev() const;
    const game::IUsSoldier* getPrevSoldier() const;
    const game::IUsStackLeader* getPrevStackLeader() const;
    const game::IAttack* getPrevAttack(const game::IAttack* thisptr) const;
    const CCustomModifier* getPrevCustomModifier() const;
    CustomAttackData getCustomAttackData(const game::IAttack* thisptr) const;
    const char* getFormattedGlobalText(const game::CMidgardID& formatId,
                                       const game::CMidgardID& baseId) const;
    void showScriptErrorMessage(const char* functionName, const char* reason) const;
    void showInvalidRetvalMessage(const char* functionName, const char* reason) const;
    void notifyModifierAddedRemoved(const game::CUmModifier* modifier, bool added) const;

    game::CMidgardID getUnitNameTxt() const;
    game::CMidgardID getUnitBaseNameTxt() const;
    game::CMidgardID getUnitDescTxt() const;
    game::CMidgardID getUnitBaseDescTxt() const;

    game::CMidgardID getAttackNameTxt(bool primary, const game::CMidgardID& baseId) const;
    game::CMidgardID getAttackBaseNameTxt(const game::IAttack* attack) const;
    game::CMidgardID getAttackDescTxt(bool primary, const game::CMidgardID& baseId) const;
    game::CMidgardID getAttackBaseDescTxt(const game::IAttack* attack) const;

    template <typename F, typename T>
    T getValue(F function, const char* functionName, const T& prev) const
    {
        try {
            if (function) {
                bindings::UnitView unitView{unit, getPrev()};
                return (*function)(unitView, prev);
            }
        } catch (const std::exception& e) {
            showScriptErrorMessage(functionName, e.what());
        }

        return prev;
    }

    template <typename F, typename T>
    T getValueAs(F function, const char* functionName, const T& prev) const
    {
        try {
            if (function) {
                bindings::UnitView unitView{unit, getPrev()};
                sol::table result = (*function)(unitView, prev);
                return result.as<T>();
            }
        } catch (const std::exception& e) {
            showScriptErrorMessage(functionName, e.what());
        }

        return prev;
    }

    template <typename F, typename T, typename P>
    T getValueParam(F function, const char* functionName, const P& param, const T& prev) const
    {
        try {
            if (function) {
                bindings::UnitView unitView{unit, getPrev()};
                return (*function)(unitView, param, prev);
            }
        } catch (const std::exception& e) {
            showScriptErrorMessage(functionName, e.what());
        }

        return prev;
    }

    template <typename F, typename T>
    T getValueNoParam(F function, const char* functionName, T def) const
    {
        try {
            if (function) {
                return (*function)();
            }
        } catch (const std::exception& e) {
            showScriptErrorMessage(functionName, e.what());
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

game::CUmModifier* createCustomModifier(const game::TUnitModifier* unitModifier,
                                        const char* scriptFileName,
                                        const game::CMidgardID* descTxt,
                                        bool display,
                                        const game::GlobalData** globalData);

} // namespace hooks

#endif // CUSTOMMODIFIER_H
