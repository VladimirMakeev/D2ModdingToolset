function getModifierDescTxt()
	return Id.new("x000tg6046")
end

function getAttackSource(unit, prev)
	return Source.Fire
end

function getImmuneToSource(unit, source, prev)
	if source == Source.Fire and prev ~= Immune.Always then
		return Immune.Once
	end

	return prev
end
