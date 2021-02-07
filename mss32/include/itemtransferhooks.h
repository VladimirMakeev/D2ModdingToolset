/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#ifndef ITEMTRANSFERHOOKS_H
#define ITEMTRANSFERHOOKS_H

namespace game {
struct CMidgardID;
struct CCityStackInterf;
struct CPhaseGame;
struct CExchangeInterf;
struct CPickUpDropInterf;
struct CSiteMerchantInterf;
} // namespace game

namespace hooks {

game::CCityStackInterf* __fastcall cityStackInterfCtorHooked(game::CCityStackInterf* thisptr,
                                                             int /*%edx*/,
                                                             void* taskOpenInterf,
                                                             game::CPhaseGame* phaseGame,
                                                             game::CMidgardID* cityId);

game::CExchangeInterf* __fastcall exchangeInterfCtorHooked(game::CExchangeInterf* thisptr,
                                                           int /*%edx*/,
                                                           void* taskOpenInterf,
                                                           game::CPhaseGame* phaseGame,
                                                           game::CMidgardID* stackLeftSide,
                                                           game::CMidgardID* stackRightSide);

game::CPickUpDropInterf* __fastcall pickupDropInterfCtorHooked(game::CPickUpDropInterf* thisptr,
                                                               int /*%edx*/,
                                                               void* taskOpenInterf,
                                                               game::CPhaseGame* phaseGame,
                                                               game::CMidgardID* stackId,
                                                               game::CMidgardID* bagId);

game::CSiteMerchantInterf* __fastcall siteMerchantInterfCtorHooked(
    game::CSiteMerchantInterf* thisptr,
    int /*%edx*/,
    void* taskOpenInterf,
    game::CPhaseGame* phaseGame,
    game::CMidgardID* stackId,
    game::CMidgardID* merchantId);

} // namespace hooks

#endif // ITEMTRANSFERHOOKS_H
