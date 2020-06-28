# C Memory Pool

## Build Instructions

In order to build the memory pool library, the following must be
installed:

+ CMake(>=2.8)
+ One of the following compilers:
  + GCC
  + MSVC

### Windows
```bat
mkdir build
cd build
cmake ..
msbuild ALL_BUILD.vcxproj
```

### Linux
```sh
mkdir build
cd build
cmake ..
make
```

### Building Documentation
If you wish to build the documentation, doxygen is required. The only
difference is the cmake command should be changed to the following:

```sh
cmake .. -DOCS=ON
```

And then you can proceed as normal. This has only been tested using
Linux.