function getModifierDescTxt()
	return Id.new("x000tg6043")
end

function getHitPoint(unit, prev)
	return prev * (1.25 - 0.5 * (unit.impl.level % 2))
end
