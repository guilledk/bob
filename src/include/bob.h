#ifndef _BOB_H_
#define _BOB_H_

//Arguments
#define DEBUG_ARGS false
#define ARG_CH    193429373 //change file, if not present "build.cf" will be used
#define ARG_C     5861493   //config file, if not present "config.b" will be used
#define ARG_F     5861496   //full rebuild ignore change file
#define ARG_D     5861494   //debug mode, if not present DM_NONE will be used

//Debug Modes
#define _DM_NONE    2090551285 //print no debug messages
#define _DM_MEDIUM  226336838  //print some debug messages
#define _DM_ALL     193486302  //print all debug messages - verbose!

//Subsystems
#define _SS_BOOT     59576652
#define _SS_CONSOLE  3879166584
#define _SS_WINDOWS  3703371152
#define _SS_NATIVE   3264802892
#define _SS_POSIX    231957816
#define _SS_EFI_APP  3658565612
#define _SS_EFI_BOOT 4189878919
#define _SS_EFI_ROM  1809421702
#define _SS_EFI_RUND 3660502087

//Build File keywords
#define VCVARSALL 1657761011 //Path to vcvarsall.bat file
#define SRC_PATH  1616936121 //Path to folder containing all the source files
#define MAIN_SRC  3339407313 //Path to source file containing main function
#define INC_PATH  4058331371 //Path to folder containing header files
#define EXE_PATH  1196332595 //Generate .exe to
#define OBJ_PATH  4027152780 //Path to generate .obj files
#define AD_PARAM  3460907162 //Additional command line parameters
#define SUB_SYS   3288834957 //Specifies the application subsystem
#define LIB_PATH  193462236  //Path to folder containing .lib files
#define LIB       863133480  //Names of .lib files to use when linking

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "list.h"

typedef enum {
	
	DM_ALL = 2,
	DM_MEDIUM = 1,
	DM_NONE = 0
	
} debugmode;

const unsigned short VERSION              = 3;
const char          *SOURCE_EXTENSIONS[3] = { ".c", ".cc", ".cpp" };
const char          *SEPARATOR            = "|";
//Message headers
const char          *INFO_H               = "[Info]: ";
const char          *DEBUG_H              = "[Debug]: ";
const char          *WARN_H               = "[Warning]: ";
const char          *ERROR_H              = "[Error]: ";


bool fullbuild  = false;
debugmode dmode = DM_MEDIUM;
char *ch_file   = "build.cf"; //bob build . change file
char *conf_fle  = "config.b";
char *ad_param  = NULL;
char *main_src  = NULL;
char *exe_path  = NULL;
char *obj_path  = NULL;
char *vcvarsall = NULL;
char *compile_command = NULL;
char *link_command = NULL;
char *subsys = NULL;
list_t *src_paths;
list_t *inc_paths;
list_t *lib_paths;
list_t *libs;
FILE *config;
list_t *sources;
list_t *hashes;
list_t *prev_sources;
list_t *prev_hashes;

void parse_args(int argc, char** argv);
void parse_conf(void);
bool missing_var(void);
void parse_chfile(void);
void create_chfile(void);
void get_src_diff(void);
void create_compile(void);
void create_link(void);

list_t* bob_sources(list_t *src_paths);
char* bob_strcat(const char *str1, const char *str2);
unsigned long bob_hashfile(const char *path);
void bob_exit(int exitcode);
const unsigned long hash(const char *str);

#endif