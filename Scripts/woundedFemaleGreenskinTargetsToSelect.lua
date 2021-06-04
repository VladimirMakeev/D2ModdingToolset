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
	-- To demonstrate various API. Also for the lulz.
	local i = 1
	while i <= #targets do
		-- Only wounded female greenskins are allowed for selection
		local target = targets[i]
		if not target.unit.impl.male and (target.unit.hp < target.unit.hpMax) and (target.unit.impl.subrace == Subrace.NeutralGreenSkin) then
			i = i + 1
		else
			targets:erase(i)
		end
	end
end
