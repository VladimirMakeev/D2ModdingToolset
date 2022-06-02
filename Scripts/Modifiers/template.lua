function canApplyAsLowerSpell()
	return false
end

function canApplyAsBoostSpell()
	return true
end

function canApplyToUnit(unit)
	-- Can apply on only on water unit
	return unit.impl.waterOnly
end

function canApplyToUnitType(unitType)
	-- Can apply on stack leader only
	return
		unitType == Unit.Noble or
		unitType == Unit.Leader or
		unitType == Unit.Summon or
		unitType == Unit.Illusion
end

function getNameTxt(unit, prev)
	-- You can use format text like "Overpowered %BASE%" to get names like "Overpowered Peasant"
	if unit.hpMax == 100 and unit.type == Leader.Rod then
		return Id.new("x000tgb004")
    elseif prev == Id.new("x000tg2201") then -- Peasant
		return Id.new("x000tg2053") -- Berzerker
	end

    return Id.new("x000tg2054") -- Anti-Paladin
end

function getDescTxt(unit, prev)
	if unit.impl.base and unit.impl.base.subrace == Subrace.Heretic then
		return Id.new("x000tg0112")
	end

	if unit.type == Leader.Rod then
		return Id.new("x160tgc536") --return "x000tgb004" -- Plants Rods
	end

    return Id.new("x000tg2054") -- Anti-Paladin
end

function getHitPoint(unit, prev)
	if unit.hpMax == 100 then
		return 1234
	end

	return prev + 69
end

function getArmor(unit, prev)
	return prev + 5
end

function getDeathAnim(unit, prev)
	-- Currently there is a bug in the game that prevents this from working
	return DeathAnimation.Heretic
end

function getRegen(unit, prev)
	return 50
end

function getXpNext(unit, prev)
	return prev / 2
end

function getXpKilled(unit, prev)
	return prev * 2
end

function getImmuneToAttack(unit, attack, prev)
	if attack == Attack.Poison then
		return Immune.Always
	end
	
	return prev
end

function getImmuneToSource(unit, source, prev)
	if source == Source.Fire and prev ~= Immune.Always then
		return Immune.Once
	end

	return prev
end

function getAtckTwice(unit, prev)
	if unit.hp / unit.hpMax < 0.5 then
		return true
	end

	return prev
end

function getEnrollCost(unit, prev)
	local cost = Currency.new(prev)
	cost.gold = cost.gold + 100
	return cost
end

function getReviveCost(unit, prev)
	local cost = Currency.new(prev)
	cost.gold = 10
	return cost
end

function getHealCost(unit, prev)
	local cost = Currency.new(prev)
	cost.gold = 1
	return cost
end

function getTrainingCost(unit, prev)
	local cost = Currency.new(prev)
	cost.gold = 1
	return cost
end

function getMovement(unit, prev)
	return prev + 25
end

function hasMovementBonus(unit, ground, prev)
	if ground == Ground.Forest then
		return true
	end
	
	return prev
end

function getScout(unit, prev)
	return prev + 1
end

function getLeadership(unit, prev)
	return prev + math.floor(unit.impl.level / 3)
end

function getNegotiate(unit, prev)
	return prev + 10
end

function hasAbility(unit, ability, prev)
	if ability == Ability.BannerUse then
		return true
	end
	
	return prev
end

function getFastRetreat(unit, prev)
	return true
end

function getLowerCost(unit, prev)
	return 25
end

function getAttackInitiative(unit, prev)
	return prev * (2 - unit.hp / unit.hpMax)
end

function getAttackReach(unit, prev)
	if unit.hp / unit.hpMax < 0.5 then
		return Reach.All
	end

	return 5 -- Some custom attack reach
end

function getAttackId(unit, prev)
	return Id.new("g000aa0164")
end

function getAttackNameTxt(unit, prev)
	return Id.new("x160tgc536")
end

function getAttackDescTxt(unit, prev)
	return Id.new("x160tgc537")
end

function getAttackClass(unit, prev)
	return Attack.Drain
end

function getAttackSource(unit, prev)
	if unit.hp > 50 then
		return Source.Fire
	end

	return 14 -- Some custom attack source
end

function getAttackPower(unit, prev)
	return 69
end

function getAttackDamage(unit, prev)
	return prev + 100
end

function getAttackHeal(unit, prev)
	return prev + 50
end

function getAttackDrain(unit, damage, prev)
	return prev + damage * 0.75
end

function getAttackLevel(unit, prev)
	return 3
end

function getAttackInfinite(unit, prev)
	return true
end

function getAttackCritHit(unit, prev)
	return true
end

function getAttackCritDamage(unit, prev)
	return 50
end

function getAttackCritPower(unit, prev)
	-- Crit chance (0 : 100)
	return 30
end

function getAttackDamRatio(unit, prev)
	return 50
end

function getAttackDrRepeat(unit, prev)
	return true
end

function getAttackDrSplit(unit, prev)
	return true
end

function getAttackWards(unit, prev)
	local result = prev
	table.insert(result, Id.new("g000um0011")) -- Add mind ward
	return result
end

function getAttack2Id(unit, prev)
	if prev == Id.new("g000000000") then
		return Id.new("g000aa9020")
	end
	
	return prev
end

function getAttack2NameTxt(unit, prev)
end

function getAttack2DescTxt(unit, prev)
end

function getAttack2Class(unit, prev)
end

function getAttack2Source(unit, prev)
end

function getAttack2Power(unit, prev)
end

function getAttack2Damage(unit, prev)
end

function getAttack2Heal(unit, prev)
end

function getAttack2Drain(unit, damage, prev)
end

function getAttack2Level(unit, prev)
end

function getAttack2Infinite(unit, prev)
end

function getAttack2CritHit(unit, prev)
end

function getAttack2CritDamage(unit, prev)
end

function getAttack2CritPower(unit, prev)
end

function getAttack2DamRatio(unit, prev)
end

function getAttack2DrRepeat(unit, prev)
end

function getAttack2DrSplit(unit, prev)
end

function getAttack2Wards(unit, prev)
end
