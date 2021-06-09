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
	for i = 1, #targets do
		local target = targets[i]
		-- Only wounded female greenskins are allowed (to demonstrate various API, also for the lulz)
		if not target.unit.impl.male and (target.unit.hp < target.unit.hpMax) and (target.unit.impl.subrace == Subrace.NeutralGreenSkin) then
			table.insert(result, target)
		end
	end
	return result
end
