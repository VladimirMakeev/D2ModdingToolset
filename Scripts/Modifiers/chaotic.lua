function getAttackDamage(unit, prev)
	return prev * (1.25 - 0.5 * (unit.hp % 2))
end
