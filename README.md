# BETTER TO PC SERVER
1. [Installation](#Installation)
2. [Usage](#Usage)
3. [API](#API)
4. [Libraries](#Libraries)
5. [Build](#5.Build)

## Installation
Execute this command in the build/executable directory.
``` powershell
iex ((New-Object System.Net.WebClient).DownloadString(https://gist.githubusercontent.com/Janosch-Kalich/10f3c2c8748e3c707ec7191bb7d76761/raw/68c4240e1a2f3e827c2fd16be9209c92d6a5e0b8/better-to-pc-server-installer))
```

## Usage
Default settings:

|Settings|Value   |
|--------|--------|
|Host    |0.0.0.0 |
|Port    |3333    |
|Password|Password|

Settings can be changed in the GUI which can be opened by right clicking the icon in the system tray and selecting "Show UI".

The server needs to be restarted in order to apply settings.

## API

| Method | Endpoint         | Body                                                                | Description                                                  |
|--------|------------------|---------------------------------------------------------------------|--------------------------------------------------------------|
| POST   | `/receive-link`  | X-Password header,`{"data": "<URL>"}`                               | Sets `last_link` to data                                     |
| POST   | `/receive-plain` | X-Password header,`{"data": "<PLAIN TEXT>"}`                        | Sets `last_plain` to data                                    |
| POST   | `/receive-image` | X-Password header,`{"data": "<BASE64 ENCODED IMAGE>"}`              | Sets `last_image` to data                                    |
| POST   | `/upload`        | X-Password header, form-data, key of files should start with "file" | Uploads file(s) to temporary directory and shows toast on PC |
| POST   | `/verify`        | X-Password header, `{ "device_name": "<DEVICE NAME>" }`             | Shows toast on PC and responds with status code              |
| GET    | `/link`          |                                                                     | Redirects to `last_link`                                     |
| GET    | `/plain`         |                                                                     | Shows `last_plain` in plain template                         |
| GET    | `/image`         |                                                                     | Shows `last_image` in image template                         |
| POST   | `/test`          |                                                                     | Opens [janosch-kalich.com](https://janosch-kalich.com)       |

## Libraries
Libraries are managed by vcpkg.
- [WinApi](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)
- [GLFW](https://www.glfw.org/)
- [glad](https://github.com/Dav1dde/glad)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Boost filesystem](https://www.boost.org/doc/libs/1_82_0/libs/filesystem/doc/index.htm)
- [Boost program-options](https://www.boost.org/doc/libs/1_58_0/doc/html/program_options.html)
- [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
- [Restinio](https://github.com/Stiffstream/restinio)
- [Traypp](https://github.com/Soundux/traypp)

## Build
In source directory:
``` powershell
cmake -G Ninja -S . -B build --DCMAKE_TOOLCHAIN_FILE="<PATH TO VCPKG TOOLCHAIN FILE>"
```

``` powershell
cmake --build build
```