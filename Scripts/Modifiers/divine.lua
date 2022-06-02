function getAttackSource(unit, prev)
	if unit.impl.level > 9 then
		return Source.Life
	end
	
	return prev
end
