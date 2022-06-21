# 8080emulator
Emulator for 8080 microprocessor.

# Goal
The goal of this project is to demonstrate working knowledge of programming and the C language, through the creation of an 8080 emulator.

# How To Build
Makefiles are used to make building this project easy.

To build it, you can use:
```
make build
```
This will create the `build` directory, which will contain an `obj` folder and a `bin` folder.

The `obj` folder contains all the objects used to compile the current project, and the `bin` folder contains the final executable.


To clean up all the object files, and the executable, use:
```
make clean
```
This will clean up the `build` directory.

# Sources Used
http://www.emulator101.com/

https://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf
