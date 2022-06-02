function getAttackDamage(unit, prev)
	return prev * unit.hp / unit.hpMax
end

function getAttackInitiative(unit, prev)
	return prev * unit.hp / unit.hpMax
end
