--[[
For complete API reference see
https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md

get<Control>Text(unit, unitImpl[, <additional arguments>])
- 'unit' represents a game unit to which the value applies. Can be nil if this is unit-type encyclopedia (capital building menu for instance).
	An instance of https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md#unit-1

- 'unitImpl' represents a game unit implementation to which the value applies.
	An instance of https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/luaApi.md#unit-implementation

- Returns text of the control. If empty string is returned, a default value will be generated.

Helper functions:
- getInterfaceText(id)
	IMPORTANT: Use it instead of hard-coded string constants to make your scripts localizable.
- replace(string, keyword, replacement)
	Introduced for use instead of built-in string.gsub (so you don't have to escape "%" with "%%" in every single keyword and replacement strings).
- applyAttackDamageRatio(damage, ratio)
	Provides accurate rated damage value. The same function is used during battle for damage calculation.
- computeAttackDamageRatio(attack, targetCount)
	Provides sequence of damage ratios for the specified target count. The same function is used during battle for damage calculation.
- getBoostDamage(level)
	Provides percentage value of Boost Damage attack of the specified level.
- getLowerDamage(level)
	Provides percentage value of Lower Damage attack of the specified level.
- getLowerInitiative(level)
	Provides percentage value of Lower Initiative attack of the specified level.
- getCustomAttackSources()
	Returns list of custom attack sources.
- getCustomAttackSourceName(source)
	Returns display name of the specified custom attack source.
- getCustomAttackReachName(reach)
	Returns display name of the specified custom attack reach.
- getCustomAttackTargetsName(reach)
	Returns description of targets for the specified custom attack reach.
--]]

function contains(array, value)
	for _, item in ipairs(array) do
		if item == value then
			return true
		end
	end

	return false
end

function attackHasDamage(attack)
	if not attack then
		return false
	end

	local attackType = attack.type
	return
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow or
		attackType == Attack.Poison or
		attackType == Attack.Frostbite or
		attackType == Attack.Blister or
		attackType == Attack.Shatter
end

function attackHasHeal(attack)
	if not attack then
		return false
	end

	return
		attackType == Attack.Heal or
		attackType == Attack.BestowWards
end

function attackHasPower(attack)
	if not attack then
		return false
	end

	local attackType = attack.type
	return
		attackType == Attack.Paralyze or
		attackType == Attack.Petrify or
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow or
		attackType == Attack.Fear or
		attackType == Attack.LowerDamage or
		attackType == Attack.LowerInitiative or
		attackType == Attack.Poison or
		attackType == Attack.Frostbite or
		attackType == Attack.Blister or
		-- Don't forget to remove Attack.Shatter if you have allowShatterAttackToMiss = false in your settings.lua
		attackType == Attack.Shatter or
		attackType == Attack.DrainLevel or
		attackType == Attack.TransformOther
end

function attackHasCrit(unitImpl, attack)
	if not attack then
		return false
	end

	local attackType = attack.type
	if
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow then
		return attack.crit or unitImpl:hasAbility(Ability.CriticalHit)
	end

	return false
end

function attackHasInfinite(attack)
	if not attack then
		return false
	end

	local attackType = attack.type
	return
		attackType == Attack.Paralyze or
		attackType == Attack.Petrify or
		attackType == Attack.BoostDamage or
		attackType == Attack.LowerDamage or
		attackType == Attack.LowerInitiative or
		attackType == Attack.Poison or
		attackType == Attack.Frostbite or
		attackType == Attack.Blister or
		attackType == Attack.TransformOther
end

function attackHasDrain(attack)
	if not attack then
		return false
	end

	local attackType = attack.type
	return
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow
end

function getAttackSourceName(source)
	if source == Source.Weapon then
		return getInterfaceText("X005TA0145") -- "Weapon"
	elseif source == Source.Mind then
		return getInterfaceText("X005TA0146") -- "Mind"
	elseif source == Source.Life then
		return getInterfaceText("X005TA0147") -- "Life"
	elseif source == Source.Death then
		return getInterfaceText("X005TA0148") -- "Death"
	elseif source == Source.Fire then
		return getInterfaceText("X005TA0149") -- "Fire"
	elseif source == Source.Water then
		return getInterfaceText("X005TA0150") -- "Water"
	elseif source == Source.Air then
		return getInterfaceText("X005TA0151") -- "Air"
	elseif source == Source.Earth then
		return getInterfaceText("X005TA0152") -- "Earth"
	else
		return getCustomAttackSourceName(source)
	end
end

function getAttackClassName(class)
	if class == Attack.Paralyze then
		return getInterfaceText("X005TA0789")
	elseif class == Attack.Petrify then
		return getInterfaceText("X005TA0790")
	elseif class == Attack.Damage then
		return getInterfaceText("X005TA0791")
	elseif class == Attack.Drain then
		return getInterfaceText("X005TA0792")
	elseif class == Attack.Heal then
		return getInterfaceText("X005TA0793")
	elseif class == Attack.Fear then
		return getInterfaceText("X005TA0794")
	elseif class == Attack.BoostDamage then
		return getInterfaceText("X005TA0795")
	elseif class == Attack.LowerDamage then
		return getInterfaceText("X005TA0796")
	elseif class == Attack.LowerInitiative then
		return getInterfaceText("X005TA0797")
	elseif class == Attack.Poison then
		return getInterfaceText("X005TA0798")
	elseif class == Attack.Frostbite then
		return getInterfaceText("X005TA0799")
	elseif class == Attack.Blister then
		return getInterfaceText("X160TA0012")
	elseif class == Attack.BestowWards then
		return getInterfaceText("X160TA0014")
	elseif class == Attack.Shatter then
		return getInterfaceText("X160TA0020")
	elseif class == Attack.Revive then
		return getInterfaceText("X005TA0800")
	elseif class == Attack.DrainOverflow then
		return getInterfaceText("X005TA0801")
	elseif class == Attack.Cure then
		return getInterfaceText("X005TA0802")
	elseif class == Attack.Summon then
		return getInterfaceText("X005TA0803")
	elseif class == Attack.DrainLevel then
		return getInterfaceText("X005TA0804")
	elseif class == Attack.GiveAttack then
		return getInterfaceText("X005TA0805")
	elseif class == Attack.Doppelganger then
		return getInterfaceText("X005TA0806")
	elseif class == Attack.TransformSelf then
		return getInterfaceText("X005TA0807")
	elseif class == Attack.TransformOther then
		return getInterfaceText("X005TA0808")
	else
		return ""
	end
end

function getAttackReachName(reach)
	if reach == Reach.Adjacent then
		return getInterfaceText("X005TA0201") -- "Adjacent units"
	elseif reach == Reach.All or reach == Reach.Any then
		return getInterfaceText("X005TA0200") -- "Any unit"
	else
		return getCustomAttackReachName(reach)
	end
end

function getAttackTargetsName(reach)
	if reach == Reach.All then
		return getInterfaceText("X005TA0674") -- "6"
	elseif reach == Reach.Any or reach == Reach.Adjacent then
		return getInterfaceText("X005TA0675") -- "1"
	else
		return getCustomAttackTargetsName(reach)
	end
end

function getImmunityAttackSources(unitImpl, immune)
	local sources = {
		Source.Weapon,
		Source.Mind,
		Source.Life,
		Source.Death,
		Source.Fire,
		Source.Water,
		Source.Air,
		Source.Earth,
		table.unpack(getCustomAttackSources())
	}

	local result = {}
	for _, source in ipairs(sources) do
		if unitImpl:getImmuneToAttackSource(source) == immune then
			table.insert(result, source)
		end
	end

	return result
end

function getImmunityAttackClasses(unitImpl, immune)
	local classes = {
		Attack.Damage,
		Attack.Drain,
		Attack.Paralyze,
		Attack.Heal,
		Attack.Fear,
		Attack.BoostDamage,
		Attack.Petrify,
		Attack.LowerDamage,
		Attack.LowerInitiative,
		Attack.Poison,
		Attack.Frostbite,
		Attack.Revive,
		Attack.DrainOverflow,
		Attack.Cure,
		Attack.Summon,
		Attack.DrainLevel,
		Attack.GiveAttack,
		Attack.Doppelganger,
		Attack.TransformSelf,
		Attack.TransformOther,
		Attack.Blister,
		Attack.BestowWards,
		Attack.Shatter
	}

	local result = {}
	for _, class in ipairs(classes) do
		if unitImpl:getImmuneToAttackClass(class) == immune then
			table.insert(result, class)
		end
	end

	return result
end

function getSplitDamageMultiplier(attack)
	-- Replace '1' with your splitDamageMultiplier from settings.lua (no script access to settings just yet)
	local splitDamageMultiplier = 1

	local attackType = attack.type
	if
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow then
		if splitDamageMultiplier ~= 1 and attack.damageSplit then
			return splitDamageMultiplier
		end
	end

	return 1
end

function getAttackDamage(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	local damage = attack.damage
	local boost = getBoostDamage(boostDamageLevel) - getLowerDamage(lowerDamageLevel)
	return math.min(damage + damage * boost / 100, unitImpl.damageMax) * getSplitDamageMultiplier(attack)
end

function getAttackDrain(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	if not attackHasDrain(attack) then
		return 0
	end

	local damage = getAttackDamage(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	local attackType = attack.type
	if attackType == Attack.Drain then
		-- Replace '0.5' with your drainAttackHeal from settings.lua (no script access to settings just yet)
		return attack:getDrain(damage) + damage * 0.5
	elseif attackType == Attack.DrainOverflow then
		-- Replace '0.5' with your drainOverflowHeal from settings.lua (no script access to settings just yet)
		return attack:getDrain(damage) + damage * 0.5
	else
		return attack:getDrain(damage)
	end
end

function getUnitArmor(unitImpl, isInBattle, fortificationArmor, shatteredArmor)
	-- Fortification armor is already added as unit modifier during battle
	local totalArmor = isInBattle and unitImpl.armor or math.min(unitImpl.armor + fortificationArmor, 90)

	-- Fortification armor is unshatterable
	return math.max(fortificationArmor, totalArmor - shatteredArmor)
end

function getNumberText(value, percent)
	return string.format(percent and "%d%%" or "%d", value)
end

function getSignedNumberText(value, percent)
	return string.format(percent and "%+d%%" or "%+d", value)
end

function getBonusNumberText(bonus, percent, negative)
	local numberText = getNumberText(math.abs(bonus), percent)
	local signText = bonus >= 0 and "+" or "-"
	if bonus >= 0 or negative then
		-- Replace hard-coded string with getInterfaceText of your positiveBonusNumber from textids.lua
		return replace(replace("\\c025;090;000;%SIGN% %NUMBER%\\c000;000;000;", "%SIGN%", signText), "%NUMBER%", numberText)
	else
		-- Replace hard-coded string with getInterfaceText of your negativeBonusNumber from textids.lua
		return replace(replace("\\c100;000;000;%SIGN% %NUMBER%\\c000;000;000;", "%SIGN%", signText), "%NUMBER%", numberText)
	end
end

function getNumberWithBonusText(numberText, bonus, percent, negative)
	if bonus == 0 then
		return numberText
	end

	-- Replace hard-coded string with getInterfaceText of your modifiedNumber from textids.lua
	return replace(replace("%NUMBER% %BONUS%", "%NUMBER%", numberText), "%BONUS%", getBonusNumberText(bonus, percent, negative))
end

function getModifiedNumberText(value, base, percent, negative, full)
	local bonus = value - base
	if bonus == 0 then
		return getNumberText(base, percent)
	end

	if base == 0 and not full then
		return getBonusNumberText(bonus, percent, negative)
	end

	return getNumberWithBonusText(getNumberText(base, percent), bonus, percent, negative)
end

function getModifiedStringText(value, modified)
	if not modified then
		return value
	end

	-- Replace hard-coded string with getInterfaceText of your modifiedValue from textids.lua
	return replace("\\c025;090;000;%VALUE%\\c000;000;000;", "%VALUE%", value)
end

function getTextWithCritText(text, critText)
	-- Replace hard-coded string with getInterfaceText of your critHitDamage from textids.lua
	return replace(replace("%DMG% (%CRIT%)", "%DMG%", text), "%CRIT%", critText)
end

function getTextWithFullCritText(text, critText)
	-- "Critical hit"
	return getTextWithCritText(text, string.format("%s %s", getInterfaceText("X160TA0017"), critText))
end

function getDamageText(value, base, maximum)
	local result = getModifiedNumberText(value, base, false)
	if value == maximum then
		-- %DMG% \c000;000;000;(\c128;000;000;Max\c000;000;000;)
		result = replace(getInterfaceText("X005TA0811"), "%DMG%", result)
	end

	return result
end

function getSplitDamageText(damageText)
	-- Replace hard-coded string with getInterfaceText of your splitDamage from textids.lua
	return replace("%DMG% split between targets", "%DMG%", damageText)
end

function getRatedDamageText(damage, critDamage, ratio)
	local result = getNumberText(applyAttackDamageRatio(damage, ratio), false)

	if critDamage ~= 0 then
		local ratedCritDamage = applyAttackDamageRatio(critDamage, ratio)
		result = getTextWithCritText(result, getNumberText(ratedCritDamage, false))
	end

	return result
end

function getRatedDamageTextForMaxTargets(damageText, attack, damage, critDamage, maxTargets)
	local ratios = computeAttackDamageRatio(attack, maxTargets)
	if #ratios < 2 then
		return damageText
	end

	if not attack.damageRatioPerTarget and #ratios > 2 then
		local targetsText = getNumberText(#ratios - 1, false)
		local ratedText = getRatedDamageText(damage, critDamage, ratios[2])

		-- Replace hard-coded string with getInterfaceText of your ratedDamageEqual from textids.lua
		return replace(replace(replace("%DMG%, (%TARGETS%x) %RATED%", "%DMG%", damageText), "%TARGETS%", targetsText), "%RATED%", ratedText)
	else
		local ratedText = ""
		for i = 2, #ratios do
			if ratedText ~= "" then
				-- Replace hard-coded string with getInterfaceText of your ratedDamageSeparator from textids.lua
				ratedText = ratedText .. ", "
			end
			ratedText = ratedText .. getRatedDamageText(damage, critDamage, ratios[i])
		end

		-- Replace hard-coded string with getInterfaceText of your ratedDamage from textids.lua
		return replace(replace("%DMG%, %RATED%", "%DMG%", damageText), "%RATED%", ratedText)
	end
end

function getAttackWithAttack2NumberText(attack1NumberText, attack2NumberText)
	if attack1NumberText == "0" then
		return attack2NumberText
	elseif attack2NumberText ~= "0" then
		-- " / %ATTACK%"
		return attack1NumberText .. replace(getInterfaceText("X005TA0785"), "%ATTACK%", attack2NumberText)
	else
		return attack1NumberText
	end
end

function getAttackNameText(attack)
	return getModifiedStringText(attack.name, attack.name ~= attack.generated.name)
end

function getAttackPowerText(unitImpl, attack)
	local result
	local generated = attack.generated
	if not attackHasPower(attack) then
		result = getNumberText(100, true)
	elseif attackHasPower(generated) then
		result = getModifiedNumberText(attack.power, generated.power, true)
	else
		result = getNumberText(attack.power, true)
	end

	if attackHasCrit(unitImpl, attack) then
		local baseCritPower = attackHasCrit(unitImpl.generated, generated) and generated.critPower or 0
		result = getTextWithFullCritText(result, getModifiedNumberText(attack.critPower, baseCritPower, true))
	end

	return result
end

function getAttackDurationText(attack)
	-- Replace hard-coded string with getInterfaceText of your instantDurationText from textids.lua
	-- Replace hard-coded string with getInterfaceText of your randomDurationText from textids.lua
	-- Replace hard-coded string with getInterfaceText of your singleTurnDurationText from textids.lua
	-- Replace hard-coded string with getInterfaceText of your wholeBattleDurationText from textids.lua
	local modified = attack.infinite ~= attack.generated.infinite
	local attackType = attack.type
	if
		attackType == Attack.Paralyze or
		attackType == Attack.Petrify or
		attackType == Attack.Poison or
		attackType == Attack.Frostbite or
		attackType == Attack.Blister then
		return getModifiedStringText(attack.infinite and "Random" or "Single turn", modified)
	elseif
		attackType == Attack.BoostDamage or
		attackType == Attack.LowerDamage or
		attackType == Attack.LowerInitiative then
		return getModifiedStringText(attack.infinite and "Whole battle" or "Single turn", modified)
	elseif attackType == Attack.TransformOther then
		return getModifiedStringText(attack.infinite and "Random" or "Whole battle", modified)
	else
		return "Instant"
	end
end

function getBoostDamageAttackDamageText(attack)
	local generated = attack.generated
	local value = getBoostDamage(attack.level)
	local base = generated.type == Attack.BoostDamage and getBoostDamage(generated.level) or value

	-- "+%BOOST%"
	local baseText = replace(getInterfaceText("X005TA0535"), "%BOOST%", getNumberText(base, false))
	return getNumberWithBonusText(baseText, value - base, true, false)
end

function getLowerDamageAttackDamageText(attack)
	local generated = attack.generated
	local value = getLowerDamage(attack.level)
	local base = generated.type == Attack.LowerDamage and getLowerDamage(generated.level) or value

	-- "-%LOWER%"
	local baseText = replace(getInterfaceText("X005TA0550"), "%LOWER%", getNumberText(base, false))
	return getNumberWithBonusText(baseText, value - base, true, true)
end

function getLowerInitiativeAttackDamageText(attack)
	local generated = attack.generated
	local value = getLowerInitiative(attack.level)
	local base = generated.type == Attack.LowerInitiative and getLowerInitiative(generated.level) or value

	-- "-%LOWER%"
	local baseText = replace(getInterfaceText("X005TA0550"), "%LOWER%", getNumberText(base, false))
	return getNumberWithBonusText(baseText, value - base, true, true)
end

function getDamageDrainAttackDamageText(unitImpl, attack, boostDamageLevel, lowerDamageLevel, maxTargets)
	local generated = attack.generated
	local damage = getAttackDamage(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	local result = getDamageText(damage, generated.damage * getSplitDamageMultiplier(generated), unitImpl.damageMax * getSplitDamageMultiplier(attack))

	local critDamage = 0
	if attackHasCrit(unitImpl, attack) then
		critDamage = damage * attack.critDamage / 100
		local baseCritDamage = attackHasCrit(unitImpl.generated, generated) and damage * generated.critDamage / 100 or 0
		result = getTextWithFullCritText(result, getModifiedNumberText(critDamage, baseCritDamage, false))
	end

	if maxTargets < 2 then
		return result
	elseif attack.damageSplit then
		return getSplitDamageText(result)
	else
		return getRatedDamageTextForMaxTargets(result, attack, damage, critDamage, maxTargets)
	end
end

function getAttackDamageText(unitImpl, attack, boostDamageLevel, lowerDamageLevel, maxTargets)
	local attackType = attack.type
	if attackType == Attack.BoostDamage then
		return getBoostDamageAttackDamageText(attack)
	elseif attackType == Attack.LowerDamage then
		return getLowerDamageAttackDamageText(attack)
	elseif attackType == Attack.LowerInitiative then
		return getLowerInitiativeAttackDamageText(attack)
	elseif
		attackType == Attack.Damage or
		attackType == Attack.Drain or
		attackType == Attack.DrainOverflow then
		return getDamageDrainAttackDamageText(unitImpl, attack, boostDamageLevel, lowerDamageLevel, maxTargets)
	elseif
		attackType == Attack.Heal or
		attackType == Attack.BestowWards then
		return getModifiedNumberText(attack.heal, attack.generated.heal, false)
	elseif
		attackType == Attack.Poison or
		attackType == Attack.Frostbite or
		attackType == Attack.Blister then
		-- DOT attacks do no scale from modifiers thus only generated damage is used
		local damage = attack.generated.damage
		return getDamageText(damage, damage, unitImpl.damageMax)
	elseif attackType == Attack.Shatter then
		-- Replace '100' with your shatterDamageMax from settings.lua (no script access to settings just yet)
		return getDamageText(attack.damage, attack.generated.damage, math.min(100, unitImpl.damageMax))
	else
		return getNumberText(0, false)
	end
end

function getAttackDrainText(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	local generated = attack.generated
	local value = getAttackDrain(unitImpl, attack, boostDamageLevel, lowerDamageLevel)
	local base = getAttackDrain(unitImpl, generated, 0, 0)
	local result = getModifiedNumberText(value, base, false)

	local attackType = attack.type
	if attackType == Attack.DrainOverflow then
		-- Replace hard-coded string with getInterfaceText of your overflowText from textids.lua
		-- Replace hard-coded string with getInterfaceText of your overflowAttack from textids.lua
		return replace(replace("%ATTACK% (%OVERFLOW%)", "%ATTACK%", result), "%OVERFLOW%", getModifiedStringText("Overflow", attackType ~= generated.type))
	else
		return result
	end
end

function getAttackSourceText(attack)
	return getModifiedStringText(getAttackSourceName(attack.source), attack.source ~= attack.generated.source)
end

function getDynUpgradeText(field, upgrade1, upgrade2, percent)
	local upgrade1Text = getSignedNumberText(upgrade1, percent)
	local upgrade2Text = getSignedNumberText(upgrade2, percent)

	-- Replace hard-coded string with getInterfaceText of your dynamicUpgradeValues from textids.lua
	return replace(replace(replace("%STAT% (%UPG1% | %UPG2% per level)", "%STAT%", field), "%UPG1%", upgrade1Text), "%UPG2%", upgrade2Text)
end

function getTxtStatsWithDynUpgradeText(text, unitImpl)
	local upg1 = unitImpl.dynUpg1
	local upg2 = unitImpl.dynUpg2
	if not upg1 or not upg2 then
		return text
	end

	-- Replace hard-coded string with getInterfaceText of your dynamicUpgradeLevel from textids.lua
	local result = replace(text, "%LEVEL%", replace("%LEVEL% (before | after %UPGLV%)", "%UPGLV%", getNumberText(unitImpl.dynUpgLvl, false)))
	result = replace(result, "%HP2%", getDynUpgradeText("%HP2%", upg1.hp, upg2.hp, false))
	result = replace(result, "%ARMOR%", getDynUpgradeText("%ARMOR%", upg1.armor, upg2.armor, false))
	result = replace(result, "%XP%", getDynUpgradeText("%XP%", upg1.xpNext, upg2.xpNext, false))

	return result
end

function getTxtStats2WithDynUpgradeText(text, unitImpl)
	local upg1 = unitImpl.dynUpg1
	local upg2 = unitImpl.dynUpg2
	if not upg1 or not upg2 then
		return text
	end

	result = replace(result, "%REGEN%", getDynUpgradeText("%REGEN%", upg1.regen, upg2.regen, false))
	result = replace(result, "%XPKILL%", getDynUpgradeText("%XPKILL%", upg1.xpKilled, upg2.xpKilled, false))

	return result
end

function getTxtAttackInfoWithDynUpgradeText(text, unitImpl)
	local upg1 = unitImpl.dynUpg1
	local upg2 = unitImpl.dynUpg2
	if not upg1 or not upg2 then
		return text
	end

	local result = replace(text, "%INIT%", getDynUpgradeText("%INIT%", upg1.initiative, upg2.initiative, false))

	if attackHasDamage(unitImpl.attack1) or attackHasDamage(unitImpl.attack2) then
		result = replace(result, "%DAMAGE%", getDynUpgradeText("%DAMAGE%", upg1.damage, upg2.damage, false))
	elseif attackHasHeal(unitImpl.attack1) or attackHasHeal(unitImpl.attack2) then
		result = replace(result, "%DAMAGE%", getDynUpgradeText("%DAMAGE%", upg1.heal, upg2.heal, false))
	end

	if attackHasPower(unitImpl.attack1) or attackHasPower(unitImpl.attack2) then
		result = replace(result, "%HIT2%", getDynUpgradeText("%HIT2%", upg1.power, upg2.power, true))
	end

	return result
end

function getLevelField(unitImpl)
	return getNumberText(unitImpl.level, false)
end

function getHp1Field(unit, unitImpl)
	return getNumberText(unit and unit.hp or unitImpl.hp, false)
end

function getHp2Field(unit, unitImpl)
	if not unit then
		return getNumberText(unitImpl.hp, false)
	end

	if unit.hpMax ~= unitImpl.hp then
		-- Unit is transformed with its original hpMax preserved (by Transform Other attack)
		return getNumberText(unit.hpMax, false)
	end

	return getModifiedNumberText(unit.hpMax, unitImpl.generated.hp, false)
end

function getArmorField(unitImpl, isInBattle, fortificationArmor, shatteredArmor)
	local unitArmor = getUnitArmor(unitImpl, isInBattle, fortificationArmor, shatteredArmor)
	return getModifiedNumberText(unitArmor, unitImpl.generated.armor, false)
end

function getXpField(unit, unitImpl, isMaxLevel)
	if isMaxLevel then
		-- \c128;000;000;Max\c000;000;000;
		return getInterfaceText("X005TA0648")
	end

	-- %XP1% / %XP2%
	local result = getInterfaceText("X005TA0649")
	result = replace(result, "%XP1%", getNumberText(unit and unit.xp or 0, false))
	result = replace(result, "%XP2%", getModifiedNumberText(unitImpl.xpNext, unitImpl.generated.xpNext, false, true))

	return result
end

function getImmuOrWardField(unitImpl, immune, removedSourceWards, removedClassWards)
	local result = ""

	local sources = getImmunityAttackSources(unitImpl, immune)
	for _, source in ipairs(sources) do
		local sourceText = getAttackSourceName(source)
		if immune == Immune.Once and contains(removedSourceWards, source) then
			-- Replace hard-coded string with getInterfaceText of your removedAttackWard from textids.lua
			sourceText = replace("\\fMedBold;\\c100;000;000;%WARD%\\c000;000;000;\\fNormal;", "%WARD%", sourceText)
		else
			sourceText = getModifiedStringText(sourceText, immune ~= unitImpl.generated:getImmuneToAttackSource(source))
		end

		if result ~= "" then
			result = result .. ", "
		end
		result = result .. sourceText
	end

	local classes = getImmunityAttackClasses(unitImpl, immune)
	for _, class in ipairs(classes) do
		local classText = getAttackClassName(class)
		if immune == Immune.Once and contains(removedClassWards, class) then
			-- Replace hard-coded string with getInterfaceText of your removedAttackWard from textids.lua
			classText = replace("\\fMedBold;\\c100;000;000;%WARD%\\c000;000;000;\\fNormal;", "%WARD%", classText)
		else
			classText = getModifiedStringText(classText, immune ~= unitImpl.generated:getImmuneToAttackClass(class))
		end

		if result ~= "" then
			result = result .. ", "
		end
		result = result .. classText
	end

	-- "None"
	return result ~= "" and result or getInterfaceText("X005TA0469")
end

function getImmuField(unitImpl)
	return getImmuOrWardField(unitImpl, Immune.Always)
end

function getWardField(unitImpl, removedSourceWards, removedClassWards)
	return getImmuOrWardField(unitImpl, Immune.Once, removedSourceWards, removedClassWards)
end

function getEffhpField(unit, unitImpl, isInBattle, fortificationArmor, shatteredArmor)
	local armor = getUnitArmor(unitImpl, isInBattle, fortificationArmor, shatteredArmor)
	if armor >= 100 then
		return getNumberText(2147483647, false) -- max int32
	end

	local hp = unit and unit.hp or unitImpl.hp
	return getNumberText(math.floor(hp * 100 / (100 - armor)), false)
end

function getRegenField(unitImpl, unitRegen)
	return getModifiedNumberText(unitRegen, unitImpl.generated.regen, true)
end

function getXpKillField(unitImpl)
	return getModifiedNumberText(unitImpl.xpKilled, unitImpl.generated.xpKilled, false, true)
end

function getApField(unit, unitImpl)
	if unit then
		return ""
	end

	-- \fmedbold;Move points:\t\fnormal;%AP%\n
	local result = getInterfaceText("X005TA0650")
	return replace(result, "%AP%", getNumberText(unitImpl.movement, false))
end

function getLdrshpField(unitImpl)
	-- \s110;\fmedbold;Leadership:\t\fnormal;%MAX%\n
	local result = getInterfaceText("X005TA0583")
	return replace(result, "%MAX%", getNumberText(unitImpl.leadership - 1, false))
end

function getNbbatField(unit)
	if not unit then
		return ""
	end

	-- \s110;\fMedBold;Battles won:\t\fNormal;%VALUE%\n
	local result = getInterfaceText("X005TA0737")
	return replace(result, "%VALUE%", getNumberText(getScenario():findStackByUnit(unit).battlesWon, false))
end

function getTwiceField(unitImpl)
	if not unitImpl.attacksTwice then
		return ""
	end

	-- "(2x) %BLANK%"
	return getModifiedStringText(replace(getInterfaceText("X005TA0786"), "%BLANK%", ""), not unitImpl.generated.attacksTwice)
end

function getAltAttackField(unitImpl)
	if not unitImpl.altAttack then
		return ""
	end

	-- "%ATTACK% or %BLANK%"
	return replace(replace(getInterfaceText("X005TA0829"), "%ATTACK%", getAttackNameText(unitImpl.altAttack)), "%BLANK%", "")
end

function getAttackField(unitImpl)
	return getAttackNameText(unitImpl.attack1)
end

function getSecondField(unitImpl)
	if not unitImpl.attack2 then
		return ""
	end

	-- " / %ATTACK%"
	return replace(getInterfaceText("X005TA0785"), "%ATTACK%", getAttackNameText(unitImpl.attack2))
end

function getHitField(unitImpl)
	return getAttackPowerText(unitImpl, unitImpl.attack1)
end

function getHit2Field(unitImpl)
	if not unitImpl.attack2 then
		return ""
	end

	-- " / %POWER%"
	return replace(getInterfaceText("X005TA0881"), "%POWER%", getAttackPowerText(unitImpl, unitImpl.attack2))
end

function getEffectField(unitImpl)
	local attackType = unitImpl.attack1.type
	if
		attackType == Attack.Heal or
		attackType == Attack.BestowWards then
		return getInterfaceText("X005TA0504") -- "Heal"
	elseif attackType == Attack.BoostDamage then
		return getInterfaceText("X005TA0534") -- "Boost"
	elseif attackType == Attack.LowerDamage then
		return getInterfaceText("X005TA0547") -- "Lower"
	elseif attackType == Attack.LowerInitiative then
		return getInterfaceText("X005TA0551") -- "Lower initiative"
	else
		return getInterfaceText("X005TA0503") -- "Damage"
	end
end

function getDurationField(unitImpl)
	if not attackHasInfinite(unitImpl.attack1) and not attackHasInfinite(unitImpl.attack2) then
		return ""
	end

	local duration = getAttackDurationText(unitImpl.attack1)
	if unitImpl.attack2 then
		-- " / %ATTACK%"
		duration = duration .. replace(getInterfaceText("X005TA0785"), "%ATTACK%", getAttackDurationText(unitImpl.attack2))
	end

	-- Replace hard-coded string with getInterfaceText of your durationText from textids.lua
	-- Replace hard-coded string with getInterfaceText of your durationDescription from textids.lua
	return replace(replace("\\fMedBold%DURATION%:\\t\\fNormal%DURATIONVALUE%\\n", "%DURATION%", "Duration"), "%DURATIONVALUE%", duration)
end

function getDamageField(unitImpl, boostDamageLevel, lowerDamageLevel)
	local maxTargets = unitImpl.attack1.maxTargets
	local result = getAttackDamageText(unitImpl, unitImpl.attack1, boostDamageLevel, lowerDamageLevel, maxTargets)
	if unitImpl.attack2 then
		result = getAttackWithAttack2NumberText(result, getAttackDamageText(unitImpl, unitImpl.attack2, boostDamageLevel, lowerDamageLevel, maxTargets))
	end

	return result
end

function getDrainField(unitImpl, boostDamageLevel, lowerDamageLevel)
	if not attackHasDrain(unitImpl.attack1) and not attackHasDrain(unitImpl.attack2) then
		return ""
	end

	local drain = getAttackDrainText(unitImpl, unitImpl.attack1, boostDamageLevel, lowerDamageLevel)
	if unitImpl.attack2 then
		drain = getAttackWithAttack2NumberText(drain, getAttackDrainText(unitImpl, unitImpl.attack2, boostDamageLevel, lowerDamageLevel))
	end

	-- Replace hard-coded string with getInterfaceText of your drainDescription from textids.lua
	return replace(replace("\\fMedBold%DRAINEFFECT%:\\t\\fNormal%DRAIN%\\n", "%DRAINEFFECT%", getInterfaceText("X005TA0792")), "%DRAIN%", drain)
end

function getSourceField(unitImpl)
	local result = getAttackSourceText(unitImpl.attack1)
	if unitImpl.altAttack then
		-- "%ATTACK% or %BLANK%"
		result = replace(replace(getInterfaceText("X005TA0829"), "%ATTACK%", getAttackSourceText(unitImpl.altAttack)), "%BLANK%", "")
	end

	return result
end

function getSource2Field(unitImpl)
	if not unitImpl.attack2 then
		return ""
	end

	-- " / %SOURCE%"
	return replace(getInterfaceText("X005TA0816"), "%SOURCE%", getAttackSourceText(unitImpl.attack2))
end

function getInitField(unitImpl, lowerInitiativeLevel)
	local attack = unitImpl.attack1
	local initiative = attack.initiative
	return getModifiedNumberText(initiative - initiative * getLowerInitiative(lowerInitiativeLevel) / 100, attack.generated.initiative, false)
end

function getReachField(unitImpl)
	local attack = unitImpl.attack1
	return getModifiedStringText(getAttackReachName(attack.reach), attack.reach ~= attack.generated.reach)
end

function getTargetsField(unitImpl)
	local attack = unitImpl.attack1
	return getModifiedStringText(getAttackTargetsName(attack.reach), attack.reach ~= attack.generated.reach)
end

function doGetTxtStatsText(unit, unitImpl, isMaxLevel, isInBattle, fortificationArmor, shatteredArmor, removedSourceWards, removedClassWards)
	-- \s110
	-- \fmedboldLevel:\t\fnormal%LEVEL%\n
	-- \fMedboldXP:\t\fNormal%XP%\n
	-- \fMedboldHP:\t\fnormal%HP1% / %HP2%\n
	-- \fMedboldArmor:\t\fNormal%ARMOR%\n
	-- \fMedboldImmunities:\t\fNormal\p110%IMMU%\mL0
	-- \fMedboldWards:\t\fNormal\p110%WARD%
	local text = getInterfaceText("X005TA0423")
	if not unit then
		-- Only add dynamic upgrade values for unit-type encyclopedia (to avoid screen cluttering)
		text = getTxtStatsWithDynUpgradeText(text, unitImpl)
	end
	text = replace(text, "%LEVEL%", getLevelField(unitImpl))
	text = replace(text, "%HP1%", getHp1Field(unit, unitImpl))
	text = replace(text, "%HP2%", getHp2Field(unit, unitImpl))
	text = replace(text, "%ARMOR%", getArmorField(unitImpl, isInBattle, fortificationArmor, shatteredArmor))
	text = replace(text, "%XP%", getXpField(unit, unitImpl, isMaxLevel))
	text = replace(text, "%IMMU%", getImmuField(unitImpl))
	text = replace(text, "%WARD%", getWardField(unitImpl, removedSourceWards, removedClassWards))
	return text
end

function doGetTxtStats2Text(unit, unitImpl, isInBattle, fortificationArmor, shatteredArmor, unitRegen, pattern)
	local text = pattern
	if not unit then
		-- Only add dynamic upgrade values for unit-type encyclopedia (to avoid screen cluttering)
		text = getTxtStats2WithDynUpgradeText(text, unitImpl)
	end
	text = replace(text, "%EFFHP%", getEffhpField(unit, unitImpl, isInBattle, fortificationArmor, shatteredArmor))
	text = replace(text, "%REGEN%", getRegenField(unitImpl, unitRegen))
	text = replace(text, "%XPKILL%", getXpKillField(unitImpl))
	return text
end

function doGetTxtLeaderInfoText(unit, unitImpl)
	-- \c000;000;000;%LDRSHP%%AP%%NBBAT%
	local text = getInterfaceText("X005TA0809")
	text = replace(text, "%AP%", getApField(unit, unitImpl))
	text = replace(text, "%LDRSHP%", getLdrshpField(unitImpl))
	text = replace(text, "%NBBAT%", getNbbatField(unit))
	return text
end

function doGetTxtAttackInfoText(unit, unitImpl, boostDamageLevel, lowerDamageLevel, lowerInitiativeLevel)
	-- NOTE that unitImpl.generated.attack1/2 is not always the same as unitImpl.attack1/2.generated.
	-- This is because unit attack can be added/removed/changed by custom modifier's getAttack(2)Id.
	-- Use unitImpl.attack1/2.generated to get naked leveled version (without modified stats) of the same attack.

	-- "%PART1%%PART2%"
	local text = getInterfaceText("X005TA0424")
	-- \s110
	-- \fMedBoldAttack:\t\p110\fNormal%TWICE%ALTATTACK%ATTACK%SECOND%\p0\n
	-- \fMedBoldChances to hit:\t\fNormal%HIT%HIT2%\n
	text = replace(text, "%PART1%", getInterfaceText("X005TA0787"))
	-- \fMedBold%EFFECT%:\t\fNormal%DAMAGE%\n
	-- \fMedBoldSource:\t\fNormal%SOURCE%SOURCE2%\n
	-- \fMedBoldInitiative:\t\fNormal%INIT%\n
	-- \fMedBoldReach:\t\fNormal%REACH%\n
	-- \fMedBoldTargets:\t\fNormal%TARGETS%
	text = replace(text, "%PART2%", getInterfaceText("X005TA0788"))
	if not unit then
		-- Only add dynamic upgrade values for unit-type encyclopedia (to avoid screen cluttering)
		text = getTxtAttackInfoWithDynUpgradeText(text, unitImpl)
	end
	text = replace(text, "%TWICE%", getTwiceField(unitImpl))
	text = replace(text, "%ALTATTACK%", getAltAttackField(unitImpl))
	text = replace(text, "%ATTACK%", getAttackField(unitImpl))
	text = replace(text, "%SECOND%", getSecondField(unitImpl))
	text = replace(text, "%HIT%", getHitField(unitImpl))
	text = replace(text, "%HIT2%", getHit2Field(unitImpl))
	text = replace(text, "%EFFECT%", getEffectField(unitImpl))
	text = replace(text, "%DURATION%", getDurationField(unitImpl))
	text = replace(text, "%DAMAGE%", getDamageField(unitImpl, boostDamageLevel, lowerDamageLevel))
	text = replace(text, "%DRAIN%", getDrainField(unitImpl, boostDamageLevel, lowerDamageLevel))
	text = replace(text, "%SOURCE%", getSourceField(unitImpl))
	text = replace(text, "%SOURCE2%", getSource2Field(unitImpl))
	text = replace(text, "%INIT%", getInitField(unitImpl, lowerInitiativeLevel))
	text = replace(text, "%REACH%", getReachField(unitImpl))
	text = replace(text, "%TARGETS%", getTargetsField(unitImpl))	
	return text
end

-- TODO: debug and remove
function getTxtStatsText(unit, unitImpl, isMaxLevel, isInBattle, fortificationArmor, shatteredArmor, removedSourceWards, removedClassWards)
	local success, result = xpcall(doGetTxtStatsText, debug.traceback, unit, unitImpl, isMaxLevel, isInBattle, fortificationArmor, shatteredArmor, removedSourceWards, removedClassWards)
	if not success then
		log(result)
	end

	return result
end

-- TODO: debug and remove
function getTxtStats2Text(unit, unitImpl, isInBattle, fortificationArmor, shatteredArmor, unitRegen, pattern)
	local success, result = xpcall(doGetTxtStats2Text, debug.traceback, unit, unitImpl, isInBattle, fortificationArmor, shatteredArmor, unitRegen, pattern)
	if not success then
		log(result)
	end

	return result
end

-- TODO: debug and remove
function getTxtLeaderInfoText(unit, unitImpl)
	local success, result = xpcall(doGetTxtLeaderInfoText, debug.traceback, unit, unitImpl)
	if not success then
		log(result)
	end

	return result
end

-- TODO: debug and remove
function getTxtAttackInfoText(unit, unitImpl, boostDamageLevel, lowerDamageLevel, lowerInitiativeLevel)
	local success, result = xpcall(doGetTxtAttackInfoText, debug.traceback, unit, unitImpl, boostDamageLevel, lowerDamageLevel, lowerInitiativeLevel)
	if not success then
		log(result)
	end

	return result
end
