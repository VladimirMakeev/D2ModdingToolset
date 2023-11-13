function getLevel(attacker, target, item, battle)
    -- transform into unit with its level minus 1 and minus attacker over-level
    return math.max(1, target.impl.level - 1 - attacker.impl.level + attacker.baseImpl.level);
end
