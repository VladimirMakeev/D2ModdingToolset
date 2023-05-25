--[[ Settings for Disciples 2 random scenario generator ]]--

settings = {
	-- These units will be ignored by generator, they will never appear.
	forbiddenUnits = {
		'g000uu6111', -- Asteroth
		'g000uu6011', -- Asteroth leader
		'g000uu8008', -- Count Flamel Crowley
		'g000uu8049', -- Gumtik Bledwater
		'g000uu8149', -- Gumtik Bledwater leader
		'g000uu6102', -- Demon Uther
		'g000uu6002', -- Demon Uther leader
		'g000uu6121', -- Drega Zul
		'g000uu6021', -- Drega Zul leader
		'g000uu8003', -- Drullia'an
		'g000uu8103', -- Drullia'an leader
		'g000uu8004', -- Gallean's Beast
		'g000uu8104', -- Gallean's Beast leader
		'g000uu6118', -- Elf Queen Taladrielle Lilem
		'g000uu6018', -- Elf Queen Taladrielle Lilem leader
		'g000uu8006', -- Lachla'an
		'g000uu8106', -- Lachla'an leader
		'g000uu6105', -- Mage Hugin
		'g000uu6005', -- Mage Hugin leader
		'g000uu8001', -- Nebiros
		'g000uu8101', -- Nebiros leader
		'g000uu6106', -- Yataa' Halli
		'g000uu6006', -- Yataa' Halli leader
		'g000uu8050', -- Sir Allemon
		'g000uu8150', -- Sir Allemon leader
		'g000uu8007', -- Dark Lachla'an
		'g000uu8107', -- Dark Lachla'an leader
		'g000uu6120', -- Uther (child)
		'g000uu6020', -- Uther (child) leader
		'g000uu6101', -- Uther (posessed child)
		'g000uu6001', -- Uther (posessed child) leader
		'g000uu6109', -- Erhog the Dark
		'g000uu6009', -- Erhog the Dark leader
		'g000uu6119', -- Erhog the Necromancer
		'g000uu6019', -- Erhog the Necromancer leader
		'g000uu6108', -- Hubert de Lalye
		'g000uu6008', -- Hubert de Lalye leader
		'g000uu8002', -- Grave Golem
		'g000uu8102', -- Grave Golem leader
	},

	-- Landmark identifiers. Landmarks are used as blocking the path and decorations.
	landmarks = {
		-- Landmarks that look good on Empire terrain
		human = {
			-- Statues
			'g000mg0042',
		    'g000mg0043',
		    'g000mg0044',
		    'g000mg0070',
			-- Fountains
		    'g000mg0083',
		    'g000mg0084',
			-- Neutral houses (good for Empire too)
		    'g000mg0091',
		    'g000mg0092',
		    'g000mg0093',
		    'g000mg0094',
		    'g000mg0095',
		    'g000mg0096',
		    'g000mg0097',
		    'g000mg0098',
			-- Empire houses
		    'g000mg0099',
		    'g000mg0100',
		    'g000mg0101',
		    'g000mg0102',
			-- Tents
		    'g000mg0115',
		    'g000mg0116',
		    'g000mg0117',
		    'g000mg0118',
		    'g000mg0119',
			-- Towers
		    'g000mg0149',
		    'g000mg0150',
		},
		-- Landmarks that look good on Mountain Clans terrain
		dwarf = {
			-- Ice, glaciers
			'g000mg0033',
		    'g000mg0034',
		    'g000mg0035',
		    'g000mg0036',
		    'g000mg0037',
		    'g000mg0038',
		    'g000mg0039',
		    'g000mg0040',
			-- Statues
		    'g000mg0045',
		    'g000mg0049',
		    'g000mg0050',
			-- Clan houses
		    'g000mg0103',
		    'g000mg0104',
		    'g000mg0105',
		    'g000mg0106',
			-- Towers
		    'g000mg0149',
		    'g000mg0150',
		},
		-- Landmarks that look good on Undead Hordes terrain
		undead = {
			'g000mg0065',
			-- Spider caves
		    'g000mg0120',
		    'g000mg0121',
			-- Skeletons
		    'g000mg0085',
		    'g000mg0086',
		    'g000mg0087',
		    'g000mg0088',
		    'g000mg0089',
		    'g000mg0090',
			-- Houses
		    'g000mg0111',
		    'g000mg0112',
		    'g000mg0113',
		    'g000mg0114',
			-- TODO: big dragon skeletons
		},
		-- Landmarks that look good on Legions of the Damned terrain
		heretic = {
			-- Obelisks
			'g000mg0004',
			'g000mg0005',
			-- Lava craters
			'g000mg0022',
			'g000mg0023',
			'g000mg0024',
			'g000mg0052',
			'g000mg0053',
			'g000mg0054',
			-- Houses
			'g000mg0107',
			'g000mg0108',
			'g000mg0109',
			'g000mg0110',
		},
		-- Landmarks that look good on Elven Alliance terrain
		elf = {
			-- Houses
			'g000mg0028',
		    'g000mg0029',
		    'g000mg0030',
		    'g000mg0130',
		    'g000mg0131',
		    'g000mg0134',
		    'g000mg0145',
		    'g000mg0146',
		    'g000mg0147',
		    'g000mg0148',
		},
		-- Landmarks that look good on neutral terrain
		neutral  = {
			-- Spider caves
			'g000mg0120',
		    'g000mg0121',
		    -- Fortress
		    'g000mg0003',
		    -- Cemeteries
		    'g000mg0025',
		    'g000mg0026',
		    'g000mg0027',
		    'g000mg0031',
		    'g000mg0032',
		    -- Statues
		    'g000mg0042',
		    'g000mg0043',
		    'g000mg0044',
		    -- Well
		    'g000mg0070',
		    -- Skeletons, don't use them all, looks like too frequent..
		    'g000mg0085',
		    --'g000mg0086',
		    --'g000mg0087',
		    --'g000mg0088',
		    --'g000mg0089',
		    'g000mg0090',
			-- Houses
		    'g000mg0091',
		    'g000mg0092',
		    'g000mg0093',
		    'g000mg0094',
		    'g000mg0095',
		    'g000mg0096',
		    'g000mg0097',
		    'g000mg0098',
			-- Tents
		    'g000mg0115',
		    'g000mg0116',
		    'g000mg0117',
		    'g000mg0118',
		    'g000mg0119',
			-- Towers
		    'g000mg0149',
		    'g000mg0150',
		},
		-- Landmarks that are visually resemble mountains
		mountain = {
			'g000mg0138',
		    'g000mg0139',
		    'g000mg0140',
		    'g000mg0141',
		    'g000mg0142',
		    'g000mg0143',
		    'g000mg0144',
		},
	},

	ruins = {
        land = {
			'g000ru0000000',
			'g000ru0000001',
			'g000ru0000002',
			'g000ru0000003',
			'g000ru0000004',
			'g000ru0000005',
			'g000ru0000006',
			'g000ru0000007',
            'g000ru0000010',
        },
        
        water = {
            'g000ru0000009'
        }
    },
	
	merchants = {
		land = {
			'g000si0000merh00',
			'g000si0000merh01',
			'g000si0000merh02',
			'g000si0000merh03',
			'g000si0000merh04',
			'g000si0000merh05',
			'g000si0000merh06',
			'g000si0000merh07',
		},
    },
	
	mercenaries = {
		land = {
			'G000SI0000MERC00',
			'G000SI0000MERC01',
			'G000SI0000MERC02',
			'G000SI0000MERC03',
		},
    },
	
	mages = {
		land = {
			'g000si0000mage00',
			'g000si0000mage01',
			'g000si0000mage02',
			'g000si0000mage03',
		}
    },
	
	trainers = {
		land = {
			'G000SI0000TRAI00',
			'G000SI0000TRAI01',
			'G000SI0000TRAI02',
			'G000SI0000TRAI03',
		}
    },
}
