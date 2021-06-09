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
	local result = {}

	-- Get targets closest to selected, including the selected (2x2 area splash)
	for i = 1, #targets do
		local target = targets[i]
		if target.column == selected.column then
			table.insert(result, target)
		elseif target.column - selected.column == 1 then
			table.insert(result, target)
		elseif target.column == 1 and selected.column == 2 then
			table.insert(result, target)
		end
	end
	
	return result
end
