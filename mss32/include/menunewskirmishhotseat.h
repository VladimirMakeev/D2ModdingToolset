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

#ifndef MENUNEWSKIRMISHHOTSEAT_H
#define MENUNEWSKIRMISHHOTSEAT_H

#include "menunewskirmish.h"

namespace game {

/**
 * New hotseat skirmish menu.
 * Represents DLG_HOTSEAT_NEW from Interf.dlg.
 */
struct CMenuNewSkirmishHotseat : public CMenuNewSkirmish
{ };

assert_size(CMenuNewSkirmishHotseat, 12);

namespace CMenuNewSkirmishHotseatApi {

struct Api
{
    using Constructor = CMenuNewSkirmishHotseat*(__thiscall*)(CMenuNewSkirmishHotseat* thisptr,
                                                              CMenuPhase* menuPhase);
    Constructor constructor;
};

Api& get();

} // namespace CMenuNewSkirmishHotseatApi

} // namespace game

#endif // MENUNEWSKIRMISHHOTSEAT_H
