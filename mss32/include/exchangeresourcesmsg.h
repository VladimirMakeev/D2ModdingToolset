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

#ifndef EXCHANGERESOURCESMSG_H
#define EXCHANGERESOURCESMSG_H

#include "currency.h"
#include "midgardid.h"
#include "netmsg.h"
#include <cstdint>

namespace game {
struct TypeDescriptor;
struct CPhaseGame;
} // namespace game

namespace hooks {

/** Describes exchange between player and a resource market site. */
struct CExchangeResourcesMsg : public game::CNetMsg
{
    CExchangeResourcesMsg();

    CExchangeResourcesMsg(const game::CMidgardID& siteId,
                          const game::CMidgardID& visitorStackId,
                          game::CurrencyType playerCurrency,
                          game::CurrencyType siteCurrency,
                          std::uint16_t amount);

    game::CMidgardID siteId;
    game::CMidgardID visitorStackId;
    game::CurrencyType playerCurrency;
    game::CurrencyType siteCurrency;
    std::uint16_t amount;
};

game::TypeDescriptor* getExchangeResourcesMsgTypeDescriptor();

void sendExchangeResourcesMsg(game::CPhaseGame* phaseGame,
                              const game::CMidgardID& siteId,
                              const game::CMidgardID& visitorStackId,
                              game::CurrencyType playerCurrency,
                              game::CurrencyType marketCurrency,
                              std::uint16_t amount);

} // namespace hooks

#endif // EXCHANGERESOURCESMSG_H
