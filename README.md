# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Adds new custom building category for unit hire.
  Buildings should be placed on the 'Other buildings' tab.
  Custom category is created by adding 'L\_CUSTOM' record to LBuild.dbf;
- Allows each race to hire up to 10 new tier-1 units in cities and capital.<br />
  New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6;
- Allows alchemists to give additional attacks to retreating allies;
- Allows vampiric attacks to deal critical damage;
- Allows to load and create scenarios with no magic (maximum spell level set to 0);
- Allows Scenario Editor to place merchants, mages, trainers and mercenaries on water tiles;
- Allows Scenario Editor to place more than 200 stacks on a map;
- Buildings up to tier 10 are supported in editor and game;
- Maximum game turn increased to 9999;
- Fixes game crash in battles with summoners involved;
- Fixes AI unit placement logic for melee units with vampiric attacks;
- Fixes Scenario Editor bug with elves race as a caster in "Cast spell on location" event effect;
- Game rules and settings can be changed in Scripts/settings.lua;
  - <details>
    <summary>Settings:</summary>
  
      - "showBanners=(true/false)" toggle show banners by default;
      - "showResources=(true/false)" toggle show resources panel by default;
      - "showLandConverted=(true/false)" toggle show percentage of land converted instead of minimap;
      - "preserveCapitalBuildings=(true/false)" allow scenarios with prebuilt capital cities;
      - "carryOverItemsMax=\[0 : (2^31 - 1)\]" changes maximum number of items the player is allowed to transfer between campaign scenarios;
      - "unitMaxDamage=\[300 : (2^31 - 1)\]" changes maximum unit damage per attack;
      - "unitMaxArmor=\[70: 100\]" changes maximum combined unit armor;
      - "stackMaxScoutRange=\[7 : 100\]" changes maximum allowed scout range for troops; 
      - "shatteredArmorMax=\[0 : 100\]" changes total armor shatter damage;
      - "shatterDamageMax=\[0 : 100\]" changes maximum armor shatter damage per attack;
      - "allowShatterAttackToMiss=(true/false)" changes whether shatter attacks can miss or not;
      - "criticalHitDamage=\[0 : 255\]" changes the percentage damage of critical hit;
      - "criticalHitChance=\[0 : 100\]" changes the percentage chance of critical hit;
      - "mageLeaderAccuracyReduction=\[0 : 100\]" allows to specify accuracy reduction per target for mage leader units;
      - "drainAttackHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN attacks damage used as heal;
      - "drainOverflowHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN\_OVERFLOW attacks damage used as heal;
      - "leveledDoppelgangerAttack=(true/false)" change doppelganger attacks to copy units using with respect to their level using 'doppelganger.lua' script;
      - "leveledTransformSelfAttack=(true/false)" change transform self attacks to compute transformed unit level using 'transformSelf.lua' script;
      - "leveledSummonAttack=(true/false)" change summon attacks to compute summoned units levels using 'summon.lua' script;
      - "disableAllowedRoundMax=\[1 : (2^31 - 1)\]" sets round in battle after which paralyze and petrify attacks starts missing targets constantly;
      - "aiAccuracyBonus" these settings allows to configure AI accuracy in battle:
        - "absolute=(true/false)" treat accuracy bonus as absolute or percentage value;
      - "missChanceSingleRoll=(true/false)" if true, switches attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;
      - "debugHooks=(true/false)" create mss32 proxy dll log files with debug info;
  </details>
- Buttons for bulk item transfer: transfer all items, potions, scrolls/wands or valuables between inventories with single click;
  - <details>
    <summary>Installation:</summary>

    Add buttons with predefined names to DLG\_CITY\_STACK, DLG\_EXCHANGE or DLG\_PICKUP\_DROP dialogs in Interf.dlg file.
    Every button is optional and can be ignored.
    Buttons and their meaning:
      - Transfer all items to the left inventory: BTN\_TRANSF\_L\_ALL;
      - Transfer all items to the right inventory: BTN\_TRANSF\_R\_ALL;
      - Transfer all potions to the left: BTN\_TRANSF\_L\_POTIONS;
      - Transfer all potions to the right: BTN\_TRANSF\_R\_POTIONS;
      - Transfer all scrolls and wands to the left: BTN\_TRANSF\_L\_SPELLS;
      - Transfer all scrolls and wands to the right: BTN\_TRANSF\_R\_SPELLS;
      - Transfer all valuables to the left: BTN\_TRANSF\_L\_VALUABLES;
      - Transfer all valuables to the right: BTN\_TRANSF\_R\_VALUABLES;
    
    Example of button description in Interf.dlg:
    ```
    BUTTON    BTN_TRANSF_L_ALL,342,424,385,459,DLG_EXCHANGE_RETURN_D,DLG_EXCHANGE_RETURN_H,DLG_EXCHANGE_RETURN_C,DLG_EXCHANGE_RETURN_D,"Transfer all items to left",0
    ```
  </details>
- Button to sell all valuables with single click;
  - <details>
    <summary>Installation:</summary>
  
    Add sell confirmation text to TApp.dbf with id X015TA0001. Text must contain keyword '%PRICE%' in it.
    In case of missing text, stub message in english will be shown.
    
    Example of confirmation text:
    ```
    Do you want to sell all valuables? Revenue will be:\n%PRICE%
    ```
    
    Add button with name BTN\_SELL\_ALL\_VALUABLES to DLG\_MERCHANT in Interf.dlg file.
    Button is optional.
    
    Example of button description in Interf.dlg:
    ```
    BUTTON	BTN_SELL_ALL_VALUABLES,417,425,460,460,DLG_CITY_STACK_RETURN_D,DLG_CITY_STACK_RETURN_H,DLG_CITY_STACK_RETURN_C,DLG_CITY_STACK_RETURN_D,"Sell all valuables",0
    ```
  </details>
- Allows to add new music tracks for battle and capital cities;
  - <details>
    <summary>Installation:</summary>
  
    New music tracks name format follows original game naming convention.    
    WAV tracks in Music folder containing 'battle' as part of their names will be played during battle.    
    WAV tracks in Music folder containing '<humn/dwrf/unde/here/elf>trk' as part of their names will be played in capital city window with accordance to race.
    
    Examples:
      - battle10.wav - new music track for battle
      - humntrk4.wav - new music track for Empire
      - heretrk15.wav - new music track for Legions of the Damned    
    </details>
- Fixes Scenario Editor to show spell cost information;
  - <details>
      <summary>ScenEdit.dlg fix:</summary>
  
      Make sure the corresponding lines of DLG\_R\_C\_SPELL changed as described below.
      
      Add 'X160TA0005' to fix elven mana name tooltip:
      ```
      IMAGE	IMG_BLUE,303,257,343,291,_RESOURCES_GREENM_B,"X160TA0005"
      ```
      Change resource names between IMG\_JAUNE and IMG\_ORANGE:
      ```
      IMAGE	IMG_JAUNE,163,257,203,291,_RESOURCES_BLACKM_B,"X100TA0098"
      IMAGE	IMG_ORANGE,28,257,68,291,_RESOURCES_BLUEM_B,"X100TA0096"
      ```
    </details>
- Cities can generate daily income depending on scenario variables with predefined names;
  - <details>
      <summary>Description:</summary>
  
      Scenario variables with predefined names are checked each turn and affect income, excluding neutrals race.
      Variables can be changed by events as any others.
      
      Variables that affect all races:
      
        - TIER_0_CITY_INCOME - income from capital city;
        - TIER_N_CITY_INCOME - income from tier N city, N = [1 : 5];
      Variables that affect specific race:
      
        - EMPIRE_TIER_0_CITY_INCOME - income from capital city for Empire only;
        - EMPIRE_TIER_N_CITY_INCOME - income from tier N city for Empire only, N = [1 : 5];
        - LEGIONS .. - income for Legions of the Damned only;
        - CLANS .. - Mountain Clans;
        - HORDES .. - Undead Hordes;
        - ELVES .. - Elven Alliance;
    </details>
- Fixes bestow wards attack modifiers bugs and restrictions;
  - <details>
    <summary>Description:</summary>
  
    - Allows bestow wards attack to apply modifiers even if there are no source wards among them;
    - Fixes bug with more than 8 simultaneously applied modifiers becoming permanent;
    - Fixes modifiers that applied on first attack of double attack;
    - Can heal and apply modifier effect at once. Set QTY\_HEAL > 0 in GAttacks.dbf and HEAL in GDynUpgr.dbf:
      - Allows to heal retreating allies in main or alternative attack;
      - Allows to heal allies when battle ends, same as ordinary heal;
    - Use revive as a secondary attack;
    - Target unit with a secondary attack even if there are no modifiers that can be applied;
    - Attack class wards are properly reset and can be reapplied during battle;
    - Modifiers with immunity handled correctly;
    </details>
- Can be used on vanilla version or with other mods installed;

### Tested on:
- Disciples 2 Rise of the Elves v3.01 (Akella, Russobit, GOG)

### Installation:
Rename original mss32.dll to mss**23**.dll, then copy mss32.dll from this repository to game folder.
Copy Scripts folder to the game folder.

### Deinstallation:
Delete mss32.dll, then rename mss**23**.dll back to mss32.dll.

### Versions prior 0.4:
Installation and deinstallation process is the same, but with binkw32.dll.

### Building from sources:
Build Debug or Release Win32 target using Visual Studio solution located in mss32 folder. 

### License
[Detours](https://github.com/microsoft/Detours), [GSL](https://github.com/microsoft/GSL), [fmt](https://github.com/fmtlib/fmt) and [sol2](https://github.com/ThePhD/sol2) submodules as well as [![Lua](https://www.andreas-rozek.de/Lua/Lua-Logo_64x64.png)](http://www.lua.org/license.html) are using their own licenses.


This modification is not made or supported by Strategy First.<br />
Disciples 2 is a trademark of Strategy First Inc.

