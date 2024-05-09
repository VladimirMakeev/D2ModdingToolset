/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2024 Vladimir Makeev.
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

#ifndef TASKOBJMOVE_H
#define TASKOBJMOVE_H

#include "taskobj.h"

namespace game {

namespace editor {

struct CObjMoveSelect;

struct CTaskObjMoveData
{
    CObjMoveSelect* objMoveSelect;
    int unknown;
    int unknown2;
    char unknown3[32];
};

assert_size(CTaskObjMoveData, 44);

/** Moves existing objects on scenario map. */
struct CTaskObjMove : public CTaskObj
{
    CTaskObjMoveData* moveData;
};

assert_size(CTaskObjMove, 32);

namespace CTaskObjMoveApi {

struct Api
{
    using Constructor = CTaskObjMove*(__thiscall*)(CTaskObjMove* thisptr, CObjectInterf* objInterf);
    Constructor constructor;
};

Api& get();

} // namespace CTaskObjMoveApi

} // namespace editor

} // namespace game

#endif // TASKOBJMOVE_H
