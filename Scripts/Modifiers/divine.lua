function getModifierDescTxt()
	return Id.new("x000tg6045")
end

function getAttackSource(unit, prev)
	if unit.impl.level > 9 then
		return Source.Life
	end
	
	return prev
end
