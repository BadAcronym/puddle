I recommend simply using the `.c/.h` files individually and compiling them alongside
your project.

The only binary is currently for testing, but static libraries may join this repo in the
future.

To build the binary from source, you'll need:
+ `premake5`
+ `mold` (`mingw`'s `lld` on windows)
+ `clang` (`mingw`'s `clang` on windows)
+ `bash` (`pwsh` on windows)

And simply run:
```
./run
```

Or, for example:
```
./run debug --compile-only
```

There is no support for the MSVC compiler or linker, as they are not very good.

Note that `gcc` and `mingw-gcc` will work, but are not actively tested by me. I do know
that `mingw` cannot interface with windows' address sanitizer, so:

For address sanitization on windows:

Make sure `clang-cl` and the MSVC address sanitizer are installed via the Microsoft
Visual Studio installer and the following lines are added to your path (the versions
vary, you'll have to change them to fit yours):

```
C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.52.36520\bin\Hostx64\x64
C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.52.36520\lib\x64
```
