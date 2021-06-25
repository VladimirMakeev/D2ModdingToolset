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
- getSelectedTargetAndOneBehind.lua - contains attack targeting logic for pierce attack reach
- getSelectedLineTargets.lua - contains attack targeting logic for wide-cleave attack reach
- getSelectedColumnTargets.lua - contains attack targeting logic for column attack reach
- getSelectedArea2x2Targets.lua - contains attack targeting logic for 2x2 area splash attack reach
- getSelectedTargetAndTwoChainedRandom.lua - contains attack targeting logic for random chain attack reach
- getWoundedFemaleGreenskinTargets.lua - contains targeting logic that only allows to reach wounded female greenskins

### API reference

#### Standalone functions
```lua
-- Writes message to 'luaDebug.log' file when debugHooks is set to true
log('Unit current level:' .. unit.impl.level)
```

#### Enumerations
```lua
Race = { Human, Undead, Heretic, Dwarf, Neutral, Elf }
```
```lua
Subrace = { Custom, Human, Undead, Heretic, Dwarf, Neutral, NeutralHuman, NeutralElf, NeutralGreenSkin,
            NeutralDragon, NeutralMarsh, NeutralWater, NeutralBarbarian, NeutralWolf, Elf }
```

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
```

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

#### Dynamic upgrade
Represents rules that applied when unit makes its progress gaining levels. Records in GDynUpgr.dbf are dynamic upgrades.

Methods:
```lua
-- Returns number of experience points added with each dynamic upgrade. XP_NEXT value from GDynUpgr.dbf.
dynUpgrade.xpNext
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
