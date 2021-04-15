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
	unitMaxArmor = 100,

	-- Maximum allowed scout range for troops [7 : 100]
	stackMaxScoutRange = 7,

	-- Total armor shatter damage [0 : 100]
	shatteredArmorMax = 100,
	-- Maximum armor shatter damage per attack [0 : 100]
	shatterDamageMax = 100,
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

	-- Round in battle after which paralyze and petrify attacks
	-- starts missing targets constantly [1 : INT_MAX]
	disableAllowedRoundMax = 40,

	aiAccuracyBonus = {
		-- Treat AI accuracy bonus as absolute value or as percentage.
		-- Absolute: accuracy += bonus;
		-- Percentage: accuracy += accuracy * bonus / 100;
		absolute = true,
		-- AI accuracy bonus on easy difficulty [-100 : 100]
		easy = -15,
		-- AI accuracy bonus on average difficulty [-100 : 100]
		average = 0,
		-- AI accuracy bonus on hard difficulty [-100 : 100]
		hard = 5,
		-- AI accuracy bonus on very hard difficulty [-100 : 100]
		veryHard = 10
	},

	-- If true, switches attacks miss check to a single random value roll
	-- instead of check against arithmetic mean of two random numbers
	missChanceSingleRoll = false,

	-- Create mss32 proxy dll log files with debug info
	debugHooks = false,
}

