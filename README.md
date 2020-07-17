# Binkw32 proxy dll for Disciples 2 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Features:
- Shows banners by default.
  Behaviour can be changed from disciple.ini file using "ShowBanners=(0/1)" setting in "Disciple" section;
- Shows resources panel by default.
  Behaviour can be changed from disciple.ini file using "ShowResources=(0/1)" setting in "Disciple" section;
- Can be used on vanilla version or with other mods installed.

### Tested on:
- [x] Disciples 2 Rise of the Elves v3.01 (Akella)
- [x] Disciples 2 Rise of the Elves v3.01 (Russobit)
- [x] Disciples 2 Rise of the Elves v3.01 (GOG)

### Installation:
Rename original binkw32.dll to binkw**23**.dll, then copy binkw32.dll from this repository to game folder.

### Deinstallation:
Delete binkw32.dll, then rename binkw**23**.dll back to binkw32.dll.

### Building from sources:
Build Debug or Release Win32 target using Visual Studio solution located in binkw32 folder. 

### License
[Detours](https://github.com/microsoft/Detours) and [fmt](https://github.com/fmtlib/fmt) submodules are using their own licenses.


This modification is not made or supported by Strategy First.
Disciples 2 is a trademark of Strategy First Inc.

