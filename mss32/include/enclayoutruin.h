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

#ifndef ENCLAYOUTRUIN_H
#define ENCLAYOUTRUIN_H

#include "enclayout.h"

namespace game {

struct CEncLayoutRuinData;
struct IMidgardObjectMap;
struct CMidRuin;

struct CEncLayoutRuin : public IEncLayout
{
    CEncLayoutRuinData* data;
};

assert_size(CEncLayoutRuin, 16);

struct CEncLayoutRuinData
{
    const IMidgardObjectMap* objectMap;
    char unknown[200];
};

assert_size(CEncLayoutRuinData, 204);

namespace CEncLayoutRuinApi {

struct Api
{
    using Update = void(__stdcall*)(const CEncLayoutRuinData* data,
                                    const CMidRuin* ruin,
                                    CDialogInterf* dialog);
    Update update;
};

Api& get();

IEncLayoutVftable* vftable();

} // namespace CEncLayoutRuinApi

} // namespace game

#endif // ENCLAYOUTRUIN_H
