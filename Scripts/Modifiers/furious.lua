function getAttackReach(unit, prev)
	if unit.hp / unit.hpMax < 0.5 then
		return Reach.All
	end

	return prev
end
