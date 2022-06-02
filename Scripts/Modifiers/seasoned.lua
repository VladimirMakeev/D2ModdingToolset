function getRegen(unit, prev)
	return prev + unit.xp * 50 / unit.impl.xpNext
end
