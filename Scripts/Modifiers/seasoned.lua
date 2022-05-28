function getModifierDescTxt()
	return Id.new("x000tg6042")
end

function getRegen(unit, prev)
	return prev + unit.xp * 50 / unit.impl.xpNext
end
