--[[ Settings for Disciples 2 Rise of the Elves v3.01 mss32 proxy dll ]]--

settings = {
	-- Show troops banners
	showBanners = true,
	-- Show resources panel
	showResources = true,
	-- Show percentage of land coverted
	showLandConverted = false,

	-- Allow scenarios with prebuilt capital cities
	preserveCapitalBuildings = false,
	-- Maximum number of items the player is allowed to transfer
	-- between campaign scenarios [0 : INT_MAX]
	carryOverItemsMax = 5,

	-- Maximum unit damage per attack [300 : INT_MAX]
	unitMaxDamage = 300,
	-- Maximum combined unit armor [70 : INT_MAX]
	-- minimum value could not be less than highest armor value
	-- of all units in GUnits.dbf
	-- For example: not less than 65 because of Onyx Gargoyle in original game
	unitMaxArmor = 90,

	-- Maximum allowed scout range for troops [7 : 100]
	stackMaxScoutRange = 7,

	-- Total armor shatter damage [0 : 100]
	shatteredArmorMax = 100,
	-- Maximum armor shatter damage per attack [0 : 100]
	shatterDamageMax = 100,
	-- Percentage of damage upgrade value that shatter attack receives when a units levels up [0 : 255]
	shatterDamageUpgradeRatio = 100,
	-- Allow shatter attacks to miss
	allowShatterAttackToMiss = false,

	-- Percentage damage of critical hit [0 : 255]
	criticalHitDamage = 5,
	-- Percentage chance of critical hit [0 : 100]
	criticalHitChance = 100,

	-- Percentage of L_DRAIN attacks damage used as heal [INT_MIN : INT_MAX]
	drainAttackHeal = 50,
	-- Percentage of L_DRAIN_OVERFLOW attacks damage used as heal [INT_MIN : INT_MAX]
	drainOverflowHeal = 50,

	-- Change doppelganger attacks to copy units with respect to their level
	leveledDoppelgangerAttack = false,
	-- Change transform self attacks to compute transformed unit level using 'transformSelf.lua' script
	leveledTransformSelfAttack = false,
	-- Change summon attacks to compute summoned units levels using 'summon.lua' script
	leveledSummonAttack = false,

	-- Change doppelganger attacks to respect enemy/ally wards and immunities to the attack class and source
	doppelgangerRespectsEnemyImmunity = false,
	doppelgangerRespectsAllyImmunity = false,

	-- Round in battle after which paralyze and petrify attacks
	-- starts missing targets constantly [1 : INT_MAX]
	disableAllowedRoundMax = 40,
	
	-- Change accuracy reduction for mage leaders per each additional target
	mageLeaderAccuracyReduction = 10,

	aiAccuracyBonus = {
		-- Treat AI accuracy bonus as absolute value or as percentage.
		-- Absolute: accuracy += bonus;
		-- Percentage: accuracy += accuracy * bonus / 100;
		absolute = true,
		-- AI accuracy bonus on easy difficulty [-127 : 127]
		easy = -15,
		-- AI accuracy bonus on average difficulty [-127 : 127]
		average = 0,
		-- AI accuracy bonus on hard difficulty [-127 : 127]
		hard = 5,
		-- AI accuracy bonus on very hard difficulty [-127 : 127]
		veryHard = 10
	},

	movementCost = {
		-- Show stacks movement cost
		show = true,
		-- Color components are all in range [0 : 255]
		textColor = {
			red = 200, green = 200, blue = 200
		},
		outlineColor = {
			red = 0, green = 0, blue = 0
		}
	},

	-- If true, switches attacks miss check to a single random value roll
	-- instead of check against arithmetic mean of two random numbers
	missChanceSingleRoll = false,

	-- Fix missing attack information in unit encyclopedia
	detailedAttackDescription = true,

	-- Create mss32 proxy dll log files with debug info
	debugHooks = false,
}

