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

#ifndef VISITORCREATESITE_H
#define VISITORCREATESITE_H

#include "d2string.h"
#include "mqpoint.h"
#include "sitecategories.h"
#include "visitors.h"

namespace game {

namespace editor {

struct CVisitorCreateSite : public CScenarioVisitor
{
    LSiteCategory category;
    CMqPoint position;
    int imgIso;
    String interfaceImage;
    String name;
    String description;
};

assert_size(CVisitorCreateSite, 80);

namespace CVisitorCreateSiteApi {

struct Api
{
    using CanApply = bool(__thiscall*)(const CVisitorCreateSite* thisptr);
    CanApply canApply;

    using Apply = bool(__thiscall*)(const CVisitorCreateSite* thisptr);
    Apply apply;
};

Api& get();

} // namespace CVisitorCreateSiteApi

} // namespace editor

} // namespace game

#endif // VISITORCREATESITE_H
