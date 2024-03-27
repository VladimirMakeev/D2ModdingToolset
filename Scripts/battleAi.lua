-- Returns true if table 't' contains specified value
function hasValue(t, value)
    for index, v in ipairs(t) do
        if v == value then
            return true
        end
    end
    
    return false
end

-- Returns true if unit is alive
function isUnitAlive(unit)
    return unit.hp > 0
end

-- Returns true if unit primary attack inflicts damage
function isUnitAttackInflictsDamage(unit)
    if not isUnitAlive(unit) then
        return false
    end
    
    local impl = unit.impl
    local attack = impl.attack1
    local attackClass = attack.type
    
    return attackClass == Attack.Damage
        or attackClass == Attack.Drain
        or attackClass == Attack.DrainOverflow
        or attackClass == Attack.Poison
        or attackClass == Attack.Blister
        or attackClass == Attack.Frostbite
end

-- Returns true if at least one unit from 'other group' is not always immune to attacks of units in 'group'
function groupCanInflictDamageToOther(group, otherGroup)
    local otherGroupUnits = otherGroup.units
    if #otherGroupUnits <= 0 then
        return true
    end
    
    local groupUnits = group.units    
    if #groupUnits <= 0 then
        return false
    end

    for i = 1, #groupUnits do
        local unit = groupUnits[i]
        if isUnitAttackInflictsDamage(unit) then
            local impl = unit.impl
            local attack = impl.attack1
            local attackClass = attack.type
            local attackSource = attack.source
            
            for j = 1, #otherGroupUnits do
                local otherUnit = otherGroupUnits[j]
                local otherUnitImpl = otherUnit.impl
                
                if otherUnitImpl:getImmuneToAttackClass(attackClass) ~= Immune.Always
                and otherUnitImpl:getImmuneToAttackSource(attackSource) ~= Immune.Always
                then
                    return true
                end
            end
        end
    end
    
    return false
end

function getCityProtection(group, ignoreCityProtection)
    local groupId = group.id
    local groupType = group.id.type
    local fort = nil

    if groupType == IdType.Stack then
        fort = getScenario():getStack(groupId).inside
    elseif groupType == IdType.Fortification then
        fort = getScenario():getFort(groupId)
    end
    
    if fort == nil or ignoreCityProtection then
        return 0
    end
    
    if fort.capital then
        return getGlobal().variables.protCap
    end
    
    return getGlobal().variables:prot(fort.tier)
end

function computeImmuneCoefficient(impl)
    local coeff = 0.0
    
    if impl:getImmuneToAttackSource(Source.Weapon) ~= Immune.NotImmune then
        coeff = coeff + 57.0
    end
    
    if impl:getImmuneToAttackSource(Source.Mind) ~= Immune.NotImmune then
        coeff = coeff + 5.0
    end
    
    if impl:getImmuneToAttackSource(Source.Life) ~= Immune.NotImmune then
        coeff = coeff + 6.0
    end
    
    if impl:getImmuneToAttackSource(Source.Death) ~= Immune.NotImmune then
        coeff = coeff + 10.0
    end
    
    if impl:getImmuneToAttackSource(Source.Fire) ~= Immune.NotImmune then
        coeff = coeff + 9.0
    end
    
    if impl:getImmuneToAttackSource(Source.Water) ~= Immune.NotImmune then
        coeff = coeff + 2.0
    end
    
    if impl:getImmuneToAttackSource(Source.Air) ~= Immune.NotImmune then
        coeff = coeff + 5.0
    end
    
    if impl:getImmuneToAttackSource(Source.Earth) ~= Immune.NotImmune then
        coeff = coeff + 3.0
    end
    
    if impl:getImmuneToAttackClass(Attack.Poison) ~= Immune.NotImmune then
        coeff = coeff + 5.0
    end
    
    return coeff
end

function computePowerCoefficient(impl)
    local attack = impl.attack1
    local attackClass = attack.type
    
    if attackClass == Attack.Heal
    or attackClass == Attack.BoostDamage
    or attackClass == Attack.Cure
    or attackClass == Attack.Summon
    or attackClass == Attack.GiveAttack
    or attackClass == Attack.Doppelganger
    then
        return 100.0
    end
    
    return attack.power
end

function computeAttackClassCoefficient(impl, ignoreCityProtection)
    local attack = impl.attack1
    local attackClass = attack.type
    
    if attackClass == Attack.Paralyze or attackClass == Attack.Petrify then
        return 30.0
    end
    
    if attackClass == Attack.Damage then
        return 1.0
    end
    
    if attackClass == Attack.Drain then
        return 1.5
    end
    
    if attackClass == Attack.Heal then
        return 1.0
    end
    
    if attackClass == Attack.Fear then
        return 30.0
    end
    
    if attackClass == Attack.BoostDamage or attackClass == Attack.BestowWards then
        return 40.0
    end
    
    if attackClass == Attack.Shatter then
        return 30.0
    end
    
    if attackClass == Attack.LowerDamage or attackClass == Attack.LowerInitiative then
        return 40.0
    end
    
    if attackClass == Attack.DrainOverflow then
        return 2.0
    end
    
    if attackClass == Attack.Summon then
        return 200.0
    end
    
    if attackClass == Attack.DrainLevel then
        return 100.0
    end
    
    if attackClass == Attack.GiveAttack then
        return 50.0
    end
    
    if attackClass == Attack.Doppelganger then
        if ignoreCityProtection == false then
            return 200.0
        end
        
        return 100.0
    end
    
    if attackClass == Attack.TransformSelf then
        return 100.0
    end
    
    if attackClass ~= Attack.TransformOther then
        return 1.0
    end
    
    if attack.reach == Reach.All then
        return 100.0
    end
    
    return 60.0
end

function computeAttackReachCoefficient(impl, enemiesCount)
    local attack = impl.attack1
    local reach = attack.reach
    
    if reach == Reach.Adjacent then
        return 1.0
    end
    
    if reach == Reach.Any then
        return 1.5
    end
    
    if reach ~= Reach.All then
        return 1.0
    end
    
    return (enemiesCount - 1) * 0.4 + 1.0
end

function getMaxDamage(unitOrItemId, impl)
    local t = unitOrItemId.type
    
    if t == IdType.UnitGlobal or t == IdType.UnitGenerated then
        local id
        if t == IdType.UnitGenerated then
            id = impl.global.id
        else
            id = impl.id
        end

        local idx = id.typeIndex
        
        -- Check IDs of leaders who can have 'Heavy Strike' ability
        if idx == 0x19
        or idx == 0x20
        or idx == 0x44
        or idx == 0x45
        or idx == 0x47
        or idx == 0x70
        or idx == 0x71
        or idx == 0x96
        or idx == 0x8009
        or idx == 0x8011
        then
            return getGame().unitMaxDamage + getGame().leaderAdditionalDamage
        end
    end
    
    return getGame().unitMaxDamage
end

function getUnitOrItemMaxDamage(unitOrItemId)
    if unitOrItemId.type ~= IdType.Unit then
        return getMaxDamage(unitOrItemId, nil)
    end
    
    local unit = getScenario():getUnit(unitOrItemId)
    local impl = unit.impl
    return getMaxDamage(impl.id, impl)
end

function computeDamageDrainHealCoefficient(unit, impl)
    local attack = impl.attack1
    local attackClass = attack.type
    
    if attackClass == Attack.Damage
    or attackClass == Attack.Drain
    or attackClass == Attack.DrainOverflow
    then
        local damage = attack.damage
        local maxDamage = getMaxDamage(unit.impl.id, unit.impl)

        return math.min(damage, maxDamage)
    elseif attackClass == Attack.Heal then
        return attack.heal
    else
        return 1.0
    end
end

function computeSecondAttackCoefficient(impl)
    local attack = impl.attack2
    if attack == nil then
        return 1.0
    end
    
    local attackClass = attack.type
    
    if attackClass == Attack.Cure then
        return 1.0
    end
    
    if attackClass == Attack.Poison
    or attackClass == Attack.Frostbite
    or attackClass == Attack.Blister
    then
        return 1.15
    end
    
    if attackClass == Attack.Revive then
        return 3.0
    end
    
    return 1.0
end

function computeUnitCoefficient(unit, ignoreCityProtection, battle, cityArmor, enemiesCount, a6)
    if isUnitAlive(unit) == false and a6 == false then
        return 0.0
    end
    
    local impl
    if battle ~= nil and battle:getUnitStatus(unit.id, BattleStatus.Transform) then
        impl = unit.original.impl
    else
        impl = unit.impl
    end
    
    local attackInfinite = impl.attack1.infinite
    local hasSecondAttack = impl.attack2 ~= nil
    
    local unitHp
    if a6 then
        unitHp = unit.hpMax
    else
        unitHp = unit.hp
    end
    
    local armor = impl.armor
    local unitArmor = armor + cityArmor
    
    local immuneCoeff = computeImmuneCoefficient(impl)
    local powerCoeff = computePowerCoefficient(impl)
    local attackClassCoeff = computeAttackClassCoefficient(impl, ignoreCityProtection)
    local attackReachCoeff = computeAttackReachCoefficient(impl, enemiesCount)
    local dmgDrainHealCoeff = computeDamageDrainHealCoefficient(unit, impl)
    local unitMaxArmor = getGame().unitMaxArmor
    
    if unitArmor > unitMaxArmor then
        unitArmor = unitMaxArmor
    end

    local v17 = unitHp / (60.0 - 60.0 * unitArmor * 0.01)

    local coeff = v17 / (unitHp / 60.0)
                * (powerCoeff
                    * 0.01
                    * dmgDrainHealCoeff
                    * attackReachCoeff
                    * attackClassCoeff
                    * ((immuneCoeff * 0.01 + 1.0) * unitHp))
                * 0.01;

    if attackInfinite then
        coeff = coeff * 1.25
    elseif hasSecondAttack then
        coeff = computeSecondAttackCoefficient(impl) * coeff
    end

    if impl.attacksTwice then
        coeff = coeff * 2.0
    end

    return coeff
end

function computeGroupCoefficient(group, ignoreCityProtection, enemiesCount, battle, ignoreCityProtection2, a8)
    local cityArmor = 0
    if ignoreCityProtection2 == false then
        cityArmor = getCityProtection(group, ignoreCityProtection)
    end

    local coeff = 0.0
    local units = group.units
    for i = 1, #units do
        local unit = units[i]
        if isUnitAlive(unit) then
            coeff = coeff + computeUnitCoefficient(unit, ignoreCityProtection, battle, cityArmor, enemiesCount, a8)
        end
    end

    return coeff
end

function sub_5DEE03(groupSubrace, otherGroup, ignoreCityProtection, position, enemiesCount, ignoreCityProtection2)
    local totalCoeff = 0.0
    local biggestStackCoeff = 0.0
    local biggestStackCoeffId = nil
    
    -- Check stacks on tiles in -10 .. +10 square around position
    for x = position.x - 10, position.x + 10, 1 do
        for y = position.y - 10, position.y + 10, 1 do
            local stack = getScenario():getStack(x, y)
            if stack ~= nil and groupSubrace ~= stack.subrace and stack.order ~= Order.Stand then
                local stackCoeff = computeGroupCoefficient(stack.group, ignoreCityProtection, enemiesCount, nil, ignoreCityProtection2, false)
                totalCoeff = totalCoeff + stackCoeff

                if biggestStackCoeff < stackCoeff then
                    biggestStackCoeff = stackCoeff
                    biggestStackCoeffId = stack.id
                end
            end
        end
    end
    
    if biggestStackCoeffId ~= nil and biggestStackCoeffId == otherGroup.id then
        return 0.0
    end
    
    return totalCoeff
end

function sub_5DECF0(group, groupCoeff, otherGroup, otherGroupCoeff, nearbyStacksCoeff)
    if not groupCanInflictDamageToOther(otherGroup, group) then
        return 0.0
    end

    local v8 = 0.0
    if groupCoeff >= otherGroupCoeff * 0.5 then
        v8 = groupCoeff + nearbyStacksCoeff
    else
        if groupCoeff >= nearbyStacksCoeff then
            v8 = groupCoeff
        else
            v8 = nearbyStacksCoeff
        end
    end
    
    local coeff = otherGroupCoeff / v8 * 0.5
    
    if coeff >= 1.0 then
        return 1.0
    end
    
    return coeff
end

function sub_5DED63(group, otherGroup, battle, position, countNearbyStacks, ignoreCityProtection)
    local unitsCount = #group.units
    local otherGroupUnitsCount = #otherGroup.units
    
    local groupCoeff = computeGroupCoefficient(group, true, otherGroupUnitsCount, battle, true, false)
    local otherGroupCoeff = computeGroupCoefficient(otherGroup, false, unitsCount, battle, ignoreCityProtection, false)

    local nearbyStacksCoeff = 0.0
    if countNearbyStacks == true then
        nearbyStacksCoeff = sub_5DEE03(group.subrace, otherGroup, false, position, unitsCount, ignoreCityProtection)
    end
    
    return sub_5DECF0(otherGroup, otherGroupCoeff, group, groupCoeff, nearbyStacksCoeff)
end

-- Returns distance between two points, length of a line segment connecting two points.
function distance(a, b)
    local x = b.x - a.x
    local y = b.y - a.y
    return math.sqrt(x * x + y * y)
end

function computeStackAndFortRelativeCoefficient(stack, enemyFort, battle, ignoreCityProtection)
    if groupCanInflictDamageToOther(stack.group, enemyFort.group) == false then
        return 0.0
    end
    
    local enemyCoeff = 0.0
    local stackUnitsCount = #stack.group.units
    local enemyStackUnitsCount = 0
    
    local enemyFortVisitor = enemyFort.visitor
    if enemyFortVisitor ~= nil then
        local enemyStackGroup = enemyFortVisitor.group
        enemyCoeff = computeGroupCoefficient(enemyStackGroup, false, stackUnitsCount, battle, ignoreCityProtection, false)
        enemyStackUnitsCount = #enemyStackGroup.units
    end
    
    enemyCoeff = enemyCoeff + computeGroupCoefficient(enemyFort.group, false, stackUnitsCount, battle, ignoreCityProtection, false)
    
    if enemyStackUnitsCount == 0 then
        enemyStackUnitsCount = #enemyFort.group.units
    end
    
    local stackCoeff = computeGroupCoefficient(stack.group, true, enemyStackUnitsCount, battle, true, false)
    local stackPosition = stack.position
    
    local enemyFortEntrance = enemyFort.entrance
    -- How is this possible, if stack is attacking the fort it means it stands next to its entrance
    -- This is original game logic
    if distance(stackPosition, enemyFortEntrance) > 10.0 then
        -- Accumulate enemy coefficient from nearby stacks
        -- that are not inside cities and with non-Stand orders
        getScenario():forEachStack(function (currStack)
            if currStack.subrace ~= enemyFort.subrace then
                return
            end
            
            if currStack.order == Order.Stand then
                return
            end
            
            if currStack.inside then
                return
            end
            
            if distance(currStack.position, enemyFortEntrance) >= 8.0 then
                return
            end
            
            local c = computeGroupCoefficient(currStack.group, false, stackUnitsCount, battle, false, false)
            enemyCoeff = enemyCoeff + c            
        end)
    end
    
    local relativeCoeff = stackCoeff / enemyCoeff * 0.5
    if relativeCoeff >= 1.0 then
        return 1.0
    end
    
    return relativeCoeff
end

function computeGroupsRelativeCoefficient(battle, activeUnitGroup, enemyGroup)
    -- Check if activeUnitGroup is not stack or enemyGroup is not fort
    if activeUnitGroup.id.type ~= IdType.Stack or enemyGroup.id.type ~= IdType.Fortification then
        local pos = Point.new(-1, -1)
        return sub_5DED63(activeUnitGroup, enemyGroup, battle, pos, false, true)
    else
        -- Enemy group is a fort
        local enemyFort = getScenario():getFort(enemyGroup.id)
        -- Active unit group is a stack
        local activeStack = getScenario():getStack(activeUnitGroup.id)
        return computeStackAndFortRelativeCoefficient(activeStack, enemyFort, battle, true)
    end
end

function isStackHaveMovement(group)
    if group.id.type == IdType.Stack then
        return getScenario():getStack(group.id).movement > 0
    end
    
    return false
end

-- Returns true if specified race is unplayable (can't be controlled by a human player)
function isRaceUnplayable(race)
    return race ~= Race.Human
        and race ~= Race.Heretic
        and race ~= Race.Undead
        and race ~= Race.Dwarf
        and race ~= Race.Elf
end

function checkGroupCanRetreat(possibleActions, activeUnit, activeUnitGroup, battle, ignoreUnplayableRaces, ignoreHumanPlayers)
    if not hasValue(possibleActions, BattleAction.Retreat) then
        -- Retreat is not a possible action. Group can't retreat
        return false
    end

    -- Don't retreat if:
    -- battle is over and healers have their last turn
    -- battle has not started yet
    if battle.afterBattle or battle.currentRound < 1 then
        return false
    end
    
    local activePlayer = nil
    if battle:isUnitAttacker(activeUnit) then
        activePlayer = battle.attackerPlayer
    else
        activePlayer = battle.defenderPlayer
    end
    
    if not ignoreHumanPlayers then
        if activePlayer.isHuman then
            return false
        end
    end
    
    if not ignoreUnplayableRaces then
        if isRaceUnplayable(activePlayer.race) then
            return false
        end
    end
    
    assert(activeUnitGroup.id.type == IdType.Stack)
    
    local stack = nil
    if activeUnitGroup.id.type == IdType.Stack then
        stack = getScenario():getStack(activeUnitGroup.id)
        
        local leader = stack.leader
        local unitType = leader.impl.type
        if unitType == Unit.Summon or unitType == Unit.Noble then
            -- Summons and thieves (nobles) never retreat from battle
            return false
        end
    end

    local order = stack.order

    local enemyGroup = nil
    if not battle:isUnitAttacker(activeUnit) then
        enemyGroup = battle.attacker.group
    else
        enemyGroup = battle.defender
    end

    -- Allow retreat if:
    -- group order is Normal or Roam
    -- group order is Attack and we are not fighting with its target group
    -- group defending and it's AI order is not Assist
    if order == Order.Normal
    or order == Order.Roam
    or order == Order.Attack
    and stack.orderTargetId ~= enemyGroup.id
    then
        if not battle:isUnitAttacker(activeUnit)
        or stack.aiOrder ~= Order.Assist
        then
            return true
        end
    end
    
    return false
end

function isAttackClassCanInflictDamage(attackClass)
    return attackClass == Attack.Damage
        or attackClass == Attack.Drain
        or attackClass == Attack.DrainOverflow
        or attackClass == Attack.Summon
        or attackClass == Attack.Poison
        or attackClass == Attack.Blister
        or attackClass == Attack.Frostbite
end

function getTargetGroupByAttackClass(attackClass, battle, allyGroup)
    -- Check if attack class targets enemy group or allies
    if attackClass == Attack.Paralyze
    or attackClass == Attack.Petrify
    or attackClass == Attack.Damage
    or attackClass == Attack.Drain
    or attackClass == Attack.Fear
    or attackClass == Attack.LowerDamage
    or attackClass == Attack.LowerInitiative
    or attackClass == Attack.Poison
    or attackClass == Attack.Frostbite
    or attackClass == Attack.Blister
    or attackClass == Attack.Shatter
    or attackClass == Attack.DrainOverflow
    or attackClass == Attack.DrainLevel
    or attackClass == Attack.TransformOther
    then
        -- Attack targets enemy group
        local attacker = battle.attacker.group
        if attacker.id == allyGroup.id then
            -- Attacker is an ally, so enemy is defender
            return battle.defender
        end
        
        -- Enemy is attacker
        return attacker
    end
    
    -- Attack targets allied group
    return allyGroup
end

-- Returns true if units in target group are always immune to specified attack class and damage source
function isTargetGroupAlwaysImmuneToAttackClassAndSource(attackClass, attackSource, allyGroup, attackTargetGroup, attackTargets)
    if allyGroup.id == attackTargetGroup.id then
        -- Game expects we can always apply attacks on units from allied group
        return false
    end
    
    local slots = attackTargetGroup.slots
    for i = 1, #attackTargets do
        local target = attackTargets[i]
        if target < 0 then
            goto continue
        end

        -- +1 because of lua 1-based indexing
        local slot = slots[target + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        local impl = unit.impl
        
        local immuneToSource = impl:getImmuneToAttackSource(attackSource) == Immune.Always
        local immuneToClass = impl:getImmuneToAttackClass(attackClass) == Immune.Always
        if not immuneToSource and not immuneToClass then
            return false
        end

        ::continue::
    end
    
    return true
end

-- Returns 
-- true/false indicating whether unit can attack or not
-- possible attack targets for specified unit in battle (if unit can attack)
function getUnitBattleAttackTargets(battle, unit)
    if battle:getUnitStatus(unit.id, BattleStatus.Retreated)
    or battle:getUnitStatus(unit.id, BattleStatus.Retreat)
    then
        return false, nil
    end

    local possibleActions
        , attackTargetGroup
        , attackPossibleTargets
        , item1TargetGroup
        , item1PossibleTargets
        , item2TargetGroup
        , item2PossibleTargets = battle:getUnitActions(unit)

    if hasValue(possibleActions, BattleAction.UseItem) then
        -- Unit can use items, thats considered enough
        return true, attackPossibleTargets
    end
    
    if not hasValue(possibleActions, BattleAction.Attack) then
        -- Unit can not attack
        return false, nil
    end
    
    local impl = unit.impl
    local attack = impl.attack1
    local attackClass = attack.type
    local attackSource = attack.source
    
    local allyGroup = nil
    if battle:isUnitAttacker(unit) then
        allyGroup = battle.attacker.group
    else
        allyGroup = battle.defender
    end
    
    local targetGroup = getTargetGroupByAttackClass(attackClass, battle, allyGroup)
    
    if not isTargetGroupAlwaysImmuneToAttackClassAndSource(attackClass, attackSource, allyGroup, targetGroup, attackPossibleTargets) then
        return true, attackPossibleTargets
    end
    
    return false, nil
end

function groupCanPerformOffenceBattleActions(battle, group, enemyGroup)
    local units = group.units
    local unitCount = #units
    if unitCount <= 0 then
        return false
    end

    for i = 1, #units do
        local unit = units[i]
        if isUnitAlive(unit) then
            local impl = unit.impl
            local attack = impl.attack1
            local attackClass = attack.type
            
            if isAttackClassCanInflictDamage(attackClass) then
                local canAttack, attackTargets = getUnitBattleAttackTargets(battle, unit)
                if canAttack then
                    if attackClass ~= Attack.Summon then
                        local attackSource = attack.source
                        local enemySlots = enemyGroup.slots
                        local enemyIsNotImmune = false
                        
                        for j = 1, #attackTargets do
                            local targetPosition = attackTargets[j]
                            -- +1 because of lua 1-based indexing
                            local slot = enemySlots[targetPosition + 1]
                            local enemyUnit = slot.unit
                            local enemyUnitImpl = enemyUnit.impl
                            
                            if enemyUnitImpl:getImmuneToAttackClass(attackClass) ~= Immune.Always
                            and enemyUnitImpl:getImmuneToAttackSource(attackSource) ~= Immune.Always
                            then
                                enemyIsNotImmune = true
                                break
                            end
                        end
                        
                        if enemyIsNotImmune then
                            return true
                        end
                    end
                end
            end
        end
    end
    
    return false
end

-- Returns unit impl with respect to transform
function getOriginalUnitImpl(unit)
    local original = unit.original
    if original ~= nil then
        return original.impl
    end
    
    return unit.impl
end

-- Returns true if unit is a leader
function isLeader(unit)
    return getOriginalUnitImpl(unit).leaderType ~= -1
end

function isGroupHasLessThanTwoUnitsAlive(battle, group, checkNonBattlingUnits)
    local units = group.units
    if #units <= 0 then
        return true
    end
    
    local aliveCount = 0
    
    for i = 1, #units do
        local unit = units[i]
        if checkNonBattlingUnits == false
        and (battle:getUnitStatus(unit.id, BattleStatus.Retreated)
            or battle:getUnitStatus(unit.id, BattleStatus.Hidden))
        then
            -- Skip unit
        else
            if isUnitAlive(unit) then
                aliveCount = aliveCount + 1
            end
        end
        
        if aliveCount >= 2 then
            return false
        end
    end
    
    return true
end

-- Returns BattleAction
function retreatGroupUnitsFromBattle(unit, group, battle)
    -- If unit at back lane or it's a leader, retreat
    if (group:getUnitPosition(unit) % 2 == 1) or isLeader(unit) then
        return BattleAction.Retreat
    end
    
    -- Check alive defenders at front lane positions
    local slots = group.slots
    local defenders = false    
    for i = 1, #slots do
        local slot = slots[i]
        if slot.frontline then
            local groupUnit = slot.unit
            if groupUnit ~= nil then
                if battle:getUnitStatus(groupUnit.id, BattleStatus.Defend)
                and battle:getUnitStatus(groupUnit.id, BattleStatus.Dead) == false
                then
                    defenders = true
                    break
                end
            end
        end
    end
    
    if defenders == false then
        -- No alive and defending units found in the front lane.
        -- Check if group have less than 2 alive units
        if isGroupHasLessThanTwoUnitsAlive(battle, group, false) then
            return BattleAction.Auto
        else
            return BattleAction.Defend
        end
    end
    
    return BattleAction.Retreat
end

-- Must return result (true or false), battleAction, target unit id, attacker unit id
function checkGroupShouldRetreat(battle, possibleActions, activeUnit, activeUnitGroup, enemyGroup, activeUnitIsAttacker, relativeCoeff)
    if battle.currentRound < 1 then
        return false, BattleAction.Retreat, Id.emptyId(), Id.emptyId()
    end

    if battle:getRetreatStatus(activeUnitIsAttacker) == Retreat.NoRetreat then
        if battle.decidedToRetreat == true and activeUnitIsAttacker == false then
            if relativeCoeff < 0.3
            and isStackHaveMovement(enemyGroup) == false
            and checkGroupCanRetreat(possibleActions, activeUnit, activeUnitGroup, battle, false, false) == true
            then
                battle:setRetreatStatus(false, Retreat.CoverAndRetreat)
            end
            
            battle:setDecidedToRetreat()
        end
        
        if battle:getRetreatStatus(activeUnitIsAttacker) == Retreat.NoRetreat
        and checkGroupCanRetreat(possibleActions, activeUnit, activeUnitGroup, battle, true, true) == true
        and groupCanPerformOffenceBattleActions(battle, activeUnitGroup, enemyGroup) == false
        then
            battle:setRetreatStatus(activeUnitIsAttacker, Retreat.FullRetreat)
        end
    end
    
    if battle:getRetreatStatus(activeUnitIsAttacker) == Retreat.FullRetreat then
        return true, BattleAction.Retreat, activeUnit.id, activeUnit.id
    end
    
    if battle:getRetreatStatus(activeUnitIsAttacker) == Retreat.CoverAndRetreat then
        local action = retreatGroupUnitsFromBattle(activeUnit, activeUnitGroup, battle)
        
        return true, action, activeUnit.id, activeUnit.id
    end
    
    return false, BattleAction.Retreat, Id.emptyId(), Id.emptyId()
end

function computePercentage(percentage, value)
    return percentage * value / 100
end

function computeDamageWithBuffs(attack, maxDamage, battle, unit, addRandomDamage, easyDifficulty)
    local attackDamage = attack.damage
    if attackDamage > maxDamage then
        attackDamage = maxDamage
    end
    
    local variables = getGlobal().variables

    local randomAdditionalDamage = 0
    if addRandomDamage then
        local batDamage = variables.batDamage
        
        if easyDifficulty then
            batDamage = batDamage * 2
        end
        
        randomAdditionalDamage = randomNumber(batDamage)
    end
    
    local damage = attackDamage + randomAdditionalDamage
    
    if battle ~= nil then
        if battle:getUnitStatus(unit.id, BattleStatus.BoostDamageLvl1) then
            damage = damage + computePercentage(variables:batBoostd(1), attackDamage)
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.BoostDamageLvl2) then
            damage = damage + computePercentage(variables:batBoostd(2), attackDamage)
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.BoostDamageLvl3) then
            damage = damage + computePercentage(variables:batBoostd(3), attackDamage)
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.BoostDamageLvl4) then
            damage = damage + computePercentage(variables:batBoostd(4), attackDamage)
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.LowerDamageLvl1) then
            damage = damage - computePercentage(variables:batLowerd(1), attackDamage)
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.LowerDamageLvl2) then
            damage = damage - computePercentage(variables:batLowerd(2), attackDamage)
        end
    end
    
    if damage < getGame().unitMinDamage then
        return getGame().unitMinDamage
    end
    
    if damage > maxDamage then
        damage = maxDamage
    end
    
    return damage
end

function attackGetDamageWithBuffs(attack, maxDamage, battle, unit)
    if not attack then
        return 0
    end

    local attackClass = attack.type
    
    if attackClass == Attack.Damage
    or attackClass == Attack.Drain
    or attackClass == Attack.DrainOverflow
    or attackClass == Attack.Poison
    or attackClass == Attack.Blister
    or attackClass == Attack.Frostbite
    then
        -- Attack inflicts damage, compute damage with buffs
        return computeDamageWithBuffs(attack, maxDamage, battle, unit, false, false)
    end
    
    -- Attack does not inflict damage
    return 0
end

function attackGetDamageWithBuffsCheckTransform(unit, battle)
    local impl = unit.impl
    local unitMaxDamage = getMaxDamage(impl.id, impl)

    local attack = impl.attack1
    if attack.type == Attack.TransformSelf then
        return attackGetDamageWithBuffs(impl.altAttack, unitMaxDamage, battle, unit)
    end
    
    local attackDamage = attackGetDamageWithBuffs(attack, unitMaxDamage, battle, unit)
    return attackGetDamageWithBuffs(impl.attack2, unitMaxDamage, battle, unit) + attackDamage
end

function isDamagingAttack(attack)
    if not attack then
        return false
    end
    
    local attackClass = attack.type
    return attackClass == Attack.Damage
        or attackClass == Attack.Drain
        or attackClass == Attack.DrainOverflow
        or attackClass == Attack.Poison
        or attackClass == Attack.Blister
        or attackClass == Attack.Frostbite
end

function getSoldierAttackSource(impl)
    local attack = impl.attack1
    if isDamagingAttack(attack) then
        return attack.source
    end
    
    local attack2 = impl.attack2
    if isDamagingAttack(attack2) then
        return attack2.source
    end
    
    local altAttack = impl.altAttack
    if altAttack ~= nil and isDamagingAttack(altAttack) then
        return altAttack.source
    end
    
    -- Unknown soldier attack source
    return -1
end

function computeArmor(unit, battle)
    local impl = unit.impl
    local armor = impl.armor
    local shattered = battle:getUnitShatteredArmor(unit)
    local fortArmor = battle:getUnitFortificationArmor(unit)
    
    armor = armor - shattered
    if armor < fortArmor then
        armor = fortArmor
    end
    
    if battle:getUnitStatus(unit.id, BattleStatus.Defend) then
        local v1 = 100 - armor
        
        local v2 = getGlobal().variables.defendBonus * 0.01 * v1
        
        local v3 = armor + v2
        
        if v3 > getGame().unitMaxArmor then
            v3 = getGame().unitMaxArmor
        end
        
        armor = v3
    end
    
    return armor
end

function computeEffectiveHp(unit, battle)
    if not isUnitAlive(unit) then
        return false
    end
    
    local armor = computeArmor(unit, battle)
    local hp = unit.hp
    
    -- Vanilla formula for effective hp
    return hp * armor / 100 + hp
end

function computeTargetUnitAiPriority(unit, battle, damageWithBuffs)
    local impl = unit.impl
    local bigUnit = impl.small == false
    local attack = impl.attack1
    local attackClass = attack.type
    
    local attack2 = impl.attack2

    if bigUnit or attack.reach == Reach.Adjacent or attackClass == Attack.BoostDamage then
        local effectiveHp = computeEffectiveHp(unit, battle)
        if effectiveHp > damageWithBuffs then
            return 10000 - effectiveHp
        else
            return 10000 + effectiveHp
        end
    end

    local unitValue = impl.xpKilled
    if attackClass == Attack.Heal or (attack2 ~= nil and attack2.type == Attack.Heal) then
        return 10000 + unitValue * 2
    end
    
    if attackClass == Attack.Paralyze or (attack2 ~= nil and attack2.type == Attack.Paralyze)
    or attackClass == Attack.Petrify or (attack2 ~= nil and attack2.type == Attack.Petrify)
    then
        return 10000 + unitValue * 8
    end
    
    if attackClass == Attack.Summon or (attack2 ~= nil and attack2.type == Attack.Summon) then
        return 10000 + unitValue * 10
    end
    
    if attackClass == Attack.TransformOther or (attack2 ~= nil and attack2.type == Attack.TransformOther) then
        return 10000 + unitValue * 9
    end
    
    if attackClass == Attack.GiveAttack or (attack2 ~= nil and attack2.type == Attack.GiveAttack) then
        return 10000 + unitValue * 3
    end
    
    return 10000 + unitValue
end

-- Returns unit that is selected as an attack target among possible targets or nil
function selectAttackTarget(battle, damageWithBuffs, attackTargetGroup, attackPossibleTargets, attackSource)
    local slots = attackTargetGroup.slots
    local maxTargetValue = 0
    local selectedUnit = nil
    
    for i = 1, #attackPossibleTargets do
        local attackTargetPosition = attackPossibleTargets[i]

        if attackTargetPosition < 0 then
            goto continue
        end

        -- +1 because of lua 1-based indexing
        local slot = slots[attackTargetPosition + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        local currentHp = unit.hp
        if not battle:getUnitStatus(unit.id, BattleStatus.Summon) and currentHp > 0 then
            if currentHp <= damageWithBuffs and computeEffectiveHp(unit, battle) <= damageWithBuffs then
                local impl = unit.impl
                local immune = impl:getImmuneToAttackSource(attackSource)
                local targetValue = computeTargetUnitAiPriority(unit, battle, damageWithBuffs)
                    
                if immune == Immune.Once
                and battle:isUnitResistantToSource(unit, attackSource) then
                    -- Target unit has resistance to attack source, reduce its value
                    targetValue = targetValue * 0.69999999
                end
                
                if targetValue > maxTargetValue and immune ~= Immune.Always then
                    maxTargetValue = targetValue
                    selectedUnit = unit
                end
            end
        end

        ::continue::
    end

    return selectedUnit
end

-- Leader can use items only if it has no other target, 'item use' action is permitted and its items can target someone
function canLeaderUseItemInBattle(selectedAttackTarget, possibleActions, item1PossibleTargets, item2PossibleTargets)
    if selectedAttackTarget ~= nil then
        return false
    end
    
    return hasValue(possibleActions, BattleAction.UseItem) and (#item1PossibleTargets or #item2PossibleTargets)
end

function sub_5D10F9(item)
    if not item then
        return 0
    end
    
    local base = item.base
    local itemType = base.type
    
    if itemType == Item.Talisman then
        return 4
    end
    
    if itemType == Item.Orb then
        return 3
    end
    
    if itemType == Item.PotionRevive then
        return 2
    end
    
    if itemType == Item.PotionHeal then
        return 1
    end
    
    return 0
end

function getLeaderArtifactAttack(unit, artifactIndex)
    local stack = getScenario():findStackByUnit(unit)
    if not stack then
        return nil
    end
    
    if stack.leader.id ~= unit.id then
        return nil
    end
    
    local artifact1 = stack:getEquippedItem(Equipment.Artifact1)
    local art1Attack = nil
    if artifact1 then
        art1Attack = artifact1.base.attack
    end
    
    local artifact2 = stack:getEquippedItem(Equipment.Artifact2)
    local art2Attack = nil
    if artifact2 then
        art2Attack = artifact2.base.attack
    end
    
    if art1Attack == nil then
        art1Attack = art2Attack
        art2Attack = nil
    end
    
    if artifactIndex == 1 then
        if art1Attack then
            return art1Attack
        end
        
        return art2Attack
    end

    if artifactIndex == 2 then
        return art2Attack
    end
    
    -- Wrong index
    return nil
end

-- Checks if attack is transform or doppelganger and returns their alternative attack
-- Otherwise returns attack itself
function getAttackWrToTransformDoppel(impl, first)
    local attack = nil
    if first then
        attack = impl.attack1
    else
        attack = impl.attack2
    end

    local attackClass = attack.type
    
    if attackClass == Attack.TransformSelf
    or attackClass == Attack.Doppelganger
    then
        if first then
            return impl.altAttack
        else
            return impl.altAttack2
        end
    end
    
    return attack
end

-- Returns unit attacks and their numbers
function getUnitAttacks(unitId, checkTransformedAttack)
    local attacks = {}
    
    local unit = getScenario():getUnit(unitId)
    local impl = unit.impl
    
    local attack = nil
    if checkTransformedAttack then
        attack = getAttackWrToTransformDoppel(impl, true)
    else
        attack = impl.attack1
    end
    
    table.insert(attacks, { attack, 0 })
    
    if impl.attack2 then
        local attack2 = nil
        
        if checkTransformedAttack then
            attack2 = getAttackWrToTransformDoppel(impl, false)
        else
            attack2 = impl.attack2
        end
        
        table.insert(attacks, { attack2, 1 })
    end
    
    local art1Attack = getLeaderArtifactAttack(unit, 1)
    if art1Attack ~= nil then
        table.insert(attacks, { art1Attack, 2 })
    end
    
    local art2Attack = getLeaderArtifactAttack(unit, 2)
    if art2Attack ~= nil then
        table.insert(attacks, { art2Attack, 2 })
    end
    
    return attacks
end

function getAttackById(id, attackNumber, checkTransformedAttack)
    if id.type == IdType.Unit then
        local attacks = getUnitAttacks(id, checkTransformedAttack)
        if attackNumber > #attacks then
            return nil
        end
        
        local pair = attacks[attackNumber]
        return pair[0]
    elseif id.type == IdType.Item then
        local item = getScenario():getItem(id)
        local itemBase = item.base
        local itemType = itemBase.type
        
        if itemType == Item.PotionBoost
        or itemType == Item.PotionHeal
        or itemType == Item.PotionRevive
        or itemType == Item.PotionPermanent
        then
            return nil
        end
        
        return itemBase.attack
    end
    
    return nil
end

function getAttackByIdAndCheckTransformed(id, attackNumber)
    return getAttackById(id, attackNumber, true)
end

function isUnitHasLessThanHalfHitPoints(unit)
    return unit.hp / unit.hpMax <= 0.5
end

function getAttackingItemTargets(attack, itemTargetGroup, itemPossibleTargets)
    local attackClass = attack.type
    
    if attackClass == Attack.Summon then
        -- Choose frontlane slots as summon item targets
        local targets = {}
        
        for i = 1, #itemPossibleTargets do
            if itemPossibleTargets[i] % 2 == 0 then
                table.insert(targets, itemPossibleTargets[i])
            end
        end
        
        return targets
    end
    
    if attackClass == Attack.Heal then
        -- Choose units that have less than 50% of hit points left
        local targets = {}
        
        local slots = itemTargetGroup.slots
        for i = 1, #itemPossibleTargets do
            local possibleTarget = itemPossibleTargets[i]
            -- +1 because of lua 1-based indexing
            local unit = slots[possibleTarget + 1]
            if isUnitHasLessThanHalfHitPoints(unit) then
                table.insert(targets, possibleTarget)
            end
        end
        
        return targets
    end
    
    return itemPossibleTargets
end

function pickRandomIfEqual(v1, v2)
    if v1 == v2 then
        return randomNumber(2) == 1
    end
    
    return false
end

-- Returns true/false, targetId
function findHealAttackTarget(battle, targetGroup, possibleTargets)
    local primaryHealRatio = 1.0
    local primaryTarget = nil
    
    local secondaryHealRatio = 1.0
    local secondaryTarget = nil
    
    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        
        if not unit then goto continue end
        
        local ratio = unit.hp / unit.hpMax
        
        if not (ratio < primaryHealRatio or pickRandomIfEqual(ratio, primaryHealRatio)) then
            goto continue
        end
        
        if battle:getUnitStatus(unit.id, BattleStatus.Retreat) then
            if secondaryTarget and battle:getUnitStatus(secondaryTarget.id, BattleStatus.Summon) then
                -- Prefer healing summoned units over retreating allies
                goto continue
            end
        elseif battle:getUnitStatus(unit.id, BattleStatus.Summon) then
            primaryTarget = unit
            primaryHealRatio = ratio
            goto continue
        end
        
        if ratio < secondaryHealRatio or pickRandomIfEqual(ratio, secondaryHealRatio) then
            secondaryTarget = unit
            secondaryHealRatio = ratio
        end
    
        ::continue::
    end
    
    local target = primaryTarget
    if not target then
        target = secondaryTarget
    end
    
    if not target then
        return false, nil
    end
    
    return true, target.id
end

-- Returns true/false, targetId
function findReviveAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local secondaryTarget = nil
    local maxRatio = 1.0
    local maxXpKilled = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        
        if not unit then goto continue end
        
        if isUnitAlive(unit) then
            if primaryTarget ~= nil then
                goto continue
            end
            
            local ratio = unit.hp / unit.hpMax
            if ratio >= maxRatio then
                if not pickRandomIfEqual(ratio, maxRatio) then
                    goto continue
                end
                
                maxRatio = ratio
                secondaryTarget = unit
            end
        else
            local impl = unit.impl
            local xpKilled = impl.xpKilled
            
            if xpKilled >= maxXpKilled or pickRandomIfEqual(xpKilled, maxXpKilled) then
                primaryTarget = unit
                maxXpKilled = xpKilled
            end
        end
        
        ::continue::
    end
    
    local target = primaryTarget
    if not target then
        target = secondaryTarget
    end
    
    if not target then
        return false, nil
    end
    
    return true, target.id
end

-- Returns true/false, targetId
function findDamageAttackTargetsForAllReach(targetGroup, possibleTargets)
    local possibleTarget = -1
    
    for i=1, #possibleTargets do
        possibleTarget = possibleTargets[i]
        if possibleTarget >= 0 then
            break
        end
    end
    
    if possibleTarget < 0 then
        return false, nil
    end
    
    local slots = targetGroup.slots
    -- +1 because of lua 1-based indexing
    local slot = slots[possibleTarget + 1]
    local unit = slot.unit
    if unit then
        return true, unit.id
    end
    
    return true, Id.summonId(possibleTarget)
end

-- Returns true/false, targetId
function findDamageAttackTargetWithAnyReach(targetGroup, possibleTargets, damage, battle, attackClass, attackSource, unitStatus)
    local maxPriority = 0
    local slots = targetGroup.slots
    local targetUnitId = Id.emptyId()
    
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        
        if possibleTarget < 0 then
            goto continue
        end
        
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end
        
        local canTargetUnit = false
        if not unitStatus then
            canTargetUnit = true
        else
            canTargetUnit = not battle:getUnitStatus(unit.id, unitStatus)
        end

        if canTargetUnit then
            local impl = unit.impl
            
            local sourceImmune = impl:getImmuneToAttackSource(attackSource)
            local classImmune = impl:getImmuneToAttackClass(attackClass)

            if sourceImmune ~= Immune.Always and classImmune ~= Immune.Always then
                local priority = computeTargetUnitAiPriority(unit, battle, damage)

                if (sourceImmune == Immune.Once and battle:isUnitResistantToSource(attackSource))
                or (classImmune == Immune.Once and battle:isUnitResistantToClass(attackClass)) then
                    priority = priority * 0.69999999
                end
                
                if priority > maxPriority then
                    maxPriority = priority
                    targetUnitId = unit.id
                end
            end
        end
        
        ::continue::
    end
    
    return targetUnitId ~= Id.emptyId(), targetUnitId
end

-- Returns targetId
function findDamageAttackTargetWithAdjacentReach(targetGroup, possibleTargets, battle, attackSource, attackClass)
    local primaryTarget = Id.emptyId()
    local nonSummonTarget = Id.emptyId()
    local secondaryTarget = Id.emptyId()
    -- These constants are taken directly from game
    local minEffHp = 999999
    local summonMinEffHp = 999999
    
    local slots = targetGroup.slots
    
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        
        if not unit then
            goto continue
        end
        
        local isSummon = battle:getUnitStatus(unit.id, BattleStatus.Summon)
        if isSummon and primaryTarget ~= Id.emptyId() then
            -- Don't update primaryTarget
            goto continue
        end
        
        local effectiveHp = computeEffectiveHp(unit, battle)
        if effectiveHp > 0 and effectiveHp < minEffHp then
            local impl = unit.impl
            
            if impl:getImmuneToAttackSource(attackSource) ~= Immune.Always
            and impl:getImmuneToAttackClass(attackClass) ~= Immune.Always then
                if isSummon then
                    if effectiveHp < summonMinEffHp or pickRandomIfEqual(effectiveHp, summonMinEffHp) then
                        summonMinEffHp = effectiveHp
                        secondaryTarget = unit.id
                    end
                else
                    minEffHp = effectiveHp
                    nonSummonTarget = unit.id
                end
            end
        end
        
        primaryTarget = nonSummonTarget
        ::continue::
    end
    
    if primaryTarget == Id.emptyId() then
        return secondaryTarget
    end
    
    return primaryTarget
end

-- Returns true/false, targetId
function findDamageAttackTargetsForNonAllReach(attack, damage, targetGroup, possibleTargets, battle)
    local reach = attack.reach
    local attackSource = attack.source
    local attackClass = attack.type
    
    if reach ~= Reach.Adjacent then
        return findDamageAttackTargetWithAnyReach(targetGroup, possibleTargets, damage, battle, attackClass, attackSource, nil)
    end
    
    local targetId = findDamageAttackTargetWithAdjacentReach(targetGroup, possibleTargets, battle, attackSource, attackClass)
    return targetId ~= Id.emptyId(), targetId
end

-- Assumption: returns true if unit with Paralyze or Petrify attack should use Block action
-- because there are no allied units that can pertorm offence battle actions
function sub_5D02D5(attackClass, battle, enemyGroup)
    if attackClass ~= Attack.Paralyze and attackClass ~= Attack.Petrify then
        return false
    end
    
    local attacker = battle.attacker
    local group = attacker.group
    if group.id == enemyGroup.id then
        group = battle.defender
    end
    
    if groupCanPerformOffenceBattleActions(battle, group, enemyGroup) then
        return false
    end
    
    return true
end

function findParalyzeOrPetrifyAttackTarget(battle, targetGroup, possibleTargets, attackClass)
    if sub_5D02D5(attackClass, battle, targetGroup) then
        -- Do not search for target
        return false, nil
    end

    local attackingUnit = nil
    local nonAtackingUnit = nil
    local retreatingUnit = nil
    local attackingUnitMaxXpKilled = 0
    local nonAttackingUnitMaxXpKilled = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit

        if not unit then
            goto continue
        end

        if not battle:getUnitStatus(unit.id, BattleStatus.Paralyze)
        and not battle:getUnitStatus(unit.id, BattleStatus.Petrify) then
            if battle:getUnitStatus(unit.id, BattleStatus.Retreat) then
                retreatingUnit = unit
            else
                local impl = unit.impl
                local xpKilled = impl.xpKilled
                if xpKilled > attackingUnitMaxXpKilled then
                    if getUnitBattleAttackTargets(battle, unit, nil) then
                        attackingUnit = unit
                        attackingUnitMaxXpKilled = xpKilled
                    elseif xpKilled > nonAttackingUnitMaxXpKilled
                    or pickRandomIfEqual(xpKilled, nonAttackingUnitMaxXpKilled) then
                        nonAtackingUnit = unit
                        nonAttackingUnitMaxXpKilled = xpKilled
                    end
                end
            end
        end

        ::continue::
    end

    local targetUnit = attackingUnit
    if not attackingUnit then
        targetUnit = nonAtackingUnit

        if not nonAtackingUnit then
            targetUnit = retreatingUnit

            if retreatingUnit ~= nil then
                local attacker = battle.attacker
                local group = attacker.group
                if group.id == targetGroup.id then
                    group = battle.defender
                end

                if groupCanPerformOffenceBattleActions(battle, group, targetGroup) then
                    targetUnit = retreatingUnit
                end
            end
        end
    end

    if not targetUnit then
        return false, nil
    end

    return targetUnit.id ~= Id.emptyId(), targetUnit.id
end

-- Returns true/false, targetId
function findDotAttackTarget(unitStatus, battle, unitOrItemId, targetGroup, possibleTargets)
    local attack = nil
    local damage = 0

    if unitOrItemId.type == IdType.Unit then
        local unit = getScenario():getUnit(unitOrItemId)
        if not unit then
            -- This should never happen
            return false, nil
        end

        local impl = unit.impl
        attack = impl.attack1
        damage = attackGetDamageWithBuffsCheckTransform(unit, battle)
    else
        local item = getScenario():getItem(unitOrItemId)
        if not item then
            -- This should never happen
            return false, nil
        end

        local itemBase = item.base
        attack = itemBase.attack
        damage = attack.damage
    end

    local attackSource = attack.source
    local attackClass = attack.type

    local ok, targetId = findDamageAttackTargetWithAnyReach(targetGroup, possibleTargets, damage, battle, attackClass, attackSource, unitStatus)
    if ok then
        return true, targetId
    end

    return findDamageAttackTargetWithAnyReach(targetGroup, possibleTargets, damage, battle, attackClass, attackSource, nil)
end

function checkImplHasDamageOrDrainOrDrainOverflowAttack( impl )
    local attack = impl.attack1
    local attackClass = attack.type

    return attackClass == Attack.Damage
        or attackClass == Attack.Drain
        or attackClass == Attack.DrainOverflow
end

-- Returns true/false, targetId
function findBoostDamageAttackTarget(battle, targetGroup, possibleTargets)
    local targetUnit = nil
    local damagedTargetUnit = nil
    local maxXpKilled = 0
    local damagedUnitMaxXpKilled = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit

        if not unit then
            goto continue
        end

        local impl = unit.impl
        if not battle:getUnitStatus(unit.id, BattleStatus.Retreat)
        and not battle:getUnitStatus(unit.id, BattleStatus.Paralyze)
        and not battle:getUnitStatus(unit.id, BattleStatus.Petrify)
        and checkImplHasDamageOrDrainOrDrainOverflowAttack(impl) then
            local xpKilled = impl.xpKilled
            if xpKilled > maxXpKilled or pickRandomIfEqual(xpKilled, maxXpKilled) then
                local hp = unit.hp
                local hpMax = unit.hpMax

                if hp / hpMax < 0.25 then
                    if xpKilled > damagedUnitMaxXpKilled
                    or pickRandomIfEqual(xpKilled, damagedUnitMaxXpKilled) then
                        damagedTargetUnit = unit
                        damagedUnitMaxXpKilled = xpKilled
                    end
                else
                    targetUnit = unit
                    maxXpKilled = xpKilled
                end
            end
        end

        ::continue::
    end

    local target = targetUnit
    if not target then
        target = damagedTargetUnit
    end

    if not target then
        return false, nil
    end

    return true, target.id
end

-- Priority of 1 considered top, 999 - lowest priority
function getUnitCurePriority(unit, battle)
    local id = unit.id

    if battle:getUnitStatus(id, BattleStatus.Transform) then
        return 1
    end

    if battle:getUnitStatus(id, BattleStatus.Paralyze)
    or battle:getUnitStatus(id, BattleStatus.Petrify) then
        return 2
    end

    if battle:getUnitStatus(id, BattleStatus.Poison) then
        return 3
    end

    if battle:getUnitStatus(id, BattleStatus.Frostbite) then
        return 4
    end

    if battle:getUnitStatus(id, BattleStatus.Blister) then
        return 5
    end

    return 999
end

-- Returns true/false, targetId
function findCureAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local primaryTargetMinValue = 99
    local primaryTargetMaxXpKilled = 0

    local secondaryTarget = nil
    local secondaryTargetMinValue = 99
    local secondaryTargetMaxXpKilled = 0

    local slots = targetGroup.slots

    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit

        if not unit then
            goto continue
        end

        local value = getUnitCurePriority(unit, battle)
        local retreating = battle:getUnitStatus(unit.id, BattleStatus.Retreat)

        if retreating or battle:getUnitStatus(unit.id, BattleStatus.Summon) then
            if not primaryTarget
            and (not retreating
                or not secondaryTarget
                or not battle:getUnitStatus(secondaryTarget.id, BattleStatus.Summon))
            and value <= secondaryTargetMinValue then
                if value < secondaryTargetMinValue then
                    secondaryTargetMaxXpKilled = 0
                end

                local impl = unit.impl
                local xpKilled = impl.xpKilled
                if xpKilled > secondaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, secondaryTargetMaxXpKilled) then
                    secondaryTarget = unit
                    secondaryTargetMinValue = value
                    secondaryTargetMaxXpKilled = xpKilled
                end
            end
        elseif value <= primaryTargetMinValue then
            if value < primaryTargetMinValue then
                primaryTargetMaxXpKilled = 0
            end

            local impl = unit.impl
            local xpKilled = impl.xpKilled
            if xpKilled > primaryTargetMaxXpKilled
            or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled) then
                primaryTarget = unit
                primaryTargetMinValue = value
                primaryTargetMaxXpKilled = xpKilled
            end
        end

        ::continue::
    end

    local targetUnit = primaryTarget
    if not targetUnit then
        targetUnit = secondaryTarget
    end

    if not targetUnit then
        return false, nil
    end

    return true, targetUnit.id
end

-- Returns true/false, targetId
function findSummonAttackTarget(targetGroup, possibleTargets)
    local slots = targetGroup.slots

    -- Pick starting index in [0 : #possibleTargets) range
    -- +1 to convert index to [1 : #possibleTargets] range because of lua 1-based indexing
    local index = randomNumber(#possibleTargets) + 1
    local initialIndex = index

    -- Search starting from random index, prefer frontline targets, if possible.
    -- If end of possibleTargets list reached, continue from beginning until starting index.
    repeat
        local target = possibleTargets[index]
        if slots[target + 1].frontline then
            break
        end
    
        index = index + 1
        if index > #possibleTargets then
            index = 1
        end
    until (index == initialIndex)

    local possibleTarget = possibleTargets[index]

    -- +1 because of lua 1-based indexing
    local slot = slots[possibleTarget + 1]
    local unit = slot.unit

    local targetId
    if unit then
        targetId = unit.id
    else
        targetId = Id.summonId(possibleTarget)
    end

    return true, targetId
end

-- Returns true/false, targetId
function findTransformOtherAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxXpKilled = 0
    local secondaryTargetMaxXpKilled = 0

    local slots = targetGroup.slots

    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit

        if not unit then
            goto continue
        end

        if battle:getUnitStatus(unit.id, BattleStatus.Transform) then
            -- Unit already transformed
            goto continue
        end

        local impl = unit.impl
        local xpKilled = impl.xpKilled

        if xpKilled > primaryTargetMaxXpKilled
        or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled) then
            local hp = unit.hp
            local hpMax = unit.hpMax

            if hp / hpMax <= 0.2
            or battle:getUnitStatus(unit.id, BattleStatus.Retreat) then
                if xpKilled > secondaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, secondaryTargetMaxXpKilled) then
                    secondaryTarget = unit
                    secondaryTargetMaxXpKilled = xpKilled
                end
            else
                primaryTarget = unit
                primaryTargetMaxXpKilled = xpKilled
            end
        end

        ::continue::
    end

    local target = primaryTarget
    if not target then
        target = secondaryTarget
    end

    if not target then
        return false, nil
    end

    return true, target.id
end

function getTargetUnit(targetPosition, targetGroup, enemyGroup)
    if targetPosition >= 0 then
        -- +1 because of lua 1-based indexing
        local slot = targetGroup.slots[targetPosition + 1]
        return slot.unit
    end

    -- +1 because of lua 1-based indexing
    local slot = enemyGroup.slots[-(targetPosition + 1) + 1]
    return slot.unit
end

function isAttackEffectiveAgainstGroup(attack, group)
    local attackClass = attack.type
    if not isAttackClassCanInflictDamage(attackClass) then
        return true
    end

    local units = group.units
    if #units <= 0 then
        -- Empty group ?!
        return false
    end

    local attackSource = attack.source
    for i=1, #units do
        local unit = units[i]
        local impl = unit.impl
        if impl:getImmuneToAttackSource(attackSource) ~= Immune.Always
        and impl:getImmuneToAttackClass(attackClass) ~= Immune.Always then
            return true
        end
    end

    -- All units are always immune to attack
    return false
end

-- Returns true/false, targetId
function findDoppelgangerAttackTarget(unitOrItemId, battle, targetGroup, possibleTargets)
    local unit = getScenario():getUnit(unitOrItemId)
    if not unit then
        -- This should never happen
        return false, nil
    end

    local enemyGroup = battle.attacker.group
    if targetGroup.id == enemyGroup.id then
        enemyGroup = battle.defender
    end

    local unitPosition = targetGroup:getUnitPosition(unit)
    local primaryTargetXpKilled = 0
    local secondaryTargetXpKilled = 0
    local primaryTarget = nil
    local secondaryTarget = nil

    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        local targetUnit = getTargetUnit(possibleTarget, targetGroup, enemyGroup)
        if not targetUnit then
            goto continue
        end

        local impl = targetUnit.impl
        local attack = impl.attack1
        if not isAttackEffectiveAgainstGroup(attack, enemyGroup) then
            goto continue
        end

        local xpKilled = impl.xpKilled
        if attack.melee == (unitPosition % 2 == 0) then
            if xpKilled > primaryTargetXpKilled or pickRandomIfEqual(xpKilled, primaryTargetXpKilled) then
                primaryTargetXpKilled = xpKilled
                primaryTarget = targetUnit
            end
        elseif primaryTarget == nil then
            if xpKilled > secondaryTargetXpKilled or pickRandomIfEqual(xpKilled, secondaryTargetXpKilled) then
                secondaryTargetXpKilled = xpKilled
                secondaryTarget = targetUnit
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

function countAliveUnitsInGroup(group)
    local units = group.units
    local aliveUnits = 0

    for i=1, #units do
        local unit = units[i]
        if isUnitAlive(unit) then
            aliveUnits = aliveUnits + 1
        end
    end

    return aliveUnits
end

-- Returns true if specified unit can (and should) perform TransformSelf attack
function isTransformSelfPossible(unit, battle)
    if battle:getUnitStatus(unit.id, BattleStatus.TransformSelf) then
        return false
    end

    local allyGroup = nil
    local enemyGroup = nil
    if battle:isUnitAttacker(unit) then
        allyGroup = battle.attacker.group
        enemyGroup = battle.defender
    else
        allyGroup = battle.defender
        enemyGroup = battle.attacker.group
    end

    local slots = allyGroup.slots

    -- +1 because of lua 1-based indexing
    if allyGroup:getUnitPosition(unit) % 2 == 1
    and (slots[0 + 1].unit or slots[2 + 1].unit or slots[4 + 1].unit) then
        -- Do not transform self if we stand at the frontlane with allied units
        return false
    end

    if unit.hp / unit.hpMax <= 0.25 then
        -- Don not transform self if we have less than 25% of health
        return false
    end

    -- Transform self only if there are less than 3 alive enemies
    return countAliveUnitsInGroup(enemyGroup) < 3
end

-- Returns true/false, targetId
function findTransformSelfAttackTarget(unitOrItemId, battle, targetGroup, possibleTargets)
    if unitOrItemId.type == IdType.Unit then
        local unit = getScenario():getUnit(unitOrItemId)
        if not unit then
            -- This should never happen
            return false, nil
        end

        if isTransformSelfPossible(unit, battle) then
            return true, unit.id
        end

        return false, nil
    end

    -- unitOrItemId belongs to item
    local minXpKilled = 99999
    local targetUnit = nil

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        if isTransformSelfPossible(unit, battle) then
            local impl = unit.impl
            local xpKilled = impl.xpKilled

            if xpKilled < minXpKilled then
                minXpKilled = xpKilled
                targetUnit = unit
            end
        end

        ::continue::
    end

    if targetUnit then
        return true, targetUnit.id
    end

    return false, nil
end

-- Returns true/false, targetId
function findDrainLevelAttackTarget(battle, targetGroup, possibleTargets)
    local slots = targetGroup.slots

    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxXpKilled = 0
    local secondaryTargetMaxXpKilled = 0
    local maxLevel = 1

    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        local impl = unit.impl
        local level = impl.level
        if level ~= 1 and level >= maxLevel then
            if level > maxLevel then
                primaryTargetMaxXpKilled = 0
            end
        
            maxLevel = level
            local xpKilled = impl.xpKilled
            if battle:getUnitStatus(unit.id, BattleStatus.Retreat) then
                if xpKilled > secondaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, secondaryTargetMaxXpKilled) then
                    secondaryTarget = unit
                    secondaryTargetMaxXpKilled = xpKilled
                end
            elseif xpKilled > primaryTargetMaxXpKilled
            or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled) then
                primaryTarget = unit
                primaryTargetMaxXpKilled = xpKilled
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

-- Returns true/false, targetId
function findGiveAttackTarget(battle, targetGroup, possibleTargets)
    local slots = targetGroup.slots

    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxXpKilled = 0
    local secondaryTargetMaxXpKilled = 0
    local secondaryTargetIsBooster = true

    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        if not battle:getUnitStatus(unit.id, BattleStatus.Retreat)
        and not battle:getUnitStatus(unit.id, BattleStatus.Paralyze)
        and not battle:getUnitStatus(unit.id, BattleStatus.Petrify)
        then
            local impl = unit.impl
            local xpKilled = impl.xpKilled
            local attack = impl.attack1
            local attackClass = attack.type

            if attackClass == Attack.Damage
            or attackClass == Attack.Drain
            or attackClass == Attack.DrainOverflow
            then
                if xpKilled > primaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled)
                then
                    local canAttack, _ = getUnitBattleAttackTargets(battle, unit)
                    if canAttack then
                        primaryTarget = unit
                        primaryTargetMaxXpKilled = xpKilled
                    end
                end
            elseif not primaryTarget
            and (xpKilled > primaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled))
            then
                local boost = attackClass == Attack.BoostDamage
                if not boost or secondaryTargetIsBooster then
                    local canAttack, _ = getUnitBattleAttackTargets(battle, unit)
                    if canAttack then
                        secondaryTarget = unit
                        secondaryTargetMaxXpKilled = xpKilled
                        secondaryTargetIsBooster = boost
                    end
                end
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

function checkBestowWardsAttackCanBePerformed(battle, unitOrItemId)
    local group = nil

    -- Strange: we accessing enemy group here ?
    -- This is what original game logic does
    if battle:isUnitAttacker(unitOrItemId) then
        group = battle.defender
    else
        group = battle.attacker.group
    end

    local units = group.units
    for i=1, #units do
        local unit = units[i]

        if not battle:getUnitStatus(unit.id, BattleStatus.Dead)
        and not battle:getUnitStatus(unit.id, BattleStatus.XpCounted)
        and not battle:getUnitStatus(unit.id, BattleStatus.Retreated)
        and not battle:getUnitStatus(unit.id, BattleStatus.Retreat)
        and not battle:getUnitStatus(unit.id, BattleStatus.Hidden)
        and not battle:getUnitStatus(unit.id, BattleStatus.Unsummoned)
        then
            -- At least one unit can be buffed with 'Bestow wards'
            return true
        end
    end

    -- No units can be buffed
    return false
end

-- Returns true/false, targetId
function findBestowWardsAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxXpKilled = 0
    local secondaryTargetMaxXpKilled = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        if battle:getUnitStatus(unit.id, BattleStatus.Retreat)
        or battle:getUnitStatus(unit.id, BattleStatus.Paralyze)
        or battle:getUnitStatus(unit.id, BattleStatus.Petrify)
        then
            goto continue
        end

        local impl = unit.impl
        local xpKilled = impl.xpKilled
        if xpKilled > primaryTargetMaxXpKilled
        or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled)
        then
            if unit.hp / unit.hpMax < 0.25 then
                if xpKilled > secondaryTargetMaxXpKilled
                or pickRandomIfEqual(xpKilled, secondaryTargetMaxXpKilled)
                then
                    secondaryTarget = unit
                    secondaryTargetMaxXpKilled = xpKilled
                end
            else
                primaryTarget = unit
                primaryTargetMaxXpKilled = xpKilled
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

-- Returns true/false, targetId
function findShatterAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxArmor = 0
    local secondaryTargetMaxArmor = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        if battle:getUnitStatus(unit.id, BattleStatus.Retreat)
        or battle:getUnitStatus(unit.id, BattleStatus.Paralyze)
        or battle:getUnitStatus(unit.id, BattleStatus.Petrify)
        then
            goto continue
        end

        local impl = unit.impl
        local armor = impl.armor
        if armor > primaryTargetMaxArmor
        or pickRandomIfEqual(armor, primaryTargetMaxArmor)
        then
            if unit.hp / unit.hpMax < 0.25 then
                if armor > secondaryTargetMaxArmor
                or pickRandomIfEqual(armor, secondaryTargetMaxArmor)
                then
                    secondaryTarget = unit
                    secondaryTargetMaxArmor = armor
                end
            else
                primaryTarget = unit
                primaryTargetMaxArmor = armor
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

-- Returns true/false, targetId
function findFearAttackTarget(battle, targetGroup, possibleTargets)
    local primaryTarget = nil
    local secondaryTarget = nil
    local primaryTargetMaxXpKilled = 0
    local secondaryTargetMaxXpKilled = 0

    local slots = targetGroup.slots
    for i=1, #possibleTargets do
        local possibleTarget = possibleTargets[i]
        -- +1 because of lua 1-based indexing
        local slot = slots[possibleTarget + 1]
        local unit = slot.unit
        if not unit then
            goto continue
        end

        if battle:getUnitStatus(unit.id, BattleStatus.Retreat) then
            goto continue
        end

        local impl = unit.impl
        local xpKilled = impl.xpKilled
        if xpKilled > primaryTargetMaxXpKilled
        or pickRandomIfEqual(xpKilled, primaryTargetMaxXpKilled)
        then
            local canAttack, _ = getUnitBattleAttackTargets(battle, unit)
            if canAttack then
                primaryTarget = unit
                primaryTargetMaxXpKilled = xpKilled
            elseif xpKilled > secondaryTargetMaxXpKilled
            or pickRandomIfEqual(xpKilled, secondaryTargetMaxXpKilled)
            then
                secondaryTarget = unit
                secondaryTargetMaxXpKilled = xpKilled
            end
        end

        ::continue::
    end

    if primaryTarget then
        return true, primaryTarget.id
    end

    if secondaryTarget then
        return true, secondaryTarget.id
    end

    return false, nil
end

-- Returns true/false, targetId
function findAttackTarget(unitOrItemId, attack, targetGroup, possibleTargets, battle)
    local attackClass = attack.type
    
    if attackClass == Attack.Heal then
        return findHealAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Damage
    or attackClass == Attack.Drain
    or attackClass == Attack.DrainOverflow
    then
        if attack.reach == Reach.All then
            return findDamageAttackTargetsForAllReach(targetGroup, possibleTargets)
        end
        
        local maxDamage = getUnitOrItemMaxDamage(unitOrItemId)
        local damage = math.min(maxDamage, attack.damage)
        return findDamageAttackTargetsForNonAllReach(attack, damage, targetGroup, possibleTargets, battle)
    end
    
    if attackClass == Attack.Paralyze then
        return findParalyzeOrPetrifyAttackTarget(battle, targetGroup, possibleTargets, attackClass)
    end
    
    if attackClass == Attack.Poison then
        return findDotAttackTarget(BattleStatus.Poison, battle, unitOrItemId, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.BoostDamage then
        return findBoostDamageAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Cure then
        return findCureAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Summon then
        return findSummonAttackTarget(targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.TransformOther then
        return findTransformOtherAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Revive then
        return findReviveAttackTarget(targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Doppelganger then
        return findDoppelgangerAttackTarget(unitOrItemId, battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.TransformSelf then
        return findTransformSelfAttackTarget(unitOrItemId, battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Petrify then
        return findParalyzeOrPetrifyAttackTarget(battle, targetGroup, possibleTargets, attackClass)
    end
    
    if attackClass == Attack.DrainLevel then
        return findDrainLevelAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.GiveAttack then
        return findGiveAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Frostbite then
        return findDotAttackTarget(BattleStatus.Frostbite, battle, unitOrItemId, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Blister then
        return findDotAttackTarget(BattleStatus.Blister, battle, unitOrItemId, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.BestowWards then
        local canAttack = checkBestowWardsAttackCanBePerformed(battle, unitOrItemId)
        if not canAttack then
            return findHealAttackTarget(battle, targetGroup, possibleTargets)
        end
        
        return findBestowWardsAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Shatter then
        return findShatterAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    if attackClass == Attack.Fear then
        return findFearAttackTarget(battle, targetGroup, possibleTargets)
    end
    
    return false, nil
end

function getNonAttackingItemTargets(item, itemTargetGroup, itemPossibleTargets)
    local base = item.base
    local itemType = base.type
    
    if itemType == Item.PotionRevive then
        local targets = {}
        for i=1, #itemPossibleTargets do
            if itemPossibleTargets[i] % 2 == 1 then
                -- Prefer reviving frontlane units
                table.insert(targets, itemPossibleTargets[i])
            end
        end
    
        return targets
    end
    
    if itemType == Item.PotionHeal then
        local slots = targetGroup.slots
    
        local targets = {}
        for i=1, #itemPossibleTargets do
            local possibleTarget = itemPossibleTargets[i]
            -- +1 because of lua 1-based indexing
            local slot = slots[possibleTarget + 1]
            
            local unit = slot.unit
            if unit ~= nil and isUnitHasLessThanHalfHitPoints(unit) then
                -- Prefer healing units that have 50% of hit points or less
                table.insert(targets, possibleTarget)
            end
        end
        
        return targets
    end
    
    return itemPossibleTargets
end

-- Returns true/false, targetId
function findHealOrRevivePotionAttackTarget(item, battle, itemTargetGroup, itemPossibleTargets)
    local base = item.base
    local itemType = base.type
    
    if itemType == Item.PotionHeal then
        return findHealAttackTarget(battle, itemTargetGroup, itemPossibleTargets)
    end
    
    if itemType == Item.PotionRevive then
        return findReviveAttackTarget(battle, itemTargetGroup, itemPossibleTargets)
    end
    
    return false, nil
end

-- Returns true/false, targetId, attackerId
function sub_5D1172(activeUnit, item, itemTargetGroup, itemPossibleTargets, battle)
    if #itemPossibleTargets <= 0 then
        return false, nil, nil
    end
    
    local attackerId = item.id
    local attack = getAttackByIdAndCheckTransformed(item.id, 1)
    if attack then
        local targets = getAttackingItemTargets(attack, itemTargetGroup, itemPossibleTargets)
        if #targets then
            local found, targetId = findAttackTarget(item.id, attack, itemTargetGroup, itemPossibleTargets, battle)
            return found, targetId, attackerId
        end
    else
        local targets = getNonAttackingItemTargets(item, itemTargetGroup, itemPossibleTargets)
        if #targets then
            local found, targetId = findHealOrRevivePotionAttackTarget(item, battle, itemTargetGroup, itemPossibleTargets)
            return found, targetId, attackerId
        end        
    end
    
    return false, nil, nil
end

function sub_5D0FA2(activeUnit, activeUnitGroup, item1TargetGroup, item1PossibleTargets, item2TargetGroup, item2PossibleTargets, battle)
    if activeUnitGroup.id.type ~= IdType.Stack then
        return false, nil, nil
    end
    
    local stack = getScenario():getStack(activeUnitGroup.id)
    if not stack then
        -- This should never happen
        return false, nil, nil
    end
    
    local item1 = stack:getEquippedItem(Equipment.Battle1)
    local item1Value = sub_5D10F9(item1)
    
    local item2 = stack:getEquippedItem(Equipment.Battle2)
    local item2Value = sub_5D10F9(item2)
    
    if item1Value < item2Value then
        if item2Value > 0 then
            local ok, targetId, attackerId = sub_5D1172(activeUnit, item2, item2TargetGroup, item2PossibleTargets, battle)
            if ok then
                return ok, targetId, attackerId
            end
        end
        
        if item1Value <= 0 then
            return false, nil, nil
        end
        
        return sub_5D1172(activeUnit, item1, item1TargetGroup, item1PossibleTargets, battle)
    end
    
    if item1Value > 0 then
        local ok, targetId, attackerId = sub_5D1172(activeUnit, item1, item1TargetGroup, item1PossibleTargets, battle)
        if ok then
            return ok, targetId, attackerId
        end
    end
    
    if item2Value > 0 then
        local ok, targetId, attackerId = sub_5D1172(activeUnit, item2, item2TargetGroup, item2PossibleTargets, battle)
        if ok then
            return ok, targetId, attackerId
        end
    end
    
    return false, nil, nil
end

function sub_5CFA6C(impl)
    local attack1 = impl.attack1
    local attack1Class = attack1.type

    if attack1Class == Attack.Damage
    or attack1Class == Attack.Drain
    or attack1Class == Attack.DrainOverflow
    then
        local attack2 = impl.attack2
        if not attack2 then
            return true
        end

        local attack2Class = attack2.type

        if attack2Class == Attack.LowerDamage
        or attack2Class == Attack.LowerInitiative
        or attack2Class == Attack.Damage
        or attack2Class == Attack.Drain
        or attack2Class == Attack.DrainOverflow
        then
            return true
        end
    end

    return false
end

-- Returns true/false
function sub_5CFB6C(impl, allyGroupId, targetGroupId)
    local attack1 = impl.attack1
    local attack1Class = attack1.type

    if attack1Class ~= Attack.Doppelganger or allyGroupId == targetGroupId then
        -- Attack is not Doppelganger or groups are the same
        return false
    end

    return true
end

-- Returns secondary attack in case when primary attack has Damage, Drain or DrainOverflow attack class
function tryGetDamageOrDrainSecondAttack(impl)
    local attack = impl.attack1
    local attackClass = attack.type

    if attackClass == Attack.Damage
    or attackClass == Attack.Drain
    or attackClass == Attack.DrainOverflow
    then
        return impl.attack2
    end

    return attack
end

function getTransformSelfAltAttackOrSecondAttack(impl, attack)
    local attack1 = impl.attack1

    if attack1.type == Attack.TransformSelf then
        return impl.altAttack
    end

    if attack and attack1.id == attack.id then
        return impl.attack2
    end
    
    return attack1
end

-- Returns true/false, target id
function sub_5D36BF(activeUnit, damage, attack2, altAttack, enemyGroup, possibleTargets, battle)
    local ok, targetId = findAttackTarget(activeUnit.id, attack2, enemyGroup, possibleTargets, battle)
    if ok then
        return true, targetId
    end

    if not altAttack then
        return false, nil
    end

    local attackClass = altAttack.type

    if attackClass ~= Attack.Damage
    and attackClass ~= Attack.Drain
    and attackClass ~= Attack.DrainOverflow
    then
        return findAttackTarget(activeUnit.id, altAttack, enemyGroup, possibleTargets, battle)
    end

    if altAttack.reach ~= Reach.All then
        return findDamageAttackTargetsForNonAllReach(altAttack, damage, enemyGroup, possibleTargets, battle)
    end

    local allyGroup = nil
    if battle:isUnitAttacker(activeUnit) then
        allyGroup = battle.attacker.group
    else
        allyGroup = battle.defender
    end

    if isTargetGroupAlwaysImmuneToAttackClassAndSource(attackClass, altAttack.source, allyGroup, enemyGroup, possibleTargets) then
        return false, nil
    end

    return findDamageAttackTargetsForAllReach(enemyGroup, possibleTargets)
end

-- Returns BattleAction, target unit id, attacker unit id
function chooseAction(
    -- Battle state
    battle, 
    -- Unit for which to choose action
    activeUnit,
    -- List of unit possible battle actions
    possibleActions,
    -- Group that is a target for a unit attack
    attackTargetGroup,
    -- List of unit attack targets, indices of attack target group slots. 
    attackPossibleTargets,
    -- Group that is a target for a `Battle1` equipped item in case of leader unit or nil if unit is not leader or don't wear item.
    item1TargetGroup,
    -- List of `Battle1` equipped item targets, indices of group slots.
    item1PossibleTargets,
    -- Group that is a target for a `Battle2` equipped item in case of leader unit or nil if unit is not leader or don't wear item.
    item2TargetGroup,
    -- List of `Battle2` equipped item targets, indices of group slots.
    item2PossibleTargets)

    local activeUnitIsAttacker = battle:isUnitAttacker(activeUnit)
    
    local activeUnitGroup = nil
    local enemyGroup = nil
    
    if activeUnitIsAttacker then
        activeUnitGroup = battle.attacker.group
        enemyGroup = battle.defender
    else
        enemyGroup = battle.attacker.group
        activeUnitGroup = battle.defender
    end
    
    local unknownGroup = nil
    if attackTargetGroup.id == enemyGroup.id then
        unknownGroup = enemyGroup
    else
        unknownGroup = activeUnitGroup
    end

    local relativeCoeff = computeGroupsRelativeCoefficient(battle, activeUnitGroup, enemyGroup)
    
    local ok,
        action,
        selectedTargetId,
        selectedAttackerId = checkGroupShouldRetreat(battle, possibleActions, activeUnit, activeUnitGroup, enemyGroup, activeUnitIsAttacker, relativeCoeff)
    
    if ok then
        -- Decision to retreat was made
        return action, selectedTargetId, selectedAttackerId
    end

    local activeUnitIsLeader = isLeader(activeUnit)
    if activeUnitIsLeader then
        if relativeCoeff <= 0.3
        and checkGroupCanRetreat(possibleActions, activeUnit, activeUnitGroup, battle, false, false)
        and not isGroupHasLessThanTwoUnitsAlive(battle, activeUnitGroup, true)
        then
            local shouldRetreat = true
            if not activeUnitIsAttacker and isStackHaveMovement(enemyGroup) then
                shouldRetreat = false
            end
            
            if shouldRetreat then
                -- Leader should retreat
                return BattleAction.Retreat, activeUnit.id, activeUnit.id
            end
        end
    end

    -- Check if we can select attack target
    local selectedAttackTarget = nil
    local damage = 0

    if #attackPossibleTargets ~= 0 then
        damage = attackGetDamageWithBuffsCheckTransform(activeUnit, battle)
        if damage > 0 then
            local attackSource = getSoldierAttackSource(activeUnit.impl)
            selectedAttackTarget = selectAttackTarget(battle, damage, unknownGroup, attackPossibleTargets, attackSource)
        end
    end
    
    -- Check leader and its items
    if activeUnitIsLeader
    and relativeCoeff <= 0.7
    and canLeaderUseItemInBattle(selectedAttackTarget, possibleActions, item1PossibleTargets, item2PossibleTargets)
    then
        -- Leader can use item in battle
        local ok,
            selectedTargetId,
            selectedAttackerId = sub_5D0FA2(activeUnit, activeUnitGroup, item1TargetGroup, item1PossibleTargets, item2TargetGroup, item2PossibleTargets, battle)
        if ok then
            -- Use item
            return BattleAction.UseItem, selectedTargetId, selectedAttackerId
        end
    end

    -- Check if we should defend
    if #attackPossibleTargets == 0
    or not hasValue(possibleActions, BattleAction.Attack) then
        -- Decision to defend was made
        return BattleAction.Defend, activeUnit.id, activeUnit.id
    end

    -- Check if we could perform attack action
    local v43 = true
    local activeUnitImpl = activeUnit.impl
    local attack = activeUnitImpl.attack1
    if attack.reach == Reach.All then
        local v1 = isTargetGroupAlwaysImmuneToAttackClassAndSource(attack.type, attack.source, activeUnitGroup, unknownGroup, attackPossibleTargets)
        local v2 = sub_5D02D5(attack.type, battle, unknownGroup)
        if not v1
        and not v2
        then
            local ok, targetId = findDamageAttackTargetsForAllReach(unknownGroup, attackPossibleTargets)
            if ok then
                -- Attack with reach 'All'
                return BattleAction.Attack, targetId, activeUnit.id
            end
        end

        v43 = false
    end

    if v43 then
        if selectedAttackTarget then
            -- Attack selected target
            return BattleAction.Attack, selectedAttackTarget.id, activeUnit.id
        end

        if sub_5CFA6C(activeUnitImpl)
        or sub_5CFB6C(activeUnitImpl, activeUnitGroup.id, attackTargetGroup.id) then
            local found, targetId = findDamageAttackTargetsForNonAllReach(attack, damage, unknownGroup, attackPossibleTargets, battle)
            if found then
                -- Attack with reaches other than 'All'
                return BattleAction.Attack, targetId, activeUnit.id
            end
        else
            local attack2 = tryGetDamageOrDrainSecondAttack(activeUnitImpl)
            local altAttack = getTransformSelfAltAttackOrSecondAttack(activeUnitImpl, attack2)

            local ok, targetId = sub_5D36BF(activeUnit, damage, attack2, altAttack, unknownGroup, attackPossibleTargets, battle)
            if ok then
                -- Attack considering secondary or alt. attacks
                return BattleAction.Attack, targetId, activeUnit.id
            end
        end
    end

    -- Nothing to attack, defend
    return BattleAction.Defend, activeUnit.id, activeUnit.id
end
