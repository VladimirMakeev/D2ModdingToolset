function getAttackDamage(unit, prev)
	return prev * (2 - unit.hp / unit.hpMax)
end

function getAttackInitiative(unit, prev)
	return prev * (2 - unit.hp / unit.hpMax)
end
