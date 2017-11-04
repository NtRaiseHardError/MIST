# How To

## Make

Using g++:

`g++ -mwindows -static-libgcc -static-libstdc++ -DWIN32_LEAN_AND_MEAN -DUNICODE -fno-rtti -Os -s -o PackerStub.exe quicklz.cpp rc4.cpp main.cpp packerstub.cpp stringxor.cpp`


**NOTE: Compiling with MSVC++ creates runtime errors.**

## Use

The purpose of `PackerStub.exe` is to provide `Packer.dll` the base unpacking stub so that it can pack a target file. Once `Packer.dll` has been compiled, this file is no longer required to exist.