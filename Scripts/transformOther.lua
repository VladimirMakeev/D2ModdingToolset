function getLevel(attacker, target, transformImpl, item)
    -- transform using target level with a minimum of transform impl level
    return math.max(target.impl.level, transformImpl.level);
end
