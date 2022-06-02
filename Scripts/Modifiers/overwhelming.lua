function getAttackDamage(unit, prev)
	return prev * (0.5 + unit.hp / unit.hpMax)
end
