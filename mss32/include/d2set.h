/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef D2SET_H
#define D2SET_H

#include "d2tree.h"

namespace game {

template <typename T>
struct SetNode : public TreeNode<T, SetNode<T>>
{ };

/** Implementation of std::set<T> used in game. */
template <typename T, typename Allocator = void*>
struct Set : public Tree<T, SetNode<T>, Allocator>
{ };

template <typename T>
struct SetIterator : public TreeIterator<T, SetNode<T>>
{ };

} // namespace game

#endif // D2SET_H
