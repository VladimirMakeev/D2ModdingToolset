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

#ifndef STACKTEMPLATECACHE_H
#define STACKTEMPLATECACHE_H

#include "midgardid.h"
#include <unordered_set>

namespace hooks {

using StacksSet = std::unordered_set<game::CMidgardID /* stack id */, game::CMidgardIDHash>;

/** Adds mapping of stack template and a stack created from it. */
void stackTemplateCacheAdd(const game::CMidgardID& stackTemplateId,
                           const game::CMidgardID& stackId);

/** Removes existing stack template and stack mapping. */
void stackTemplateCacheRemove(const game::CMidgardID& stackTemplateId,
                              const game::CMidgardID& stackId);

/** Returns stacks created from template or nullptr if no stacks were created. */
const StacksSet* stackTemplateCacheFind(const game::CMidgardID& stackTemplateId);

/** Returns true if at least 1 stack was created from specified template. */
bool stackTemplateCacheCheck(const game::CMidgardID& stackTemplateId);

/** Clears entire cache. */
void stackTemplateCacheClear();

} // namespace hooks

#endif // STACKTEMPLATECACHE_H
