# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Adds new custom building category for unit hire.
  Buildings should be placed on the 'Other buildings' tab.
  Custom category is created by adding 'L\_CUSTOM' record to LBuild.dbf;
- Allows each race to hire up to 10 new tier-1 units in cities and capital.<br />
  New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6;
- Allows alchemists to give additional attacks to retreating allies;
- Allows to load and create scenarios with no magic (maximum spell level set to 0);
- Allows Scenario Editor to place merchants, mages, trainers and mercenaries on water tiles;
- Buildings up to tier 10 are supported in editor and game;
- Maximum game turn increased to 9999;
- Fixes game crash in battles with summoners involved;
- Fixes AI unit placement logic for melee units with vampiric attacks;
- Fixes Scenario Editor bug with elves race as a caster in "Cast spell on location" event effect;
- Game rules and settings can be changed in Disciple.ini, section \[Disciple\];
  - <details>
    <summary>Settings:</summary>
  
      - "UnitMaxDamage=\[300 : (2^31 - 1)\]" changes maximum unit damage per attack;
      - "UnitMaxArmor=\[70: 100\]" changes maximum combined unit armor;
      - "StackMaxScoutRange=\[7 : 100\]" changes maximum allowed scout range for troops; 
      - "ShatteredArmorMax=\[0 : 100\]" changes total armor shatter damage;
      - "ShatterDamageMax=\[0 : 100\]" changes maximum armor shatter damage per attack;
      - "CriticalHitDamage=\[0 : 255\]" changes the percentage damage of critical hit;
      - "ShowBanners=(0/1)" toggle show banners by default;
      - "ShowResources=(0/1)" toggle show resources panel by default;
      - "ShowLandConverted=(0/1)" toggle show percentage of land converted instead of minimap;
  </details>
- Buttons for bulk item transfer: transfer all items, potions, scrolls/wands or valuables between inventories with single click.
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
- Can be used on vanilla version or with other mods installed;

### Tested on:
- Disciples 2 Rise of the Elves v3.01 (Akella, Russobit, GOG)

### Installation:
Rename original mss32.dll to mss**23**.dll, then copy mss32.dll from this repository to game folder.

### Deinstallation:
Delete mss32.dll, then rename mss**23**.dll back to mss32.dll.

### Versions prior 0.4:
Installation and deinstallation process is the same, but with binkw32.dll.

### Building from sources:
Build Debug or Release Win32 target using Visual Studio solution located in mss32 folder. 

### License
[Detours](https://github.com/microsoft/Detours), [GSL](https://github.com/microsoft/GSL) and [fmt](https://github.com/fmtlib/fmt) submodules are using their own licenses.


This modification is not made or supported by Strategy First.<br />
Disciples 2 is a trademark of Strategy First Inc.

