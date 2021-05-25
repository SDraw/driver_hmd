# Driver HMD [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
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

## Controls
* **Left/right arrows**: Move localy left/right.
* **Up/down arrow:** Move localy forward/backward.
* **Num8/Num2:** Move localy up/down.
* **Num7/Num9:** Rotate localy up/down.
* **Num4/Num6:** Rotate localy left/right.
* **Num1/Num3:** Rotate localy anti-clockwise/clockwise.
* If you have VR controllers connected:
  * **Grip double-press and hold of left controller:** Start rotation changing. Release grip to stop.
    * **Trigger hold:** Start global position changing. Release trigger to stop.

Changes are saved on SteamVR shutdown and loaded on SteamVR startup.
  
## Building
* Clone repository with git
* Initialize submodules
```
git submodule update --init --depth=1
```
* Open `driver_hmd.sln` solution in Visual Studio 2013.
