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

#include "custommodifier.h"
#include "dynamiccast.h"
#include "game.h"
#include "mempool.h"
#include "utils.h"

namespace hooks {

static struct
{
    game::RttiInfo<game::IUsUnitVftable> usUnit;
    game::RttiInfo<game::IUsSoldierVftable> usSoldier;
    game::RttiInfo<game::CUmModifierVftable> umModifier;
    game::RttiInfo<game::IUsStackLeaderVftable> usStackLeader;
    game::RttiInfo<game::IAttackVftable> attack;
    game::RttiInfo<game::IAttackVftable> attack2;
} rttiInfo;

void initRttiInfo();
void initVftable(CCustomModifier* thisptr);

static inline CCustomModifier* castUnitToCustomModifier(const game::IUsUnit* unit)
{
    return (CCustomModifier*)unit;
}

static inline CCustomModifier* castSoldierToCustomModifier(const game::IUsSoldier* soldier)
{
    return reinterpret_cast<CCustomModifier*>((uintptr_t)soldier
                                              - offsetof(CCustomModifier, usSoldier));
}

static inline CCustomModifier* castModifierToCustomModifier(const game::CUmModifier* modifier)
{
    return reinterpret_cast<CCustomModifier*>((uintptr_t)modifier
                                              - offsetof(CCustomModifier, umModifier));
}

static inline CCustomModifier* castStackLeaderToCustomModifier(
    const game::IUsStackLeader* stackLeader)
{
    return reinterpret_cast<CCustomModifier*>((uintptr_t)stackLeader
                                              - offsetof(CCustomModifier, usStackLeader));
}

CCustomModifier* castAttackToCustomModifier(const game::IAttack* attack)
{
    if (attack->vftable == &rttiInfo.attack.vftable) {
        return reinterpret_cast<CCustomModifier*>((uintptr_t)attack
                                                  - offsetof(CCustomModifier, attack));
    } else if (attack->vftable == &rttiInfo.attack2.vftable) {
        return reinterpret_cast<CCustomModifier*>((uintptr_t)attack
                                                  - offsetof(CCustomModifier, attack2));
    }

    return nullptr;
}

game::IUsUnit* CCustomModifier::getPrev()
{
    return umModifier.data->prev;
}

game::IUsSoldier* CCustomModifier::getPrevSoldier()
{
    return game::gameFunctions().castUnitImplToSoldier(getPrev());
}

game::IUsStackLeader* CCustomModifier::getPrevStackLeader()
{
    return game::gameFunctions().castUnitImplToStackLeader(getPrev());
}

game::IAttack* CCustomModifier::getPrevAttack(const game::IAttack* current)
{
    auto prev = getPrevSoldier();
    if (!prev)
        return nullptr;

    if (current == &attack) {
        return prev->vftable->getAttackById(prev);
    } else if (current == &attack2) {
        return prev->vftable->getSecondAttackById(prev);
    }

    return nullptr;
}

bool CCustomModifier::isLeaderOnly()
{
    // TODO: test script to contain only leader modification functions
    return false;
}

CCustomModifier* customModifierCtor(CCustomModifier* thisptr,
                                    const char* script,
                                    const game::CMidgardID* id,
                                    const game::GlobalData** globalData)
{
    using namespace game;

    // Lazy init makes sure that vftable hooks (if any) are already applied
    initRttiInfo();

    thisptr->usUnit.id = emptyId;
    CUmModifierApi::get().constructor(&thisptr->umModifier, id, globalData);
    new (&thisptr->script) std::string(script);
    initVftable(thisptr);

    return thisptr;
}

CCustomModifier* customModifierCopyCtor(CCustomModifier* thisptr, const CCustomModifier* src)
{
    using namespace game;

    thisptr->usUnit.id = src->usUnit.id;
    CUmModifierApi::get().copyConstructor(&thisptr->umModifier, &src->umModifier);
    new (&thisptr->script) std::string(src->script);
    initVftable(thisptr);

    return thisptr;
}

void customModifierDtor(CCustomModifier* thisptr, char flags)
{
    using namespace game;

    thisptr->script.~basic_string();

    CUmModifierApi::get().destructor(&thisptr->umModifier);

    if (flags & 1) {
        Memory::get().freeNonZero(thisptr);
    }
}

void __fastcall unitDtor(game::IUsUnit* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castUnitToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

game::IUsUnitExtension* __fastcall unitCast(const game::IUsUnit* thisptr,
                                            int /*%edx*/,
                                            const char* rawTypeName)
{
    using namespace game;

    const auto& rtti = RttiApi::rtti();
    const auto& rttiApi = RttiApi::get();
    auto& typeInfoRawName = *rttiApi.typeInfoRawName;

    auto thiz = castUnitToCustomModifier(thisptr);
    auto prev = thiz->getPrev();

    if (!strcmp(rawTypeName, typeInfoRawName(rtti.IUsSoldierType))) {
        return (IUsUnitExtension*)&thiz->usSoldier;
    } else if (!strcmp(rawTypeName, typeInfoRawName(rtti.IUsStackLeaderType))) {
        auto prevStackLeader = prev->vftable->cast(thisptr, rawTypeName);
        return prevStackLeader ? (IUsUnitExtension*)&thiz->usStackLeader : nullptr;
    }

    return prev->vftable->cast(prev, rawTypeName);
}

const game::LUnitCategory* __fastcall unitGetCategory(const game::IUsUnit* thisptr, int /*%edx*/)
{
    auto prev = castUnitToCustomModifier(thisptr)->getPrev();
    return prev->vftable->getCategory(prev);
}

void __fastcall soldierDtor(game::IUsSoldier* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castSoldierToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

void __fastcall modifierDtor(game::CUmModifier* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castModifierToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

game::CUmModifier* __fastcall modifierCopy(const game::CUmModifier* thisptr, int /*%edx*/)
{
    using namespace game;

    auto thiz = castModifierToCustomModifier(thisptr);

    auto copy = (CCustomModifier*)Memory::get().allocate(sizeof(CCustomModifier));
    customModifierCopyCtor(copy, thiz);
    return &copy->umModifier;
}

bool __fastcall modifierCanApplyToStackWithLeadership(const game::CUmModifier* thisptr,
                                                      int /*%edx*/,
                                                      const int* leadership)
{
    return true;
}

bool __fastcall modifierCanApplyToUnit(const game::CUmModifier* thisptr, const game::IUsUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto thiz = castModifierToCustomModifier(thisptr);
    if (thiz->isLeaderOnly())
        return fn.castUnitImplToStackLeader(unit) != nullptr;
    else
        return fn.castUnitImplToSoldier(unit) != nullptr;
}

void __fastcall stackLeaderDtor(game::IUsStackLeader* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castStackLeaderToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

void __fastcall attackDtor(game::IAttack* thisptr, int /*%edx*/, char flags)
{
    auto thiz = castAttackToCustomModifier(thisptr);
    customModifierDtor(thiz, flags);
}

void initUnitRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usUnit;
    replaceRttiInfo(info, CUmUnitApi::vftable().usUnit, false);

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&unitDtor;
    vftable.cast = (IUsUnitVftable::Cast)&unitCast;
    vftable.getCategory = (IUsUnitVftable::GetCategory)&unitGetCategory;
}

void initSoldierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usSoldier;
    replaceRttiInfo(info, CUmUnitApi::vftable().usSoldier);

    auto& vftable = info.vftable;
    vftable.destructor = (IUsUnitExtensionVftable::Destructor)&soldierDtor;
    // TODO: replace !all! vftable members, do not copy original vftable
}

void initModifierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.umModifier;
    replaceRttiInfo(info, CUmUnitApi::vftable().umModifier);

    auto& vftable = info.vftable;
    vftable.destructor = (CUmModifierVftable::Destructor)&modifierDtor;
    vftable.copy = (CUmModifierVftable::Copy)&modifierCopy;
    vftable.canApplyToStackWithLeadership =
        (CUmModifierVftable::CanApplyToStackWithLeadership)&modifierCanApplyToStackWithLeadership;
    vftable.canApplyToUnit = (CUmModifierVftable::CanApplyToUnit)&modifierCanApplyToUnit;
    // TODO: replace !all! vftable members, do not copy original vftable
}

void initStackLeaderRttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IUsStackLeader for dynamic casts so we should be fine
    // without RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.usStackLeader;
    info.locator = nullptr;

    auto& vftable = info.vftable;
    vftable.destructor = (IUsUnitExtensionVftable::Destructor)&stackLeaderDtor;
    // TODO: replace !all! vftable members
}

void initAttackRttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IAttack for dynamic casts so we should be fine
    // without RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.attack;
    info.locator = nullptr;

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&attackDtor;
    // TODO: replace !all! vftable members
}

void initAttack2RttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IAttack for dynamic casts so we should be fine
    // without RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.attack2;
    info.locator = nullptr;

    auto& vftable = info.vftable;
    vftable.destructor = (IMidObjectVftable::Destructor)&attackDtor;
    // TODO: replace !all! vftable members
}

void initRttiInfo()
{
    static bool initialized = false;
    if (!initialized) {
        initUnitRttiInfo();
        initSoldierRttiInfo();
        initModifierRttiInfo();
        initStackLeaderRttiInfo();
        initAttackRttiInfo();
        initAttack2RttiInfo();
        initialized = true;
    }
}

void initVftable(CCustomModifier* thisptr)
{
    thisptr->usUnit.vftable = &rttiInfo.usUnit.vftable;
    thisptr->usSoldier.vftable = &rttiInfo.usSoldier.vftable;
    thisptr->umModifier.vftable = &rttiInfo.umModifier.vftable;
    thisptr->usStackLeader.vftable = &rttiInfo.usStackLeader.vftable;
    thisptr->attack.vftable = &rttiInfo.attack.vftable;
    thisptr->attack2.vftable = &rttiInfo.attack2.vftable;
}

game::CUmModifier* createCustomModifier(const char* script,
                                        const game::CMidgardID* id,
                                        const game::GlobalData** globalData)
{
    using namespace game;

    auto customModifier = (CCustomModifier*)Memory::get().allocate(sizeof(CCustomModifier));
    customModifierCtor(customModifier, script, id, globalData);
    return &customModifier->umModifier;
}

} // namespace hooks
