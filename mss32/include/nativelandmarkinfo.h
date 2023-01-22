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

#ifndef NATIVELANDMARKINFO_H
#define NATIVELANDMARKINFO_H

#include "landmarkinfo.h"
#include "position.h"
#include "rsgid.h"

namespace hooks {

class NativeLandmarkInfo final : public rsg::LandmarkInfo
{
public:
    NativeLandmarkInfo(const rsg::CMidgardID& landmarkId,
                       const rsg::Position& size,
                       rsg::LandmarkType type,
                       bool mountain)
        : landmarkId{landmarkId}
        , size{size}
        , landmarkType{type}
        , mountain{mountain}
    { }

    ~NativeLandmarkInfo() override = default;

    const rsg::CMidgardID& getLandmarkId() const override
    {
        return landmarkId;
    }

    rsg::LandmarkType getLandmarkType() const override
    {
        return landmarkType;
    }

    const rsg::Position& getSize() const override
    {
        return size;
    }

    bool isMountain() const override
    {
        return mountain;
    }

private:
    rsg::CMidgardID landmarkId;
    rsg::Position size{1, 1};
    rsg::LandmarkType landmarkType{rsg::LandmarkType::Misc};
    bool mountain{};
};

} // namespace hooks

#endif // NATIVELANDMARKINFO_H
