/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "game.h"
#include "version.h"
#include <array>

namespace game {

// clang-format off
static std::array<Functions, 3> functions = {{
	// Akella
    Functions{
		(RespopupInitFunc)0x4893ad,
		(ToggleShowBannersInitFunc)0x5b4015
	},
    // Russobit
    Functions{
		(RespopupInitFunc)0x4893ad,
		(ToggleShowBannersInitFunc)0x5b4015
	},
    // Gog
    Functions{
		(RespopupInitFunc)0x488f96,
		(ToggleShowBannersInitFunc)0x5b32db
	}
}};

static std::array<Variables, 3> variables = {{
	// Akella
	Variables{
		(int*)0x837ac8,
		(unsigned char*)0x837acc
	},
	// Russobit
	Variables{
		(int*)0x837ac8,
		(unsigned char*)0x837acc
	},
	// Gog
	Variables{
		(int*)0x835a78,
		(unsigned char*)0x835a7c
	}
}};
// clang-format on

Functions& gameFunctions()
{
    return functions[static_cast<int>(hooks::gameVersion())];
}

Variables& gameVariables()
{
    return variables[static_cast<int>(hooks::gameVersion())];
}

} // namespace game
