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

#include "isolayers.h"
#include "version.h"
#include <array>

namespace game {

// clang-format off
static std::array<IsoLayers, 4> layers = {{
    // Akella
    IsoLayers{
        (CIsoLayer*)0x83a62c,
        (CIsoLayer*)0x83a654,
        (CIsoLayer*)0x83a5a4,
        (CIsoLayer*)0x83a5bc,
        (CIsoLayer*)0x83a594,
        (CIsoLayer*)0x83a5b4,
        (CIsoLayer*)0x83a610,
        (CIsoLayer*)0x83a5f8,
        (CIsoLayer*)0x83a5ac,
        (CIsoLayer*)0x83a5ec,
        (CIsoLayer*)0x83a68c,
        (CIsoLayer*)0x83a65c,
        (CIsoLayer*)0x83a630,
        (CIsoLayer*)0x83a5f0,
        (CIsoLayer*)0x83a628,
        (CIsoLayer*)0x83a660,
        (CIsoLayer*)0x83a608,
        (CIsoLayer*)0x83a680,
        (CIsoLayer*)0x83a5c4,
        (CIsoLayer*)0x83a580,
        (CIsoLayer*)0x83a624,
        (CIsoLayer*)0x83a658,
        (CIsoLayer*)0x83a684,
        (CIsoLayer*)0x83a5f4,
        (CIsoLayer*)0x83a604,
        (CIsoLayer*)0x83a5c0,
        (CIsoLayer*)0x83a5c8,
        (CIsoLayer*)0x83a668,
        (CIsoLayer*)0x83a5b8,
        (CIsoLayer*)0x83a67c,
        (CIsoLayer*)0x83a5fc,
        (CIsoLayer*)0x83a58c,
        (CIsoLayer*)0x83a690,
        (CIsoLayer*)0x83a614,
        (CIsoLayer*)0x83a674,
        (CIsoLayer*)0x83a5a0,
        (CIsoLayer*)0x83a678,
        (CIsoLayer*)0x83a588,
        (CIsoLayer*)0x83a59c,
        (CIsoLayer*)0x83a600,
        (CIsoLayer*)0x83a61c,
        (CIsoLayer*)0x83a5a8,
        (CIsoLayer*)0x83a670,
        (CIsoLayer*)0x83a664,
        (CIsoLayer*)0x83a584,
        (CIsoLayer*)0x83a688,
        (CIsoLayer*)0x83a5b0,
        (CIsoLayer*)0x83a620,
        (CIsoLayer*)0x83a66c,
        (CIsoLayer*)0x83a590,
        (CIsoLayer*)0x83a598,
        (CIsoLayer*)0x83a694,
    },
    // Russobit
    IsoLayers{
        (CIsoLayer*)0x83a62c,
        (CIsoLayer*)0x83a654,
        (CIsoLayer*)0x83a5a4,
        (CIsoLayer*)0x83a5bc,
        (CIsoLayer*)0x83a594,
        (CIsoLayer*)0x83a5b4,
        (CIsoLayer*)0x83a610,
        (CIsoLayer*)0x83a5f8,
        (CIsoLayer*)0x83a5ac,
        (CIsoLayer*)0x83a5ec,
        (CIsoLayer*)0x83a68c,
        (CIsoLayer*)0x83a65c,
        (CIsoLayer*)0x83a630,
        (CIsoLayer*)0x83a5f0,
        (CIsoLayer*)0x83a628,
        (CIsoLayer*)0x83a660,
        (CIsoLayer*)0x83a608,
        (CIsoLayer*)0x83a680,
        (CIsoLayer*)0x83a5c4,
        (CIsoLayer*)0x83a580,
        (CIsoLayer*)0x83a624,
        (CIsoLayer*)0x83a658,
        (CIsoLayer*)0x83a684,
        (CIsoLayer*)0x83a5f4,
        (CIsoLayer*)0x83a604,
        (CIsoLayer*)0x83a5c0,
        (CIsoLayer*)0x83a5c8,
        (CIsoLayer*)0x83a668,
        (CIsoLayer*)0x83a5b8,
        (CIsoLayer*)0x83a67c,
        (CIsoLayer*)0x83a5fc,
        (CIsoLayer*)0x83a58c,
        (CIsoLayer*)0x83a690,
        (CIsoLayer*)0x83a614,
        (CIsoLayer*)0x83a674,
        (CIsoLayer*)0x83a5a0,
        (CIsoLayer*)0x83a678,
        (CIsoLayer*)0x83a588,
        (CIsoLayer*)0x83a59c,
        (CIsoLayer*)0x83a600,
        (CIsoLayer*)0x83a61c,
        (CIsoLayer*)0x83a5a8,
        (CIsoLayer*)0x83a670,
        (CIsoLayer*)0x83a664,
        (CIsoLayer*)0x83a584,
        (CIsoLayer*)0x83a688,
        (CIsoLayer*)0x83a5b0,
        (CIsoLayer*)0x83a620,
        (CIsoLayer*)0x83a66c,
        (CIsoLayer*)0x83a590,
        (CIsoLayer*)0x83a598,
        (CIsoLayer*)0x83a694,
    },
    // Gog
    IsoLayers{
        (CIsoLayer*)0x8385d8,
        (CIsoLayer*)0x8385fc,
        (CIsoLayer*)0x838554,
        (CIsoLayer*)0x83856c,
        (CIsoLayer*)0x838544,
        (CIsoLayer*)0x838564,
        (CIsoLayer*)0x8385bc,
        (CIsoLayer*)0x838588,
        (CIsoLayer*)0x83855c,
        (CIsoLayer*)0x83857c,
        (CIsoLayer*)0x838638,
        (CIsoLayer*)0x838604,
        (CIsoLayer*)0x8385dc,
        (CIsoLayer*)0x838580,
        (CIsoLayer*)0x8385d4,
        (CIsoLayer*)0x838608,
        (CIsoLayer*)0x8385b8,
        (CIsoLayer*)0x83862c,
        (CIsoLayer*)0x838574,
        (CIsoLayer*)0x838530,
        (CIsoLayer*)0x8385d0,
        (CIsoLayer*)0x838600,
        (CIsoLayer*)0x838630,
        (CIsoLayer*)0x838584,
        (CIsoLayer*)0x8385b4,
        (CIsoLayer*)0x838570,
        (CIsoLayer*)0x838578,
        (CIsoLayer*)0x838614,
        (CIsoLayer*)0x838568,
        (CIsoLayer*)0x838628,
        (CIsoLayer*)0x83858c,
        (CIsoLayer*)0x83853c,
        (CIsoLayer*)0x83863c,
        (CIsoLayer*)0x8385c0,
        (CIsoLayer*)0x838620,
        (CIsoLayer*)0x838550,
        (CIsoLayer*)0x838624,
        (CIsoLayer*)0x838538,
        (CIsoLayer*)0x83854c,
        (CIsoLayer*)0x838590,
        (CIsoLayer*)0x8385c8,
        (CIsoLayer*)0x838558,
        (CIsoLayer*)0x83861c,
        (CIsoLayer*)0x838610,
        (CIsoLayer*)0x838534,
        (CIsoLayer*)0x838634,
        (CIsoLayer*)0x838560,
        (CIsoLayer*)0x8385cc,
        (CIsoLayer*)0x838618,
        (CIsoLayer*)0x838540,
        (CIsoLayer*)0x838548,
        (CIsoLayer*)0x838640,
    },
    // Scenario Editor
    IsoLayers{
        (CIsoLayer*)0x6661ac,
        (CIsoLayer*)0x6661d4,
        (CIsoLayer*)0x666124,
        (CIsoLayer*)0x66613c,
        (CIsoLayer*)0x666114,
        (CIsoLayer*)0x666134,
        (CIsoLayer*)0x666190,
        (CIsoLayer*)0x666178,
        (CIsoLayer*)0x66612c,
        (CIsoLayer*)0x66616c,
        (CIsoLayer*)0x66620c,
        (CIsoLayer*)0x6661dc,
        (CIsoLayer*)0x6661b0,
        (CIsoLayer*)0x666170,
        (CIsoLayer*)0x6661a8,
        (CIsoLayer*)0x6661e0,
        (CIsoLayer*)0x666188,
        (CIsoLayer*)0x666200,
        (CIsoLayer*)0x666144,
        (CIsoLayer*)0x666100,
        (CIsoLayer*)0x6661a4,
        (CIsoLayer*)0x6661d8,
        (CIsoLayer*)0x666204,
        (CIsoLayer*)0x666174,
        (CIsoLayer*)0x666184,
        (CIsoLayer*)0x666140,
        (CIsoLayer*)0x666148,
        (CIsoLayer*)0x6661e8,
        (CIsoLayer*)0x666138,
        (CIsoLayer*)0x6661fc,
        (CIsoLayer*)0x66617c,
        (CIsoLayer*)0x66610c,
        (CIsoLayer*)0x666210,
        (CIsoLayer*)0x666194,
        (CIsoLayer*)0x6661f4,
        (CIsoLayer*)0x666120,
        (CIsoLayer*)0x6661f8,
        (CIsoLayer*)0x666108,
        (CIsoLayer*)0x66611c,
        (CIsoLayer*)0x666180,
        (CIsoLayer*)0x66619c,
        (CIsoLayer*)0x666128,
        (CIsoLayer*)0x6661f0,
        (CIsoLayer*)0x6661e4,
        (CIsoLayer*)0x666104,
        (CIsoLayer*)0x666208,
        (CIsoLayer*)0x666130,
        (CIsoLayer*)0x6661a0,
        (CIsoLayer*)0x6661ec,
        (CIsoLayer*)0x666110,
        (CIsoLayer*)0x666118,
        (CIsoLayer*)0x666214,
    },
}};
// clang-format on

IsoLayers& isoLayers()
{
    return layers[static_cast<int>(hooks::gameVersion())];
}

} // namespace game
