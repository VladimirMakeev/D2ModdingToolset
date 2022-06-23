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

#ifndef ENCLAYOUTCITY_H
#define ENCLAYOUTCITY_H

#include "enclayout.h"

namespace game {

struct CEncLayoutCityData;
struct IMidgardObjectMap;
struct CFortification;

struct CEncLayoutCity : public IEncLayout
{
    CEncLayoutCityData* data;
};

assert_size(CEncLayoutCity, 16);

struct CEncLayoutCityData
{
    const IMidgardObjectMap* objectMap;
    char unknown[156];
};

assert_size(CEncLayoutCityData, 160);

namespace CEncLayoutCityApi {

struct Api
{
    using Update = void(__thiscall*)(CEncLayoutCity* thisptr,
                                     const IMidgardObjectMap* objectMap,
                                     const CFortification* fort,
                                     CDialogInterf* dialog);
    Update update;
};

Api& get();

IEncLayoutVftable* vftable();

} // namespace CEncLayoutCityApi

} // namespace game

#endif // ENCLAYOUTCITY_H
