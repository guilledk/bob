#include "bob.h"

//bob.exe exitcodes
//0 - success
//1 - file error
//2 - argument error

int main (int argc, char** argv) {
	
	printf("bob v%u\n", VERSION);
	
	for (int i = 1; i< argc; i++) {
		unsigned long arghash = hash(*(argv+i));
		switch(arghash) {
		
		case ARG_CH : {
		
			i++;
			ch_file = *(argv+i);
			if(ch_file == NULL || ch_file[0] == '\0') {
				printf("Wrong argument to option '-ch'\n");
				bob_exit(2);
			}
				
			printf("Using change file: '%s'\n", ch_file);
			break;
		
		}
		case ARG_C  : {
		
			i++;
			conf_fle = *(argv+i);
			if(conf_fle == NULL || conf_fle[0] == '\0') {
				printf("Wrong argument to option '-c'\n");
				bob_exit(2);
			}
			printf("Using build file: '%s'\n", conf_fle);
			break;
		
		}
		case ARG_D  : {
			
			i++;
			char *debug_mode_str = *(argv+i);
			if(debug_mode_str == NULL || debug_mode_str[0] == '\0') {
				printf("Wrong argument to option '-d'\n");
				bob_exit(2);
			}
			switch(hash(debug_mode_str)) {
			
			case _DM_ALL    : {
				dmode = DM_ALL;
				printf("Debug mode: ALL\n");
				break;
			}
			case _DM_MEDIUM : {
				dmode = DM_MEDIUM;
				printf("Debug mode: MEDIUM\n");
				break;
			}
			case _DM_NONE   : {
				dmode = DM_NONE;
				printf("Debug mode: NONE\n");
				break;
			}
			default         : {
				printf("Debug mode: '%s' not recognized!\n", debug_mode_str);
				bob_exit(2);
				break;
			}
			
			}
			break;
			
		}
		default     : {
			printf("Can't parse argument: %s\nhash: %lu\n", *(argv+i), arghash);
			bob_exit(2);
			break;
		}
			
		}
	}
	
	if(conf_fle == NULL){
		printf("Null configuration file!\n");
		bob_exit(1);
	}
	config = fopen(conf_fle,"r");
	if(config == NULL){
		printf("Can't open build configuration file!\n");
		bob_exit(1);
	}
	
	inc_paths = lnew();
	lib_paths = lnew();
	libs      = lnew();
	
	//Parse config file
	printf("-CONFIG FILE BEGIN-\n");
	unsigned short comments = 0;
	unsigned short blanks   = 0;
	char line[1024];
	while(fgets(line, sizeof line, config) != NULL) {
		if(line[0] == '\n') {
			blanks++;
			continue;
		}
		if(line[0] == '#') {
			comments++;
			continue;
		}
		//Strip \n
		line[strcspn(line, "\n")] = 0;
		char *sline = strtok(line,SEPARATOR);
		char *tmp = strtok(NULL,SEPARATOR);
		switch(hash(sline)) {
		
		case AD_PARAM  : {
			ad_param = (char*)malloc(strlen(tmp));
			strcpy(ad_param,tmp);
			printf("Additional parameters: %s\n", ad_param);
			break;
		}
		
		case SRC_PATH  : {
			src_path = (char*)malloc(strlen(tmp));
			strcpy(src_path,tmp);
			printf("Source path:           %s\n", src_path);
			break;
		}
		case MAIN_SRC  : {
			main_src = (char*)malloc(strlen(tmp));
			strcpy(main_src,tmp);
			printf("Main source:           %s\n", main_src);
			break;
		}
		case INC_PATH  : {
			
			gvalue inc_path;
			inc_path._str = (char*)malloc(strlen(tmp));
			strcpy(inc_path._str,tmp);
			
			ladd(inc_paths,gvtnew(inc_path,T_STR));
			printf("Include path:          %s\n", tmp);
			break;
		}
		case EXE_PATH  : {
			exe_path = (char*)malloc(strlen(tmp));
			strcpy(exe_path,tmp);
			printf("Executable path:       %s\n", exe_path);
			break;
		}
		case OBJ_PATH  : {
			obj_path = (char*)malloc(strlen(tmp));
			strcpy(obj_path,tmp);
			printf("Object path:           %s\n", obj_path);
			break;
		}
		case LIB_PATH  : {
			
			gvalue lib_path;
			lib_path._str = (char*)malloc(strlen(tmp));
			strcpy(lib_path._str,tmp);
			
			ladd(lib_paths,gvtnew(lib_path,T_STR));
			printf("Library path:          %s\n", tmp);
			break;
		}
		case LIB       : {
			
			gvalue lib;
			lib._str = (char*)malloc(strlen(tmp));
			strcpy(lib._str,tmp);
			
			ladd(libs,gvtnew(lib,T_STR));
			printf("Library:               %s\n", tmp);
			break;
		}
		case VCVARSALL : {
			vcvarsall = (char*)malloc(strlen(tmp));
			strcpy(vcvarsall,tmp);
			printf("vcvarsall:             %s\n", tmp);
			break;
		}
		default : {
			printf("Syntax error, hash:    %u\n", hash(sline));
			break;
		}
			
		}
	}
	printf("--CONFIG FILE END--\nComments:    %u\nBlank lines: %u\n", comments, blanks);
	
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
	
	prev_sources = lnew();
	prev_hashes = lnew();
	
	//READ CHANGE FILE IF IT EXISTS
	if(_stat(ch_file,&buf) == 0) {
		printf("Change file exists!\n");
		if(ch_file == NULL){
			printf("Null change file!\n");
			bob_exit(1);
		}
		FILE *chfile = fopen(ch_file,"r");
		if(chfile == NULL){
			printf("Can't open change file!\n");
			bob_exit(1);
		}
		while(fgets(line, sizeof line, chfile) != NULL) {
			char *filename = strtok(line,SEPARATOR);
			char *unconvhash = strtok(NULL,SEPARATOR);
			unsigned long filehash = strtoul(unconvhash, (char**)NULL, 10);
			
			gvalue filename_gv;
			filename_gv._str = (char*)malloc(strlen(filename));
			strcpy(filename_gv._str,filename);
			ladd(prev_sources,gvtnew(filename_gv,T_STR));
			
			gvalue filehash_gv;
			filehash_gv._ulong = filehash;
			ladd(prev_hashes,gvtnew(filehash_gv,T_ULONG));
			
		}
		fclose(chfile);
		printf("Change file contents:\n");
		printf("Prev files:  ");
		lprint(prev_sources);
		printf("Prev hashes: ");
		lprint(prev_hashes);
	}
	
	sources = bob_sources(src_path);
	hashes = lnew();
	
	//Creating this builds change file.
	node_t *cur = sources->head;
	for(int i = 0; i < sources->size; i++){
		gvalue src_hash;
		char *fullpath = bob_strcat(src_path,"\\");
		src_hash._ulong = bob_hashfile(bob_strcat(fullpath,cur->value->value._str));
		ladd(hashes,gvtnew(src_hash,T_ULONG));
		cur = cur->next;
		free(fullpath);
	}
	//TODO print this only if debug on
	printf("Files:  ");
	lprint(sources);
	printf("Hashes: ");
	lprint(hashes);
	
	{ //Write new change file
		
		FILE *chfile = fopen(ch_file,"w");
		if(chfile == NULL){
			printf("Can't open change file!\n");
			bob_exit(1);
		}
		node_t *curs = sources->head;
		node_t *curh = hashes->head;
		printf("-NEW CHANGE FILE BEGIN-\n");
		for(int i = 0; i < sources->size; i++){
			printf("%s%s%lu\n", curs->value->value._str, SEPARATOR, curh->value->value._ulong);
			fprintf(chfile, "%s%s%lu\n", curs->value->value._str, SEPARATOR, curh->value->value._ulong);
			curs = curs->next;
			curh = curh->next;
		}
		fclose(chfile);
		printf("--NEW CHANGE FILE END--\n");
		
	}
	
	{ //Compare previous source hashes and current hashes
		node_t *cur = sources->head;
		for(int i = 0; i < sources->size; i++){
			int index = lhas(prev_sources,cur->value);
			if(index != -1){
				node_t *prevh = lgetat(prev_hashes,index);
				node_t *curh = lgetat(hashes,i);
				if(ncmp(prevh,curh) && strcmp(bob_strcat(src_path,bob_strcat("\\",cur->value->value._str)),main_src) != 0){
					cur->value->value._str = NULL;
				}
			}
			cur = cur->next;
		}
	}
	
	char *compile_command = "cl /c ";
	//Concat additional parameters
	if(ad_param) {
		compile_command = bob_strcat(compile_command,bob_strcat(ad_param," "));
	}
	
	{ //Concatenate all the sources to the compile command
		
		node_t *cur = sources->head;
		for(int i = 0; i < sources->size; i++){
			printf("%s\n", cur->value->value._str);
			if(!cur->value->value._str)
				continue;
			
			compile_command = bob_strcat(compile_command, bob_strcat(src_path,bob_strcat("\\",bob_strcat(cur->value->value._str," "))));
			cur = cur->next;
		}
		
	}

	//Concat obj path
	compile_command = bob_strcat(compile_command,bob_strcat("/Fo.\\", obj_path));
	
	{ //Concatenate all the include directorys to the compile command
		
		node_t *cur = inc_paths->head;
		for(int i = 0; i < inc_paths->size; i++){
			compile_command = bob_strcat(compile_command, bob_strcat(" /I.\\", cur->value->value._str));
			cur = cur->next;
		}
		
	}
	
	char *link_command = bob_strcat("link ",bob_strcat(obj_path,"*.obj /ENTRY:mainCRTStartup"));
	
	{ //Concatenate all libpaths
		
		node_t *cur = lib_paths->head;
		for(int i = 0; i < lib_paths->size; i++){
			link_command = bob_strcat(link_command, bob_strcat(" /LIBPATH:.\\", cur->value->value._str));
			cur = cur->next;
		}
	}
	
	{ //Concatenate all libs
		
		node_t *cur = libs->head;
		for(int i = 0; i < libs->size; i++){
			link_command = bob_strcat(link_command, bob_strcat(" ", cur->value->value._str));
			cur = cur->next;
		}
	}
	
	//Concat exe path
	link_command = bob_strcat(link_command,bob_strcat(" /OUT:", exe_path));
	
	printf("\nCompiler command:\n\t%s\n\n", compile_command);
	printf("Linker command:\n\t%s\n\n", link_command);
	
	system(vcvarsall);
	system(compile_command);
	system(link_command);
	
	free(compile_command);
	
	bob_exit(0);
	
}

list_t* bob_sources(const char *path) {
	
	DIR *dp;
	struct dirent *ep;
	list_t *src_list = lnew();
	
	dp = opendir(path);
	if (dp == NULL)
		return NULL;
	
	while (ep = readdir(dp)) {
		bool is_src = false;
		for(int i = 0; i < 3; i++){
			char *ext = strrchr(ep->d_name,'.');
			if(ext && strcmp(ext,SOURCE_EXTENSIONS[i]) == 0){
				is_src = true;
				break;
			}
		}
		if(is_src) {
			gvalue filename;
			filename._str = (char*)malloc(strlen(ep->d_name));
			strcpy(filename._str,ep->d_name);
			ladd(src_list,gvtnew(filename,T_STR));
		}
	}
		
	closedir(dp);
	
	return src_list;
	
}

char* bob_strcat(const char *str1, const char *str2) {
	
	unsigned short len1 = strlen(str1);
	unsigned short len2 = strlen(str2);
	
	char *newstr = (char*)malloc(len1 + len2 + 1);
	strcpy(newstr, str1);
	strcat(newstr, str2);
	
	return newstr;
	
}

unsigned long bob_hashfile(const char *path) {
	
	FILE *fileptr = fopen(path,"r");
	if(!fileptr){
		printf("Error getting file hash! %s\n", path);
		return 0;
	}
	if(fseek(fileptr,0,SEEK_END)) {
		printf("fseek Error! %s\n", path);
		return 0;
	}
	long fsize = ftell(fileptr);
	printf("Hashing file of size: %ld\n", fsize);
	if(fseek(fileptr,0,SEEK_SET)) {
		printf("fseek Error! %s\n", path);
		return 0;
	}
	
	char *filecontent = (char*)malloc(fsize + 1);
	fread(filecontent, fsize, 1, fileptr);
	fclose(fileptr);
	filecontent[fsize] = 0;
	unsigned long fhash = hash(filecontent);
	free(filecontent);
	return fhash;
	
}

void bob_exit(int exitcode) {
	if(config)
		fclose(config);
	
	if(ad_param)
		free(ad_param);
	if(main_src)
		free(main_src);
	if(src_path)
		free(src_path);
	if(exe_path)
		free(exe_path);
	if(obj_path)
		free(obj_path);
	
	if(inc_paths)
		lfree(inc_paths);
	if(lib_paths)
		lfree(lib_paths);
	if(libs)
		lfree(libs);
	if(sources)
		lfree(sources);
	if(hashes)
		lfree(hashes);
	if(prev_sources)
		lfree(prev_sources);
	if(prev_hashes)
		lfree(prev_hashes);
	
	system("PAUSE");
	exit(exitcode);
}

const unsigned long hash(const char *str){
	
	unsigned long hash = 5381;  
    int c;
 
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
	
}