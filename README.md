# hypertext
**hypertext** is a C-written library that shall make your life a little less stressful when dealing with [RFC 2616](https://tools.ietf.org/html/rfc2616) (`shall` defined as per [RFC 2119](https://tools.ietf.org/html/rfc2119)).  
hypertext only requires standard C; it doesn't support HTTP/2, HTTP/3, WebSockets or other web technologies (yet).

## Issues
- The doxygen-based documentation is quite broken; doxygen skips values and whatnot.

## Usage
Documentation can be generated via doxygen.  
Apart from that, the `Tests` directory contains a few tests, which might be a useful introduction into hypertext.

## Build
Using CMake:
```sh 
# Generate a build directory:
cmake -B build -DBUILD_SHARED=ON -DBUILD_TESTS=ON # Use these flags to turn on building hypertext as a shared library and/or tests.

# Build within the generated build directory:
cmake --build build --config Debug # Use the config switch to set the configuration for generators allowing multiple configurations, like Visual Studio.
```

## Install
In case you want to install hypertext to a special fancy directory, you can use CMake again:
```sh
# This requires a build directory containing a build:
cmake --install build --prefix Install --config Release # Use the prefix switch to set a directory to install the contents within, and the config switch as shown above.
```

## Test
CTest is used to test hypertext.  
Once `BUILD_TESTS` is turned on, four tests will be built.

| Name | Description
|---|---|
| `hypertext_test_request_creation` | Tests the creation of a request. |
| `hypertext_test_response_creation` | Tests the creation of a response. | 
| `hypertext_test_request_parsing` | Tests the parsing of a request. |
| `hypertext_test_response_parsing` | Test the parsing of a response. |

# Documentation
doxygen can be used to generate the documentation.
```sh
doxygen Doxyfile
```
