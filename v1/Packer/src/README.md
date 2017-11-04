# How To

## Make

Using `windres` to compile `PackerStub.exe` resource:

`windres Resource.rc -O coff -o resource.res`

Using `g++` to compile `Packer.dll`:

`g++ -DUNICODE -shared -O -o Packer.dll main.cpp packer.cpp quicklz.cpp rc4.cpp resource.res -lshlwapi`

## Use

Once compiled, place into the same directory as the GUI. The `PackerStub.exe` is no longer required.