I recommend simply using the `.c/.h` files individually and compiling them alongside
your project.

The only binary is currently for testing, but static libraries may join this repo in the
future.

To build the binary from source, you'll need:
+ `premake5`
+ `clang` (`mingw` on windows)
+ `bash` (`pwsh` on windows)

Note that `gcc` and `mingw-gcc` will work, but are not actively tested by me. I do know
that `mingw-gcc` cannot interface with windows' address sanitizer, so only
`release/debug` builds there.

There is no support for the MSVC compiler or linker, as they are not very good.

And simply run:
```
./run
```

Or, for example:
```
./run debug --compile-only
```

For address sanitization on windows:

Make sure the MSVC address sanitizer is installed via the Microsoft Visual Studio
installer and is added to your path (the versions vary, you'll have to change them
to fit yours):
```
C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.52.36520\bin\Hostx86\x64
```
