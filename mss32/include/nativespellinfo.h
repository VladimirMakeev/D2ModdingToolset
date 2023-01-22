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

#ifndef NATIVESPELLINFO_H
#define NATIVESPELLINFO_H

#include "rsgid.h"
#include "spellinfo.h"

namespace hooks {

class NativeSpellInfo final : public rsg::SpellInfo
{
public:
    NativeSpellInfo(const rsg::CMidgardID& spellId, int value, int level, rsg::SpellType spellType)
        : spellId{spellId}
        , value{value}
        , level{level}
        , spellType{spellType}
    { }

    ~NativeSpellInfo() override = default;

    const rsg::CMidgardID& getSpellId() const override
    {
        return spellId;
    }

    rsg::SpellType getSpellType() const override
    {
        return spellType;
    }

    int getValue() const override
    {
        return value;
    }

    int getLevel() const override
    {
        return level;
    }

private:
    rsg::CMidgardID spellId;
    int value{};
    int level{};
    rsg::SpellType spellType{rsg::SpellType::Attack};
};

} // namespace hooks

#endif // NATIVESPELLINFO_H
