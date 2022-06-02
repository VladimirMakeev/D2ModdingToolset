function getAttackHeal(unit, prev)
	return prev + 20
end

function getImmuneToSource(unit, source, prev)
	if source == Source.Death and prev ~= Immune.Always then
		return Immune.Once
	end
	
	return prev
end
