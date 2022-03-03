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

#include "scripts.h"
#include "attackview.h"
#include "categoryids.h"
#include "dynupgradeview.h"
#include "idview.h"
#include "leaderview.h"
#include "locationview.h"
#include "log.h"
#include "point.h"
#include "scenariovariableview.h"
#include "scenarioview.h"
#include "scenvariablesview.h"
#include "stackview.h"
#include "tileview.h"
#include "unitimplview.h"
#include "unitslotview.h"
#include "unitview.h"
#include "utils.h"

namespace hooks {

static void doBindApi(sol::state& lua)
{
    using namespace game;

    // clang-format off
    lua.new_enum("Race",
        "Human", RaceId::Human,
        "Undead", RaceId::Undead,
        "Heretic", RaceId::Heretic,
        "Dwarf", RaceId::Dwarf,
        "Neutral", RaceId::Neutral,
        "Elf", RaceId::Elf
    );

    lua.new_enum("Subrace",
        "Custom", SubRaceId::Custom,
        "Human", SubRaceId::Human,
        "Undead", SubRaceId::Undead,
        "Heretic", SubRaceId::Heretic,
        "Dwarf", SubRaceId::Dwarf,
        "Neutral", SubRaceId::Neutral,
        "NeutralHuman", SubRaceId::NeutralHuman,
        "NeutralElf", SubRaceId::NeutralElf,
        "NeutralGreenSkin", SubRaceId::NeutralGreenSkin,
        "NeutralDragon", SubRaceId::NeutralDragon,
        "NeutralMarsh", SubRaceId::NeutralMarsh,
        "NeutralWater", SubRaceId::NeutralWater,
        "NeutralBarbarian", SubRaceId::NeutralBarbarian,
        "NeutralWolf", SubRaceId::NeutralWolf,
        "Elf", SubRaceId::Elf
    );

    lua.new_enum("Terrain",
        "Human", TerrainId::Human,
        "Dwarf", TerrainId::Dwarf,
        "Heretic", TerrainId::Heretic,
        "Undead", TerrainId::Undead,
        "Neutral", TerrainId::Neutral,
        "Elf", TerrainId::Elf
    );

    lua.new_enum("Ground",
        "Plain", GroundId::Plain,
        "Forest", GroundId::Forest,
        "Water", GroundId::Water,
        "Mountain", GroundId::Mountain
    );

    lua.new_enum("LeaderType",
        "Fighter", LeaderId::Fighter,
        "Explorer", LeaderId::Explorer,
        "Mage", LeaderId::Mage,
        "Rod", LeaderId::Rod,
        "Noble", LeaderId::Noble
    );

    lua.new_enum("LeaderAbility",
        "Incorruptible", LeaderAbilityId::Incorruptible,
        "WeaponMaster", LeaderAbilityId::WeaponMaster,
        "WandScrollUse", LeaderAbilityId::WandScrollUse,
        "WeaponArmorUse", LeaderAbilityId::WeaponArmorUse,
        "BannerUse", LeaderAbilityId::BannerUse,
        "JewelryUse", LeaderAbilityId::JewelryUse,
        "Rod", LeaderAbilityId::Rod,
        "OrbUse", LeaderAbilityId::OrbUse,
        "TalismanUse", LeaderAbilityId::TalismanUse,
        "TravelItemUse", LeaderAbilityId::TravelItemUse,
        "CriticalHit", LeaderAbilityId::CriticalHit
    );

    lua.new_enum("Attack",
        "Damage", AttackClassId::Damage,
        "Drain", AttackClassId::Drain,
        "Paralyze", AttackClassId::Paralyze,
        "Heal", AttackClassId::Heal,
        "Fear", AttackClassId::Fear,
        "BoostDamage", AttackClassId::BoostDamage,
        "Petrify", AttackClassId::Petrify,
        "LowerDamage", AttackClassId::LowerDamage,
        "LowerInitiative", AttackClassId::LowerInitiative,
        "Poison", AttackClassId::Poison,
        "Frostbite", AttackClassId::Frostbite,
        "Revive", AttackClassId::Revive,
        "DrainOverflow", AttackClassId::DrainOverflow,
        "Cure", AttackClassId::Cure,
        "Summon", AttackClassId::Summon,
        "DrainLevel", AttackClassId::DrainLevel,
        "GiveAttack", AttackClassId::GiveAttack,
        "Doppelganger", AttackClassId::Doppelganger,
        "TransformSelf", AttackClassId::TransformSelf,
        "TransformOther", AttackClassId::TransformOther,
        "Blister", AttackClassId::Blister,
        "BestowWards", AttackClassId::BestowWards,
        "Shatter", AttackClassId::Shatter
    );

    lua.new_enum("Source",
        "Weapon", AttackSourceId::Weapon,
        "Mind", AttackSourceId::Mind,
        "Life", AttackSourceId::Life,
        "Death", AttackSourceId::Death,
        "Fire", AttackSourceId::Fire,
        "Water", AttackSourceId::Water,
        "Earth", AttackSourceId::Earth,
        "Air", AttackSourceId::Air
    );

    lua.new_enum("Reach",
        "All", AttackReachId::All,
        "Any", AttackReachId::Any,
        "Adjacent", AttackReachId::Adjacent
    );
    // clang-format on

    bindings::UnitView::bind(lua);
    bindings::UnitImplView::bind(lua);
    bindings::UnitSlotView::bind(lua);
    bindings::DynUpgradeView::bind(lua);
    bindings::ScenarioView::bind(lua);
    bindings::LocationView::bind(lua);
    bindings::Point::bind(lua);
    bindings::IdView::bind(lua);
    bindings::ScenVariablesView::bind(lua);
    bindings::ScenarioVariableView::bind(lua);
    bindings::TileView::bind(lua);
    bindings::StackView::bind(lua);
    bindings::LeaderView::bind(lua);
    bindings::GroupView::bind(lua);
    bindings::AttackView::bind(lua);
    lua.set_function("log", [](const std::string& message) { logDebug("luaDebug.log", message); });
}

std::optional<sol::state> loadScriptFile(const std::filesystem::path& path,
                                         bool alwaysExists,
                                         bool bindApi)
{
    if (!alwaysExists && !std::filesystem::exists(path))
        return std::nullopt;

    sol::state lua;
    sol::protected_function_result result;
    std::string pathString = path.string();
    if (bindApi) {
        static std::unordered_map<std::string, std::string> sources;
        if (sources.find(pathString) == sources.end())
            sources[pathString] = readFile(path);

        if (sources[pathString].empty()) {
            showErrorMessageBox(fmt::format("Failed to read '{:s}' script file", pathString));
            return std::nullopt;
        }

        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table,
                           sol::lib::os);
        doBindApi(lua);

        result = lua.safe_script(sources[pathString].c_str(),
                                 [](lua_State*, sol::protected_function_result pfr) {
                                     return pfr;
                                 });
    } else {
        result = lua.load_file(pathString)();
    }

    if (!result.valid()) {
        const sol::error err = result;
        showErrorMessageBox(fmt::format("Failed to load script '{:s}'.\n"
                                        "Reason: '{:s}'",
                                        pathString, err.what()));
        return std::nullopt;
    }

    return {std::move(lua)};
}

sol::state createLuaState(bool bindApi)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table,
                       sol::lib::os);

    if (bindApi) {
        doBindApi(lua);
    }

    return std::move(lua);
}

} // namespace hooks
