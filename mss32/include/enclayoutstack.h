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

#ifndef ENCLAYOUTSTACK_H
#define ENCLAYOUTSTACK_H

#include "enclayout.h"
#include "midgardid.h"

namespace game {

struct CEncLayoutStackData;
struct IMidgardObjectMap;
struct CMidStack;
struct IMqImage2;

struct CEncLayoutStack : public IEncLayout
{
    CEncLayoutStackData* data;
};

assert_size(CEncLayoutStack, 16);

// Similar to GroupAdapterUnitData
struct CEncLayoutStackGroupData
{
    bool unknown;
    bool unknown1;
    bool boostApplied[6];
    bool lowerApplied[6];
    bool potionApplied[6];
    bool canLevelUp[6];
    bool hasHighLevel[6];
    int highLevel[6];
};

assert_size(CEncLayoutStackGroupData, 56);

struct CEncLayoutStackData
{
    IMidgardObjectMap* objectMap;
    CMidgardID stackId;
    CMidgardID playerId;
    SmartPointer functor;
    IMqImage2* spellBoostIcon;
    IMqImage2* spellLowerIcon;
    IMqImage2* potionBoostIcon;
    IMqImage2* upgrageIcon;
    IMqImage2* highLevelIcons[3];
    CMqRect imgLowerArea;
    CMqRect imgBoostArea;
    CEncLayoutStackGroupData group;
    CMqRect unitTxtAreas[6];
    CMqRect unitImgAreas[6];
};

assert_size(CEncLayoutStackData, 328);

namespace CEncLayoutStackApi {

struct Api
{
    using Update = void(__thiscall*)(CEncLayoutStack* thisptr,
                                     const IMidgardObjectMap* objectMap,
                                     const CMidStack* stack,
                                     CDialogInterf* dialog);
    Update update;
};

Api& get();

IEncLayoutVftable* vftable();

} // namespace CEncLayoutStackApi

} // namespace game

#endif // ENCLAYOUTSTACK_H
