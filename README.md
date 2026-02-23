# C++ Request Library
A lightweight HTTP request library for C++ built on top of libcurl.

* C++ Version: 20
* CMake Version: 3.28+
* Compiler: GCC 14+

## Dependencies
* [libcurl](https://curl.se/libcurl/)
* [vcpkg](https://github.com/microsoft/vcpkg) (package manager)

## Features
* HTTP Methods Available:
    * GET
    * POST
    * PUT
    * DELETE
* Method chaining API
* Custom request headers
* Request body support
* Configurable timeout
* Error message retrieval

## Build
### Prerequisites
* GCC 14+
* CMake 3.28+
* Ninja
* vcpkg with libcurl installed
```bash
vcpkg install curl
```

### Compile
* Set CMake Preset
```bash
cmake --preset default
```
* Build
```bash
cmake --build --preset default
```

## Usage
```cpp
#include "request.hpp"

Request req;
req.setUrl("https://api.example.com/data")
   .setMethod(HttpMethod::GET)
   .setHeader("Authorization", "Bearer token")
   .send();

std::cout << req.getResponseCode() << "\n";
std::cout << req.getResponseBody() << "\n";
```
