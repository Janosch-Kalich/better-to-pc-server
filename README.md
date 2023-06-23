# BETTER TO PC SERVER
1. [Installation](#Installation)
2. [Usage](#Usage)
3. [API](#API)
4. [Libraries](#Libraries)
5. [Build](#5.Build)

## Installation
TODO: Link to installer

## Usage
Default settings:

|Settings|Value   |
|--------|--------|
|Host    |0.0.0.0 |
|Port    |3333    |
|Password|Password|

TODO: explain GUI

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
TODO: Add missing libs
- [WinApi](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)
- [Boost filesystem](https://www.boost.org/doc/libs/1_82_0/libs/filesystem/doc/index.htm)
- [Boost program-options](https://www.boost.org/doc/libs/1_58_0/doc/html/program_options.html)
- [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
- [Restinio](https://github.com/Stiffstream/restinio)
- [Traypp](https://github.com/Soundux/traypp)

## Build
In source directory:
TODO: more detailed command
``` powershell
cmake -G Ninja -S . -B build --DCMAKE_TOOLCHAIN_FILE="<PATH TO VCPKG TOOLCHAIN FILE>"
```

``` powershell
cmake --build build
```
