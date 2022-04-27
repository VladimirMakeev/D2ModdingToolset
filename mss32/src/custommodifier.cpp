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
#include "mempool.h"
#include "utils.h"

namespace hooks {

static struct
{
    game::RttiInfo<game::IUsUnitVftable> usUnit;
    game::RttiInfo<game::IUsSoldierVftable> usSoldier;
    game::RttiInfo<game::CUmModifierVftable> umModifier;
    game::RttiInfo<game::IUsStackLeaderVftable> usStackLeader;
} rttiInfo;

void initRttiInfo();

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

CCustomModifier* customModifierCtor(CCustomModifier* thisptr,
                                    const char* script,
                                    const game::CMidgardID* id,
                                    const game::GlobalData** globalData)
{
    using namespace game;

    const auto& umUnitApi = CUmUnitApi::get();

    // Lazy init makes sure that vftable hooks (if any) are already applied
    initRttiInfo();

    thisptr->usUnit.unitId = emptyId;

    CUmModifierApi::get().constructor(&thisptr->umModifier, id, globalData);

    new (&thisptr->script) std::string(script);

    thisptr->usUnit.vftable = &rttiInfo.usUnit.vftable;
    thisptr->usSoldier.vftable = &rttiInfo.usSoldier.vftable;
    thisptr->umModifier.vftable = &rttiInfo.umModifier.vftable;
    thisptr->usStackLeader.vftable = &rttiInfo.usStackLeader.vftable;

    return thisptr;
}

void customModifierDtor(CCustomModifier* thisptr, char flags)
{
    using namespace game;

    thisptr->script.~basic_string();

    thisptr->umModifier.vftable->destructor(&thisptr->umModifier, flags);

    if (flags & 1) {
        Memory::get().freeNonZero(thisptr);
    }
}

void __fastcall unitDtor(game::IUsUnit* thisptr, int /*%edx*/, char flags)
{
    auto customModifier = castUnitToCustomModifier(thisptr);
    customModifierDtor(customModifier, flags);
}

void __fastcall soldierDtor(game::IUsSoldier* thisptr, int /*%edx*/, char flags)
{
    auto customModifier = castSoldierToCustomModifier(thisptr);
    customModifierDtor(customModifier, flags);
}

void __fastcall modifierDtor(game::CUmModifier* thisptr, int /*%edx*/, char flags)
{
    auto customModifier = castModifierToCustomModifier(thisptr);
    customModifierDtor(customModifier, flags);
}

void __fastcall stackLeaderDtor(game::IUsStackLeader* thisptr, int /*%edx*/, char flags)
{
    auto customModifier = castStackLeaderToCustomModifier(thisptr);
    customModifierDtor(customModifier, flags);
}

void initUnitRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usUnit;
    replaceRttiInfo(info, CUmUnitApi::vftable().usUnit);

    info.vftable.destructor = (IMidObjectVftable::Destructor)&unitDtor;
    // TODO: replace vftable members
}

void initSoldierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.usSoldier;
    replaceRttiInfo(info, CUmUnitApi::vftable().usSoldier);

    info.vftable.destructor = (IUsUnitExtensionVftable::Destructor)&soldierDtor;
    // TODO: replace vftable members
}

void initModifierRttiInfo()
{
    using namespace game;

    auto& info = rttiInfo.umModifier;
    replaceRttiInfo(info, CUmUnitApi::vftable().umModifier);

    info.vftable.destructor = (CUmModifierVftable::Destructor)&modifierDtor;
    // TODO: replace vftable members
}

void initStackLeaderRttiInfo()
{
    using namespace game;

    // None of the existing RTTI can be reused as this class has unique offset.
    // Lucky for us, the game is not using IUsStackLeader for dynamic casts so we should be fine
    // without RTTI. Otherwise, we would need to either patch dynamicCast or create our own RTTI.
    auto& info = rttiInfo.usStackLeader;
    info.locator = nullptr;

    info.vftable.destructor = (IUsUnitExtensionVftable::Destructor)&stackLeaderDtor;
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
        initialized = true;
    }
}

} // namespace hooks
