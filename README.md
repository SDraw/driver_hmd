OpenVR driver for turning desktop display into HMD.

# Installation
* Download latest release.
* Extract archive to `SteamVR/drivers` folder.
* Add line in section `steamvr` of `Steam/config/steamvr.vrsettings` file:
```JSON
"activateMultipleDrivers": true,
```

# Configuration
Edit `resources/settings.xml` for your display:
* **resolution:** full resolution of your monitor.
* **fps:** maximal frames per second of your monitor.

# Notes
If you have controllers `hmd_monitor` performs HMD position and rotation transformation based on input of left controller:
* Grip double-press: perform rotation changing.
  * Trigger hold: perform position changing.
  
# Building
* Clone repository with git
* Initialize submodules
```
git submodule update --init --depth=1
```
* Open `driver_hmd.sln` solution in Visual Studio 2013.
