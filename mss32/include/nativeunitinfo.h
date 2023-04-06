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

#ifndef NATIVEUNITINFO_H
#define NATIVEUNITINFO_H

#include "rsgid.h"
#include "unitinfo.h"

namespace hooks {

class NativeUnitInfo final : public rsg::UnitInfo
{
public:
    NativeUnitInfo(const rsg::CMidgardID& unitId,
                   const rsg::CMidgardID& raceId,
                   const rsg::CMidgardID& nameId,
                   int level,
                   int value,
                   rsg::UnitType type,
                   rsg::SubRaceType subrace,
                   rsg::ReachType reach,
                   rsg::AttackType attackType,
                   int hp,
                   int move,
                   int leadership,
                   bool bigUnit,
                   bool male)
        : unitId{unitId}
        , raceId{raceId}
        , nameId{nameId}
        , level{level}
        , value{value}
        , unitType{type}
        , subrace{subrace}
        , reach{reach}
        , attackType{attackType}
        , hitPoints{hp}
        , move{move}
        , leadership{leadership}
        , bigUnit{bigUnit}
        , male{male}
    { }

    ~NativeUnitInfo() override = default;

    const rsg::CMidgardID& getUnitId() const override
    {
        return unitId;
    }

    const rsg::CMidgardID& getRaceId() const override
    {
        return raceId;
    }

    const rsg::CMidgardID& getNameId() const override
    {
        return nameId;
    }

    int getLevel() const override
    {
        return level;
    }

    int getValue() const override
    {
        return value;
    }

    rsg::UnitType getUnitType() const override
    {
        return unitType;
    }

    rsg::SubRaceType getSubrace() const override
    {
        return subrace;
    }

    rsg::ReachType getAttackReach() const override
    {
        return reach;
    }

    rsg::AttackType getAttackType() const override
    {
        return attackType;
    }

    int getHp() const override
    {
        return hitPoints;
    }

    int getMove() const override
    {
        return move;
    }

    int getLeadership() const override
    {
        return leadership;
    }

    bool isBig() const override
    {
        return bigUnit;
    }

    bool isMale() const override
    {
        return male;
    }

private:
    rsg::CMidgardID unitId;
    rsg::CMidgardID raceId;
    rsg::CMidgardID nameId;
    int level{};
    int value{};
    rsg::UnitType unitType{rsg::UnitType::Soldier};
    rsg::SubRaceType subrace{rsg::SubRaceType::Neutral};
    rsg::ReachType reach{rsg::ReachType::Adjacent};
    rsg::AttackType attackType{rsg::AttackType ::Damage};
    int hitPoints{};
    int move{};
    int leadership{};
    bool bigUnit{};
    bool male{};
};

} // namespace hooks

#endif // NATIVEUNITINFO_H
