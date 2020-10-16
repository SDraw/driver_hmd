# Driver HMD [![Build status](https://ci.appveyor.com/api/projects/status/lfnha0ipgxmuawb9?svg=true)](https://ci.appveyor.com/project/SDraw/driver-hmd) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
SteamVR driver for turning desktop display into HMD.

## Installation
* Download latest release.
* Extract archive to `SteamVR/drivers` folder.
* Add line in section `steamvr` of `Steam/config/steamvr.vrsettings` file:
```JSON
"activateMultipleDrivers": true,
```

## Configuration
Edit `resources/settings.xml` for your display:
* **resolution:** full resolution of your monitor.
* **fps:** maximal frames per second of your monitor.

## Notes
If you have controllers `hmd_monitor` performs HMD position and rotation transformation based on input of left controller:
* Grip double-press: perform rotation changing.
  * Trigger hold: perform position changing.

Changes are saved on SteamVR shutdown and loaded on SteamVR startup.
  
## Building
* Clone repository with git
* Initialize submodules
```
git submodule update --init --depth=1
```
* Open `driver_hmd.sln` solution in Visual Studio 2013.
