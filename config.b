#Executable architecture
TARGET_ARCH=x64

#Source Paths
SRC_PATH=src

#Entry point
MAIN_SRC=src\bob.c

#Include paths
INC_PATH=src\include\

#Compiled object files path
OBJ_PATH=bin\obj\

#Compiles executable path
EXE_PATH=bin\bob.exe

AD_PARAM=/Zi /MT /EHsc /Oy- /Ob0

#Subsystem
SUB_SYS=CONSOLE