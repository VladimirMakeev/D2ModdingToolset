/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "settings.h"
#include <Windows.h>
#include <algorithm>
#include <limits>
#include <string>

namespace hooks {

UserSettings settings;

const UserSettings& userSettings()
{
    return settings;
}

void readUserSettings(const std::filesystem::path& iniFilePath)
{
    const std::string iniPath{iniFilePath.string()};
    const char disciple[] = "Disciple";

    settings.unitMaxDamage = GetPrivateProfileInt(disciple, "UnitMaxDamage", settings.unitMaxDamage,
                                                  iniPath.c_str());

    settings.unitMaxArmor = GetPrivateProfileInt(disciple, "UnitMaxArmor", settings.unitMaxArmor,
                                                 iniPath.c_str());

    settings.stackScoutRangeMax = GetPrivateProfileInt(disciple, "StackMaxScoutRange",
                                                       settings.stackScoutRangeMax,
                                                       iniPath.c_str());

    const auto defaultShatteredMax{settings.shatteredArmorMax};
    settings.shatteredArmorMax = GetPrivateProfileInt(disciple, "ShatteredArmorMax",
                                                      defaultShatteredMax, iniPath.c_str());
    settings.shatteredArmorMax = std::clamp(settings.shatteredArmorMax, 0, defaultShatteredMax);

    auto criticalHitDamage = GetPrivateProfileInt(disciple, "CriticalHitDamage",
                                                  settings.criticalHitDamage, iniPath.c_str());
    constexpr auto critMax{std::numeric_limits<std::uint8_t>::max()};

    settings.criticalHitDamage = std::clamp<std::uint32_t>(criticalHitDamage, 0, critMax);

    settings.showResources = GetPrivateProfileInt(disciple, "ShowResources", settings.showResources,
                                                  iniPath.c_str())
                             != 0;

    settings.showBanners = GetPrivateProfileInt(disciple, "ShowBanners", settings.showBanners,
                                                iniPath.c_str())
                           != 0;

    settings.showLandConverted = GetPrivateProfileInt(disciple, "ShowLandConverted",
                                                      settings.showLandConverted, iniPath.c_str())
                                 != 0;

    settings.debugMode = GetPrivateProfileInt(disciple, "DebugHooks", settings.debugMode,
                                              iniPath.c_str())
                         != 0;
}

} // namespace hooks
