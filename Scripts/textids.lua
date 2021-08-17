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

		-- Defines text id to mark Critical Hit attacks in unit encyclopedia.
		-- Fallback text id is the standard "X160TA0017".
		critHitAttack = "",

		-- Defines text id to format Critical Hit damage in unit encyclopedia.
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

		-- Text ids for L_OWN_RESOURCE event condition
		ownResource = {
			-- Message to show when there are more than two conditions of type
			-- Fallback text is "At most two conditions of type "Own resource" is allowed per event."
			tooMany = "",
			-- Message to show when two conditions are mutually exclusive
			-- Fallback text is "Conditions of type "Own resource" are mutually exclusive."
			mutuallyExclusive = "",
		},
	}
}
