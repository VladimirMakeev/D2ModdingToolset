# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Can be used on vanilla version or with other mods installed;
- <details>
    <summary>Adds new custom building category for unit hire;</summary>

    Buildings should be placed on the 'Other buildings' tab. Custom category is created by adding 'L\_CUSTOM' record to LBuild.dbf.
  </details>
- <details>
    <summary>Allows each race to hire up to 10 new tier-1 units in cities and capital;</summary>

    New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6.
  </details>
- Allows scenarios with prebuilt capital cities;
- Allows banners, resources panel and converted land percentage to be displayed by default;
- Allows to display movement cost for each individual step of parties;
- Allows to specify a maximum allowed scout range for parties; 
- Allows to set maximum unit damage and armor;
- Allows to set critical hit damage and chance;
- Allows to set accuracy bonuses for different game difficulty modes;
- Allows to set accuracy reduction for mage leaders per each additional target;
- Allows to set a number of battle round after which paralyze and petrify attacks will constantly miss;
- Allows to switch attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;
- Allows alchemists to give additional attacks to retreating allies;
- Allows to set vampiric attacks heal ratio;
- Allows vampiric attacks to deal critical damage;
- Allows to customize shatter attacks: maximum damage per hit, maximum armor that can be shattered, whether attack can miss or not, and its upgrade ratio;
- Allows doppelganger, transform and summon attacks to produce leveled versions of units;
- Allows doppelganger attacks to respect enemy and ally wards and immunities to the attack class and source;
- Allows to set a maximum number of items the player is allowed to transfer between campaign scenarios;
- Allows to load and create scenarios with no magic (maximum spell level set to 0);
- Allows Scenario Editor to place merchants, mages, trainers and mercenaries on water tiles;
- Allows Scenario Editor to place more than 200 stacks on a map;
- Buildings up to tier 10 are supported in editor and game;
- Maximum game turn increased to 9999;
- Fixes game crash in battles with summoners involved;
- Fixes game crash when AI controlled unit with transform self attack uses alternative attack with 'adjacent' attack range;
- Fixes game crash on 144x144 maps that occurs if there is a party standing on a lower-left or lower-right edge of the map;
- Fixes AI unit placement logic for melee units with vampiric attacks;
- Fixes Scenario Editor bug with elves race as a caster in "Cast spell on location" event effect;
- <details>
    <summary>Buttons for bulk item transfer: transfer all items, potions, scrolls/wands or valuables between inventories with single click;</summary>

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
- <details>
    <summary>Button to sell all valuables with single click;</summary>

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
- <details>
    <summary>Allows to add new music tracks for battle and capital cities;</summary>

    New music tracks name format follows original game naming convention.    
    WAV tracks in Music folder containing 'battle' as part of their names will be played during battle.    
    WAV tracks in Music folder containing '<humn/dwrf/unde/here/elf>trk' as part of their names will be played in capital city window with accordance to race.

    Examples:
      - battle10.wav - new music track for battle
      - humntrk4.wav - new music track for Empire
      - heretrk15.wav - new music track for Legions of the Damned    
  </details>
- <details>
    <summary>Fixes Scenario Editor to show spell cost information;</summary>

    Make sure the corresponding lines of DLG\_R\_C\_SPELL in ScenEdit.dlg are changed as described below.

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
- <details>
    <summary>Cities can generate daily income depending on scenario variables with predefined names;</summary>

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
- <details>
    <summary>Fixes bestow wards attack modifiers bugs and restrictions;</summary>

    - Fixes wards becoming permanent when more than 8 modifiers are applied simultaneously;
    - Fixes wards becoming permanent on warded unit transformation;
    - Fixes disappearing of modifiers applied on first attack of double attack;
    - Fixes attack source wards being reset incorrectly if its modifier also contains hp, regen or armor element;
    - Fixes attack class wards not being reset preventing it from being reapplied during battle;
    - Fixes incorrect handling of existing unit immunities;
    - Allows bestow wards attack to apply modifiers even if there are no source wards among them;
    - Allows to target unit with a secondary attack even if there are no modifiers that can be applied;
    - Allows to heal and apply modifier effect at once. Set QTY\_HEAL > 0 in GAttacks.dbf and HEAL in GDynUpgr.dbf:
      - Allows to heal retreating allies in main or alternative attack;
      - Allows to heal allies when battle ends, same as ordinary heal;
    - Allows to use revive as a secondary attack;
  </details>
- <details>
    <summary>Allows attack class immunity modifiers to be applied (also works with vanilla version);</summary>

    Works by adding IMMUNITYC and IMMUNITYCATC columns to GmodifL.dbf (similar to IMMUNITY and IMMUNITYCAT).
    Then adding a new record with TYPE 14 and filling the columns accordingly to attack class and immunity.
  </details>

### Settings:
The following settings can be changed in Scripts/settings.lua:
<details>
  <summary>User Interface</summary>

  - "showBanners=(true/false)" toggles showing of banners by default;
  - "showResources=(true/false)" toggles showing of resources panel by default;
  - "showLandConverted=(true/false)" toggles showing of converted land percentage instead of minimap by default;
  - "movementCost" allows to display movement cost for each individual step of parties:
    - "show=(true/false)" toggles displaying of movement cost;
    - "textColor" specifies a color (RGB) of the text:
      - "red=\[0 : 255\]";
      - "green=\[0 : 255\]";
      - "blue=\[0 : 255\]";
    - "outlineColor" specifies a color (RGB) of the text outline:
      - "red=\[0 : 255\]";
      - "green=\[0 : 255\]";
      - "blue=\[0 : 255\]";
</details>
<details>
  <summary>Battle Mechanics</summary>

  - "unitMaxDamage=\[300 : (2^31 - 1)\]" changes maximum unit damage per attack;
  - "unitMaxArmor=\[70: 100\]" changes maximum combined unit armor;
  - "shatteredArmorMax=\[0 : 100\]" changes total armor shatter damage;
  - "shatterDamageMax=\[0 : 100\]" changes maximum armor shatter damage per attack;
  - "shatterDamageUpgradeRatio=\[0 : 255\]" sets a percentage of damage upgrade value that shatter attack receives when a units levels up;
  - "allowShatterAttackToMiss=(true/false)" changes whether shatter attacks can miss or not;
  - "criticalHitDamage=\[0 : 255\]" changes the percentage damage of critical hit;
  - "criticalHitChance=\[0 : 100\]" changes the percentage chance of critical hit;
  - "drainAttackHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN attacks damage used as heal;
  - "drainOverflowHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN\_OVERFLOW attacks damage used as heal;
  - "doppelgangerRespectsEnemyImmunity=(true/false)" sets whether doppelganger attack checks for enemy ward/immunity to the attack class and source;
  - "doppelgangerRespectsAllyImmunity=(true/false)" sets whether doppelganger attack checks for ally ward/immunity to the attack class and source;
  - "leveledDoppelgangerAttack=(true/false)" changes doppelganger attacks to copy units using with respect to their level using 'doppelganger.lua' script;
  - "leveledTransformSelfAttack=(true/false)" changes transform self attacks to compute transformed unit level using 'transformSelf.lua' script;
  - "leveledSummonAttack=(true/false)" changes summon attacks to compute summoned units levels using 'summon.lua' script;
  - "unrestrictedBestowWards=(true/false)" fixes bestow wards attack modifiers bugs and restrictions, see more details under Features section;
  - "disableAllowedRoundMax=\[1 : (2^31 - 1)\]" sets a number of battle round after which paralyze and petrify attacks will constantly miss;
  - "missChanceSingleRoll=(true/false)" if true, switches attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;
  - "mageLeaderAccuracyReduction=\[0 : 100\]" allows to set accuracy reduction for mage leaders per each additional target;
  - "aiAccuracyBonus" allows to configure AI accuracy in battle for different game difficulty modes:
    - "easy=\[-127 : 127\]";
    - "average=\[-127 : 127\]";
    - "hard=\[-127 : 127\]";
    - "veryHard=\[-127 : 127\]";
    - "absolute=(true/false)" determines how accuracy bonuses are treated - as absolute or percentage values;
</details>
<details>
  <summary>Misc</summary>

  - "preserveCapitalBuildings=(true/false)" allows scenarios with prebuilt capital cities;
  - "carryOverItemsMax=\[0 : (2^31 - 1)\]" changes maximum number of items the player is allowed to transfer between campaign scenarios;
  - "stackMaxScoutRange=\[7 : 100\]" changes maximum allowed scout range for parties; 
  - "debugHooks=(true/false)" create mss32 proxy dll log files with debug info;
</details>

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

