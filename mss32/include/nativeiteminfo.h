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

#ifndef NATIVEITEMINFO_H
#define NATIVEITEMINFO_H

#include "iteminfo.h"
#include "rsgid.h"

namespace hooks {

class NativeItemInfo final : public rsg::ItemInfo
{
public:
    NativeItemInfo(const rsg::CMidgardID& itemId, int value, rsg::ItemType itemType)
        : itemId{itemId}
        , value{value}
        , itemType{itemType}
    { }

    ~NativeItemInfo() override = default;

    const rsg::CMidgardID& getItemId() const override
    {
        return itemId;
    }

    rsg::ItemType getItemType() const override
    {
        return itemType;
    }

    int getValue() const override
    {
        return value;
    }

private:
    rsg::CMidgardID itemId;
    int value{};
    rsg::ItemType itemType{rsg::ItemType::Valuable};
};

} // namespace hooks

#endif // NATIVEITEMINFO_H
