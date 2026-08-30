# Enhanced Death Camera

SKSE plugin that improves death camera

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## User Requirements
* [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
	* Needed for SSE/AE
* [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
	* Needed for VR

## Building
```
git clone https://github.com/powerof3/EnhancedDeathCamera.git
cd EnhancedDeathCamera
git submodule update --init --recursive
```

### SSE
```
cmake --preset vs2022-windows-vcpkg-se
cmake --build --preset vs2022-windows-vcpkg-se
```
### AE
```
cmake --preset vs2022-windows-vcpkg-ae
cmake --build --preset vs2022-windows-vcpkg-ae
```

## License
[MIT](LICENSE)
