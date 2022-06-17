# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:

#### General
- Can be used on vanilla version or with other mods installed;
- Allows players to search and create PvP matches without external software using custom lobby server. Currently only for [Motlin's mod](https://dis2modding.fandom.com/ru/wiki/Мод_Мотлина).
- Increases maximum game turn to 9999;
- Allows to load and create scenarios with no magic (maximum spell level set to 0);
- Buildings up to tier 10 are supported in editor and game;
- <details>
    <summary>Adds new custom building category for unit hire;</summary>

    - Place new buildings on the 'Other buildings' tab using game resources;
    - Create custom category by adding `L_CUSTOM` record to `LBuild.dbf`.
  </details>
- <details>
    <summary>Allows each race to hire up to 10 new tier-1 units in cities and capital;</summary>

    For each new unit, add a new column `SOLDIER_N` to `Grace.dbf`, where N starts from 6.
  </details>
- <details>
    <summary>Allows scenarios with prebuilt capital cities;</summary>

    - Enable `preserveCapitalBuildings` in [settings.lua](Scripts/settings.lua);
    - Start a scenario;
    - Build desired buildings in a capital;
    - Save the scenario;
    - Move the saved game from 'SaveGame' folder to 'Exports';
    - Restart the scenario.
  </details>
- <details>
    <summary>Allows to set a maximum number of items the player is allowed to transfer between campaign scenarios;</summary>

    Specify `carryOverItemsMax` in [settings.lua](Scripts/settings.lua).
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
    <summary>Provides debug log files to help mod makers;</summary>

    Enable `debugHooks` in [settings.lua](Scripts/settings.lua).<br />
    **Don't forget to turn it off in release package of your mod to avoid cluttering and improve performance.**
  </details>

#### User interface
- <details>
    <summary>Allows banners, resources panel and converted land percentage to be displayed by default;</summary>

    Use the following settings in [settings.lua](Scripts/settings.lua):
    - `showBanners`
    - `showResources`
    - `showLandConverted`
  </details>
- <details>
    <summary>Buttons for bulk item transfer: transfer all items, potions, scrolls/wands or valuables between inventories with single click;</summary>

    Add buttons with predefined names to `DLG_CITY_STACK`, `DLG_EXCHANGE` or `DLG_PICKUP_DROP` dialogs in Interf.dlg file.<br />
    Every button is optional and can be ignored.<br />
    Buttons and their meaning:
    - Transfer all items to the left inventory: `BTN_TRANSF_L_ALL`;
    - Transfer all items to the right inventory: `BTN_TRANSF_R_ALL`;
    - Transfer all potions to the left: `BTN_TRANSF_L_POTIONS`;
    - Transfer all potions to the right: `BTN_TRANSF_R_POTIONS`;
    - Transfer all scrolls and wands to the left: `BTN_TRANSF_L_SPELLS`;
    - Transfer all scrolls and wands to the right: `BTN_TRANSF_R_SPELLS`;
    - Transfer all valuables to the left: `BTN_TRANSF_L_VALUABLES`;
    - Transfer all valuables to the right: `BTN_TRANSF_R_VALUABLES`;

    Example of button description in `Interf.dlg`:
    ```
    BUTTON    BTN_TRANSF_L_ALL,342,424,385,459,DLG_EXCHANGE_RETURN_D,DLG_EXCHANGE_RETURN_H,DLG_EXCHANGE_RETURN_C,DLG_EXCHANGE_RETURN_D,"Transfer all items to left",0
    ```
  </details>
- <details>
    <summary>Button to sell all valuables with a single click;</summary>

    - Add sell confirmation text to `TApp.dbf`. The text must contain `%PRICE%` keyword in it;
    - Specify id of the text in `sellAllValuables` field inside [textids.lua](Scripts/textids.lua) (default id is X015TA0001);
    - Add button with name `BTN_SELL_ALL_VALUABLES` to `DLG_MERCHANT` in `Interf.dlg` file.

    In case of missing text, the following default message will be shown:
    ```
    Do you want to sell all valuables? Revenue will be:\n%PRICE%
    ```

    Example of button description in Interf.dlg:
    ```
    BUTTON	BTN_SELL_ALL_VALUABLES,417,425,460,460,DLG_CITY_STACK_RETURN_D,DLG_CITY_STACK_RETURN_H,DLG_CITY_STACK_RETURN_C,DLG_CITY_STACK_RETURN_D,"Sell all valuables",0
    ```
  </details>
- <details>
    <summary>Button to sell all items with a single click;</summary>

    - Add sell confirmation text to `TApp.dbf`. The text must contain `%PRICE%` keyword in it;
    - Specify id of the text in `sellAllItems` field inside [textids.lua](Scripts/textids.lua).
    - Add button with name `BTN_SELL_ALL` to `DLG_MERCHANT` in `Interf.dlg` file.

    In case of missing text, the following default message will be shown:
    ```
    Do you want to sell all items? Revenue will be:\n%PRICE%
    ```

    Example of button description in Interf.dlg:
    ```
    BUTTON	BTN_SELL_ALL,417,425,460,460,DLG_CITY_STACK_RETURN_D,DLG_CITY_STACK_RETURN_H,DLG_CITY_STACK_RETURN_C,DLG_CITY_STACK_RETURN_D,"Sell all items",0
    ```
  </details>
- <details>
    <summary>Button to show grid in game;</summary>

    Add toggle button with name `TOG_GRID` to `DLG_ISO_LAND` in Interf.dlg.

    Example of button description in Interf.dlg:
    ```
    TOGGLE  TOG_GRID,128,63,160,92,DLG_ISO_GRID_N,DLG_ISO_GRID_H,DLG_ISO_GRID_C,DLG_ISO_GRID_D,DLG_ISO_GRID_H,DLG_ISO_GRID_H,DLG_ISO_GRID_H,"",71
    ```
  </details>
- <details>
    <summary>Adds missing attack information in unit encyclopedia;</summary>

    - Enable `unitEncyclopedia.detailedAttackDescription` in [settings.lua](Scripts/settings.lua);
    - Add interface text for the following entries in `TApp.dbf` and `TAppEdit.dbf`:
        - `infiniteAttack`
        - `critHitAttack`
        - `critHitDamage`
        - `ratedDamage`
        - `ratedDamageEqual`
        - `ratedDamageSeparator`
        - `splitDamage`
        - `modifiedValue`
        - `drainDescription`
        - `drainEffect`
        - `overflowAttack`
        - `overflowText`
    - Specify corresponding text ids in [textids.lua](Scripts/textids.lua);
    - (Optional) Consider adding drain attack description:
        - Find text constants with ids `X005TA0787` and `X005TA0788` in `TApp.dbf` and `TAppEdit.dbf`;
        - Add `%DRAIN%` keyword where you like to put the description (propose to place it after damage field like `%DAMAGE%\n%DRAIN%`);
        - The keyword is replaced with empty string if attack has no drain effect;
        - Note that you can freely move content between `X005TA0787` and `X005TA0788` if you run out of length limit (because the two strings simply merged together in `X005TA0424`).
    
    The following information is added:
    - Damage of secondary attack if its not either poison, blister or frostbite;
    - Power (if applicable) and source (if it matters) of alternative attack;
    - Value of boost/lower damage if its secondary attack;
    - Value of lower initiative;
    - Critical hit indication;
    - Infinite effect indication;
    - Drain attack description;
    - Custom attack sources;
    - Custom attack reaches;
    - Custom attack damage ratios;
    - Custom unit modifiers.
  </details>

- <details>
    <summary>Shows effective HP in unit encyclopedia;</summary>

    Add text box with name `TXT_EFFECTIVE_HP` to `DLG_R_C_UNIT` in `Interf.dlg` and `ScenEdit.dlg` files.<br />
    Specify text id from `TApp.dbf` and `TAppEdit.dbf` that contains key `%HP%`.
    
    Example of text box description in Interf.dlg:
    ```
    TEXT    TXT_EFFECTIVE_HP,468,95,663,231,,"X015TA0002",""
    ```
  </details>

#### Strategic map
- <details>
    <summary>Allows to display movement cost for each individual step of parties;</summary>

    See `movementCost` category in [settings.lua](Scripts/settings.lua):
    - Enable `show` to display movement cost;
    - `textColor` can be used to specify a color (RGB) of the text;
    - `outlineColor` can be used to specify a color (RGB) of the text outline.
  </details>
- <details>
    <summary>Allows to specify a maximum allowed scout range for parties;</summary>
    
    Specify `stackMaxScoutRange` in [settings.lua](Scripts/settings.lua).
  </details>
- <details>
    <summary>Allows unit regeneration modifiers to stack;</summary>

    Enable `cumulativeUnitRegeneration` under `modifiers` category in [settings.lua](Scripts/settings.lua).<br />
    By default, the game picks single highest value, then sums it with lord, terrain and city bonuses;
  </details>
- <details>
    <summary>Adds new event conditions;</summary>

    - Replace `LEvCond.dbf` with [LEvCond.dbf](Examples/LEvCond.dbf);
    - Add contents of [ScenEdit.dlg](Examples/ScenEdit.dlg) to `ScenEdit.dlg`;
    - Translate menus and buttons if needed. Also, add translated text ids to [textids.lua](Scripts/textids.lua);
    - Add translations for brief (`BRIEF`) and full (`DESCR`) event condition descriptions to `TAppEdit.dbf`;
    - Set ids of these translations to corresponding columns in `LEvCond.dbf`;
    - Text strings in `INFO` column must contain predefined keys for game to show actual game data.

    Examples:
    - `TEXT` is `L_OWN_RESOURCE`, `INFO` refers to `Own %COND% %GOLD% gold, %INFERNAL% infernal, %LIFE% life, %DEATH% death, %RUNIC% runic, %GROVE% grove`;
    - `TEXT` is `L_GAME_MODE`, `INFO` refers to `Game mode: %MODE%`;
    - `TEXT` is `L_PLAYER_TYPE`, `INFO` refers to `Player is controlled by %TYPE%`;
    - `TEXT` is `L_SCRIPT`, `INFO` refers to `Script: %DESC%`;
    - `TEXT` is `L_VARIABLE_CMP` does not use text id from `INFO` column, so it can be set as 'g0000000000'.   
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
    <summary>Neutral cities can use race-specific graphics;</summary>

    Add cities graphics with names `G000FT0000NE<tier><race>` to isoAnim.ff and isoStill.ff.<br />
    `tier` is a city tier, values from 1 to 5.<br />
    `race` is a race suffix:
    - `DW` for Mountain Clans;
    - `EL` for Elven Alliance;
    - `HE` for Legions of the Damned;
    - `HU` for Empire;
    - `UN` for Undead Hordes;    
  </details>
- Allows Scenario Editor to place merchants, mages, trainers and mercenaries on water tiles;
- Allows Scenario Editor to place more than 200 stacks on a map;

#### Battle mechanics
- <details>
    <summary>Allows to set maximum unit damage and armor;</summary>
    
    Specify `unitMaxDamage` and `unitMaxArmor` in [settings.lua](Scripts/settings.lua).
  </details>
- <details>
    <summary>Allows to set critical hit damage and chance;</summary>

    Specify `criticalHitDamage` and `criticalHitChance` in [settings.lua](Scripts/settings.lua).

    Aside from that, these numbers can be customized for each particular attack:
    - Add `CRIT_DAM` (Numeric, size 3) and `CRIT_POWER` (Numeric, size 3) columns to `Gattacks.dbf`;
    - Check `CRIT_HIT` column to enable critical hit for attacks that you wish to customize;
    - `CRIT_DAM` specifies a critical hit damage (0-255%). Falls back to `criticalHitDamage` if empty;
    - `CRIT_POWER` specifies a critical hit chance (0-100%). Falls back to `criticalHitChance` if empty;
    - Add interface text for `critHitAttack` and `critHitDamage` in `TApp.dbf` and `TAppEdit.dbf`;
    - Specify corresponding text ids in [textids.lua](Scripts/textids.lua).

    ![image](https://user-images.githubusercontent.com/5180699/155902276-44d843ab-b799-4997-ad3c-886bf657107f.png)
  </details>
- <details>
    <summary>Allows to set AI accuracy bonuses for different game difficulty modes;</summary>

    See `aiAccuracyBonus` category in [settings.lua](Scripts/settings.lua):
    - Specify `easy`, `average`, `hard` and `veryHard` according to difficulty modes;
    - `absolute` determines how accuracy bonuses are treated - as absolute or percentage values.
  </details>
- <details>
    <summary>Allows to set accuracy reduction for mage leaders per each additional target;</summary>

    Specify `mageLeaderAccuracyReduction` in [settings.lua](Scripts/settings.lua).
  </details>
- <details>
    <summary>Allows to set a number of battle round after which paralyze and petrify attacks will constantly miss;</summary>

    Specify `disableAllowedRoundMax` in [settings.lua](Scripts/settings.lua).
  </details>
- <details>
    <summary>Allows to switch attacks miss check to a single random value roll instead of check against arithmetic mean of two random numbers;</summary>

    Enable `missChanceSingleRoll` in [settings.lua](Scripts/settings.lua).
  </details>
- Allows alchemists to give additional attacks to retreating allies;
- <details>
    <summary>Allows to set vampiric attacks heal ratio;</summary>

    Specify `drainAttackHeal` and `drainOverflowHeal` in [settings.lua](Scripts/settings.lua).
  </details>
- Allows vampiric attacks to deal critical damage;
- <details>
    <summary>Allows to customize shatter attacks: maximum damage per hit, maximum armor that can be shattered, whether attack can miss or not, and its upgrade ratio;</summary>

    Use the following settings in [settings.lua](Scripts/settings.lua):
    - `shatteredArmorMax`
    - `shatterDamageMax`
    - `shatterDamageUpgradeRatio`
    - `allowShatterAttackToMiss`
  </details>
- <details>
    <summary>Allows doppelganger, transform-self, transform-other, drain-level and summon attacks to produce leveled versions of units;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/158679921-7b6d82b1-2c48-44bb-91c6-fa1983893eab.mp4)

    Use the following settings in [settings.lua](Scripts/settings.lua):
    - `leveledDoppelgangerAttack`
    - `leveledTransformSelfAttack`
    - `leveledTransformOtherAttack`
    - `leveledDrainLevelAttack`
    - `leveledSummonAttack`
  </details>
- <details>
    <summary>Allows doppelganger attacks to respect enemy and ally wards and immunities to the attack class and source;</summary>

    Enable `doppelgangerRespectsEnemyImmunity` and `doppelgangerRespectsAllyImmunity` in [settings.lua](Scripts/settings.lua).
  </details>
- <details>
    <summary>Allows transform-self attack to not consume a unit turn for transformation;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/124916545-76550200-dffb-11eb-8b12-3147e40ef04b.mp4)

    - Enable `freeTransformSelfAttack` in [settings.lua](Scripts/settings.lua);
    - `freeTransformSelfAttackInfinite` allows the attack to be used infinite number of times per single turn;
    - Logic can be customized using `getFreeAttackNumber` function in [transformSelf.lua](Scripts/transformSelf.lua).
  </details>
- <details>
    <summary>Allows transformed leaders (doppelganger, drain-level, transform-self/other attacks) to use battle items (potions, orbs and talismans);</summary>

    See `allowBattleItems` category in [settings.lua](Scripts/settings.lua):
    - `onTransformOther`
    - `onTransformSelf`
    - `onDrainLevel`
    - `onDoppelganger`
  </details>
- <details>
    <summary>Increases total ward limit for bestow-wards attack from 8 to 48;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/118235525-8ccc5a80-b49d-11eb-81ba-51e394d960b2.mp4)

    Up to 8 modifiers can be applied to each unit (8 x 6 = 48 total):
    - Enable `unrestrictedBestowWards` in [settings.lua](Scripts/settings.lua);
    - If needed, set `QTY_WARDS` to 5 (6, 7 or 8) and create additional `WARD5` (6, 7, 8) columns in `GAttacks.dbf`.
  </details>
- <details>
    <summary>Allows attack class immunity modifiers to be applied (e.g. Banner of Fear Warding);</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/117055075-6bdc6a80-ad23-11eb-87f8-fcbb6f465657.mp4)

    - Add `IMMUNITYC` and `IMMUNECATC` columns to `GmodifL.dbf` (similar to `IMMUNITY` and `IMMUNECAT`);
    - Add a new record with `TYPE` 14, and fill the columns accordingly to attack class and immunity.
    
    Note that this also works in pure vanilla version.
  </details>
- <details>
    <summary>Supports custom attack sources;</summary>

    - Add a name for a custom source to `TApp.dbf` and `TAppEdit.dbf`;
    - Add `NAME_TXT` (Character, size 10) and `IMMU_AI_R` (Numeric, size 2) columns to `LattS.dbf`;
    - Add a new entry in `LattS.dbf`;
    - Specify the custom source `ID` and `TEXT` accordingly;
    - Specify the id of the source name from `TApp.dbf` in `NAME_TXT` ('X005TA0153' for instance);
    - Specify `IMMU_AI_R`: AI rating of the source immunity - used to determine how powerful a unit with such ward or immunity is. The greater - the better. For example, elemental immunities have average rating of 5, while weapon immunity has 57. Can be omitted - 5 is the default;
    - Consider adding vertical align to unit encyclopedia fields to properly accommodate custom attack source text:
        - Find text constants with ids `X005TA0787` and `X005TA0788` in `TApp.dbf` and `TAppEdit.dbf`;
        - Note how attack name fields `%TWICE%%ALTATTACK%%ATTACK%%SECOND%` are enclosed in vertical align `\p110;` and `\p0;`;
        - Use the same technique to enclose `%SOURCE%%SOURCE2%` field in `X005TA0788` (like `\p110;%SOURCE%%SOURCE2%\p0;`);
        - Note that you can freely move content between `X005TA0787` and `X005TA0788` if you run out of length limit (because the two strings simply merged together in `X005TA0424`).

    ![image](https://user-images.githubusercontent.com/5180699/122281194-ac1e3380-cef2-11eb-902a-29821d0ceae5.png)

    **Note** that the `SOURCE` column is limited to 1 digit in `GAttacks.dbf`.<br />
    This means that only 2 additional sources (with id 8 and 9) can be added by default.<br />
    The limit can be lifted by extending the `SOURCE` column size to 2 digits (similar to `CLASS`).<br />
    For example, using Sdbf: go to main manu Table > Change structure, set `SOURCE` size to 2 and hit save:
    ![image](https://user-images.githubusercontent.com/5180699/117063431-7f8cce80-ad2d-11eb-8765-b0cadaa80567.png)
  </details>
- <details>
    <summary>Supports custom attack reaches;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/122282606-46cb4200-cef4-11eb-9774-e479edc00d21.mp4). Customizable via Lua scripting and additional columns in LAttR.dbf.<br />
    [Scripts](Scripts) includes example targeting scripts demonstrated in the video above.<br />
    [Examples](Examples) includes an example of LAttR.dbf.<br />

    Additional columns of LAttR.dbf:
    - `REACH_TXT` (Character, size 10) specifies an id for 'Reach' encyclopedia description from `TApp.dbf` and `TAppEdit.dbf`. For example 'X005TA0201' is the standard 'Adjacent units';
    - `TARGET_TXT` (Character, size 10) is similar to `REACH_TXT` but for 'Targets' entry (either '1' or '6' in vanilla);
    - `SEL_SCRIPT` (Character, size 48) contains a file name of a targeting script from [Scripts](Scripts). The script determines which units allowed to be **selected** for attack;
    - `ATT_SCRIPT` (Character, size 48) is similar to `SEL_SCRIPT`, but determines which units will be **affected** by attack;
    - `MRK_TARGTS` (Logical) determines whether `ATT_SCRIPT` should be used to also mark targets with circle animation on battlefield. Usually should be **true**, except when the attack affects random targets (`L_CHAIN` for instance);
    - `MAX_TARGTS` (Numeric, size 1) specifies maximum number of targets that can be affected by attack. Used for AI rating calculations, and for damage ratio display formatting in unit encyclopedia;
    - `MELEE` (Logical) determines whether the attack considered as melee. Used by AI for unit hiring, positioning and targeting;
    - Consider adding vertical align to unit encyclopedia fields to properly accommodate custom attack reach text:
        - Find text constants with ids `X005TA0787` and `X005TA0788` in `TApp.dbf` and `TAppEdit.dbf`;
        - Note how attack name fields `%TWICE%%ALTATTACK%%ATTACK%%SECOND%` are enclosed in vertical align `\p110;` and `\p0;`;
        - Use the same technique to enclose `%REACH%` and `%TARGETS%` fields in `X005TA0788` (like `\p110;%REACH%\p0;`);
        - Note that you can freely move content between `X005TA0787` and `X005TA0788` if you run out of length limit (because the two strings simply merged together in `X005TA0424`).

    Example descriptions for `TARGET_TXT`:
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

    **Note** that the `REACH` column is limited to 1 digit in `GAttacks.dbf`.<br />
    This means that only 6 additional reaches (id 4-9) can be added by default.<br />
    The limit can be lifted by extending the `REACH` column size to 2 digits (similar to `CLASS`).<br />
    For example, using Sdbf: go to main manu Table > Change structure, set `REACH` size to 2 and hit save:
    ![image](https://user-images.githubusercontent.com/5180699/124194675-af5c1680-dad1-11eb-97d3-a59637594b37.png)
  </details>
- <details>
    <summary>Supports custom attack damage ratios for additional targets;</summary>

    The main purpose is to complement custom attack reaches.<br />
    Allows to reduce or increase incoming damage for additional attack targets:
    - Add `DAM_RATIO` (Numeric, size 3), `DR_REPEAT` (Logical) and `DAM_SPLIT` (Logical) columns to `Gattacks.dbf`;
    - `DAM_RATIO` specifies a portion of the attack damage received by additional targets (0-255%). 100 or empty is the vanilla behavior;
    - `DR_REPEAT` specifies whether the `DAM_RATIO` should be applied for every consequent target;
    - `DAM_SPLIT` specifies whether the attack damage (`QTY_DAM`) is split between all the affected targets;
    - `splitDamageMultiplier` in [settings.lua](Scripts/settings.lua) specifies multiplier for `DAM_SPLIT` damage for better late-game scaling (default of 300 max damage split among 6 targets is miserable);
    - Add interface text for the following entries in `TApp.dbf` and `TAppEdit.dbf`:
        - `ratedDamage`
        - `ratedDamageEqual`
        - `ratedDamageSeparator`
        - `splitDamage`
    - Specify corresponding text ids in [textids.lua](Scripts/textids.lua);
    - Consider adding vertical align to unit encyclopedia fields to properly accommodate damage ratio text:
        - Find text constants with ids `X005TA0787` and `X005TA0788` in `TApp.dbf` and `TAppEdit.dbf`;
        - Note how attack name fields `%TWICE%%ALTATTACK%%ATTACK%%SECOND%` are enclosed in vertical align `\p110;` and `\p0;`;
        - Use the same technique to enclose `%DAMAGE%` field in `X005TA0788` (like `\p110;%DAMAGE%\p0;`);
        - Note that you can freely move content between `X005TA0787` and `X005TA0788` if you run out of length limit (because the two strings simply merged together in `X005TA0424`).

    ![image](https://user-images.githubusercontent.com/5180699/124194976-2b565e80-dad2-11eb-8395-58614dcd669f.png)

    How `DR_REPEAT` works:
    - For instance, if `QTY_DAM` = 50 and `DAM_RATIO` = 20, then the first target receives 50, second - 10 (50 * 0,2), third - 2 (10 * 0,2) and so on.

    How `DAM_SPLIT` works:
    - For instance, if `QTY_DAM` = 100 and there are 3 affected targets, then every target will receive 33 damage;
    - `DAM_RATIO` and `DR_REPEAT` also apply, but determine a ratio between additional and first target damage. For instance, if `QTY_DAM` = 100, `DAM_RATIO` = 25 and there are 3 affected targets then the damage distribution will be 67-17-17 (67 * 0,25 ~ 17; 67 + 17 + 17 ~ 100).
    
    How `splitDamageMultiplier` works:
    - For instance, if the multiplier is 6 and unit attack damage is 300, then its final damage will be 300 * 6 = 1800;
    - Take the multiplier into account when picking base values for `QTY_DAM` in `Gattacks.dbf` along with `DAMAGE` in `GDynUpgr.dbf`;
    - The multiplied damage is correctly displayed in unit encyclopedia.
  </details>
- <details>
    <summary>Supports custom unit modifiers;</summary>

    ![Demo video](https://user-images.githubusercontent.com/5180699/169149545-9f6e8284-6325-4ccd-a555-5db8851569e4.mp4). Customizable via Lua scripting.<br />
    [Scripts/Modifiers](Scripts/Modifiers) includes example modifier scripts.<br />
    [template.lua](Scripts/Modifiers/template.lua) contains a complete list of available functions.<br />

    - Add `L_CUSTOM` category to `LModifS.dbf` or simply copy the file from [Examples](Examples);
    - Add `SCRIPT` (Character, size 40) column to `Gmodif.dbf`;
    - Add `DESC_TXT` (Character, size 10) column to `Gmodif.dbf`;
    - Add `DISPLAY` (Logical) column to `Gmodif.dbf`;
    - (Optional) Setup modifiers panel for Unit Encyclopedia:
        - Include `LBOX_MODIFIERS` and `TXT_MODIFIERS` elements to `Interf.dlg` and `ScenEdit.dlg`. [Interf.dlg](Examples/Modifiers/Interf.dlg) and [ScenEdit.dlg](Examples/Modifiers/ScenEdit.dlg) contain examples of modified `DLG_R_C_UNIT` dialog of Unit Encyclopedia;
        - Note that the examples intentionally hide or reposition some native elements: 'Leader abilities', 'Leadership', 'Battles won', locked-unit / upgrade-needed indicators;
        - Try extending the dialog bounds or otherwise rearrange its elements to properly accomodate all the available elements as you like;
        - Specify `modifiersCaption` text in `TApp.dbf` and `TAppEdit.dbf` and its id in a corresponding entry of [textids.lua](Scripts/textids.lua);
        - Repeat for `modifiersEmpty`, `modifierDescription` and `nativeModifierDescription` entries;
    - (Optional) Add a new modifier description and icon (it will be displayed in Scenario Editor and Unit Encyclopedia):
        - Create a description in `Tglobal.dbf`. You can use rich formatting like `\fMedBold;Born Leader\n\fSmall;+1 leadership every 3 levels.\fNormal;`;
        - Create `31x36px` icon in `Icons.ff` (using special software like `D2ResExplorer`). **Its name should correspond to modifier id** to be linked with it (similar to spell icons and other game resources), like `G000UM9048`;
    - Add a new modifier entry in `Gmodif.dbf` (see example [Gmodif.dbf](Examples/Modifiers/Gmodif.dbf)):
        - Specify new `MODIF_ID` (use format `gXXXum9XXX` if you want it to be available as Scenario Editor modifier);
        - Specify `SOURCE` id that corresponds to `L_CUSTOM` category added earlier (example `LModifS.dbf` uses `4` as the id);
        - Specify `SCRIPT` file name that will be used for this modifier (omit file path, for example `berserk.lua`);
        - (Optional) Specify `DESC_TXT` id that corresponds to `Tglobal.dbf` entry added earlier (defaults to the standard stub `x000tg6000`);
        - (Optional) Specify `DISPLAY` that controls whether the modifier should appear in modifiers panel for Unit Encyclopedia (defaults to false);
    - Refer to [Scripts/Modifiers](Scripts/Modifiers) examples and [luaApi](luaApi.md) to create your modifier script;
	- (Optional) Define dynamic display functions inside your modifier script to control its appearance on modifiers panel depending on a current unit:
		- Specify `getModifierDisplay(unit, prev)` function to enable or disable modifier display (defaults to `DISPLAY` in `Gmodif.dbf`);
		- Specify `getModifierDescTxt(unit, prev)` function to redefine modifier description (defaults to `DESC_TXT` in `Gmodif.dbf`);
		- Specify `getModifierIconName(unit, prev)` function to redefine modifier icon (defaults to `MODIF_ID` in `Gmodif.dbf`);
		- See [template.lua](Scripts/Modifiers/template.lua) for examples.
    - Try assigning the created modifier to item, potion or spell, or simply use it as Scenario Editor modifier;
    - Consider adding vertical align to unit encyclopedia fields to properly accommodate custom modifier bonuses text:
        - Find text constants with ids `X005TA0787` and `X005TA0788` in `TApp.dbf` and `TAppEdit.dbf`;
        - Note how attack name fields `%TWICE%%ALTATTACK%%ATTACK%%SECOND%` are enclosed in vertical align `\p110;` and `\p0;`;
        - Use the same technique to enclose `%HIT%%HIT2%` and `%DAMAGE%` fields (like `\p110;%DAMAGE%\p0;`);
        - Note that you can freely move content between `X005TA0787` and `X005TA0788` if you run out of length limit (because the two strings simply merged together in `X005TA0424`).

    ![image](https://user-images.githubusercontent.com/5180699/171748030-5af2d922-58fc-4427-915c-003203192bdd.png)
    ![image](https://user-images.githubusercontent.com/5180699/171749199-09c93dc2-cf3d-4760-a00a-52ce9550814e.png)
    ![image](https://user-images.githubusercontent.com/5180699/171748309-30751f19-0217-4cb3-9608-e337dad3e894.png)
    ![image](https://user-images.githubusercontent.com/5180699/171749123-d29bd060-6a92-41da-b212-733bb39f04db.png)
    ![image](https://user-images.githubusercontent.com/5180699/171748086-b4614eef-f8b1-48db-a172-7fe64328abc9.png)
  </details>
- <details>
    <summary>Supports native unit modifiers;</summary>
    
    Allows to assign 'native' modifiers to unit types.<br>
    That is, a modifier will be permanently applied to all units of the specified type - existing in a scenario or newly created / hired.<br>
    When unit changes its type (transforms or upgrades), modifiers native to its previous type are automatically removed, and new modifiers that are native to the new type are applied.<br>
    Native modifiers are not stored in scenario file, thus you can freely manipulate it without scenario file being affected in any way.
    - Copy [GUmodif.dbf](Examples/Modifiers/GUmodif.dbf) to 'Globals' directory;
    - `UNIT_ID` specifies id of a unit from `GUnits.dbf`. Use empty id (`g000000000`) if you want a modifier to be applied to **every single unit, note it impacts performance so try to avoid this if its not necessary**;
    - `MODIF_1`, `MODIF_2`, ..., `MODIF_N` specifies modifier id from `Gmodif.dbf`. In can be either standard or custom modifier.
  </details>

### Bug fixes:
- Fixes game crash in battles with summoners involved;
- Fixes game crash when AI controlled unit with transform self attack uses alternative attack with 'adjacent' attack range;
- Fixes game crash on 144x144 maps that occurs if there is a party standing on a lower-left or lower-right edge of the map;
- Fixes game crash in unit encyclopedia showing doppelganger info when it copied stack leader;
- Fixes AI unit placement logic for melee units with vampiric attacks;
- Fixes AI targeting for single lower-damage and lower-initiative attacks;
- Fixes AI targeting for shatter attacks where it tends to pick a most armored target ignoring general prioritization;
- Fixes AI targeting for shatter attacks where it assumes that armored targets always stay armored, even if their armor is already shattered;
- <details>
    <summary>Fixes AI targeting where it incorrectly calculates effective HP of armored targets, getting significantly lower values than intended;</summary>

    Enable `fixEffectiveHpFormula` in [settings.lua](Scripts/settings.lua).
  </details>
- Fixes incorrect function of transform-self attack in cases where its alternative attack targets allies (heal, summon, etc.);
- Fixes an issue with transform-other attack where it selects melee vs ranged transform based on attacker position rather than on target position;
- Removes persistent target highlighting for transform-self attack (circle animations for all possible targets on the ground), so it no longer interferes with highlighting for targets that are actually going to be affected by the attack;
- Fixes missing modifiers of alternative attacks (![demo video](https://user-images.githubusercontent.com/5180699/125460215-144ef648-5497-4674-b9d6-ac7d2fa95125.mp4));
- Fixes unit transformation (doppelganger, drain-level, transform-self/other attacks) to include HP modifiers into current hp recalculation, thus unit is not getting "damaged" upon transforming (![demo video](https://user-images.githubusercontent.com/5180699/156921183-b1f4748d-cb4f-40c8-a9b1-b911e9cc91dd.mp4));
- Fixes unit transformation (drain-level, transform-self/other and untransform-effect attacks) to update unit attack count for current battle round (for example, Holy Avenger transformed into Imp before its turn will no longer attack twice);
- <details>
    <summary>Fixes bestow-wards attack bugs and restrictions;</summary>

    - Fixes wards (hereafter called "modifiers") becoming permanent when more than 8 of them are applied simultaneously;
    - Fixes modifiers becoming permanent after modified unit is transformed;
    - Fixes modifiers getting lost after modified unit is untransformed;
    - Fixes disappearing of modifiers applied on first attack of double attack;
    - Fixes attack source wards being reset incorrectly if its modifier also contains hp, regen or armor element;
    - Fixes attack class wards not being reset preventing it from being reapplied during battle;
    - Fixes incorrect handling of existing unit immunities;
    - Allows to apply modifiers even if there are no attack source wards among them;
    - Allows to target unit with a secondary attack even if there are no modifiers that can be applied;
    - Allows to heal and apply modifiers at once. Set `QTY_HEAL` > 0 in `GAttacks.dbf` and `HEAL` in `GDynUpgr.dbf`:
      - Allows to heal retreating allies by primary or secondary attack;
      - Allows to heal allies when battle ends, same as ordinary heal;
    - Allows to use revive as a secondary attack.
  </details>
- Fixes Scenario Editor bug with elves race as a caster in "Cast spell on location" event effect;
- <details>
    <summary>Fixes Scenario Editor to show spell cost information;</summary>

    Make sure the corresponding lines of `DLG_R_C_SPELL` in `ScenEdit.dlg` are changed as described below.

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
- Fixes inability to 'Defend' as second attack if no other action except 'Instant resolve' and 'Auto battle' is available (the bug generally avoided by toggling 'Auto battle');

### Scripting:
The toolset uses Lua for settings and advanced game mechanics. See [luaApi](luaApi.md) for detailed information.

### Tested on:
Disciples 2 Rise of the Elves v3.01 (Akella, Russobit, GOG).

### Installation:
- Rename original mss32.dll to mss**23**.dll;
- Copy mss32.dll from this repository to game folder;
- Copy [Scripts](Scripts) folder to the game folder.

### Deinstallation:
- Delete mss32.dll;
- Rename mss**23**.dll back to mss32.dll;
- Delete [Scripts](Scripts) folder from the game folder.

### Versions prior 0.4:
Installation and deinstallation process is the same, but with binkw32.dll.

### Building from sources:
Build Debug or Release Win32 target using Visual Studio solution located in mss32 folder. 

### License
[Detours](https://github.com/microsoft/Detours), [GSL](https://github.com/microsoft/GSL), [fmt](https://github.com/fmtlib/fmt) and [sol2](https://github.com/ThePhD/sol2) submodules as well as [![Lua](https://www.andreas-rozek.de/Lua/Lua-Logo_64x64.png)](http://www.lua.org/license.html) are using their own licenses.


This modification is not made or supported by Strategy First.<br />
Disciples 2 is a trademark of Strategy First Inc.

