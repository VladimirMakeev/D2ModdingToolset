function getLeadership(unit, prev)
	return prev + math.floor(unit.impl.level / 3)
end

function getAttackInitiative(unit, prev)
	return prev + 5 * math.floor(unit.impl.level / 3)
end
