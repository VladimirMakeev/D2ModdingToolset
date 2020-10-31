# Modding toolset for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Fixes game crash in battles with summoners involved;
- Allows each race to hire up to 10 new tier-1 units in cities and capital.<br />
  New units are specified by adding a new columns SOLDIER\_N to Grace.dbf, N starting from 6.
- Allows to load scenarios with no magic (maximum spell level set to 0);
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
Rename original binkw32.dll to binkw**23**.dll, then copy binkw32.dll from this repository to game folder.

### Deinstallation:
Delete binkw32.dll, then rename binkw**23**.dll back to binkw32.dll.

### Building from sources:
Build Debug or Release Win32 target using Visual Studio solution located in mss32 folder. 

### License
[Detours](https://github.com/microsoft/Detours), [GSL](https://github.com/microsoft/GSL) and [fmt](https://github.com/fmtlib/fmt) submodules are using their own licenses.


This modification is not made or supported by Strategy First.<br />
Disciples 2 is a trademark of Strategy First Inc.

