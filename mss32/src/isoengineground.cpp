/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#include "isoengineground.h"
#include "version.h"
#include <array>

namespace game::CGroundTextureApi {

// clang-format off
static std::array<IMqTextureVftable*, 4> vftables = {{
    // Akella
    (IMqTextureVftable*)0x6e40c4,
    // Russobit
    (IMqTextureVftable*)0x6e40c4,
    // Gog
    (IMqTextureVftable*)0x6e2064,
    // Scenario Editor
    (IMqTextureVftable*)0x5d54b4,
}};

static std::array<CIsoEngineGroundVftable*, 4> isoEngineVftables = {{
    // Akella
    (CIsoEngineGroundVftable*)0x6e40b4,
    // Russobit
    (CIsoEngineGroundVftable*)0x6e40b4,
    // Gog
    (CIsoEngineGroundVftable*)0x6e2054,
    // Scenario Editor
    (CIsoEngineGroundVftable*)0x5d54a4,
}};

static std::array<void*, 4> borderMaskBuffers = {{
    // Akella
    (void*)0x837df0,
    // Russobit
    (void*)0x837df0,
    // Gog
    (void*)0x835da0,
    // Scenario Editor
    (void*)0x663a60,
}};
// clang-format on

IMqTextureVftable* vftable()
{
    return vftables[static_cast<int>(hooks::gameVersion())];
}

CIsoEngineGroundVftable* isoEngineVftable()
{
    return isoEngineVftables[static_cast<int>(hooks::gameVersion())];
}

void* borderMaskBuffer()
{
    return borderMaskBuffers[static_cast<int>(hooks::gameVersion())];
}

} // namespace game::CGroundTextureApi
