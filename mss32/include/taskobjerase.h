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

#ifndef TASKOBJERASE_H
#define TASKOBJERASE_H

#include "taskobj.h"

namespace game {

namespace editor {

struct CTaskObjEraseData
{
    char unknown[12];
    SmartPointer ptr;
    SmartPointer ptr2;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
};

assert_size(CTaskObjEraseData, 52);

struct CTaskObjErase : public CTaskObj
{
    CTaskObjEraseData* taskEraseData;
};

assert_size(CTaskObjErase, 32);

namespace CTaskObjEraseApi {

struct Api
{
    using Constructor = CTaskObjErase*(__thiscall*)(CTaskObjErase* thisptr,
                                                    CObjectInterf* objInterf);
    Constructor constructor;
};

Api& get();

} // namespace CTaskObjEraseApi

} // namespace editor

} // namespace game

#endif // TASKOBJERASE_H
