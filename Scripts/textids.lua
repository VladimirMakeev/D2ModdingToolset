--[[
Text ids for Disciples 2 Rise of the Elves v3.01 mss32 proxy dll

Refer to the documentation to find exact meaning of each text id
https://github.com/VladimirMakeev/D2ModdingToolset/blob/master/README.md
--]]

textids = {
	-- Interface text ids from either TApp.dbf or TAppEdit.dbf
	interf = {
		-- Defines text id to use as "sell all valuables" message.
		-- The text must contain keyword "%PRICE%".
		-- Fallback text is "Do you want to sell all valuables? Revenue will be:\n%PRICE%"
		sellAllValuables = "X015TA0001",

		-- Defines text id to use as "sell all items" message.
		-- The text must contain keyword "%PRICE%".
		-- Fallback text is "Do you want to sell all items? Revenue will be:\n%PRICE%"
		sellAllItems = "",

		-- Defines format id for effect duration description in unit encyclopedia.
		-- The text must contain keywords "%DURATION%" and "%DURATIONVALUE%".
		-- Fallback text is "\fMedBold;%DURATION%:\t\fNormal;%DURATIONVALUE%\n"
		durationDescription = "",

		-- Defines text id of effect duration in unit encyclopedia.
		-- Fallback text is "Duration".
		durationText = "",

		-- Defines text id of instant effect duration in unit encyclopedia.
		-- Fallback text is "Instant".
		instantDurationText = "",

		-- Defines text id of random effect duration in unit encyclopedia.
		-- Fallback text is "Random".
		randomDurationText = "",

		-- Defines text id of single-turn effect duration in unit encyclopedia.
		-- Fallback text is "Single turn".
		singleTurnDurationText = "",

		-- Defines text id of whole-battle effect duration in unit encyclopedia.
		-- Fallback text is "Whole battle".
		wholeBattleDurationText = "",

		-- Defines text id to mark Critical Hit attacks in unit encyclopedia.
		-- Fallback text id is the standard "X160TA0017".
		critHitAttack = "",

		-- Defines text id to format Critical Hit text in unit encyclopedia.
		-- Despite its name, it is used to format both crit hit damage and power (chance to hit).
		-- The text must contain keywords "%DMG%" and "%CRIT%".
		-- Fallback text is "%DMG% (%CRIT%)".
		critHitDamage = "",

		-- Defines text id to format rated damage in unit encyclopedia.
		-- The text must contain keywords "%DMG%" and "%RATED%".
		-- Fallback text is "%DMG%, %RATED%".
		ratedDamage = "",

		-- Defines text id to format equal rated damage in unit encyclopedia.
		-- The text must contain keywords "%DMG%", %TARGETS% and "%RATED%".
		-- Fallback text is "%DMG%, (%TARGETS%x) %RATED%".
		ratedDamageEqual = "",

		-- Defines text id for rated damage separator in unit encyclopedia.
		-- Fallback text is ", ".
		ratedDamageSeparator = "",

		-- Defines text id to format split damage in unit encyclopedia.
		-- The text must contain keyword "%DMG%".
		-- Fallback text is "%DMG%, split between targets".
		splitDamage = "",

		-- Defines modified text representation in unit encyclopedia.
		-- Modified text includes attack name, source, reach, etc.
		-- The text must contain keyword "%VALUE%".
		-- Fallback text is "\c025;090;000;%VALUE%\c000;000;000;".
		modifiedValue = "",

		-- Defines modified number representation in unit encyclopedia.
		-- The text must contain keywords "%NUMBER%" and "%BONUS%".
		-- Fallback text is "%NUMBER% %BONUS%".
		modifiedNumber = "",

		-- Defines modified number with total representation in unit encyclopedia.
		-- Used for HP and XP bonus display.
		-- The text must contain keywords "%TOTAL%" and "%BONUS%".
		-- Fallback text is "%TOTAL% (%BONUS%)".
		modifiedNumberTotal = "",

		-- Defines positive bonus number representation in unit encyclopedia.
		-- The text must contain keywords "%NUMBER%" and "%SIGN%".
		-- Note that sign can be either + or - depending on stat.
		-- Fallback text is "\c025;090;000;%SIGN% %NUMBER%\c000;000;000;".
		positiveBonusNumber = "",

		-- Defines negative bonus number representation in unit encyclopedia.
		-- The text must contain keywords "%NUMBER%" and "%SIGN%".
		-- Note that sign can be either + or - depending on stat.
		-- Fallback text is "\c100;000;000;%SIGN% %NUMBER%\c000;000;000;".
		negativeBonusNumber = "",

		-- Defines modifiers list caption in unit encyclopedia.
		-- Fallback text is "\fMedBold;Effects:\fNormal;"
		modifiersCaption = "",

		-- Defines empty modifiers list content in unit encyclopedia.
		-- Fallback text id is "X005TA0676" (empty leader abilities).
		modifiersEmpty = "",

		-- Defines format id for modifier description in unit encyclopedia.
		-- The text must contain keyword "%DESC%".
		-- Fallback text is "\vC;%DESC%"
		modifierDescription = "",

		-- Defines format id for native modifier description in unit encyclopedia.
		-- The text must contain keyword "%DESC%".
		-- Fallback text is "\vC;\fMedBold;%DESC%\fNormal;"
		nativeModifierDescription = "",

		-- Defines format id for drain description in unit encyclopedia.
		-- The text must contain keywords "%DRAINEFFECT%" and "%DRAIN%".
		-- Fallback text is "\fMedBold;%DRAINEFFECT%:\t\fNormal;%DRAIN%\n"
		drainDescription = "",

		-- Defines text id of drain effect in unit encyclopedia.
		-- Fallback text id is the standard "X005TA0792".
		drainEffect = "",

		-- Defines text id to mark Drain Overflow attacks in unit encyclopedia.
		-- Fallback text is "Overflow".
		overflowAttack = "",

		-- Defines text id to format Drain Overflow text in unit encyclopedia.
		-- The text must contain keywords "%ATTACK%" and "%OVERFLOW%".
		-- Fallback text is "%ATTACK% (%OVERFLOW%)".
		overflowText = "",

		-- Defines text id to format dynamic upgrade level in unit encyclopedia.
		-- The text must contain keyword %STAT%.
		-- "%UPGLV%" is optional and can be ommited if you don't want to display upgrade level.
		-- Fallback text is "%STAT% (level-ups weaken at %UPGLV%)".
		dynamicUpgradeLevel = "",

		-- Defines text id to format dynamic upgrades text in unit encyclopedia.
		-- The text must contain keywords "%STAT%", %UPG1% and %UPG2%.
		-- Fallback text is "%STAT% (%UPG1% | %UPG2% per level)".
		dynamicUpgradeValues = "",
		
		-- Defines text id to format broken (removed) wards in unit encyclopedia.
		-- The text must contain keyword "%WARD%", or it can be an empty string to not display broken wards at all.
		-- Fallback text is "\fMedBold;\c100;000;000;%WARD%\c000;000;000;\fNormal;".
		removedAttackWard = "",

		-- Defines text id to mark Infinite attacks in unit encyclopedia.
		-- Fallback text is "Lasting".
		infiniteAttack = "",

		-- Defines text id to format Infinite text in unit encyclopedia.
		-- The text must contain keywords "%ATTACK%" and "%INFINITE%".
		-- Fallback text is "%ATTACK% (%INFINITE%)".
		infiniteText = "",
	},

	-- Text ids related to events logic
	events = {
		-- Text ids related to event conditions
		conditions = {
			-- Text ids for L_OWN_RESOURCE event condition
			ownResource = {
				-- Message to show when there are more than two conditions of type
				-- Fallback text is "At most two conditions of type "Own resource" is allowed per event."
				tooMany = "",
				-- Message to show when two conditions are mutually exclusive
				-- Fallback text is "Conditions of type "Own resource" are mutually exclusive."
				mutuallyExclusive = "",
			},
	
			-- Text ids for L_GAME_MODE event condition
			gameMode = {
				-- Message to show when there are more than one condition of type
				-- Fallback text "Only one condition of type "Game mode" is allowed per event."
				tooMany = "",
				-- Text id for single player game mode
				-- Fallback text "single player"
				single = "",
				-- Text id for hotseat game mode
				-- Fallback text "hotseat"
				hotseat = "",
				-- Text id for online game mode
				-- Fallback text "online"
				online = "",
			},
	
			-- Text ids for L_PLAYER_TYPE event condition
			playerType = {
				-- Message to show when there are more than one condition of type
				-- Fallback text "Only one condition of type "Player type" is allowed per event."
				tooMany = "",
				-- Text id for human player type
				-- Fallback text "human"
				human = "",
				-- Text id for AI player type
				-- Fallback text "AI"
				ai = "",
			},

			-- Text ids for L_VARIABLE_CMP event condition
			variableCmp = {
				-- Text id for equality comparison
				-- Fallback text "is equal to"
				equal = "",
				-- Text id for difference comparison
				-- Fallback text "is not equal to"
				notEqual = "",
				-- Fallback text "is greater than"
				greater = "",
				-- Fallback text "is greater or equal to"
				greaterEqual = "",
				-- Fallback text "is less than"
				less = "",
				-- Fallback text "is less or equal to"
				lessEqual = "",
			},
		},
	},

	-- Text ids related to custom lobby
	lobby = {
		-- Server name shown in network protocols list
		-- Fallback text "Lobby server"
		serverName = "",
		-- Shown when client could not connect due to server being unresponsive
		-- Fallback text "Failed to connect.\nLobby server not responding"
		serverNotResponding = "",
		-- Client's connection attemt failed
		-- Fallback text "Connection attempt failed"
		connectAttemptFailed = "",
		-- Client could not connect due to server being full
		-- Fallback text "Lobby server is full"
		serverIsFull = "",
		-- Game files hash failed to compute
		-- Fallback text "Could not compute hash"
		computeHashFailed = "",
		-- Client could not request hash check from server
		-- Fallback text "Could not request game integrity check"
		requestHashCheckFailed = "",
		-- Client hash is wrong
		-- Fallback text "Game integrity check failed"
		wrongHash = "",
		-- Player entered wrong room password
		-- Fallback text "Wrong room password"
		wrongRoomPassword = "",
	},
    
    generator = {
        -- Description text for randomly generated scenarios
        -- Fallback text "Random scenario based on template '%TMPL%'. Seed: %SEED%. Starting gold: %GOLD%. Roads: %ROADS%%. Forest: %FOREST%%."
        description = "",
        -- Generator could not process game data from dbf tables or .ff files
        -- Error details are logged in mssProxyError.log
        -- Fallback text "Could not read game data needed for scenario generator.\nSee mssProxyError.log for details"
        wrongGameData = "",
        -- Error occured during scenario generation
        -- Fallback text "Error during random scenario map generation.\nSee mssProxyError.log for details".
        generationError = "",
        -- Generator failed to create scenario after specified number of attempts
        -- Fallback text "Could not generate scenario map after %NUM% attempts.\nPlease, adjust template contents or settings"
        limitExceeded = "",
    },
}
