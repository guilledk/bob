#include "bob.h"

int main (int argc, char** argv) {
	
	printf("Bob The Builder v%u\n", VERSION);
	config = fopen(".\\config.bf","r");
	if(config == NULL){
		printf("Can't open build configuration file!\n");
		bob_exit(1);
	}
	
	inc_paths = sl_new();
	lib_paths = sl_new();
	libs      = sl_new();
	
	//Parse config file
	char line[512];
	while(fgets(line, sizeof line, config) != NULL) {
		if(line[0] != '\n' && line[0] != '#') {
			//Strip \n
			line[strcspn(line, "\n")] = 0;
			char *sline = strtok(line,":");
			char *tmp = strtok(NULL,":");
			switch(hash(sline)) {
				
			case SRC_PATH : {
				src_path = (char*)malloc(strlen(tmp));
				strcpy(src_path,tmp);
				printf("Source path:        %s\n", src_path);
				break;
			}
			case MAIN_SRC : {
				main_src = (char*)malloc(strlen(tmp));
				strcpy(main_src,tmp);
				printf("Main source:        %s\n", main_src);
				break;
			}
			case INC_PATH : {
				sl_add(inc_paths,tmp);
				printf("Include path:       %s\n", tmp);
				break;
			}
			case EXE_PATH : {
				exe_path = (char*)malloc(strlen(tmp));
				strcpy(exe_path,tmp);
				printf("Executable path:    %s\n", exe_path);
				break;
			}
			case OBJ_PATH : {
				obj_path = (char*)malloc(strlen(tmp));
				strcpy(obj_path,tmp);
				printf("Object path:        %s\n", obj_path);
				break;
			}
			case LIB_PATH : {
				sl_add(lib_paths,tmp);
				printf("Library path:       %s\n", tmp);
				break;
			}
			case LIB      : {
				sl_add(libs,tmp);
				printf("Library:            %s\n", tmp);
				break;
			}
			default : {
				printf("Syntax error, hash: %u\n", hash(sline));
				break;
			}
				
			}
		}
	}
	
	if( main_src == NULL ||
	    src_path == NULL ||
		inc_paths->size == 0 ||
		exe_path == NULL ||
		obj_path == NULL) {

		printf("VARIABLE NOT SET IN BUILD FILE!.\n");
		bob_exit(1);
		
	}
	
	struct _stat buf;
	if(_stat(src_path,&buf) == 0) { //Exists
		if (buf.st_mode & _S_IFDIR) {
			printf("'%s' found.\n", src_path);
		} else {
			printf("'%s' is not a directory.\n", src_path);
			bob_exit(1);
		}
	} else {
		printf("'%s' not found.\n", src_path);
		bob_exit(1);
	}
	
	char *compile_comand = "cl /EHsc /MD /c ";
	
	bob_exit(0);
	
}

void bob_exit(int exitcode) {
	if(config)
		fclose(config);
	
	if(main_src)
		free(main_src);
	if(src_path)
		free(src_path);
	if(exe_path)
		free(exe_path);
	if(obj_path)
		free(obj_path);
	
	if(inc_paths)
		free(inc_paths);
	if(lib_paths)
		free(lib_paths);
	if(libs)
		free(libs);
	
	system("PAUSE");
	exit(exitcode);
}