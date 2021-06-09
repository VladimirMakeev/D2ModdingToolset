--[[
For complete API reference see
https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md

If the script is used as SEL_SCRIPT, no unit is selected yet (selected.position == -1).

Unit positions on a battlefield are mirrored.
Frontline positions are even, backline - odd.
1 0    0 1
3 2 vs 2 3
5 4    4 5
--]]

function getTargets(attacker, selected, allies, targets, targetsAreAllies)
	local result = { selected }

	-- Add 2 random closest targets (chain attack)
	math.randomseed(os.time())
	local current = selected
	for n = 1, 2 do
		-- Get closest targets (excluding already picked)
		local closest = {}
		for i = 1, #targets do
			local target = targets[i]
			if math.abs(target.line - current.line) < 2 and math.abs(target.column - current.column) < 2 then
				local unique = true
				for j = 1, #result do
					if result[j] == target then
						unique = false
						break
					end
				end				
				if unique then
					table.insert(closest, target)
				end
			end
		end
		
		-- Halt if no unique closest targets found
		if #closest == 0 then
			break
		end
		
		-- Pick a random closest target
		current = closest[math.random(#closest)]
		table.insert(result, current)
	end
	
	return result
end
