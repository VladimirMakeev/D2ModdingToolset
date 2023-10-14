/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef LOVECHAT_H
#define LOVECHAT_H

#include "d2assert.h"

namespace game {

struct TextMessage;
struct ILoveChatVftable;

// Yes, this name was used in original game source code
struct ILoveChat
{
    ILoveChatVftable* vftable;
};

struct ILoveChatVftable
{
    using Destructor = void(__thiscall*)(ILoveChat* thisptr, char flags);
    Destructor destructor;

    using Method1 = void(__thiscall*)(ILoveChat* thisptr,
                                      const TextMessage* chatMessage,
                                      const char* messageText,
                                      bool a4);
    Method1 method1;
};

assert_vftable_size(ILoveChatVftable, 2);

} // namespace game

#endif // LOVECHAT_H
