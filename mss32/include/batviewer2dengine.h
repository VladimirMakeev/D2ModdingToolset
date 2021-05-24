/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef BATVIEWER2DENGINE_H
#define BATVIEWER2DENGINE_H

namespace game {

struct CBatViewer2DEngine;
struct IMqImage2;
struct CMqPoint;

namespace BatViewer2DEngineApi {

struct Api
{
    using ClearTargetMarks = void(__thiscall*)(CBatViewer2DEngine* thisptr);
    ClearTargetMarks clearTargetMarks;

    using DrawTargetMark = void(__thiscall*)(CBatViewer2DEngine* thisptr,
                                             IMqImage2* image,
                                             CMqPoint* imagePosition,
                                             bool isUnitOnTheLeft,
                                             int unitPosition);
    DrawTargetMark drawTargetMark;
};

Api& get();

} // namespace BatViewer2DEngineApi

} // namespace game

#endif // BATVIEWER2DENGINE_H
