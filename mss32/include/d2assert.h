/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef D2ASSERT_H
#define D2ASSERT_H

#include <cstddef>

#define assert_size(type, size)                                                                    \
    static_assert(sizeof(type) == (size),                                                          \
                  "Size of " #type " structure must be exactly " #size " bytes")

#define assert_vftable_size(vtable, count)                                                         \
    static_assert(sizeof(vtable) == (count) * sizeof(void*),                                       \
                  #vtable " must have exactly " #count " methods")

#define assert_offset(type, field, offset)                                                         \
    static_assert(offsetof(type, field) == (offset),                                               \
                  #type "::" #field " offset must be " #offset " bytes")

#endif // D2ASSERT_H
