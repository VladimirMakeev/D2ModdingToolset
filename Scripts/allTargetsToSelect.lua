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
	-- Do nothing - allow any target to be selected
end
