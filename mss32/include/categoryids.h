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

#ifndef CATEGORYIDS_H
#define CATEGORYIDS_H

namespace game {

/** Race ids from LRace.dbf. */
enum class RaceId : int
{
    Human = 0,
    Undead,
    Heretic,
    Dwarf,
    Neutral,
    Elf
};

/** Subrace ids from LSubRace.dbf. */
enum class SubRaceId : int
{
    Custom,
    Human,
    Undead,
    Heretic,
    Dwarf,
    Neutral,
    NeutralHuman,
    NeutralElf,
    NeutralGreenSkin,
    NeutralDragon,
    NeutralMarsh,
    NeutralWater,
    NeutralBarbarian,
    NeutralWolf,
    Elf
};

/** Ai attitude ids from LaiAtt.dbf. */
enum class AiAttitudeId : int
{
    Small = 0,
    Medium,
    Large,
    Humongous
};

/** Unit branch ids from LunitB.dbf. */
enum class UnitBranchId : int
{
    Fighter = 0,
    Archer,
    Mage,
    Special,
    Sideshow,
    Hero,
    Noble,
    Summon
};

/** Building type ids from Lbuild.dbf. */
enum class BuildingId : int
{
    Guild,
    Heal,
    Magic,
    Unit
};

/** Difficulty level ids from LDiff.dbf. */
enum class DifficultyLevelId : int
{
    Easy,
    Average,
    Hard,
    VeryHard
};

/**
 * Unit ids from LUnitC.dbf.
 * LUnitC.dbf also defines neutral leader and neutral soldier but game does not use them.
 */
enum class UnitId : int
{
    Soldier,
    Noble,
    Leader,
    Summon = 4,
    Illusion,
    Guardian = 8
};

/** Attack reach ids from LAttR.dbf. */
enum class AttackReachId : int
{
    All = 1,
    Any,
    Adjacent
};

/** Attack class ids from LAttC.dbf. */
enum class AttackClassId : int
{
    Damage = 1,
    Drain,
    Paralyze,
    Heal = 6,
    Fear,
    BoostDamage,
    Petrify,
    LowerDamage,
    LowerInitiative,
    Poison,
    Frostbite,
    Revive,
    DrainOverflow,
    Cure,
    Summon,
    DrainLevel,
    GiveAttack,
    Doppelganger,
    TransformSelf,
    TransformOther,
    Blister,
    BestowWards,
    Shatter
};

/** Attack source ids from LAttS.dbf. */
enum class AttackSourceId : int
{
    Weapon,
    Mind,
    Life,
    Death,
    Fire,
    Water,
    Earth,
    Air
};

/** Death animation ids from LDthAnim.dbf. */
enum class DeathAnimationId : int
{
    Human = 1,
    Heretic,
    Dwarf,
    Undead,
    Neutral,
    Dragon,
    Ghost,
    Elf
};

/** Immune category ids from LImmune.dbf. */
enum class ImmuneId : int
{
    Notimmune = 1,
    Once,
    Always
};

/** Lord category ids from LLord.dbf. */
enum class LordId : int
{
    Mage,
    Warrior,
    Diplomat
};

/** Order ids from LOrder.dbf. */
enum class OrderId : int
{
    Normal = 1,
    Stand,
    Guard,
    AttackStack,
    DefendStack,
    SecureCity,
    Roam,
    MoveToLocation,
    DefendLocation,
    Bezerk,
    Assist,
    Steal = 13,
    DefendCity
};

/** Items ids from LmagItm.dbf. */
enum class ItemId : int
{
    Armor,
    Jewel,
    Weapon,
    Banner,
    PotionBoost,
    PotionHeal,
    PotionRevive,
    PotionPermanent,
    Scroll,
    Wand,
    Valuable,
    Orb,
    Talisman,
    TravelItem,
    Special
};

/** Fortification ids from LFort.dbf. */
enum class FortId : int
{
    Capital = 2,
    Village = 4
};

/** Site ids from LSite.dbf. */
enum class SiteId : int
{
    Merchant = 2,
    MageTower = 4,
    Mercenaries,
    Trainer
};

/** Modifier source ids from LModifS.dbf. */
enum class ModifierSourceId : int
{
    Unit = 0,
    Stack = 1,
    Attack = 3,
};

/** Modifier element types from LmodifE.dbf. */
enum class ModifierElementType : int
{
    QtyTarget = 0,
    ScoutingRange = 1,
    Leadership = 2,
    Power = 3,
    QtyDamage = 4,
    Armor = 5,
    Hp = 6,
    MoveAllowance = 7,
    Initiative = 9,
    MoveAbility = 10,
    LeaderAbility = 11,
    Immunity = 12,
    Regeneration = 13,
    Immunityclass = 14,
    AttackDrain = 15,
    FastRetreat = 16,
    LowerCost = 17,
};

/** Ground ids from Lground.dbf. */
enum class GroundId : int
{
    Plain = 0,
    Forest = 1,
    Water = 3,
    Mountain = 4,
};

/** Leader ability ids from LleadA.dbf. */
enum class LeaderAbilityId : int
{
    Zealot = 1,
    Incorruptible = 3,
    WeaponMaster = 6,
    WandScrollUse = 7,
    WeaponArmorUse = 8,
    BannerUse = 9,
    JewelryUse = 10,
    Rod = 13,
    OrbUse = 14,
    TalismanUse = 15,
    TravelItemUse = 16,
    CriticalHit = 17,
};

/** Terrain ids from LTerrain.dbf. */
enum class TerrainId : int
{
    Human = 1,
    Dwarf,
    Heretic,
    Undead,
    Neutral,
    Elf
};

/** Event condition ids from LEvCond.dbf. */
enum class EventConditionId : int
{
    Frequency,
    Location = 2,
    EnterCity,
    OwnCity,
    KillStack,
    OwnItem,
    LeaderOwnItem,
    Diplomacy,
    Alliance,
    LootRuin,
    TransformLand,
    VisitSite,
    LeaderToZone = 14,
    LeaderToCity,
    ItemToLocation,
    StackExists,
    VariableInRange,
};

/** Event effect ids from LEvEffct.dbf. */
enum class EventEffectId : int
{
    Win,
    CreateStack,
    CastSpell,
    CastSpellMap,
    ChangeOwner,
    MoveBeside,
    Battle,
    EnableEvent,
    GiveSpell,
    GiveItem,
    MoveStack,
    Ally,
    Diplomacy,
    Unfog,
    RemoveMountain,
    RemoveLandmark,
    ChangeObjective,
    Popup,
    ChangeOrder,
    DestroyItem,
    RemoveStack,
    ChangeLandmark,
    ChangeTerrain,
    ModifyVariable,
};

/** Landmark ids from LLMCat.dbf. */
enum class LandmarkId : int
{
    Misc,
    Building,
    Structure,
    Terrain,
};

/** Leader ids from LLeadC.dbf. */
enum class LeaderId : int
{
    Fighter = 1,
    Explorer,
    Mage,
    Rod,
    Noble,
};

/** Spell ids from LSpell.dbf. */
enum class SpellId : int
{
    Attack,
    Lower,
    Heal,
    Boost,
    Summon,
    Fog = 6,
    Unfog,
    RestoreMove,
    Invisibility,
    RemoveRod,
    ChangeTerrain,
    GiveWards,
};

/** AI spell ids from LAiSpell.dbf. */
enum class AiSpellId : int
{
    All = 1,
    None,
    Human,
    Undead,
    Heretic,
    Dwarf,
};

/** Resource ids from Lres.dbf. */
enum class ResourceId : int
{
    Gold,
    InfernalMana, /**< L_RED */
    LifeMana,     /**< L_YELLOW */
    DeathMana,    /**< L_ORANGE */
    RunicMana,    /**< L_WHITE */
    GroveMana,    /**< L_BLUE */
};

} // namespace game

#endif // CATEGORYIDS_H
