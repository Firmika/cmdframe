# cmdframe
A lightweight command line menu library.

## Repositoriy structure
+ `cmdframe`: source code of `CmdFrame` library.
+ `demo`: a small demo of `CmdFrame` library.

## How to import this library
### Method 1: pack as library
1. Use `make` to generate a library `libcmdframe.a`
2. copy this static library along with header files in `cmdframe/include` to your project
3. Remember to add library options `-lncurses -lmenu` to your project when compiling.

### Method 2: copy all source code
1. Copy all source files in directory `cmdframe` to your project
2. Remember to add library options `-lncurses -lmenu` to your project when compiling

## Todo:
Update documentation for this library.