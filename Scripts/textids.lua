--[[ Text ids for Disciples 2 Rise of the Elves v3.01 mss32 proxy dll ]]--

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

		-- Defines text id to mark Infinite attacks in unit encyclopedia.
		-- Fallback text is "Lasting".
		infiniteAttack = "",

		-- Defines text id to format Infinite text in unit encyclopedia.
		-- The text must contain keywords "%ATTACK%" and "%INFINITE%".
		-- Fallback text is "%ATTACK% (%INFINITE%)".
		infiniteText = "",

		-- Defines text id to mark Critical Hit attacks in unit encyclopedia.
		-- Fallback text id is the standard "X160TA0017".
		critHitAttack = "",

		-- Defines text id to format Critical Hit text in unit encyclopedia.
		-- Despite its name, it is used to format all crit fields (name, power and damage).
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
}
