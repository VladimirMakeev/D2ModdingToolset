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
#include "battlemsgdata.h"
#include "battlemsgdataview.h"
#include "categoryids.h"
#include "currencyview.h"
#include "diplomacyview.h"
#include "dynupgradeview.h"
#include "fogview.h"
#include "fortview.h"
#include "gameutils.h"
#include "globalvariablesview.h"
#include "groupview.h"
#include "idview.h"
#include "itembaseview.h"
#include "itemview.h"
#include "locationview.h"
#include "log.h"
#include "midstack.h"
#include "modifierview.h"
#include "playerview.h"
#include "point.h"
#include "ruinview.h"
#include "scenariovariableview.h"
#include "scenarioview.h"
#include "scenvariablesview.h"
#include "stackview.h"
#include "tileview.h"
#include "unitimplview.h"
#include "unitslotview.h"
#include "unitview.h"
#include "unitviewdummy.h"
#include "utils.h"
#include <fmt/format.h>
#include <mutex>
#include <thread>

extern std::thread::id mainThreadId;

namespace hooks {

struct PathHash
{
    std::size_t operator()(std::filesystem::path const& p) const noexcept
    {
        return std::filesystem::hash_value(p);
    }
};

static void bindApi(sol::state& lua)
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

    lua.new_enum("Lord",
        "Mage", LordId::Mage,
        "Warrior", LordId::Warrior,
        "Diplomat", LordId::Diplomat
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

    lua.new_enum("Unit",
        "Soldier", UnitId::Soldier,
        "Noble", UnitId::Noble,
        "Leader", UnitId::Leader,
        "Summon", UnitId::Summon,
        "Illusion", UnitId::Illusion,
        "Guardian", UnitId::Guardian
    );

    lua.new_enum("Leader",
        "Fighter", LeaderId::Fighter,
        "Explorer", LeaderId::Explorer,
        "Mage", LeaderId::Mage,
        "Rod", LeaderId::Rod,
        "Noble", LeaderId::Noble
    );

    lua.new_enum("Ability",
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

    lua.new_enum("Item",
        "Armor", ItemId::Armor,
        "Jewel", ItemId::Jewel,
        "Weapon", ItemId::Weapon,
        "Banner", ItemId::Banner,
        "PotionBoost", ItemId::PotionBoost,
        "PotionHeal", ItemId::PotionHeal,
        "PotionRevive", ItemId::PotionRevive,
        "PotionPermanent", ItemId::PotionPermanent,
        "Scroll", ItemId::Scroll,
        "Wand", ItemId::Wand,
        "Valuable", ItemId::Valuable,
        "Orb", ItemId::Orb,
        "Talisman", ItemId::Talisman,
        "TravelItem", ItemId::TravelItem,
        "Special", ItemId::Special
    );

    lua.new_enum("Equipment",
        "Banner", EquippedItemIdx::Banner,
        "Tome", EquippedItemIdx::Tome,
        "Battle1", EquippedItemIdx::Battle1,
        "Battle2", EquippedItemIdx::Battle2,
        "Artifact1", EquippedItemIdx::Artifact1,
        "Artifact2", EquippedItemIdx::Artifact2,
        "Boots", EquippedItemIdx::Boots
    );

    lua.new_enum("Immune",
        "NotImmune", ImmuneId::Notimmune,
        "Once", ImmuneId::Once,
        "Always", ImmuneId::Always
    );

    lua.new_enum("DeathAnimation",
        "Human", DeathAnimationId::Human,
        "Heretic", DeathAnimationId::Heretic,
        "Dwarf", DeathAnimationId::Dwarf,
        "Undead", DeathAnimationId::Undead,
        "Neutral", DeathAnimationId::Neutral,
        "Dragon", DeathAnimationId::Dragon,
        "Ghost", DeathAnimationId::Ghost,
        "Elf", DeathAnimationId::Elf
    );

    lua.new_enum("BattleStatus",
        "XpCounted", BattleStatus::XpCounted,
        "Dead", BattleStatus::Dead,
        "Paralyze", BattleStatus::Paralyze,
        "Petrify", BattleStatus::Petrify,
        "DisableLong", BattleStatus::DisableLong,
        "BoostDamageLvl1", BattleStatus::BoostDamageLvl1,
        "BoostDamageLvl2", BattleStatus::BoostDamageLvl2,
        "BoostDamageLvl3", BattleStatus::BoostDamageLvl3,
        "BoostDamageLvl4", BattleStatus::BoostDamageLvl4,
        "BoostDamageLong", BattleStatus::BoostDamageLong,
        "LowerDamageLvl1", BattleStatus::LowerDamageLvl1,
        "LowerDamageLvl2", BattleStatus::LowerDamageLvl2,
        "LowerDamageLong", BattleStatus::LowerDamageLong,
        "LowerInitiative", BattleStatus::LowerInitiative,
        "LowerInitiativeLong", BattleStatus::LowerInitiativeLong,
        "Poison", BattleStatus::Poison,
        "PoisonLong", BattleStatus::PoisonLong,
        "Frostbite", BattleStatus::Frostbite,
        "FrostbiteLong", BattleStatus::FrostbiteLong,
        "Blister", BattleStatus::Blister,
        "BlisterLong", BattleStatus::BlisterLong,
        "Cured", BattleStatus::Cured,
        "Transform", BattleStatus::Transform,
        "TransformLong", BattleStatus::TransformLong,
        "TransformSelf", BattleStatus::TransformSelf,
        "TransformDoppelganger", BattleStatus::TransformDoppelganger,
        "TransformDrainLevel", BattleStatus::TransformDrainLevel,
        "Summon", BattleStatus::Summon,
        "Retreated", BattleStatus::Retreated,
        "Retreat", BattleStatus::Retreat,
        "Hidden", BattleStatus::Hidden,
        "Defend", BattleStatus::Defend,
        "Unsummoned", BattleStatus::Unsummoned
    );
    // clang-format on

    bindings::UnitView::bind(lua);
    bindings::UnitViewDummy::bind(lua);
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
    bindings::FortView::bind(lua);
    bindings::RuinView::bind(lua);
    bindings::GroupView::bind(lua);
    bindings::AttackView::bind(lua);
    bindings::CurrencyView::bind(lua);
    bindings::ItemBaseView::bind(lua);
    bindings::ItemView::bind(lua);
    bindings::PlayerView::bind(lua);
    bindings::ModifierView::bind(lua);
    bindings::BattleMsgDataView::bind(lua);
    bindings::DiplomacyView::bind(lua);
    bindings::FogView::bind(lua);
    bindings::GlobalVariablesView::bind(lua);

    lua.set_function("log", [](const std::string& message) { logDebug("luaDebug.log", message); });
}

// https://sol2.readthedocs.io/en/latest/threading.html
// Lua has no thread safety. sol does not force thread safety bottlenecks anywhere.
// Treat access and object handling like you were dealing with a raw int reference (int&).
sol::state& getLua()
{
    static std::unique_ptr<sol::state> mainThreadLua;
    static std::unique_ptr<sol::state> workerThreadLua;

    auto& lua = std::this_thread::get_id() == mainThreadId ? mainThreadLua : workerThreadLua;
    if (lua == nullptr) {
        lua = std::make_unique<sol::state>();
        lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table,
                            sol::lib::os, sol::lib::string);
        bindApi(*lua);
    }

    return *lua;
}

const std::string& getSource(const std::filesystem::path& path)
{
    static std::unordered_map<std::filesystem::path, std::string, PathHash> sources;
    static std::mutex sourcesMutex;

    const std::lock_guard<std::mutex> lock(sourcesMutex);

    auto it = sources.find(path);
    if (it != sources.end())
        return it->second;

    auto& source = sources[path];
    source = readFile(path);
    return source;
}

bindings::ScenarioView getScenario()
{
    return {getObjectMap()};
}

sol::environment executeScript(const std::string& source,
                               sol::protected_function_result& result,
                               bool bindScenario)
{
    auto& lua = getLua();

    // Environment prevents cluttering of global namespace by scripts
    // making each script run isolated from others.
    sol::environment env{lua, sol::create, lua.globals()};
    result = lua.safe_script(source, env,
                             [](lua_State*, sol::protected_function_result pfr) { return pfr; });

    if (bindScenario) {
        env["getScenario"] = &getScenario;
    }

    return env;
}

std::optional<sol::environment> executeScriptFile(const std::filesystem::path& path,
                                                  bool alwaysExists,
                                                  bool bindScenario)
{
    if (!alwaysExists && !std::filesystem::exists(path))
        return std::nullopt;

    const auto& source = getSource(path);
    if (source.empty()) {
        showErrorMessageBox(fmt::format("Failed to read '{:s}' script file.", path.string()));
        return std::nullopt;
    }

    sol::protected_function_result result;
    auto env = executeScript(source, result, bindScenario);
    if (!result.valid()) {
        const sol::error err = result;
        showErrorMessageBox(fmt::format("Failed to execute script '{:s}'.\n"
                                        "Reason: '{:s}'",
                                        path.string(), err.what()));
        return std::nullopt;
    }

    return {std::move(env)};
}

std::optional<sol::function> getScriptFunction(const sol::environment& environment,
                                               const char* name,
                                               bool alwaysExists)
{
    const sol::object object = environment[name];
    const sol::type objectType = object.get_type();

    if (objectType != sol::type::function) {
        if (alwaysExists) {
            showErrorMessageBox(
                fmt::format("'{:s}' is not a function, type: {:d}.", name, (int)objectType));
        }
        return std::nullopt;
    }

    return object.as<sol::function>();
}

std::optional<sol::function> getScriptFunction(const std::filesystem::path& path,
                                               const char* name,
                                               std::optional<sol::environment>& environment,
                                               bool alwaysExists,
                                               bool bindScenario)
{
    environment = executeScriptFile(path, alwaysExists, bindScenario);
    if (!environment)
        return std::nullopt;

    auto function = getScriptFunction(*environment, name, false);
    if (!function && alwaysExists) {
        showErrorMessageBox(
            fmt::format("Could not find function '{:s}' in script '{:s}'.", name, path.string()));
    }

    return function;
}

std::optional<sol::protected_function> getProtectedScriptFunction(
    const sol::environment& environment,
    const char* name,
    bool alwaysExists)
{
    const sol::object object = environment[name];
    const sol::type objectType = object.get_type();

    if (objectType != sol::type::function) {
        if (alwaysExists) {
            showErrorMessageBox(
                fmt::format("'{:s}' is not a function, type: {:d}.", name, (int)objectType));
        }
        return std::nullopt;
    }

    return object.as<sol::protected_function>();
}

} // namespace hooks
