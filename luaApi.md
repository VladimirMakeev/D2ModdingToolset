# Lua API for D2 modding toolset

### Overview
All scripts are expected to be written using [Lua](https://www.lua.org/) language [v5.4.1](https://www.lua.org/ftp/lua-5.4.1.tar.gz) and should be placed in Scripts folder.
Scripts folder itself should be placed in the game folder.

### Currently used scripts and their meanings:
- settings.lua - mss32 proxy dll settings that changes game rules
- doppelganger.lua - logic that computes level of doppelganger transform (category L\_DOPPELGANGER)
- transformSelf.lua - computes unit level and determines free attacks to give for transform-self attacks (category L\_TRANSFORM\_SELF)
- transformOther.lua - computes unit level for transform-other attacks (category L\_TRANSFORM\_OTHER)
- summon.lua - computes summoned unit level for summon attacks (category L\_SUMMON)
- textids.lua - contains interface text mapping for custom functionality
- getAllTargets.lua - contains selection/attack targeting logic for any/all attack reach
- getAdjacentTargets.lua - contains selection/attack targeting logic for adjacent/all-adjacent attack reach
- getSelectedTargetAndAllAdjacentToIt.lua - contains attack targeting logic for selective-cleave attack reach
- getSelectedTargetAndOneAdjacentToIt.lua - contains attack targeting logic for single selective-cleave attack reach
- getSelectedTargetAndOneBehindIt.lua - contains attack targeting logic for pierce attack reach
- getSelectedLineTargets.lua - contains attack targeting logic for wide-cleave attack reach
- getSelectedColumnTargets.lua - contains attack targeting logic for column attack reach
- getSelectedArea2x2Targets.lua - contains attack targeting logic for 2x2 area splash attack reach
- getSelectedTargetAndTwoChainedRandom.lua - contains attack targeting logic for random chain attack reach
- getSelectedTargetAndOneRandom.lua - contains attack targeting logic for additional random target
- getWoundedFemaleGreenskinTargets.lua - contains targeting logic that only allows to reach wounded female greenskins
- [Scripts/Modifiers](Scripts/Modifiers) contain custom modifier script examples

### API reference

#### Standalone functions
##### log
Writes message to `luaDebug.log` file when `debugHooks` is set to true in `settings.lua`.
```lua
log('Unit current level:' .. unit.impl.level)
```
##### getScenario
Returns current [scenario](luaApi.md#scenario).
The function only accessible to scripts where scenario access is appropriate:
- `summon.lua`
- `doppelganger.lua`
- `transformSelf.lua`
- `transformOther.lua`
- `drainLevel.lua`
- custom attack reach scripts
- custom unit modifier script

`checkEventCondition` has `scenario` as its argument so `getScenario` is not bound to it.
```lua
getScenario():getUnit(unitId)
```
##### randomNumber
Generates random number in range \[0 : maxValue) using ingame generator
```lua
local n = randomNumber(100)
```
##### getGlobal
Returns [global data storage](luaApi.md#global) used by game.
```lua
local data = getGlobal()
local variables = data.variables
```

---

#### Enumerations

##### Race
```lua
Race = { Human, Undead, Heretic, Dwarf, Neutral, Elf }
```
##### Subrace
```lua
Subrace = { Custom, Human, Undead, Heretic, Dwarf, Neutral, NeutralHuman, NeutralElf, NeutralGreenSkin,
            NeutralDragon, NeutralMarsh, NeutralWater, NeutralBarbarian, NeutralWolf, Elf }
```
##### Lord
```lua
Lord = { Mage, Warrior, Diplomat }
```
##### Terrain
```lua
Terrain = { Human, Dwarf, Heretic, Undead, Neutral, Elf }
```

##### Ground
```lua
Ground = { Plain, Forest, Water, Mountain }
```

##### Unit
```
Unit = { Soldier, Noble, Leader, Summon, Illusion, Guardian }
```

##### Leader
```
Leader = { Fighter, Explorer, Mage, Rod, Noble }
```

##### Ability
```
Ability = { Incorruptible, WeaponMaster, WandScrollUse, WeaponArmorUse, BannerUse, JewelryUse,
            Rod, OrbUse, TalismanUse, TravelItemUse, CriticalHit }
```

##### Attack
```
Attack = { Damage, Drain, Paralyze, Heal, Fear, BoostDamage, Petrify, LowerDamage, LowerInitiative,
           Poison, Frostbite, Revive, DrainOverflow, Cure, Summon, DrainLevel, GiveAttack,
           Doppelganger, TransformSelf, TransformOther, Blister, BestowWards, Shatter }
```

##### Source
```
Source = { Weapon, Mind, Life, Death, Fire, Water, Earth, Air }
```

##### Reach
```
Reach = { All, Any, Adjacent }
```

##### Immune
```
Immune = { NotImmune, Once, Always }
```

##### Item
```
Item = { Armor, Jewel, Weapon, Banner, PotionBoost, PotionHeal, PotionRevive,
         PotionPermanent, Scroll, Wand, Valuable, Orb, Talisman, TravelItem, Special }
```

##### Equipment
```
Equipment = { Banner, Tome, Battle1, Battle2, Artifact1, Artifact2, Boots }
```

##### DeathAnimation
```
DeathAnimation = { Human, Heretic, Dwarf, Undead, Neutral, Dragon, Ghost, Elf }
```

##### BattleStatus
```
BattleStatus = {
    XpCounted,             -- Unit was killed and its experience points were counted
    Dead,                  -- Unit dead
    Paralyze,              -- Unit paralyzed
    Petrify,               -- Unit petrified
    DisableLong,           -- Long disable applied (paralyze, petrify or fear)
    BoostDamageLvl1,       -- 25% boost
    BoostDamageLvl2,       -- 50% boost
    BoostDamageLvl3,       -- 75% boost
    BoostDamageLvl4,       -- 100% boost
    BoostDamageLong,       -- Long damage boost (until battle is over or lower damage applied)
    LowerDamageLvl1,       -- 50% lower damage
    LowerDamageLvl2,       -- 33% lower damage
    LowerDamageLong,       -- Long lower damage (until battle is over or removed)
    LowerInitiative,       -- 50% lower initiative
    LowerInitiativeLong,   -- Long lower initiative
    Poison,                -- Poison dot
    PoisonLong,            -- Long poison applied
    Frostbite,             -- Frostbite dot
    FrostbiteLong,         -- Long frostbite applied
    Blister,               -- Blister dot
    BlisterLong,           -- Long blister applied
    Cured,                 -- Cure applied
    Transform,             -- Unit transformed by another unit
    TransformLong,         -- Long transformation applied by another unit
    TransformSelf,         -- Unit transfomed himself
    TransformDoppelganger, -- Doppelganger transformation
    TransformDrainLevel,   -- Drain level applied
    Summon,                -- Unit was summoned during battle
    Retreated,             -- Unit retreated from battle
    Retreat,               -- Unit is retreating
    Hidden,                -- Unit is hidden. For example, while leader dueling a thief
    Defend,                -- Defend was used in this round
    Unsummoned             -- Unsummon effect applied
}
```

##### BattleAction
```
BattleAction = { Attack, Skip, Retreat, Wait, Defend, Auto, UseItem }
```

##### Retreat
```
Retreat = { NoRetreat, CoverAndRetreat, FullRetreat }
```

##### Relation
```
Relation = { War, Neutral, Peace }
```

##### Order
```
Order = { Normal, Stand, Guard, AttackStack, DefendStack, SecureCity,
          Roam, MoveToLocation, DefendLocation, Bezerk, Assist, Steal, DefendCity }
```

##### IdType
```
IdType = {
    Empty,              -- Empty id
    ApplicationText,    -- Entries of TApp.dbf and TAppEdit.dbf
    Building,           -- Entries of Guild.dbf
    Race,               -- Entries of GRace.dbf
    Lord,               -- Entries of GLord.dbf
    Spell,              -- Entries of GSpells.dbf
    UnitGlobal,         -- Unit implementations, entries of GUnits.dbf
    UnitGenerated,      -- Runtime-generated unit implementations
    UnitModifier,       -- Unit modifiers, entries of GModif.dbf
    Attack,             -- Attacks, entries of GAttacks.dbf
    TextGlobal,         -- Entries of TGlobal.dbf
    LandmarkGlobal,     -- Entries of GLmark.dbf
    ItemGlobal,         -- Base items, entries of GItem.dbf
    NobleAction,        -- Noble (thief) actions, entries of GAction.dbf
    DynamicUpgrade,     -- Dynamic upgrade rules, entries of GDynUpgr.dbf
    DynamicAttack,      -- Runtime-generated unit primary attacks
    DynamicAltAttack,   -- Runtime-generated unit primary alternative attacks
    DynamicAttack2,     -- Runtime-generated unit secondary attacks
    DynamicAltAttack2,  -- Runtime-generated unit secondary alternative attacks
    CampaignFile,       -- Campaign files
    Plan,               -- Utility for fast object lookup by map coordinates
    ObjectCount,        -- Number of objects in scenario file
    ScenarioFile,       -- Scenario files
    Map,                -- Scenario map
    MapBlock,           -- Blocks of scenario map
    ScenarioInfo,       -- Scenario information
    SpellEffects,
    Fortification,      -- Capitals and villages
    Player,             -- Players in scenario
    PlayerKnownSpells,  -- Spells known by player in scenario
    Fog,                -- Fog of war for player in scenario
    PlayerBuildings,    -- Capital buildings
    Road,               -- Roads on scenario map
    Stack,              -- Stacks in scenario
    Unit,               -- Units in scenario
    Landmark,           -- Landmarks in scenario
    Item,               -- Items in scenario
    Bag,                -- Bags in scenario
    Site,               -- Sites in scenario
    Ruin,               -- Ruins in scenario
    Tomb,               -- Grave markers in scenario
    Rod,                -- Rods in scenario
    Crystal,            -- Gold mines and mana sources in scenario
    Diplomacy,          -- Diplomacy rules in scenario
    SpellCast,
    Location,           -- Location on scenario map
    StackTemplate,      -- Stack templates in scenario
    Event,              -- Events in scenario
    StackDestroyed,     -- Information about stacks defeated in scenario
    TalismanCharges,    -- Talisman charges counter in scenario
    Mountains,          -- Mountains in scenario
    SubRace,            -- Subraces in scenario
    SubRaceType,        -- Entries of GSubRace.dbf
    QuestLog,           -- Scenario quest log
    TurnSummary,        -- Brief information about last turn in scenario
    ScenarioVariable    -- Scenario variables
}
```

##### Resource
```
Resource = { Gold, InfernalMana, LifeMana, DeathMana, RunicMana, GroveMana }
```

---

#### Point
Represents point in 2D space.

Methods:
```lua
-- Creates point with both coordinates set to 0.
Point.new()
-- Creates point with specified coordinates.
-- For example: x = 1, y = 3
Point.new(1, 3)
-- Converts point to string '(x, y)'
tostring(point)
-- Access x coordinate for reading and writing
local x = point.x
point.x = x + 1
-- Access y coordinate for reading and writing
local y = point.y
point.y = y + 1
```

---

#### Id
Represents object identifier. Identifiers used to search scenario objects.

Methods:
```lua
-- Creates Id from string
Id.new('S143KC0001')
-- Returns empty identifier
Id.emptyId()
-- Converts Id to string
tostring(id)
-- Returns integer representation of id.
-- Can be used as Lua table key for best performance.
id.value
-- Returns identified object index among the same type of scenario objects (units, stacks, items, etc.).
-- Can be used as Lua table key for best performance.
id.typeIndex
```
##### type
Returns [type](luaApi.md#idtype) of identifier.
Identifier type can help to distinguish one object from another.
```lua
id.type
```
##### summonId
Creates special id for summoning units in battle using specified position in group.
Position in group should be in \[0 : 5\] range.
```lua
Id.summonId(possibleTarget)
```

---

#### Global
Represents global data storage used by game.
Allows to access contents of dbf files in 'Globals' folder of the game.

Methods:
##### variables
Returns [global variables](luaApi.md#global-variables).
```lua
local v = getGlobal().variables
```

---

#### Global Variables
Allows to access contents of `GVars.dbf`.

Methods:
```lua
-- Instructor skill bonus experience, 'WEAPN_MSTR'
weapnMstr
-- Max additional initiative points that randomly added to unit in battle. 'BAT_INIT'
batInit
-- Max additional damage points that randomly added to unit damage in battle. 'BAT_DAMAGE'
batDamage
-- 'BAT_ROUND'
batRound
-- 'BAT_BREAK'
batBreak
-- 'BAT_BMODIF'
batBModif
-- Initiative debuff. 'BATLOWERI'
batLoweri
-- Maximum number of abilities leader can learn. 'LDRMAXABIL'
ldrMaxAbil
-- Spy discovery chance per turn. 'SPY_DISCOV'
spyDiscov
-- Damage from thief action 'poison city'. 'POISON_C'
poisonC
-- Damage from thief action 'poison stack'. 'POISON_S'
poisonS
-- Bribe multiplier. 'BRIBE'
bribe
-- 'STEAL_RACE'
stealRace
-- 'STEAL_NEUT'
stealNeut
-- Minimal riot duration in days. 'RIOT_MIN'
riotMin
-- Maximal riot duration in days. 'RIOT_MAX'
riotMax
-- Percentage of riot damage. 'RIOT_DMG'
riotDmg
-- Percentage of the original price of the items at sale. 'SELL_RATIO'
sellRatio
-- Land transformation after city capture. 'T_CAPTURE'
tCapture
-- Land transformation per turn by capital. 'T_CAPITAL'
tCapital
-- Range of land transformation by rod per turn. 'ROD_RANGE'
rodRange
-- Profit per mana crystal or gold mine per turn. 'CRYSTAL_P'
crystalP
-- 'CONST_URG'
constUrg
-- Bonus per day regeneration for fighter leader. 'REGEN_LWAR'
regenLwar
-- Bonus per day regeneration for units in ruins. 'REGEN_RUIN'
regenRuin
-- Diplomacy level representing peace. 'D_PEACE'
dPeace
-- Diplomacy level representing war. 'D_WAR'
dWar
-- Diplomacy level representing neutrality. 'D_NEUTRAL'
dNeutral
-- 'D_GOLD'
dGold
-- 'D_MK_ALLY'
dMkAlly
-- 'D_ATTACK_SC'
dAttakSc
-- 'D_ATTACK_FO'
dAttakFo
-- 'D_ATTACK_PC'
dAttakPc
-- 'D_ROD'
dRod
-- 'D_REF_ALLY'
dRefAlly
-- 'D_BK_ALLY'
dBkAlly
-- 'D_NOBLE'
dNoble
-- 'D_BKA_CHANCE'
dBkaChnc
-- 'D_BKA_TURN'
dBkaTurn
-- Capital protection. 'PROT_CAP'
protCap
-- Additional gold on easy difficulty. 'BONUS_E'
bonusE
-- Additional gold on average difficulty. 'BONUS_A'
bonusA
-- Additional gold on hard difficulty. 'BONUS_H'
bonusH
-- Additional gold on very hard difficulty. 'BONUS_V'
bonusV
-- Income increase on easy difficulty. 'INCOME_E'
incomeE
-- Income increase on average difficulty. 'INCOME_A'
incomeA
-- Income increase on hard difficulty. 'INCOME_H'
incomeH
-- Income increase on very hard difficulty. 'INCOME_V'
incomeV
-- 'GU_RANGE'
guRange
-- 'PA_RANGE'
paRange
-- 'LO_RANGE'
loRange
-- Armor bonus when unit uses defend in battle. 'DFENDBONUS'
defendBonus
-- 'TALIS_CHRG'
talisChrg
-- Chance to get spells with capture of a capital. 'GAIN_SPELL'
gainSpell
```
##### rodCost
Rod placement [cost](luaApi.md#currency). `ROD_COST`
```lua
variables.rodCost
```
##### morale
Input tier values must be in range \[1 : 6\]. `MORALE_n`
```lua
variables:morale(1)
```
##### batBoostd
Damage boost values for various levels. Levels must be in range \[1 : 4\]. `BATBOOSTDn`
```lua
variables:batBoostd(4)
```
##### batLowerd
Damage debuff values for various levels. Levels must be in range \[1 : 2\]. `BATLOWERDn`
```lua
variables:batLowerd(1)
```
##### tCity
Land transformation per turn by cities of different tiers. Tier must be in range \[1 : 5\]. `T_CITYn`
```lua
variables:tCity(1)
```
##### prot
City protection values for various tier levels. Tier must be in range \[1 : 6\]. In case of tier 6, returns `protCap`. `PROT_n`
```lua
variables:prot(3)
```
##### splPwr
Input tier values must be in range \[1 : 5\]. `SPLPWR_n`
```lua
variables:splPwr(2)
```

---

#### Modifier
Represents unit modifier. Modifiers wrap [unit implementation](luaApi.md#unit-implementation).

Methods:
##### id
Returns modifier [id](luaApi.md#id). `MODIF_ID` value from `Gmodif.dbf`.
```lua
modifier.id
```

---

#### Unit
Represents game unit that participates in a battle, takes damage and performs attacks.
Unit can also be a leader. Leaders are main units in stacks.

Methods:
```lua
-- Returns unit's current experience points.
unit.xp
-- Returns unit's current hit points.
unit.hp
-- Returns unit's maximum hit points.
unit.hpMax
```
##### id
Returns unit [id](luaApi.md#id). This is different to id of [unit implementation](luaApi.md#unit-implementation).
The value is unique for every unit on scenario map.
```lua
unit.id
```
##### impl
Returns unit's current implementation.
Current implementation describes unit stats according to its levels
and possible transformations applied during battle.
```lua
unit.impl
```
##### baseImpl
Returns unit's base implementation.
Base implementation is a record in GUnits.dbf that describes unit basic stats.
```lua
unit.baseImpl
```
##### leveledImpl
Returns unit's leveled (generated) implementation.
Leveled implementation is unit's current implementation without modifiers,
or base implementation plus upgrades from GDynUpgr.dbf according to unit's level.
This does not include leader upgrades from GleaUpg.dbf, because the upgrades are modifiers.
```lua
unit.leveledImpl
```
##### original
Returns original [unit dummy](luaApi.md#unit-dummy) that represents unit state before transformation,
or `nil` if unit is not transformed.
The state does not include any unit modifiers thus contains only [leveled implementation](luaApi.md#leveledImpl).
Unit can be transformed by transform-self, transform-other, drain-level or doppelganger attack.
```lua
unit.original
```
##### originalModifiers
Returns array of original [modifiers](luaApi.md#modifier) that were applied to unit before transformation,
or empty array if unit is not transformed.
Usually, modifiers are reapplied after transformation, but there are cases where some modifiers are incompatible with a new form,
thus not getting applied to it.
```lua
unit.originalModifiers
```

---

#### Unit dummy
Represents preserved state of game unit. Used, for instance, to preserve unit state before transformation, so it can be restored later.
Methods are identical to [unit](luaApi.md#unit-1), except that there is no [original](luaApi.md#original) and [originalModifiers](luaApi.md#originalModifiers).

---

#### Unit implementation
Represents unit template. Records in GUnits.dbf are unit implementations.

##### How unit implementation works and why it is different to [unit](luaApi.md#unit-1)
Unit implementation is a unit template that can be used by different individual units on scenario map. It is different to unit, because different unit instances can have the same implementation. For example, you can have 3 Squires of level 1 in your party, each having the same implementation.

There are 3 different stages of unit implementation that build on top of each other:
- `Global`, corresponds to a record from `GUnits.dbf`;
  - Returned by [unit.baseImpl](luaApi.md#baseImpl) or [impl.global](luaApi.md#global);
  - Its `id` corresponds to `UNIT_ID` from `GUnits.dbf`.
- `Generated`, equals `Global` plus level upgrades from `GDynUpgr.dbf` (if any);
  - Returned by [unit.leveledImpl](luaApi.md#leveledImpl) or [impl.generated](luaApi.md#generated);
  - Its `id` is _different to_ `id` of inherited `Global` implementation;
  - If unit has no level upgrades, `unit.leveledImpl`/`impl.generated` equals `unit.baseImpl`/`impl.global`.
- `Modified`, equals `Generated` plus applied modifiers from `Gmodif.dbf` (if any).
  - Returned by [unit.impl](luaApi.md#impl);
  - Its `id` _equals to_ `id` of inherited `Generated` implementation;
  - If unit has no modifiers, `unit.impl` equals `unit.leveledImpl`/`impl.generated`.

Unit implementation changes when unit:
- Gets an upgrade, does not matter if it transforms to higher tier unit or simply gets over-level;
- Gets transformed: by Transform-Self, Transform-Other, Drain-Level, or Doppelganger attack;
- Gets [modified](luaApi.md#modifier): when consuming a potion, affected by a spell, equipping an item, getting a leader upgrade, etc.;

Methods:
```lua
-- Returns unit's implementation level. LEVEL value from GUnits.dbf.
impl.level
-- Returns experience points needed for next level. XP_NEXT value from GUnits.dbf.
impl.xpNext
-- Returns experience points granted for killing the unit. XP_KILLED value from GUnits.dbf.
impl.xpKilled
-- Returns unit's hit points. HIT_POINT value from GUnits.dbf.
impl.hp
-- Returns unit's armor. ARMOR value from GUnits.dbf.
impl.armor
-- Returns unit's regen. REGEN value from GUnits.dbf.
impl.regen
-- Returns unit's race. ID value from Lrace.dbf. See Race enumeration for all possible values.
impl.race
-- Returns unit's subrace. ID value from LSubRace.dbf. See Subrace enumeration for all possible values.
impl.subrace
-- Indicates if the unit is small (occupies single slot). SIZE_SMALL value from GUnits.dbf.
impl.small
-- Indicates if the unit is male. SEX_M value from GUnits.dbf.
impl.male
-- Indicates if the unit is water only. WATER_ONLY value from GUnits.dbf.
impl.waterOnly
-- Indicates if the unit attacks twice. ATCK_TWICE value from GUnits.dbf.
impl.attacksTwice
-- Returns level after which dynUpgrade2 rules are applied. DYN_UPG_LV from GUnits.dbf.
impl.dynUpgLvl
-- Returns dynamic upgrade 1.
impl.dynUpg1
-- Returns dynamic upgrade 2.
impl.dynUpg2
-- Returns primary attack or nil if no primary attack used.
impl.attack1
-- Returns secondary attack or nil if no secondary attack used.
impl.attack2
-- Returns alternative attack or nil if no alternative attack used.
impl.altAttack
-- Returns leader maximum movement points (or 0 if unit is not a leader).
impl.movement
-- Returns leader scouting range (or 0 if unit is not a leader).
impl.scout
-- Returns current leadership value (or 0 if unit is not a leader).
impl.leadership
```
##### id
Returns unit implementation [id](luaApi.md#id). `UNIT_ID` value from `GUnits.dbf`.
```lua
impl.id
```
##### base
Returns base [unit implementation](luaApi.md#unit-implementation). `BASE_UNIT` value from `GUnits.dbf`.
```lua
impl.base
```
##### type
Returns unit [type](luaApi.md#unit).
```lua
impl.type
```
##### leaderType
Returns [leader](luaApi.md#leader) type (or -1 if unit is not a leader).
```lua
impl.leaderType
```
##### hasAbility
Returns true if leader has specified [ability](luaApi.md#ability) (or false if unit is not a leader).
```lua
impl:hasAbility(Ability.TalismanUse)
```
##### hasMoveBonus
Returns true if leader has movement bonus on specified [ground](luaApi.md#ground) (or false if unit is not a leader).
```lua
impl:hasMoveBonus(Ground.Water)
```
##### global
Returns global [unit implementation](luaApi.md#unit-implementation) - a record from `GUnits.dbf`.
Same as `unit.baseImpl`.
```lua
impl.global
```
##### generated
Returns generated [unit implementation](luaApi.md#unit-implementation).
Equals `global` plus upgrades from `GDynUpgr.dbf` according to unit's level.
Same as `unit.leveledImpl`.
```lua
impl.generated
```
##### modifiers
Returns array of applied [modifiers](luaApi.md#modifier).
```lua
impl.modifiers
```
##### hasModifier
Returns true if the implementation has [modifier](luaApi.md#modifier) specified by [id](luaApi.md#id-2) or id string.
```lua
impl:hasModifier("G000UM5021")
impl:hasModifier(Id.new("G000UM5021"))
```
##### getImmuneToAttackClass
Returns [immune type](luaApi.md#immune) for specified [attack type](luaApi.md#attack).
```lua
impl:getImmuneToAttackClass(Attack.Paralyze)
```
##### getImmuneToAttackSource
Returns [immune type](luaApi.md#immune) for specified [attack source](luaApi.md#source).
```lua
impl:getImmuneToAttackSource(Source.Water)
```

---

#### Unit slot
Represents one of the twelve unit slots on battlefield.
Unit positions on a battlefield are mirrored.
Frontline positions are even, backline - odd.
```
1 0    0 1
3 2 vs 2 3
5 4    4 5
```

Methods:
```lua
-- Returns a unit that occupies the slot.
slot.unit
-- Returns a position of the slot (0-5).
slot.position
-- Returns a line index of the slot: 0 - frontline, 1 - backline.
slot.line
-- Returns a column index of the slot: 0 - top, 1 - middle, 2 - bottom.
slot.column
-- Indicates if the slot is on the frontline.
slot.frontline
-- Indicates if the slot is on the backline.
slot.backline
-- Returns a distance between two slots (used for adjacent slot calculations).
slot.distance(otherSlot)
```

---

#### Group
Represents 6 unit slots.

Methods:
##### id
Returns group [id](luaApi.md#id).
```lua
group.id
```
##### slots
Returns group as array of 6 [unit slots](luaApi.md#unit-slot).
```lua
group.slots
```
##### units
Returns group [units](luaApi.md#unit-1).
```lua
group.units
```
##### hasUnit
Returns true if group has specified [unit](luaApi.md#unit-1) or [unit id](luaApi.md#id-2).
```lua
group:hasUnit(unit)
group:hasUnit(Id.new('S143UN0001'))
```
##### getUnitPosition
Returns unit position in group, or -1 if unit not found.
```lua
group:getUnitPosition(unit)
group:getUnitPosition(unit.id)
```
##### subrace
Returns group [subrace](luaApi.md#subrace).
In case of group inside [ruin](luaApi.md#ruin), returns -1 since ruins do not belong to subraces.
```lua
group.subrace
```

---

#### Fog
Represents player's fog of war.

Methods:
##### getFog
Returns true if specified map position is covered by fog of war.
Map position can be specified by pair of coordinates or a [point](luaApi.md#point).
```lua
local hidden = fog:getFog(3, 7)
```

---

#### Player
Represents game player including AI and neutrals.

Methods:
##### id
Returns player [id](luaApi.md#id). The value is unique for every player on scenario map.
```lua
player.id
```
##### race
Returns player [race](luaApi.md#race).
```lua
player.race
```
##### lord
Returns player [lord](luaApi.md#lord).
```lua
player.lord
```
##### bank
Returns player [bank](luaApi.md#currency).
```lua
player.bank
```
##### human
Returns true if player is human (not AI).
```lua
player.human
```
##### alwaysAi
Returns true if player is always AI.
```lua
player.alwaysAi
```
##### fog
Returns player's [fog of war](luaApi.md#fog).
In fully loaded scenario, player objects always have fog of war. During scenario loading this property can return `nil`.
```lua
local fog = player.fog
if fog == nil then
    return
end
```

---

#### Stack
Represents [group](luaApi.md#group) of 6 [unit slots](luaApi.md#unit-slot) on a map. One of the units is a leader.

Methods:
##### id
Returns stack [id](luaApi.md#id). The value is unique for every stack on scenario map.
```lua
stack.id
```
##### position
Returns stack position as a [point](luaApi.md#point).
```lua
stack.position
```
##### owner
Returns [player](luaApi.md#player) that owns the stack. Neutral stacks are owned by neutral player.
```lua
stack.owner
```
##### inside
Returns [fort](luaApi.md#fort) that this stack is visiting or `nil` if none.
```lua
stack.inside
```
##### group
Returns stack units as a [group](luaApi.md#group).
```lua
stack.group
```
##### leader
Returns stack leader [unit](luaApi.md#unit-1).
```lua
stack.leader
```
##### subrace
Returns stack [subrace](luaApi.md#subrace).
```lua
stack.subrace
```
##### inventory
Returns array of inventory [items](luaApi.md#item-2). This includes equipped items.
```lua
stack.inventory
```
##### getEquippedItem
Returns equipped [item](luaApi.md#item-2) by [equipment](luaApi.md#equipment) value.
```lua
stack:getEquippedItem(Equipment.Boots)
```
##### order
Returns stack [order](luaApi.md#order).
```lua
stack.order
```
##### orderTargetId
Returns stack's order target [id](luaApi.md#id).
```lua
stack.orderTargetId
```
##### aiOrder
Returns stack [AI order](luaApi.md#order).
```lua
stack.aiOrder
```
```lua
--- Returns stack current movement points.
stack.movement
--- Returns true if stack is invisible.
stack.invisible
--- Returns number of battles won by the stack.
stack.battlesWon
```

---

#### Fort
Represents Capital or City on a map. Fort contains a garrison [group](luaApi.md#group) of 6 [unit slots](luaApi.md#unit-slot).
Note that the garrison group is different to a group of visiting [stack](luaApi.md#stack).

Methods:
##### id
Returns fort [id](luaApi.md#id). The value is unique for every fort on scenario map.
```lua
fort.id
```
##### position
Returns fort position as a [point](luaApi.md#point).
```lua
fort.position
```
##### owner
Returns [player](luaApi.md#player) that owns the fort. Neutral forts are owned by neutral player.
```lua
fort.owner
```
##### group
Returns fort units as a [group](luaApi.md#group).
```lua
fort.group
```
##### visitor
Returns visitor [stack](luaApi.md#stack), or `nil` if none.
```lua
fort.visitor
```
##### subrace
Returns fort [subrace](luaApi.md#subrace).
```lua
fort.subrace
```
##### inventory
Returns array of inventory [items](luaApi.md#item-2).
```lua
fort.inventory
```
##### capital
Returns true if fort is a capital city.
```lua
fort.capital
```
##### tier
Returns fort tier (level). Tiers are in range \[1 : 6\]. Tier 6 corresponds to the capital city.
```lua
fort.tier
```

---

#### Merchant item
Represents item sold by [merchant](luaApi.md#merchant).

Methods:
##### base
Returns [base item](luaApi.md#itembase).
```lua
merchantItem.base
```
##### amount
Returns amount of items in merchant stock.
```lua
merchantItem.amount
```

---

#### Merchant
Represents Merchant on a map.

Methods:
##### id
Returns merchant [id](luaApi.md#id). The value is unique for every merchant on scenario map.
```lua
merchant.id
```
##### position
Returns merchant position as a [point](luaApi.md#point).
```lua
merchant.position
```
##### visitors
Returns list of [players](luaApi.md#player) that have visited the merchant.
```lua
merchant.visitors
```
##### items
Returns list of [merchant items](luaApi.md#merchant-item).
```lua
merchant.items
```
##### temple
Returns true if merchant can be used as a temple for AI to heal.
```lua
merchant.temple
```

---

#### Mercenary unit
Represents unit for hire in mercenary camp.

Methods:
##### impl
Returns [unit implementation](luaApi.md#unit-implementation).
```lua
mercenary.impl
```
##### unique
Returns true is unit can be hired only once.
```lua
mercenaryUnit.unique
```

---

#### Mercenary
Represents Mercenary camp on a map.

Methods:
##### id
Returns mercenary camp [id](luaApi.md#id). The value is unique for every mercenary camp on scenario map.
```lua
mercenary.id
```
##### position
Returns mercenary camp position as a [point](luaApi.md#point).
```lua
mercenary.position
```
##### visitors
Returns list of [players](luaApi.md#player) that have visited the mercenary camp.
```lua
mercenary.visitors
```
##### units
Returns list of [mercenary units](luaApi.md#mercenary-unit).
```lua
mercenary.units
```

---

#### Trainer
Represents Trainer on a map.

Methods:
##### id
Returns trainer [id](luaApi.md#id). The value is unique for every trainer on scenario map.
```lua
trainer.id
```
##### position
Returns trainer position as a [point](luaApi.md#point).
```lua
trainer.position
```
##### visitors
Returns list of [players](luaApi.md#player) that have visited the trainer.
```lua
trainer.visitors
```

---

#### Ruin
Represents Ruin on a map. Ruin contains a garrison [group](luaApi.md#group) of 6 [unit slots](luaApi.md#unit-slot).

Methods:
##### id
Returns ruin [id](luaApi.md#id). The value is unique for every ruin on scenario map.
```lua
ruin.id
```
##### position
Returns ruin position as a [point](luaApi.md#point).
```lua
ruin.position
```
##### looter
Returns [player](luaApi.md#player) that looted the ruin, or `nil` if none.
```lua
ruin.looter
```
##### group
Returns ruin units as a [group](luaApi.md#group).
```lua
ruin.group
```
##### item
Returns [item](luaApi.md#item-2) reward for looting the ruin.
```lua
ruin.item
```
##### cash
Returns [cash](luaApi.md#currency) reward for looting the ruin.
```lua
ruin.cash
```

---

#### Rod
Represents rod object in scenario. Rods are planted to transform terrain and capture resources.

Methods:
##### id
Returns rod [id](luaApi.md#id). The value is unique for every rod on scenario map.
```lua
rod.id
```
##### position
Returns copy of rod position as a [point](luaApi.md#point).
```lua
rod.position
```
##### owner
Returns [player](luaApi.md#player) that planted the rod.
```lua
rod.owner
```

---

#### Crystal
Represents gold mine or mana source on a map.

Methods:
##### id
Returns crystal [id](luaApi.md#id).
```lua
crystal.id
```
##### position
Returns crystal position as a [point](luaApi.md#point).
```lua
crystal.position
```
##### resource
Returns crystal [resource](luaApi.md#resource) type.
```lua
crystal.resource
```

---

#### Dynamic upgrade
Represents rules that applied when unit makes its progress gaining levels. Records in GDynUpgr.dbf are dynamic upgrades.

Methods:
```lua
-- Returns number of experience points added with each dynamic upgrade. XP_NEXT value from GDynUpgr.dbf.
dynUpgrade.xpNext
```

---

#### Location
Represents location object in scenario.

Methods:
##### id
Returns location [identifier](luaApi.md#id).
```lua
location.id
```
##### position
Returns copy of location position as a [point](luaApi.md#point).
```lua
location.position
```
##### radius
Returns radius of location
```lua
location.radius
```

---

#### ScenarioVariable
Represents scenario variable used by events.

Methods:
```lua
-- Returns variable name
variable.name
-- Returns variable value
variable.value
```

---

#### ScenarioVariables
Stores scenario variables, allows searching them by name.

Methods:
##### getVariable
Searches for [ScenarioVariable](luaApi.md#scenariovariable) by its name, reeturns `nil` if not found.
```lua
local variable = variables:getVariable('VAR1')
if (variable == nil) then
    return
end
```

---

#### Tile
Represents map tile.

Methods:
##### terrain
Returns tile [terrain](luaApi.md#terrain) type.
```lua
tile.terrain
```
##### ground
Returns tile [ground](luaApi.md#ground) type.
```lua
tile.ground
```

---

#### Diplomacy
Represents diplomacy relations between [races](luaApi.md#race) in [scenario](luaApi.md#scenario).

Methods:
##### getCurrentRelation
Returns current diplomacy relations value between two [races](luaApi.md#race) in range \[0 : 100\].
```lua
local current = diplomacy:getCurrentRelation(race1, race2)
```
##### getPreviousRelation
Returns previous diplomacy relations value between two [races](luaApi.md#race) in range \[0 : 100\].
```lua
local prev = diplomacy:getPreviousRelation(race1, race2)
```
##### getAlliance
Returns true if two [races](luaApi.md#race) are in alliance.
```lua
local allies = diplomacy:getAlliance(race1, race2)
```
##### getAllianceTurn
Returns turn number when two [races](luaApi.md#race) made an alliance.
Returns zero if races are not in alliance.
```lua
local turn = diplomacy:getAllianceTurn(race1, race2)
```
##### getAlwaysAtWar
Returns true if two [races](luaApi.md#race) are always at war.
```lua
local atWar = diplomacy:getAlwaysAtWar(race1, race2)
```
##### getAiCouldNotBreakAlliance
Returns true if diplomacy relations prohibit AI-controlled [races](luaApi.md#race) from breaking alliance.
```lua
local couldNotBreak = diplomacy:getAiCouldNotBreakAlliance(race1, race2)
```
##### getRelationType
Returns [relation type](luaApi.md#relation) according to diplomacy relations value.
```lua
-- Value to type mapping (D_WAR and D_NEUTRAL can be found in GVars.dbf):
-- 0       D_WAR           D_NEUTRAL         100
-- |   War   |     Neutral     |     Peace    |
--
local relation = diplomacy:getRelationType(relationValue)
```

---

#### Scenario
Represents scenario map with all its objects and state.

Methods:
##### getLocation
Searches for [Location](luaApi.md#location) by id string or [Id](luaApi.md#id), returns `nil` if not found.
```lua
local location = scenario:getLocation('S143LO0001')
if (location == nil) then
    return
end
```
##### variables
Returns [ScenarioVariables](luaApi.md#scenariovariables). If scenario has no variables defined, returns `nil`.
```lua
local variables = scenario.variables
if (variables == nil) then
    return
end
```
##### getTile
Searches for [Tile](luaApi.md#tile) by pair of coordinates or [Point](luaApi.md#point), returns `nil` if not found.
```lua
local tile = scenario:getTile(3, 5)
if (tile == nil) then
    return
end
```
##### getStack
Searches for [stack](luaApi.md#stack) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local stack = scenario:getStack(10, 15)
if (stack == nil) then
    return
end
```
##### getFort
Searches for [fort](luaApi.md#fort) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local fort = scenario:getFort(10, 15)
if (fort == nil) then
    return
end
```
##### getRuin
Searches for [ruin](luaApi.md#ruin) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local ruin = scenario:getRuin(10, 15)
if (ruin == nil) then
    return
end
```
##### getRod
Searches for [rod](luaApi.md#rod) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local rod = scenario:getRod(10, 15)
if (rod == nil) then
    return
end
```
##### getPlayer
Searches for [player](luaApi.md#player) by id string or [id](luaApi.md#id), returns `nil` if not found.
```lua
local player = scenario:getPlayer('S143PL0000')
if (player == nil) then
    return
end
```
##### getUnit
Searches for [unit](luaApi.md#unit-1) by id string or [unit id](luaApi.md#id-2), returns `nil` if not found.
```lua
local unit = scenario:getUnit('S143UN0001')
if (unit == nil) then
    return
end
```
##### getItem
Searches for [item](luaApi.md#item-2) by id string or item [id](luaApi.md#id), returns `nil` if not found.
```lua
local item = scenario:getItem('S143IM0001')
if not item then
  return
end
```
##### getCrystal
Searches for [crystal](luaApi.md#crystal) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local crystal = scenario:getCrystal('S143CR0003')
if not crystal then
  return
end
```
##### getMerchant
Searches for [merchant](luaApi.md#merchant) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local merchant = scenario:getCrystal('S143SI0001')
if not merchant then
  return
end
```
##### getMercenary
Searches for [mercenary camp](luaApi.md#mercenary) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local mercenary = scenario:getCrystal('S143SI0002')
if not mercenary then
  return
end
```
##### getTrainer
Searches for [trainer](luaApi.md#trainer) by:
- id string
- [id](luaApi.md#id)
- pair of coordinates
- [point](luaApi.md#point)

Returns `nil` if not found.
```lua
local trainer = scenario:getCrystal('S143SI0005')
if not trainer then
  return
end
```
##### findStackByUnit
Searches for [stack](luaApi.md#stack) that has specified [unit](luaApi.md#unit-1) among all the stacks in the whole [scenario](luaApi.md#scenario).
You can also use unit id string or [id](luaApi.md#id).
Returns `nil` if not found.
```lua
local stack = scenario:findStackByUnit(unit)
if stack == nil then
    return
end
```
**Note** that this search is heavy in terms of performance, so you probably want to minimize excessive calls and use variables to store its results.
##### findFortByUnit
Searches for [fort](luaApi.md#fort) that has specified [unit](luaApi.md#unit-1) in its garrison among all the forts in the whole [scenario](luaApi.md#scenario).
Only garrison units are counted, visiting stack is ignored.
You can also use unit id string or [id](luaApi.md#id).
Returns `nil` if not found.
```lua
local fort = scenario:findFortByUnit(unit)
if fort == nil then
    return
end
```
**Note** that this search is heavy in terms of performance, so you probably want to minimize excessive calls and use variables to store its results.
##### findRuinByUnit
Searches for [ruin](luaApi.md#ruin) that has specified [unit](luaApi.md#unit-1) among all the ruins in the whole [scenario](luaApi.md#scenario).
You can also use unit id string or [id](luaApi.md#id).
Returns `nil` if not found.
```lua
local ruin = scenario:findRuinByUnit(unit)
if ruin == nil then
    return
end
```
**Note** that this search is heavy in terms of performance, so you probably want to minimize excessive calls and use variables to store its results.
##### day
Returns number of current day in game.
```lua
scenario.day
```
##### size
Returns scenario map size.
```lua
scenario.size
```
##### diplomacy
Returns object that holds [diplomacy](luaApi.md#diplomacy) relations between races.
Fully loaded scenario always have diplomacy relations. During scenario loading this property can return `nil`.
```lua
local diplomacy = scenario.diplomacy
if diplomacy == nil then
    return
end
```
##### forEachStack
Searches for every [stack](luaApi.md#stack) on a map and calls specified function on it.
```lua
scenario:forEachStack(function (stack)
  log('Visit stack ' .. tostring(stack.id))
end)
```
##### forEachLocation
Searches for every [location](luaApi.md#location) on a map and calls specified function on it.
```lua
scenario:forEachLocation(function (location)
  log('Visit location ' .. tostring(location.id))
end)
```
##### forEachFort
Searches for every [fort](luaApi.md#fort) on a map and calls specified function on it.
```lua
scenario:forEachFort(function (city)
  log('Visit city ' .. tostring(city.id))
end)
```
##### forEachRuin
Searches for every [ruin](luaApi.md#ruin) on a map and calls specified function on it.
```lua
scenario:forEachRuin(function (ruin)
  log('Visit ruin ' .. tostring(ruin.id))
end)
```
##### forEachRod
Searches for every [rod](luaApi.md#rod) on a map and calls specified function on it.
```lua
scenario:forEachRod(function (rod)
  log('Visit rod ' .. tostring(rod.id))
end)
```
##### forEachPlayer
Searches for every [player](luaApi.md#player) on a map and calls specified function on it.
```lua
scenario:forEachPlayer(function (player)
  log('Visit player ' .. tostring(player.id))
end)
```
##### forEachUnit
Searches for every [unit](luaApi.md#unit-1) on a map and calls specified function on it.
```lua
scenario:forEachUnit(function (unit)
  log('Visit unit ' .. tostring(unit.id))
end)
```
##### forEachCrystal
Searches for every [crystal](luaApi.md#crystal) on a map and calls specified function on it.
```lua
scenario:forEachCrystal(function (crystal)
  log('Visit crystal ' .. tostring(crystal.id))
end)
```
##### forEachMerchant
Searches for every [merchant](luaApi.md#merchant) on a map and calls specified function on it.
```lua
scenario:forEachMerchant(function (merchant)
  log('Visit merchant ' .. tostring(merchant.id))
end)
```
##### forEachMercenary
Searches for every [mercenary camp](luaApi.md#mercenary) on a map and calls specified function on it.
```lua
scenario:forEachMercenary(function (mercenary)
  log('Visit mercenary ' .. tostring(mercenary.id))
end)
```
##### forEachTrainer
Searches for every [trainer](luaApi.md#trainer) on a map and calls specified function on it.
```lua
scenario:forEachTrainer(function (trainer)
  log('Visit trainer ' .. tostring(trainer.id))
end)
```

---

#### Attack
Represents attack of [Unit implementation](luaApi.md#unit-implementation).

Methods:
##### id
Returns attack [id](luaApi.md#id). This is different for every dynamic upgrade unit gets.
```lua
attack.id
```
##### type
Returns attack [type](luaApi.md#attack).
```lua
attack.type
```
##### source
Returns attack [source](luaApi.md#source).
```lua
attack.source
```
##### reach
Returns attack [reach](luaApi.md#reach).
```lua
attack.reach
```
##### wards
Returns array of [modifiers](luaApi.md#modifier) applied by bestow wards attack.
```lua
attack.wards
```
```lua
--- Returns attack initiative value.
attack.initiative
--- Returns attack power (accuracy).
attack.power
--- Returns damage the attack can inflict. Damage depends on attack type.
attack.damage
--- Returns healing the attack can apply. Healing depends on attack type.
attack.heal
--- Returns true if attack has long effect duration. Effect depends on attack type.
attack.infinite
--- Returns true if attack can inflict critical damage.
attack.crit
--- Returns level for boost damage, lower damage and lower initiative attacks.
attack.level
--- Returns true if attack is melee (L_ADJACENT or custom reach marked as MELEE in LAttR.dbf).
attack.melee
--- Returns maximum number of targets (1, 6 or MAX_TARGTS value for custom reach in LAttR.dbf).
attack.maxTargets
--- Returns critical damage percent [0 : 255].
attack.critDamage
--- Returns critical damage chance [0 : 100].
attack.critPower
--- Returns damage ratio [0 : 255] for additional targets.
attack.damageRatio
--- Returns true if damage ratio reapplied for each consecutive target.
attack.damageRatioPerTarget
--- Returns true if damage is split among targets.
attack.damageSplit
```

---

#### Currency
Represents game currency, mana and gold united.

Methods:
##### new
Creates new currency from existing object.
```lua
Currency.new(existing)
```
##### infernalMana
Returns or sets amount of infernal mana.
```lua
currency.infernalMana
```
##### lifeMana
Returns or sets amount of life mana.
```lua
currency.lifeMana
```
##### deathMana
Returns or sets amount of death mana.
```lua
currency.deathMana
```
##### runicMana
Returns or sets amount of runic mana.
```lua
currency.runicMana
```
##### groveMana
Returns or sets amount of grove mana.
```lua
currency.groveMana
```
##### gold
Returns or sets amount of gold.
```lua
currency.gold
```

---

#### Item base
Represents base item of any type (described in GItem.dbf).

Methods:
##### id
Returns item [id](luaApi.md#id). `ITEM_ID` value from `GItem.dbf`.
```lua
base.id
```
##### type
Returns item [type](luaApi.md#item).
```lua
base.type
```
##### value
Returns item [value](luaApi.md#currency).
```lua
base.value
```
##### unitImpl
Returns related [Unit implementation](luaApi.md#unit-implementation).
For instance: in case of "Angel Orb", Angel unit implementation is returned.
```lua
base.unitImpl
```
##### attack
Returns [Attack](luaApi.md#attack-1) that this item performs (in case of orb or talisman), or `nil` if no attack is associated with the item.
For instance: in case of "Orb of Fire", corresponding attack from `Gattacks.dbf` is returned.
```lua
base.attack
```

---

#### Item
Represents item object in the current scenario.

Methods:
##### id
Returns item [id](luaApi.md#id). This is different to id of [Item base](luaApi.md#itembase).
The value is unique for every item on scenario map.
```lua
item.id
```
##### base
Returns [Item base](luaApi.md#itembase).
```lua
item.base
```
##### sellValue
Returns item sell [value](luaApi.md#currency), it accounts global sell ratio and used talisman charges (if applicable).
```lua
item.sellValue
```

---

#### Battle
Represents battle information.

Methods:
##### getUnitStatus
Returns whether a unit with a specified [id](luaApi.md#id) has a specified [battle status](luaApi.md#battlestatus).
```lua
if battle:getUnitStatus(unit.id, BattleStatus.Defend) then
    -- Do something scary
end
```
##### currentRound
Returns current round in battle. Round counting starts from 1, but there is a special round 0 when units with 'Doppelganger' [attacks](luaApi.md#attack) present.
```lua
battle.currentRound
```
##### autoBattle
Returns true if autobattle mode is turned on.
```lua
battle.autoBattle
```
##### attackerPlayer
Returns [player](luaApi.md#player) that started battle or `nil` if not found.
```lua
battle.attackerPlayer
```
##### defenderPlayer
Returns [player](luaApi.md#player) that was attacked or `nil` if not found.
```lua
battle.defenderPlayer
```
##### attacker
Returns [stack](luaApi.md#stack) that started battle.
Only stacks can initiate battles.
```lua
battle.attacker
```
##### defender
Returns [group](luaApi.md#group) that was attacked.
Defender group can represent units of a [stack](luaApi.md#stack), [fort](luaApi.md#fort) or [ruin](luaApi.md#ruin).
Use `group.id.type` to get actual type of a group.
```lua
battle.defender
```
##### isUnitAttacker
Returns true if [unit](luaApi.md#unit-1) belongs to attacker group.
Method also accepts unit [ids](luaApi.md#id).
```lua
battle:isUnitAttacker(unit)
-- Same check with unit id
battle:isUnitAttacker(unit.id)
```
##### getUnitActions
Returns possible actions and attack options for specified [unit](luaApi.md#unit-1).
Method also accepts unit [ids](luaApi.md#id).
Returns:
- list of [battle actions](luaApi.md#BattleAction) that unit can perform.
- [group](luaApi.md#group) that is a target for a unit attack.
- list of unit attack targets, indices of attack target group slots. 
- [group](luaApi.md#group) that is a target for a `Battle1` equipped item in case of leader unit.
- list of `Battle1` equipped item targets, indices of group slots.
- [group](luaApi.md#group) that is a target for a `Battle2` equipped item in case of leader unit.
- list of `Battle2` equipped item targets, indices of group slots.
```lua
local actions,
    attackTargetGroup,
    attackTargets,
    item1TargetGroup,
    item1Targets,
    item2TargetGroup,
    item2Targets = battle:getUnitActions(unit)
```
##### getRetreatStatus
Returns [retreat status](luaApi.md#Retreat) of attacker or defender group.
```lua
local attackerStatus = battle:getRetreatStatus(true)
local defenderStatus = battle:getRetreatStatus(false)
```
##### decidedToRetreat
Returns true if decision about groups retreat was made and should not be reconsidered.
```lua
battle.decidedToRetreat
```
##### afterBattle
Returns true if battle is over but healers can make one more turn to heal allies.
```lua
battle.afterBattle
```
##### getUnitShatteredArmor
Returns amount of unit armor shattered in battle so far.
Method also accepts unit [ids](luaApi.md#id).
```lua
battle:getUnitShatteredArmor(unit)
-- Same but using id
battle:getUnitShatteredArmor(unit.id)
```
##### getUnitFortificationArmor
Returns armor that is granted to unit by fortification, if any.
Method also accepts unit [ids](luaApi.md#id).
```lua
battle:getUnitFortificationArmor(unit)
-- Same but using id
battle:getUnitFortificationArmor(unit.id)
```
##### isUnitResistantToSource
Returns true if specified unit is resistant to [attack source](luaApi.md#source).
Method also accepts unit [ids](luaApi.md#id).
```lua
battle:isUnitResistantToSource(unit, attackSource)
```
##### isUnitResistantToClass
Returns true if specified unit is resistant to [attack class](luaApi.md#attack).
Method also accepts unit [ids](luaApi.md#id).
```lua
battle:isUnitResistantToClass(unit, attackSource)
```
##### setRetreatStatus
Sets [retreat status](luaApi.md#Retreat) of attacker or defender group.
This method can be only used in AI battle action script.
```lua
-- Attacker group is going to fully retreat
battle:setRetreatStatus(true, Retreat.FullRetreat)
-- Do not retreat defender group 
battle:setRetreatStatus(false, Retreat.NoRetreat)
```
##### setDecidedToRetreat
Notifies battle state that the decision about groups retreat was made and it is final.
This method can be only used in AI battle action script.
```lua
battle:setDecidedToRetreat()
```

---

### Examples

#### doppelganger.lua
`doppelganger` and `target` have type [Unit](luaApi.md#unit-1).
`item` is [Item](luaApi.md#item-2) used to perform the attack.
`battle` specifies an information about current [battle](luaApi.md#battle).
```lua
function getLevel(doppelganger, target, item, battle)
    -- Get current doppelganger implementation
    local impl = doppelganger.impl
    -- Get target unit implementation
    local targImpl = target.impl

    -- Get least level value from both
    local level = math.min(impl.level, targImpl.level)

    -- Make sure doppelganger transform level is not lesser than target's base
    local baseImpl = target.baseImpl
    if (level < baseImpl.level) then
        level = baseImpl.level
    end

    return level
end
```

#### transformSelf.lua
`unit` has type [Unit](luaApi.md#unit-1).
`transformImpl` is [Unit implementation](luaApi.md#unit-implementation).
`item` is [Item](luaApi.md#item-2) used to perform the attack.
`battle` specifies an information about current [battle](luaApi.md#battle).
```lua
function getLevel(unit, transformImpl, item, battle)
    -- Transform into current level or level of resulting unit's template, whichever is bigger.
    return math.max(unit.impl.level, transformImpl.level)
end
```

#### transformOther.lua
`attacker` and `target` have type [Unit](luaApi.md#unit-1).
`transformImpl` is [Unit implementation](luaApi.md#unit-implementation).
`item` is [Item](luaApi.md#item-2) used to perform the attack.
`battle` specifies an information about current [battle](luaApi.md#battle).
```lua
function getLevel(attacker, target, transformImpl, item, battle)
    -- transform using target level with a minimum of transform impl level
    return math.max(target.impl.level, transformImpl.level);
end
```

`attacker` and `target` have type [Unit](luaApi.md#unit-1).
`item` is [Item](luaApi.md#item-2) used to perform the attack.
`battle` specifies an information about current [battle](luaApi.md#battle).
#### drainLevel.lua
```lua
function getLevel(attacker, target, item, battle)
    -- transform into unit with its level minus 1 and minus attacker over-level
    return math.max(1, target.impl.level - 1 - attacker.impl.level + attacker.baseImpl.level);
end
```

#### summon.lua
`summoner` has type [Unit](luaApi.md#unit-1).
`summonImpl` is [Unit implementation](luaApi.md#unit-implementation).
`item` is [Item](luaApi.md#item-2) used to perform the attack.
`battle` specifies an information about current [battle](luaApi.md#battle).
```lua
function getLevel(summoner, summonImpl, item, battle)
    -- Use base level of summon if cheap item is used to summon it
    if item and item.base.value.gold < 500 then
        return summonImpl.level
    end

    -- Summon unit with level twice as big as summoner level
    -- or with level of summon implementation, whichever is bigger.
    local impl = summoner.impl
    local summonerLevel = impl.level

    local summonLevel = summonImpl.level

    return math.max(summonerLevel * 2, summonLevel)
end
```

See [Scripts](Scripts) directory for additional examples.

---

### Targeting scripts (custom attack reaches, specified via Globals\LAttR.dbf)
Targeting scripts are used to specify either selection or attack targets of custom attack reach:
- **Selection** targets are targets that can be **selected (clicked)** (specified as `SEL_SCRIPT` in `LAttR.dbf`);
- **Attack** targets are targets that will be **affected by attack** (specified as `ATT_SCRIPT` in `LAttR.dbf`).
For instance, in case of "pierce" attack, you can only click adjacent targets, but the attack will not only affect the selected target but also the one behind it (if any).
Thus the "pierce" attack uses **getAdjacentTargets.lua as selection** script and **getSelectedTargetAndOneBehindIt.lua as attack** script.

Targeting scripts use uniform `getTargets` function for both selection and attack scripts with the following arguments:
- `attacker` is the [unit slot](luaApi.md#unit-slot) of the attacker unit;
- `selected` is the [unit slot](luaApi.md#unit-slot) of the unit that was selected (clicked). `selected.position == -1` and `selected.unit == nil` if this is selection script (no target is clicked yet);
- `allies` are [unit slots](luaApi.md#unit-slot) of all the allies on the battlefield (excluding the attacker);
- `targets` are [unit slots](luaApi.md#unit-slot) of all the targets on the battlefield on which the attack can be performed. For instance, if targets are allies and the attack is Revive, then it will only include dead allies that can be revived;
- `targetsAreAllies` specified whether targets are allies;
- `item` specifies an [item](luaApi.md#item-2) (orb or talisman) used to perform the attack, or `nil` if no item is used;
- `battle` specifies an information about current [battle](luaApi.md#battle);
- `isMarking` specified whether the script is being called to mark targets visually on the battlefield. Can be used to provide consistent visual representation for randomized scripts, as soft alternative to `MRK_TARGTS` flag in `LAttR.dbf`. Always `false` if this is selection script.

#### Example of attack script of pierce attack (getSelectedTargetAndOneBehindIt.lua)
```lua
function getTargets(attacker, selected, allies, targets, targetsAreAllies, item, battle, isMarking)
    -- Get the selected target and the one behind it (pierce attack)
    local result = {selected}
    for i = 1, #targets do
        local target = targets[i]
        if target.backline and target.position == selected.position + 1 then
            table.insert(result, target)
            break
        end
    end
    return result
end
```

---

### Event condition examples
#### Check if all tiles in location have the same terrain (Human)
```lua
-- You can use lambda functions freely
local forEachTile = function (location, f)
    local pos = location.position
    -- Use integers for tile coordinates
    local halfR = math.floor(location.radius / 2)
    local startX = pos.x - halfR
    local startY = pos.y - halfR
    local endX = pos.x + halfR
    local endY = pos.y + halfR
    
    for x = startX,endX,1 do
        for y = startY,endY,1 do
            f(x, y)
        end
    end
end

local location = scenario:getLocation('S143LO0000')
if (location == nil) then
    return false
end

local tilesTotal = location.radius * location.radius
local count = 0

forEachTile(location, function (x, y)
    local tile = scenario:getTile(x, y)
    if (tile == nil) then
        return false
    end
    
    if (tile.terrain == Terrain.Human) then
        count = count + 1
    end
end)

return tilesTotal == count
```

---

### Custom modifiers
Modifiers in the game stack on top of each other, making ordered modifiers chain.<br>
First modifier takes base values from the unit it modifies, next - values modified by the first modifier and so on.<br>
Custom modifiers allow you to modify every stat of unit and its attacks in a single script file with a number of uniform functions.<br>

Most of the custom modifier functions have the following form:
```lua
function getSomething(unit, prev)
    if someCondition then
        return modifiedValue
    end

    return prev
end
```
`unit` has type [Unit](luaApi.md#unit-1). The unit is presented in a state before the current modifier is applied.<br>
`prev` is a previous value of the stat. It is either a base value or a value modified by the previous modifier.

Check [Scripts/Modifiers](Scripts/Modifiers) for script examples.<br>
[template.lua](Scripts/Modifiers/template.lua) contains a complete list of available functions.

#### Due to how modifiers chain work, you have no direct access to final unit stats
For example:
- Lets say we have a unit with base of `50` initiative;
- Then we give it a potion that increases damage by 50% **if unit initiative** `> 60`;
- Then we give it another potion that increases initiative to `70`.

The damage bonus **will not work** in this case, even though a final unit initiative is `70` that is `> 60`.
This is because **damage modifier applied earlier than initiative modifier**.
If we get `unit.impl.attack1.initiative` from the damage modifier script it will be `50`, because **initiative modifier is not applied yet**.

#### Dangerous way around to get final unit stats
The limitation described above **can be avoided** by getting unit instance via `getScenario:getUnit(unit.id)`.<br>
But you have to be **very careful** using this approach, as you can easily fall into a deadloop.

Lets see a **bad example** where we created a modifier that grants bonus armor and regen depending on each other:
```lua
function getArmor(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.regen / 5
end

function getRegen(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.armor / 10
end
```
Or it can be two different modifiers, does not matter:
```lua
-- MyBonusArmorMod.lua
function getArmor(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.regen / 5
end
```
and
```lua
-- MyBonusRegenMod.lua
function getRegen(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.armor / 10
end
```
When this modifier(s) applied to a unit, we are getting circular dependence here: **final armor depends on final regen while final regen depends on final armor**.<br>
Imagine what happens when the game tries to get unit armor:<br>
It calls `getArmor` that calls `getRegen` that calls `getArmor` that calls `getRegen` that calls `getArmor` that calls `getRegen` that calls `getArmor`...and so on until your **game hang or crash to desktop**.

As a **good example**, you could refer to a third stat, thus avoiding deadloop condition:
```lua
-- MyBonusArmorMod.lua
function getArmor(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.regen / 5
end
```
and
```lua
-- MyBonusRegenMod.lua
function getRegen(unit, prev)
    local finalUnit = getScenario():getUnit(unit.id)
    return prev + finalUnit.impl.level / 10
end
```
This way, regen depends on level, and armor depends on regen and there is no circular dependence in this case.<br>
**Remember that this is subject for all modifiers that can potentially happen to be applied to the same unit.**
