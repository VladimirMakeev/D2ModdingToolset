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

#include "stacktemplatecache.h"
#include <unordered_map>

namespace hooks {

using StackTemplateCache = std::unordered_map<game::CMidgardID /* stack template id */,
                                              StacksSet /* stacks created from template */,
                                              game::CMidgardIDHash>;

static StackTemplateCache cache;

void stackTemplateCacheAdd(const game::CMidgardID& stackTemplateId, const game::CMidgardID& stackId)
{
    StacksSet& stacksCreatedFromTemplate{cache[stackTemplateId]};

    stacksCreatedFromTemplate.insert(stackId);
}

void stackTemplateCacheRemove(const game::CMidgardID& stackTemplateId,
                              const game::CMidgardID& stackId)
{
    const auto it{cache.find(stackTemplateId)};
    if (it == cache.cend()) {
        // No mapping found
        return;
    }

    StacksSet& stacksCreatedFromTemplate{it->second};
    stacksCreatedFromTemplate.erase(stackId);
}

const StacksSet* stackTemplateCacheFind(const game::CMidgardID& stackTemplateId)
{
    const auto it{cache.find(stackTemplateId)};
    if (it == cache.cend()) {
        return nullptr;
    }

    return &it->second;
}

bool stackTemplateCacheCheck(const game::CMidgardID& stackTemplateId)
{
    const StacksSet* stacks{stackTemplateCacheFind(stackTemplateId)};
    return stacks && stacks->size();
}

void stackTemplateCacheClear()
{
    cache.clear();
}

} // namespace hooks
