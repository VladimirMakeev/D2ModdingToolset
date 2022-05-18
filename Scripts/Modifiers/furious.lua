function getModifierDescTxt()
	return Id.new("x000tg6039")
end

function getAttackReach(unit, prev)
	if unit.hp / unit.hpMax < 0.5 then
		return Reach.All
	end

	return prev
end
