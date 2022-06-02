function getArmor(unit, prev)
	return prev + 50 * (1 - unit.hp / unit.hpMax)
end
