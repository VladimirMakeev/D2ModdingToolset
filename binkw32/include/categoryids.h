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
enum DeathAnimationId : int
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
enum ImmuneId : int
{
    Notimmune = 1,
    Once,
    Always
};

} // namespace game

#endif // CATEGORYIDS_H
