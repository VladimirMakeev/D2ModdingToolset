function getModifierDescTxt()
	return Id.new("x000tg6048")
end

function getLeadership(unit, prev)
	return prev + math.floor(unit.impl.level / 3)
end

function getAttackInitiative(unit, prev)
	return prev + 5 * math.floor(unit.impl.level / 3)
end
