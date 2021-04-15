function getLevel(unit, transformImpl)
	-- transform using current level
	return math.max(unit.impl.level, transformImpl.level);
end
