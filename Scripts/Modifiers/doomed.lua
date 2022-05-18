function getModifierDescTxt()
	return Id.new("x000tg6047")
end

function getAttackDamage(unit, prev)
	return prev * unit.hp / unit.hpMax
end

function getAttackInitiative(unit, prev)
	return prev * unit.hp / unit.hpMax
end
