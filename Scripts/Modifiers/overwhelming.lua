function getModifierDescTxt()
	return Id.new("x000tg6040")
end

function getAttackDamage(unit, prev)
	return prev * (0.5 + unit.hp / unit.hpMax)
end
