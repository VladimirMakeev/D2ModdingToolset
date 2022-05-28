function getModifierDescTxt()
	return Id.new("x000tg6038")
end

function getAttackDamage(unit, prev)
	return prev * (2 - unit.hp / unit.hpMax)
end

function getAttackInitiative(unit, prev)
	return prev * (2 - unit.hp / unit.hpMax)
end
