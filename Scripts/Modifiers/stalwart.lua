function getModifierDescTxt()
	return Id.new("x000tg6041")
end

function getArmor(unit, prev)
	return prev + 50 * (1 - unit.hp / unit.hpMax)
end
