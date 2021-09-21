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
    local result = {selected}

    local others = {}
    for i = 1, #targets do
        local target = targets[i]
        if target ~= selected then
            table.insert(others, target)
        end
    end

    if #others > 0 then
        -- Pick any other target randomly
        math.randomseed(os.time())
        local other = others[math.random(#others)]
        table.insert(result, other)
    end

    return result
end
