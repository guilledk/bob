# Bob
######A build tool for people that hate build tools.

Bob is a simple CLI tool for compiling and linking C/C++ applications, for now it supports Microsoft's C/C++ compiler but it was built with portability in mind.

###Features:
  - Only builds changed files
  - Can build projects with extra libraries
  - More to come!
  
It only requires a file with five variables to start building your project!
Example build config file:
```
#Comments start with '#'
#'|' is the separator
#Source directory path
SRC_PATH|src

#Source file containing main()
MAIN_SRC|src\bob.c

#Include path, more than one can be set
INC_PATH|src\include\

#Compiled object files output path
OBJ_PATH|bin\obj\

#Compiled executable output path
EXE_PATH|bin\bob.exe

#vcvarsall.bat file location, this is not required but its recommended
VCVARSALL|"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
```
You must have either Visual Studio or [Visual C++ Build Tools](https://blogs.msdn.microsoft.com/vcblog/2016/03/31/announcing-the-official-release-of-the-visual-c-build-tools-2015/)(Recommended).

For more info about config file syntax and command parameters that `bob` accepts, look in the wiki.

  
###Download

[Current Release](https://github.com/Darthkpo/bob/releases/latest)

###Building from source

Download `bob` and run:
```
bob
```
at the top of the repo.

###License

MIT

###Contributing

I will accept pull requests.
If anyone wants to fix memory leaks, go ahead.

###Why?

I'm one of those people that likes to do everything by hand, before bob I was using a batch file that contained the full compiler and linker command and simply built all the .c and .cc files. At first everything was fine until the command was 4000 characters long and it took 5 mins to build everything. So I started working with bob.
Honestly this is mostly for my self, but I figured that if anyone wants to contribute, why not? I'm also taking it as an opportunity to learn.
