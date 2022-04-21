# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Can be used on vanilla version or with other mods installed;
- Allows players to search and create PvP matches without external software using custom lobby server. Currently only for [Motlin's mod](https://dis2modding.fandom.com/ru/wiki/Мод_Мотлина).
- <details>
    <summary>Adds new custom building category for unit hire;</summary>

    Buildings should be placed on the 'Other buildings' tab. Custom category is created by adding 'L\_CUSTOM' record to LBuild.dbf.
  </details>
- <details>
    <summary>Allows each race to hire up to 10 new tier-1 units in cities and capital;</summary>

    New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6.
  </details>
- <details>
    <summary>Allows scenarios with prebuilt capital cities;</summary>

    - Enable 'preserveCapitalBuildings' in 'Scripts\\settings.lua';
    - Start a scenario;
    - Build desired buildings in a capital;
    - Save the scenario;
    - Move the saved game from SaveGame folder to Exports;
    - Restart the scenario.
  </details>
- Allows banners, resources panel and converted land percentage to be displayed by default;
- Allows to display movement cost for each individual step of parties;
- Allows to specify a maximum allowed scout range for parties; 
- Allows to set maximum unit damage and armor;
- <details>
    <summary>Allows to set critical hit damage and chance;</summary>

    There are global settings criticalHitDamage / criticalHitChance (see Settings section for details).<br />
    Aside from that, these numbers can be customized for each particular attack:
    - Add CRIT_DAM (Numeric, size 3) and CRIT_POWER (Numeric, size 3) columns to Gattacks.dbf;
    - Check CRIT_HIT column to enable critical hit for attacks that you wish to customize;
    - CRIT_DAM specifies a critical hit damage (0-255%). Falls back to the global setting (criticalHitDamage) if empty;
    - CRIT_POWER specifies a critical hit chance (0-100%). Falls back to the global setting (criticalHitChance) if empty.

    ![image](https://user-images.githubusercontent.com/5180699/155902276-44d843ab-b799-4997-ad3c-886bf657107f.png)

    **Note** that critical hit representation in unit encyclopedia can be customized. Check [textids.lua](Scripts/textids.lua).
  </details>
- Allows to set accuracy bonuses for different game difficulty modes;
- Allows to set accuracy reduction for mage leaders per each additional target;
- Allows to set a number of battle round after which paralyze and petrify attacks will constantly miss;
- Allows to switch attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;
- Allows alchemists to give additional attacks to retreating allies;
- Allows to set vampiric attacks heal ratio;
- Allows vampiric attacks to deal critical damage;
- Allows to customize shatter attacks: maximum damage per hit, maximum armor that can be shattered, whether attack can miss or not, and its upgrade ratio;
- Allows doppelganger, transform-self, transform-other, drain-level and summon attacks to produce leveled versions of units (![demo video](https://user-images.githubusercontent.com/5180699/158679921-7b6d82b1-2c48-44bb-91c6-fa1983893eab.mp4));
- Allows doppelganger attacks to respect enemy and ally wards and immunities to the attack class and source;
- Allows transform-self attack to not consume a unit turn for transformation (![demo video](https://user-images.githubusercontent.com/5180699/124916545-76550200-dffb-11eb-8b12-3147e40ef04b.mp4));
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
- Fixes AI targeting for single lower-damage and lower-initiative attacks;
- Fixes AI targeting for shatter attacks where it tends to pick a most armored target ignoring general prioritization;
- Fixes AI targeting for shatter attacks where it assumes that armored targets always stay armored, even if their armor is already shattered;
- Fixes AI targeting where it incorrectly calculates effective HP of armored targets, getting significantly lower values than intended;
- Fixes incorrect function of transform-self attack in cases where its alternative attack targets allies (heal, summon, etc.);
- Fixes an issue with transform-other attack where it selects melee vs ranged transform based on attacker position rather than on target position;
- Removes persistent target highlighting for transform-self attack (circle animations for all possible targets on the ground), so it no longer interferes with highlighting for targets that are actually going to be affected by the attack;
- Fixes missing modifiers of alternative attacks (![demo video](https://user-images.githubusercontent.com/5180699/125460215-144ef648-5497-4674-b9d6-ac7d2fa95125.mp4));
- Fixes unit transformation (doppelganger, drain-level, transform-self/other attacks) to include HP modifiers into current hp recalculation, thus unit is not getting "damaged" upon transforming (![demo video](https://user-images.githubusercontent.com/5180699/156921183-b1f4748d-cb4f-40c8-a9b1-b911e9cc91dd.mp4));
- Fixes unit transformation (drain-level, transform-self/other and untransform-effect attacks) to update unit attack count for current battle round (for example, Holy Avenger transformed into Imp before its turn will no longer attack twice);
- Fixes leader transformation (doppelganger, drain-level, transform-self/other attacks) to allow usage of battle items (potions, orbs and talismans);
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
    <summary>Button to sell all valuables with a single click;</summary>

    - Add sell confirmation text to TApp.dbf. The text must contain '%PRICE%' keyword in it;
    - Specify id of the text in 'sellAllValuables' field inside 'Scripts\\textids.lua' (default id is X015TA0001).

    In case of missing text, the following default message will be shown:
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
    <summary>Button to sell all items with a single click;</summary>

    - Add sell confirmation text to TApp.dbf. The text must contain '%PRICE%' keyword in it;
    - Specify id of the text in 'sellAllItems' field inside 'Scripts\\textids.lua'.

    In case of missing text, the following default message will be shown:
    ```
    Do you want to sell all items? Revenue will be:\n%PRICE%
    ```

    Add button with name BTN\_SELL\_ALL to DLG\_MERCHANT in Interf.dlg file.
    Button is optional.

    Example of button description in Interf.dlg:
    ```
    BUTTON	BTN_SELL_ALL,417,425,460,460,DLG_CITY_STACK_RETURN_D,DLG_CITY_STACK_RETURN_H,DLG_CITY_STACK_RETURN_C,DLG_CITY_STACK_RETURN_D,"Sell all items",0
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
    <summary>Fixes bestow wards attack bugs and restrictions;</summary>

    - Fixes wards (hereafter called "modifiers") becoming permanent when more than 8 of them are applied simultaneously;<br />
      Up to 8 modifiers can be applied to each unit (8 x 6 = 48 total).<br />
      If needed, set QTY_WARDS to 5 (6, 7 or 8) and create additional WARD5 (6, 7, 8) columns in GAttacks.dbf.<br />
      ![Video example](https://user-images.githubusercontent.com/5180699/118235525-8ccc5a80-b49d-11eb-81ba-51e394d960b2.mp4)
    - Fixes modifiers becoming permanent after modified unit is transformed;
    - Fixes disappearing of modifiers applied on first attack of double attack;
    - Fixes attack source wards being reset incorrectly if its modifier also contains hp, regen or armor element;
    - Fixes attack class wards not being reset preventing it from being reapplied during battle;
    - Fixes incorrect handling of existing unit immunities;
    - Allows to apply modifiers even if there are no attack source wards among them;
    - Allows to target unit with a secondary attack even if there are no modifiers that can be applied;
    - Allows to heal and apply modifiers at once. Set QTY\_HEAL > 0 in GAttacks.dbf and HEAL in GDynUpgr.dbf:
      - Allows to heal retreating allies by primary or secondary attack;
      - Allows to heal allies when battle ends, same as ordinary heal;
    - Allows to use revive as a secondary attack;
  </details>
- <details>
    <summary>Allows attack class immunity modifiers to be applied (e.g. banner of fear warding);</summary>

    - Add IMMUNITYC and IMMUNECATC columns to GmodifL.dbf (similar to IMMUNITY and IMMUNECAT);
    - Add a new record with TYPE 14, and fill the columns accordingly to attack class and immunity.

    Also works in pure vanilla version.<br />
    ![Video example](https://user-images.githubusercontent.com/5180699/117055075-6bdc6a80-ad23-11eb-87f8-fcbb6f465657.mp4)

  </details>
- <details>
    <summary>Fixes missing attack information in unit encyclopedia;</summary>

    - Damage of secondary attack if its not either poison, blister or frostbite;
    - Power (if applicable), source and reach of alternative attack;
    - Value of boost/lower damage if its secondary attack;
    - Value of lower initiative;
    - Critical hit indication;
    - Infinite effect indication;
    - Custom attack sources;
    - Custom attack reaches;
    - Custom attack damage ratios.

    Some text can be customized, like critical-hit / infinite-effect indication (check [textids.lua](Scripts/textids.lua) for complete list):
    - Add desired text to TApp.dbf and TAppEdit.dbf;
    - Specify text id in a corresponding field inside 'Scripts\\textids.lua'.
  </details>
- <details>
    <summary>Supports custom attack sources;</summary>

    - Add a name for a custom source to TApp.dbf and TAppEdit.dbf;
    - Add NAME_TXT (Character, size 10) and IMMU_AI_R (Numeric, size 2) columns to LattS.dbf;
    ![image](https://user-images.githubusercontent.com/5180699/122281972-8cd3d600-cef3-11eb-8795-69c09d4da288.png)
    - Add a new entry in LattS.dbf;
    - Specify the custom source ID and TEXT accordingly;
    - Specify the id of the source name from TApp.dbf in NAME_TXT ('X005TA0153' for instance);
    - Specify IMMU_AI_R: AI rating of the source immunity - used to determine how powerful a unit with such ward or immunity is. The greater - the better. For example, elemental immunities have average rating of 5, while weapon immunity has 57. Can be omitted - 5 is the default.

    ![image](https://user-images.githubusercontent.com/5180699/122281194-ac1e3380-cef2-11eb-902a-29821d0ceae5.png)

    **Note** that the SOURCE column is limited to 1 digit in GAttacks.dbf.<br />
    This means that only 2 additional sources (with id 8 and 9) can be added by default.<br />
    The limit can be lifted by extending the SOURCE column size to 2 digits (similar to CLASS).<br />
    For example, using Sdbf: go to main manu Table > Change structure, set SOURCE size to 2 and hit save:
    ![image](https://user-images.githubusercontent.com/5180699/117063431-7f8cce80-ad2d-11eb-8765-b0cadaa80567.png)
  </details>
- <details>
    <summary>Supports custom attack reaches;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/122282606-46cb4200-cef4-11eb-9774-e479edc00d21.mp4). Customizable via Lua scripting and additional columns in LAttR.dbf.<br />
    [Scripts](Scripts) includes example targeting scripts demonstrated in the video above.<br />
    [Examples](Examples) includes an example of LAttR.dbf.<br />

    Additional columns of LAttR.dbf:
    - REACH_TXT (Character, size 10) specifies an id for 'Reach' encyclopedia description from TApp.dbf and TAppEdit.dbf. For example 'X005TA0201' is the standard 'Adjacent units';
    - TARGET_TXT (Character, size 10) similar to REACH_TXT but for 'Targets' entry (either '1' or '6' in vanilla);
    - SEL_SCRIPT (Character, size 48) contains a file name of a targeting script from Scripts directory. The script determines which units allowed to be **selected** for attack;
    - ATT_SCRIPT (Character, size 48) similar to SEL_SCRIPT, but determines which units will be **affected** by attack;
    - MRK_TARGTS (Logical) determines whether ATT_SCRIPT should be used to also mark targets with circle animation on battlefield. Usually should be **true**, except when the attack affects random targets (L_CHAIN for instance);
    - MAX_TARGTS (Numeric, size 1) specifies maximum number of targets that can be affected by attack. Used for AI rating calculations, and for damage ratio display formatting in unit encyclopedia;
    - MELEE (Logical) determines whether the attack considered as melee. Used by AI for unit hiring, positioning and targeting.

    Example descriptions for TARGET_TXT:
    ```
    X005TA1000 All adjacent units
    X005TA1001 The target and all units adjacent to it
    X005TA1002 The target and one unit adjacent to it
    X005TA1003 All units in the adjacent line
    X005TA1004 The target and the unit behind it
    X005TA1005 The target and the unit behind it
    X005TA1006 All units in the target line
    X005TA1007 All units in the target column
    X005TA1008 All units in 2x2 area
    X005TA1009 Randomly bounces to 2 additional targets
    X005TA1010 The target and one random unit
    ```

    **Note** that the REACH column is limited to 1 digit in GAttacks.dbf.<br />
    This means that only 6 additional reaches (id 4-9) can be added by default.<br />
    The limit can be lifted by extending the REACH column size to 2 digits (similar to CLASS).<br />
    For example, using Sdbf: go to main manu Table > Change structure, set REACH size to 2 and hit save:
    ![image](https://user-images.githubusercontent.com/5180699/124194675-af5c1680-dad1-11eb-97d3-a59637594b37.png)
  </details>
- <details>
    <summary>Supports custom attack damage ratio for additional targets;</summary>

    The main purpose is to complement custom attack reaches.<br />
    Allows to reduce or increase incoming damage for additional attack targets:
    - Add DAM_RATIO (Numeric, size 3), DR_REPEAT (Logical) and DAM_SPLIT (Logical) columns to Gattacks.dbf;
    - DAM_RATIO specifies a portion of the attack damage received by additional targets (0-255%). 100 or empty is the vanilla behavior;
    - DR_REPEAT specifies whether the DAM_RATIO should be applied for every consequent target. For instance, if QTY_DAM = 50 and DAM_RATIO = 20, then the first target receives 50, second - 10 (50 * 0,2), third - 2 (10 * 0,2) and so on.
    - DAM_SPLIT specifies whether the attack damage (QTY_DAM) is split between all the affected targets. Total damage is multiplied by 'splitDamageMultiplier' setting for better late-game scaling (default of 300 max damage split among 6 targets is miserable).<br />
    For instance, if QTY_DAM = 100 and there are 3 affected targets, then every target will receive 33 damage.<br />
    DAM_RATIO and DR_REPEAT also apply, but determine a ratio between additional and first target damage.<br />
    For instance, if QTY_DAM = 100, DAM_RATIO = 25 and there are 3 affected targets then the damage distribution will be 67-17-17 (67 * 0,25 ~ 17; 67 + 17 + 17 ~ 100).

    ![image](https://user-images.githubusercontent.com/5180699/124194976-2b565e80-dad2-11eb-8395-58614dcd669f.png)

    **Note** that damage ratio representation in unit encyclopedia can be customized. Check [textids.lua](Scripts/textids.lua).
  </details>
- <details>
    <summary>Adds new event conditions;</summary>

    - Replace LEvCond.dbf in globals folder with the file from [Examples](Examples/LEvCond.dbf).
    - Add contents of [ScenEdit.dlg](Examples/ScenEdit.dlg) to interf\ScenEdit.dlg.
    - Translate menus and buttons if needed. Also, add translated text ids to [textids.lua](Scripts/textids.lua).
    - Add translations for brief (BRIEF) and full (DESCR) event condition descriptions to interf\TAppEdit.dbf.
    - Set ids of these translations to corresponding columns in LEvCond.dbf.
    - Text strings in INFO column must contain predefined keys for game to show actual game data.

    Examples:
    - L_OWN\_RESOURCE: ```"Own %COND% %GOLD% gold, %INFERNAL% infernal, %LIFE% life, %DEATH% death, %RUNIC% runic, %GROVE% grove."```
    - L_GAME_MODE: ```"Game mode: %MODE%"```
    - L_PLAYER_TYPE: ```"Player is controlled by %TYPE%"```
    - L_SCRIPT: ```"Script: %DESC%"```
    - L_VARIABLE_CMP does not use text id from INFO column, so it can be set as 'g0000000000'.   
  </details>
    
### Settings:
The following settings can be changed in [settings.lua](Scripts/settings.lua):
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
  - "detailedAttackDescription=(true/false)" fixes missing attack information in unit encyclopedia;
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
  - "splitDamageMultiplier=\[1 : 6\]" multiplies total damage dealt by split damage (DAM_SPLIT). See 'custom attack damage ratio' under Features section;
  - "drainAttackHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN attacks damage used as heal;
  - "drainOverflowHeal=\[-(2^31 - 1) : (2^31 - 1)\]" changes percentage of L\_DRAIN\_OVERFLOW attacks damage used as heal;
  - "doppelgangerRespectsEnemyImmunity=(true/false)" sets whether doppelganger attack checks for enemy ward/immunity to the attack class and source;
  - "doppelgangerRespectsAllyImmunity=(true/false)" sets whether doppelganger attack checks for ally ward/immunity to the attack class and source;
  - "leveledDoppelgangerAttack=(true/false)" changes doppelganger attacks to copy units using with respect to their level using 'Scripts\\doppelganger.lua' script;
  - "leveledTransformSelfAttack=(true/false)" changes transform self attacks to compute transformed unit level using 'Scripts\\transformSelf.lua' script;
  - "leveledTransformOtherAttack=(true/false)" changes transform other attacks to compute transformed unit level using 'Scripts\\transformOther.lua' script;
  - "leveledDrainLevelAttack=(true/false)" changes drain level attacks to compute transformed unit level using 'drainLevel.lua' script;
  - "leveledSummonAttack=(true/false)" changes summon attacks to compute summoned units levels using 'Scripts\\summon.lua' script;
  - "unrestrictedBestowWards=(true/false)" increases total wards limit per caster from 8 to 48, see more details under Features section;
  - "freeTransformSelfAttack=(true/false)" allows transform-self attack to not consume a unit turn for transformation (once per turn), see more details under Features section;
  - "freeTransformSelfAttackInfinite=(true/false)" allows free transform-self attack to be used infinite number of times per single turn;
  - "freeTransformSelfAttackUntilAction=(true/false)" restricts transform-self attack to be free only until other action is performed (normal attack or item usage);
  - "disableAllowedRoundMax=\[1 : (2^31 - 1)\]" sets a number of battle round after which paralyze and petrify attacks will constantly miss;
  - "missChanceSingleRoll=(true/false)" if true, switches attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;
  - "mageLeaderAccuracyReduction=\[0 : 100\]" allows to set accuracy reduction for mage leaders per each additional target;
  - "aiAccuracyBonus" allows to configure AI accuracy in battle for different game difficulty modes:
    - "easy=\[-127 : 127\]";
    - "average=\[-127 : 127\]";
    - "hard=\[-127 : 127\]";
    - "veryHard=\[-127 : 127\]";
    - "absolute=(true/false)" determines how accuracy bonuses are treated - as absolute or percentage values;
  - "allowBattleItems" allows transformed leaders to use battle items:
    - "onTransformOther=(true/false)" if leader is transformed by TransformOther attack (Witch, orb/talisman, artifact effect, etc.);
    - "onTransformSelf=(true/false)" if leader is transformed by TransformSelf attack (Wolf Lord, orb/talisman, artifact effect, etc.);
    - "onDrainLevel=(true/false)" if leader's level is drained by DrainLevel attack (Wight, orb/talisman, artifact effect, etc.);
    - "onDoppelganger=(true/false)" if leader transformed himself by Doppelganger attack;
</details>
<details>
  <summary>Misc</summary>

  - "preserveCapitalBuildings=(true/false)" allows scenarios with prebuilt capital cities;
  - "carryOverItemsMax=\[0 : (2^31 - 1)\]" changes maximum number of items the player is allowed to transfer between campaign scenarios;
  - "stackMaxScoutRange=\[7 : 100\]" changes maximum allowed scout range for parties; 
  - "debugHooks=(true/false)" create mss32 proxy dll log files with debug info;
</details>

### Scripting:
The toolset uses Lua for settings and advanced game mechanics. See [luaApi](luaApi.md) for detailed information.

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

