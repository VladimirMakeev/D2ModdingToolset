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
-- Returns unit's current implementation.
-- Current implementation describes unit stats according to its levels
-- and possible transformations applied during battle.
unit.impl
-- Returns unit's base implementation.
-- Base implementation is a record in GUnits.dbf that describes unit basic stats.
unit.baseImpl
-- Returns unit's leveled (generated) implementation.
-- Leveled implementation is unit's current implementation without modifiers,
-- or base implementation plus upgrades from GDynUpgr.dbf according to unit's level.
-- This does not include leader upgrades from GleaUpg.dbf, because the upgrades are modifiers.
unit.leveledImpl
```
##### id
Returns unit [id](luaApi.md#id). This is different to id of [unit implementation](luaApi.md#unit-implementation).
The value is unique for every unit on scenario map.
```lua
unit.id
```

---

#### Unit implementation
Represents unit template. Records in GUnits.dbf are unit implementations.

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
Returns [fort](luaApi.md#fort) that this stack is visiting or nil if none.
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
Returns array of inventory [items](luaApi.md#item-1). This includes equipped items.
```lua
stack.inventory
```
##### getEquippedItem
Returns equipped [item](luaApi.md#item-1) by [equipment](luaApi.md#equipment) value.
```lua
stack:getEquippedItem(Equipment.Boots)
```
```lua
--- Returns stack current movement points.
stack.movement
--- Returns true if stack is invisible.
stack.invisible
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
Returns visitor [stack](luaApi.md#stack), or nil if none.
```lua
fort.visitor
```
##### subrace
Returns fort [subrace](luaApi.md#subrace).
```lua
fort.subrace
```
##### inventory
Returns array of inventory [items](luaApi.md#item-1).
```lua
fort.inventory
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
Returns [player](luaApi.md#player) that looted the ruin, or nil if none.
```lua
ruin.looter
```
##### group
Returns ruin units as a [group](luaApi.md#group).
```lua
ruin.group
```
##### item
Returns [item](luaApi.md#item-1) reward for looting the ruin.
```lua
ruin.item
```
##### cash
Returns [cash](luaApi.md#currency) reward for looting the ruin.
```lua
ruin.cash
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
Searches for [ScenarioVariable](luaApi.md#scenariovariable) by its name, reeturns nil if not found.
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

#### Scenario
Represents scenario map with all its objects and state.

Methods:
##### getLocation
Searches for [Location](luaApi.md#location) by id string or [Id](luaApi.md#id), returns nil if not found.
```lua
local location = scenario:getLocation('S143LO0001')
if (location == nil) then
    return
end
```
##### variables
Returns [ScenarioVariables](luaApi.md#scenariovariables). If scenario has no variables defined, returns nil.
```lua
local variables = scenario.variables
if (variables == nil) then
    return
end
```
##### getTile
Searches for [Tile](luaApi.md#tile) by pair of coordinates or [Point](luaApi.md#point), returns nil if not found.
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

Returns nil if not found.
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

Returns nil if not found.
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

Returns nil if not found.
```lua
local ruin = scenario:getRuin(10, 15)
if (ruin == nil) then
    return
end
```
##### getPlayer
Searches for [player](luaApi.md#player) by id string or [id](luaApi.md#id), returns nil if not found.
```lua
local player = scenario:getPlayer(stack)
if (player == nil) then
    return
end
```
##### getUnit
Searches for [unit](luaApi.md#unit-1) by id string or [unit id](luaApi.md#id-2), returns nil if not found.
```lua
local unit = scenario:getUnit('S143UN0001')
if (unit == nil) then
    return
end
```
##### findStackByUnit
Searches for [stack](luaApi.md#stack) that has specified [unit](luaApi.md#unit-1) among all the stacks in the whole [scenario](luaApi.md#scenario).
You can also use unit id string or [id](luaApi.md#id).
Returns nil if not found.
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
Returns nil if not found.
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
Returns nil if not found.
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

### Examples

#### doppelganger.lua
```lua
-- 'doppelganger' and 'target' are both of type Unit.
function getLevel(doppelganger, target)
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
```lua
function getLevel(unit, transformImpl)
    -- Transform into current level or level of resulting unit's template, whichever is bigger.
    return math.max(unit.impl.level, transformImpl.level)
end
```

#### transformOther.lua
`attacker` and `target` has type [Unit](luaApi.md#unit-1).
`transformImpl` is [Unit implementation](luaApi.md#unit-implementation).
`item` is [Item](luaApi.md#item-1).
```lua
function getLevel(attacker, target, transformImpl, item)
    -- transform using target level with a minimum of transform impl level
    return math.max(target.impl.level, transformImpl.level);
end
```

`attacker` and `target` has type [Unit](luaApi.md#unit-1).
`item` is [Item](luaApi.md#item-1).
#### drainLevel.lua
```lua
function getLevel(attacker, target, item)
    -- transform into unit with its level minus 1 and minus attacker over-level
    return math.max(1, target.impl.level - 1 - attacker.impl.level + attacker.baseImpl.level);
end
```

#### summon.lua
`summoner` has type [Unit](luaApi.md#unit-1).
`summonImpl` is [Unit implementation](luaApi.md#unit-implementation).
`item` is [Item](luaApi.md#item-1).
```lua
function getLevel(summoner, summonImpl, item)
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
- **Selection** targets are targets that can be **selected (clicked)** (specified as SEL_SCRIPT in LAttR.dbf);
- **Attack** targets are targets that will be **affected by attack** (specified as ATT_SCRIPT in LAttR.dbf).

For instance, in case of "pierce" attack, you can only click adjacent targets, but the attack will not only affect the selected target but also the one behind it (if any).

Thus the "pierce" attack uses **getAdjacentTargets.lua as selection** script and **getSelectedTargetAndOneBehindIt.lua as attack** script.
#### getSelectedTargetAndOneBehindIt.lua
```lua
--[[
'attacker' is the unit slot of the attacker unit
'selected' is the unit slot of the unit that was selected (clicked)
'allies' are unit slots of all the allies on the battlefield (excluding the attacker)
'targets' are unit slots of all the targets on the battlefield on which the attack can be performed (for instance,
  if targets are allies and the attack is Revive, then it will only include dead allies that can be revived)
'targetsAreAllies' specified whether targets are allies
--]]
function getTargets(attacker, selected, allies, targets, targetsAreAllies)
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
