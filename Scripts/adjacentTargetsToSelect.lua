--[[
For complete API reference see
https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md

Unit positions on a battlefield are mirrored.
Frontline positions are even, backline - odd.
1 0    0 1
3 2 vs 2 3
5 4    4 5
--]]
function filterTargetsToSelect(attacker, allies, targets, targetsAreAllies)
	-- If targets are enemies and the attacker stands on the backline (position % 2 ~= 0)
	if not targetsAreAllies and attacker.backline then
		for i = 1, #allies do
			-- If any ally stands on the frontline (position % 2 == 0)
			if allies[i].frontline then
				-- Ally prevents us to reach adjacent targets
				targets:clear()
				return
			end
		end
	end

	-- Determine closest distance to adjacent target (not counting self)
	local closestDistance = 99
	for i = 1, #targets do
		local target = targets[i]
		if target ~= attacker then
			local distance = target:distance(attacker)
			if closestDistance > distance then
				closestDistance = distance
			end
		end
	end

	-- Allow closest targets only (including self)
	local i = 1
	while i <= #targets do
		local target = targets[i]
		if target ~= attacker then
			if target:distance(attacker) > closestDistance then
				targets:erase(i)
			else
				i = i + 1
			end
		end
	end
end
