# Lua API for D2 modding toolset

### Overview
All scripts are expected to be written using [Lua](https://www.lua.org/) language [v5.4.1](https://www.lua.org/ftp/lua-5.4.1.tar.gz) and should be placed in Scripts folder.
Scripts folder itself should be placed in the game folder.

### Currently used scripts and their meanings:
- settings.lua - mss32 proxy dll settings that changes game rules
- doppelganger.lua - logic that computes level of doppelganger transform (category L\_DOPPELGANGER)
- transformSelf.lua - computes unit level for self transform attacks (category L\_TRANSFORM\_SELF)
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

### API reference

#### Standalone functions
```lua
-- Writes message to 'luaDebug.log' file when debugHooks is set to true
log('Unit current level:' .. unit.impl.level)
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
##### Terrain
```lua
Terrain = { Human, Dwarf, Heretic, Undead, Neutral, Elf }
```

##### Ground
```lua
Ground = { Plain, Forest, Water, Mountain }
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

##### ItemCategory
```
ItemCategory = { Armor, Jewel, Weapon, Banner, PotionBoost, PotionHeal, PotionRevive,
           PotionPermanent, Scroll, Wand, Valuable, Orb, Talisman, TravelItem, Special }
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
```

---

#### Unit
Represents game unit that participates in a battle, takes damage and performs attacks.

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
```

---

#### Leader
Represents leader unit. Leaders are main units in stacks. Leader has all methods of [units](luaApi.md#unit).

Methods:
##### type
Returns leader [type](luaApi.md#leader).
```lua
leader.type
```
##### hasAbility
Returns true if leader has specified [ability](luaApi.md#ability).
```lua
leader:hasAbility(Ability.TalismanUse)
```
##### hasMoveBonus
Returns true if leader has movement bonus on specified [ground](luaApi.md#ground).
```lua
leader:hasMoveBonus(Ground.Water)
```
```lua
-- Returns leader maximum movement points.
leader.movement
-- Returns leader scouting range.
leader.scout
-- Returns current leadership value.
leader.leadership
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
--- Returns primary attack or nil if no primary attack used.
impl.attack1
--- Returns secondary attack or nil if no secondary attack used.
impl.attack2
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

---

#### Stack
Represents [group](luaApi.md#group) of 6 [units](luaApi.md#unit-slot) on a map. One of the units is a [leader](luaApi.md#leader-1).

Methods:
##### group
Returns stack units as a [group](luaApi.md#group).
```lua
stack.group
```
##### leader
Returns stack [leader](luaApi.md#leader-1).
```lua
stack.leader
```
##### subrace
Returns stack [subrace](luaApi.md#subrace).
```lua
stack.subrace
```
```lua
--- Returns stack current movement points.
stack.movement
--- Returns true if stack is inside a city.
stack.inside
--- Returns true if stack is invisible.
stack.invisible
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
Searches for [Stack](luaApi.md#stack) by id string, [Id](luaApi.md#id), pair of coordinates or [Point](luaApi.md#point), returns nil if not found.
```lua
local stack = scenario:getStack(10, 15)
if (stack == nil) then
    return
end
```
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
Represents attack of [Unit Implementation](luaApi.md#unit-implementation).

Methods:
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
```

---

#### Currency
Represents game currency, mana and gold united.

Methods:
##### infernalMana
Returns amount of infernal mana.
```lua
currency.infernalMana
```
##### lifeMana
Returns amount of life mana.
```lua
currency.lifeMana
```
##### deathMana
Returns amount of death mana.
```lua
currency.deathMana
```
##### runicMana
Returns amount of runic mana.
```lua
currency.runicMana
```
##### groveMana
Returns amount of grove mana.
```lua
currency.groveMana
```
##### gold
Returns amount of gold.
```lua
currency.gold
```

---

#### ItemBase
Represents base item of any type (described in GItem.dbf).

Methods:
##### category
Returns item [category](luaApi.md#item-category).
```lua
base.category
```
##### name
Returns item name.
```lua
base.name
```
##### description
Returns item description.
```lua
base.description
```
##### value
Returns item [value](luaApi.md#currency).
```lua
base.value
```
##### unitImpl
Returns related [unit Implementation](luaApi.md#unit-implementation).
For instance: in case of "Angel Orb", Angel unit implementation is returned.
```lua
base.unitImpl
```

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
```lua
-- 'unit' has type Unit, 'transformImpl' is a Unit implementation
function getLevel(unit, transformImpl)
    -- Transform into current level or level of resulting unit's template, whichever is bigger.
    return math.max(unit.impl.level, transformImpl.level)
end
```

#### summon.lua
```lua
-- 'summoner' has type Unit, 'summonImpl' is a Unit implementation
function getLevel(summoner, summonImpl)
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
