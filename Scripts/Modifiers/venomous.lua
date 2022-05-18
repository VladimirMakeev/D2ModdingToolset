function getModifierDescTxt()
	return Id.new("x000tg6051")
end

function getAttack2Id(unit, prev)
	if prev == Id.new("g000000000") then
		return Id.new("g000aa9020")
	end
	
	return prev
end

function getImmuneToAttack(unit, attack, prev)
	if attack == Attack.Poison then
		return Immune.Always
	end
	
	return prev
end
