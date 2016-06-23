#include "bob.h"

int main (int argc, char** argv) {
	
	printf("Bob The Builder v%u\n", VERSION);
	FILE *config = fopen(".\\config.bf","r");
	if(config == NULL){
		
		printf("Can't open build configuration file!\n");
		getchar();
		exit(1);
	}
	
	char *main_src;
	char *src_path;
	char *exe_path;
	char *obj_path;
	strlist *inc_paths = sl_new();
	strlist *lib_paths = sl_new();
	strlist *libs      = sl_new();
	
	//Parse config file
	char line[512];
	while(fgets(line, sizeof line, config) != NULL) {
		if(line[0] != '\n' && line[0] != '#') {
			char *sline = strtok(line,":");
			switch(hash(sline)) {
				
			case SRC_PATH : {
				src_path = strtok(NULL,":");
				printf("Source path:        %s", src_path);
				break;
			}
			case MAIN_SRC : {
				main_src = strtok(NULL,":");
				printf("Main source:        %s", main_src);
				break;
			}
			case INC_PATH : {
				char *newinc_path = strtok(NULL,":");
				sl_add(inc_paths,newinc_path);
				printf("Include path:       %s", newinc_path);
				break;
			}
			case EXE_PATH : {
				exe_path = strtok(NULL,":");
				printf("Executable path:    %s", exe_path);
				break;
			}
			case OBJ_PATH : {
				obj_path = strtok(NULL,":");
				printf("Object path:        %s", obj_path);
				break;
			}
			case LIB_PATH : {
				char *newlib_path = strtok(NULL,":");
				sl_add(lib_paths,newlib_path);
				printf("Library path:       %s", newlib_path);
				break;
			}
			case LIB      : {
				char *newlib = strtok(NULL,":");
				sl_add(libs,newlib);
				printf("Library:            %s", newlib);
				break;
			}
			default : {
				printf("Syntax error, hash: %u\n", hash(sline));
				break;
			}
				
			}
		}
	}
	printf("\n");
	free(inc_paths);
	fclose(config);
	system("PAUSE");
	exit(0);
	
}