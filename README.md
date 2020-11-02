# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Adds new custom building category for unit hire.
  Buildings should be placed on the 'Other buildings' tab.
  Custom category is created by adding 'L\_CUSTOM' record to LBuild.dbf;
- Allows each race to hire up to 10 new tier-1 units in cities and capital.<br />
  New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6;
- Allows to load and create scenarios with no magic (maximum spell level set to 0);
- Buildings up to tier 10 are supported in editor and game;
- Fixes game crash in battles with summoners involved;
- Fixes AI unit placement logic for melee units with vampiric attacks;
- Shows banners by default.<br />
  Behaviour can be changed from disciple.ini file using "ShowBanners=(0/1)" setting in "Disciple" section;
- Shows resources panel by default.<br />
  Behaviour can be changed from disciple.ini file using "ShowResources=(0/1)" setting in "Disciple" section;
- Percentage of land converted can be shown by default instead of minimap.<br />
  Use "ShowLandConverted=(0/1)" setting in "Disciple" section;
- Can be used on vanilla version or with other mods installed.

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

