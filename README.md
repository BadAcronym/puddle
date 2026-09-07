I recommend simply using the `.c/.h` files individually and compiling them alongside
your project.

The only binary is currently for testing, but static libraries may join this repo in the
future.

To build the testing binary from source, you'll need:
+ `gcc or clang`
+ `mold` on linux, although using any other linker is a one-line change
+ `bash` (`pwsh` on windows)

And simply run:
```
./run
```

Or, for example:
```
./run debug --compile-only
```

Alternatively, you can read the `./run` script to extract the compiler commands it would
emit and just use whatever compiler you want.

There is no support for the MSVC compiler or linker, as they are not very good.

`gcc` cannot interface with windows' address sanitizer, so:

For address sanitization on windows:

Make sure `clang-cl` and the MSVC address sanitizer are installed via the Microsoft
Visual Studio installer and the following is added to your path:
```
C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\lib\clang\22\lib\windows\
```
If the path does not exist, try looking under a different clang version.

NOT a specific MSVC version that looks like this:
```
C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.52.36520\bin\Hostx64\x64
```
