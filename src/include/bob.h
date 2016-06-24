#ifndef _BOB_H_
#define _BOB_H_

#define SRC_PATH 1616936121
#define MAIN_SRC 3339407313
#define INC_PATH 4058331371
#define EXE_PATH 1196332595
#define OBJ_PATH 4027152780
#define LIB_PATH  193462236
#define LIB       863133480

#define MAX_ARG_SIZE 256

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "strlist.h"

const unsigned short VERSION = 1;
const char          *SOURCE_EXTENSIONS[3] = { ".c", ".cc", ".cpp" };

char *main_src = NULL;
char *src_path = NULL;
char *exe_path = NULL;
char *obj_path = NULL;
strlist *inc_paths;
strlist *lib_paths;
strlist *libs;
FILE *config;

void bob_exit(int exitcode);

#endif