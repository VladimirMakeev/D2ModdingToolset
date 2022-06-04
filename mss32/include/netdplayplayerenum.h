/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef NETDPLAYPLAYERENUM_H
#define NETDPLAYPLAYERENUM_H

#include "d2string.h"
#include "mqnetplayerenum.h"

namespace game {

/** Thin wrapper of player data for Direct Play. */
struct CNetDPlayPlayerEnum : public IMqNetPlayerEnum
{
    int playerNetId;
    String name;
};

assert_size(CNetDPlayPlayerEnum, 24);

} // namespace game

#endif // NETDPLAYPLAYERENUM_H
