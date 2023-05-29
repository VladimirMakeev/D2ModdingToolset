function getLevel(unit, transformImpl, item)
	-- transform using current level
	return math.max(unit.impl.level, transformImpl.level);
end

-- Used by freeTransformSelfAttack setting to determine a number of free attacks to give
function getFreeAttackNumber(attacksDone, attacksRemain, hadDoubleAttack, hasDoubleAttack)
	-- Uncomment the following lines to deny free transformation on second attack
	--if attacksDone > 0 then
	--	return 0

	if not hadDoubleAttack and hasDoubleAttack and attacksDone == 0 then
		-- Give 2 extra attacks if transforming from single to double attack.
		-- attacksDone prevents infinite abuse of 2 extra attacks:
		-- do normal attack -> transform to single-attack -> transform back to double-attack
		-- -> do normal attack -> ...
		return 2
	elseif hadDoubleAttack and not hasDoubleAttack and attacksRemain > 0 then
		-- Give nothing if transforming from double to single attack with attacks remain
		return 0
	-- Uncomment the following lines to deny free transformation on second attack if transforming to single
	--elseif hadDoubleAttack and not hasDoubleAttack and attacksDone > 0 then
	--	return 0
	else
		-- Give 1 extra attack to compensate transformation
		return 1
	end
end
