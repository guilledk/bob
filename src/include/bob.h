#ifndef _BOB_H_
#define _BOB_H_

//Arguments
#define ARG_CH    193429373 //change file, if not present "bbuild.cf" will be used
#define ARG_C     5861493   //config file, if not present "buildcfg.bf" will be used

//Build File keywords
#define VCVARSALL 1657761011 //Path to vcvarsall.bat file
#define SRC_PATH  1616936121 //Path to folder containing all the source files
#define MAIN_SRC  3339407313 //Path to source file containing main function
#define INC_PATH  4058331371 //Path to folder containing header files
#define EXE_PATH  1196332595 //Generate .exe to
#define OBJ_PATH  4027152780 //Path to generate .obj files
#define LIB_PATH  193462236  //Path to folder containing .lib files
#define LIB       863133480  //Names of .lib files to use when linking

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "list.h"

const unsigned short VERSION              = 1;
const char          *SOURCE_EXTENSIONS[3] = { ".c", ".cc", ".cpp" };
const char          *SEPARATOR            = "|";

char *ch_file   = "bbuild.cf"; //bob build . change file
char *conf_fle  = "buildcfg.bf";
char *main_src  = NULL;
char *src_path  = NULL;
char *exe_path  = NULL;
char *obj_path  = NULL;
char *vcvarsall = NULL;
list_t *inc_paths;
list_t *lib_paths;
list_t *libs;
FILE *config;

list_t* bob_sources(const char *path);
char* bob_strcat(const char *str1, const char *str2);
unsigned long bob_hashfile(const char *path);
void bob_exit(int exitcode);
const unsigned long hash(const char *str);

#endif