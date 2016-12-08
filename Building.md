This page contains instructions for building the program from source.

# Prerequisites #

## Hardware ##

This program uses SSE2 instructions (through compiler intrinsics) to optimize critical parts of the computation. Therefore, the CPU must be Intel or AMD with SSE2 support. Both 32-bit and 64-bit are supported, but 64-bit offers superior runtime performance.

## Operating System ##

This program is a command line utility. It supports both Windows and Linux. The testing environments include:
  * Windows 7
  * Windows 8 Consumer Preview
  * Fedora 16
  * Ubuntu 11.10

## Compiler ##

The source code is written in C++ and uses a subset of C++0x features (such as lambda expression, `array` header, etc). Therefore the compiler (and the bundled STL) must implement the related parts of C++0x standard. The following compilers have been tested to work (optionally with some tweaks, see below):

  * Microsoft Visual C++ 2010 (including Express)
  * Microsoft Visual C++ 11 Beta
  * GNU C++ Compiler 4.5.3
  * Intel C++ Compiler 12.1.3 (with libstdc++ bundled with gcc 4.5.3)
  * clang compiler 3.1 (with libstdc++ bundled with gcc 4.5.3)

The performance of the executable generated by the various compilers ranks roughly in the order: clang > GCC > icc ~ MSVC. The performance difference between each pair of successive compilers is about 5%. In addition, 64-bit code can be faster than 32-bit code by as much as 40%, probably due to the availability of more registers.

### Compiling with MSVC ###

If you are building a 32-bit executable, a bug in the bundled `vector::resize()` implementation prevents the code from compiling and generates the following error:
```
error C2719: '_Val': formal parameter with __declspec(align('16')) won't be aligned
```
The workaround is to change the following line in `<vector>` from
```
void resize(size_type _Newsize, _Ty _Val)
```
to
```
void resize(size_type _Newsize, const _Ty &_Val)
```
and rebuild. If you are building a 64-bit executable, there is no issue.

### Compiling with Intel C++ Compiler ###

To use ICC 12.1.3 to build the code, the `libstdc++` implementation it uses must come from a version of GCC 4.5.x or below. This is because the STL implementation bundled with GCC 4.6 and above uses C++0x features not supported by ICC.

### Compiling with clang Compiler ###

For similar reasons as ICC, an appropriate `libstdc++` version should be selected with clang. In addition, the latest version of the compiler is required to support the used features in the STL implementation. The combination of clang 3.1svn + gcc 4.5.3 has been tested to work.

# Getting the Source #

The source code is hosted in an SVN repository on Google Code. You can check out the source code using the following command (or via a GUI such as TortoiseSVN for Windows):
```sh
svn checkout https://mastermind-strategy.googlecode.com/svn/trunk/ mastermind-strategy```
This includes all source code as well as documentation.

# Compiling and Linking #

## Under Windows ##

A VC++ 2010 solution file is included in the root directory of the source. Load this solution in VC2010 and build it. You can choose Debug or Release, and Win32 or x64. Note that only the executable will be built; for documentation, you need to find it online from the project's wiki pages.

## Under Linux ##

The preferred way to build the source is using `cmake`. If you have it installed, you can issue the following commands:
```sh

cd /path/to/source/ # root directory where you check out the source
mkdir build         # make a directory to place binary and intermediate files
cd build
cmake ..            # configure and generate Makefile
make                # compile and link the binary
bin/mmstrat -h      # test run the program
man man/mmstrat.1   # display manual page```

Alternatively, a Code::Blocks project file is provided in the root directory of the source code. However, no manual page will be generated if you build with Code::Blocks.

# Testing #

Building and running the tests is optional, but this can be helpful to verify that your build is good. If you have `perl` installed, you can run
```sh
make test```
if you are building from a Makefile. Alternatively, you can run
```sh
test-mmstrat.pl /path/to/mmstrat```
to manually run the tests. Some tests take longer to run than others, but all the tests should finish within 30 seconds and should all pass.