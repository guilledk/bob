#include "bob.h"

//bob.exe exitcodes
//0 - success
//1 - file error
//2 - argument error
//3 - compiler error
//4 - linker error

void parse_args(int argc, char** argv) {
	
	for (int i = 1; i< argc; i++) {
		unsigned long arghash = hash(*(argv+i));
		switch(arghash) {
		
		case ARG_CH : {
		
			i++;
			ch_file = *(argv+i);
			if(ch_file == NULL || ch_file[0] == '\0') {
				printf("%sWrong argument to option '-ch'\n", ERROR_H);
				bob_exit(2);
			}
			
			if(DEBUG_ARGS)
				printf("%sUsing change file: '%s'\n", DEBUG_H, ch_file);
			break;
		
		}
		case ARG_C  : {
		
			i++;
			conf_fle = *(argv+i);
			if(conf_fle == NULL || conf_fle[0] == '\0') {
				printf("%sWrong argument to option '-c'\n", ERROR_H);
				bob_exit(2);
			}
			if(DEBUG_ARGS)
				printf("%sUsing build file: '%s'\n", DEBUG_H, conf_fle);
			break;
		
		}
		case ARG_D  : {
			
			i++;
			char *debug_mode_str = *(argv+i);
			if(debug_mode_str == NULL || debug_mode_str[0] == '\0') {
				printf("%sWrong argument to option '-d'\n", ERROR_H);
				bob_exit(2);
			}
			switch(hash(debug_mode_str)) {
			
			case _DM_ALL    : {
				dmode = DM_ALL;
				if(DEBUG_ARGS)
					printf("%sDebug mode: ALL\n", DEBUG_H);
				break;
			}
			case _DM_MEDIUM : {
				dmode = DM_MEDIUM;
				if(DEBUG_ARGS)
					printf("%sDebug mode: MEDIUM\n", DEBUG_H);
				break;
			}
			case _DM_NONE   : {
				dmode = DM_NONE;
				if(DEBUG_ARGS)
					printf("%sDebug mode: NONE\n", DEBUG_H);
				break;
			}
			default         : {
				printf("%sDebug mode: '%s' not recognized!\n", ERROR_H, debug_mode_str);
				bob_exit(2);
				break;
			}
			
			}
			break;
			
		}
		case ARG_F  : {
			
			fullbuild = true;
			if(DEBUG_ARGS)
				printf("%sFull build, ignoring change file!\n", DEBUG_H);
			break;
			
		}
		default     : {
			printf("%sCan't parse argument: %s\nhash: %lu\n", ERROR_H, *(argv+i), arghash);
			bob_exit(2);
			break;
		}
			
		}
	}
	
}

void parse_conf(void) {
	
	if(conf_fle == NULL){
		printf("%sNull configuration file!\n", ERROR_H);
		bob_exit(1);
	}
	config = fopen(conf_fle,"r");
	if(config == NULL){
		printf("%sCan't open build configuration file!\n", ERROR_H);
		bob_exit(1);
	}
	
	if(dmode == 2)
		printf("-[CONFIG FILE BEGIN]-\n");
	unsigned short comments = 0;
	unsigned short blanks   = 0;
	char line[1024];
	unsigned short lines = 0;
	while(fgets(line, sizeof line, config) != NULL) {
		lines++;
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
			ad_param = (char*)malloc(strlen(tmp) + 1);
			strcpy(ad_param,tmp);
			if(dmode == 2)
				printf("Additional parameters:   %s\n", ad_param);
			break;
		}
		
		case TARGET_ARCH : {
			
			unsigned long h_tmp = hash(tmp);
			if( h_tmp == _ARCH_X86 ||
			    h_tmp == _ARCH_X64 ||
				h_tmp == _ARCH_ARM ||
				h_tmp == _ARCH_X86_ARM ||
				h_tmp == _ARCH_X64_ARM ||
				h_tmp == _ARCH_X86_AMD64 ||
				h_tmp == _ARCH_AMD64_X86 ) {
					
				trgt_arch = (char*)malloc(strlen(tmp) + 1);
				strcpy(trgt_arch,tmp);
				if(dmode == 2)
					printf("Target architecture: %s\n", trgt_arch);
					
			} else {
				printf("%sUnrecognized target architecture on line %u, hash: '%lu'", ERROR_H, lines, h_tmp);
				bob_exit(1);
			}
			break;
			
		}
		
		case SUB_SYS  : {
			
			unsigned long h_tmp = hash(tmp);
			if( h_tmp == _SS_BOOT     ||
			    h_tmp == _SS_CONSOLE  ||
				h_tmp == _SS_WINDOWS  ||
				h_tmp == _SS_NATIVE   ||
				h_tmp == _SS_POSIX    ||
				h_tmp == _SS_EFI_APP  ||
				h_tmp == _SS_EFI_BOOT ||
				h_tmp == _SS_EFI_ROM  ||
				h_tmp == _SS_EFI_RUND ) {
				
				subsys = (char*)malloc(strlen(tmp) + 1);
				strcpy(subsys,tmp);
				if(dmode == 2)
					printf("Subsystem:           %s\n", subsys);
				
			} else {
				printf("%sUnrecognized subsystem on line %u, hash: '%lu'\n", ERROR_H, lines, h_tmp);
				bob_exit(1);
			}
			break;
		}
		
		case SRC_PATH  : {
			gvalue src_path;
			src_path._str = (char*)malloc(strlen(tmp) + 1);
			strcpy(src_path._str,tmp);
			
			ladd(src_paths,gvtnew(src_path,T_STR));
			if(dmode == 2)
				printf("Source path:             %s\n", tmp);
			break;
		}
		case MAIN_SRC  : {
			main_src = (char*)malloc(strlen(tmp) + 1);
			strcpy(main_src,tmp);
			if(dmode == 2)
				printf("Main source:             %s\n", main_src);
			break;
		}
		case INC_PATH  : {
			
			gvalue inc_path;
			inc_path._str = (char*)malloc(strlen(tmp) + 1);
			strcpy(inc_path._str,tmp);
			
			ladd(inc_paths,gvtnew(inc_path,T_STR));
			if(dmode == 2)
				printf("Include path:            %s\n", tmp);
			break;
		}
		case EXE_PATH  : {
			exe_path = (char*)malloc(strlen(tmp) + 1);
			strcpy(exe_path,tmp);
			if(dmode == 2)
				printf("Executable path:         %s\n", exe_path);
			break;
		}
		case OBJ_PATH  : {
			obj_path = (char*)malloc(strlen(tmp) + 1);
			strcpy(obj_path,tmp);
			if(dmode == 2)
				printf("Object path:             %s\n", obj_path);
			break;
		}
		case LIB_PATH  : {
			
			gvalue lib_path;
			lib_path._str = (char*)malloc(strlen(tmp) + 1);
			strcpy(lib_path._str,tmp);
			
			ladd(lib_paths,gvtnew(lib_path,T_STR));
			if(dmode == 2)
				printf("Library path:            %s\n", tmp);
			break;
		}
		case LIB       : {
			
			gvalue lib;
			lib._str = (char*)malloc(strlen(tmp) + 1);
			strcpy(lib._str,tmp);
			
			ladd(libs,gvtnew(lib,T_STR));
			if(dmode == 2)
				printf("Library:                 %s\n", tmp);
			break;
		}
		case VCVARSALL : {
			vcvarsall = (char*)malloc(strlen(tmp) + 1);
			strcpy(vcvarsall,tmp);
			if(dmode == 2)
				printf("vcvarsall:               %s\n", tmp);
			break;
		}
		default : {
			printf("%sSyntax error on line %u, hash: '%u'\n", ERROR_H, lines, hash(sline));
			bob_exit(1);
			break;
		}
			
		}
	}
	if(dmode == 2)
		printf("Comments:    %u\nBlank lines: %u\n--[CONFIG FILE END]--\n", comments, blanks);
	
}

void parse_conf2(void) {

	if(conf_fle == NULL){
		printf("%sNull configuration file!\n", ERROR_H);
		bob_exit(1);
	}
	config = fopen(conf_fle,"r");
	if(config == NULL){
		printf("%sCan't open build configuration file!\n", ERROR_H);
		bob_exit(1);
	}
	
	if(dmode == 2)
		printf("-[CONFIG FILE BEGIN]-\n");
	
	int cur_fchar;
	
	bool prev_fwd_slash = false;
	
	while((cur_fchar = fgetc(config)) != EOF) {
		
		switch((char)cur_fchar){
		
		case '*' : {
			
			if(prev_fwd_slash)
				while((char)(cur_fchar = fgetc(config)) != '*')
					continue;
			
			prev_fwd_slash = false;
			break;
		}
		
		case '/' : {
			
			prev_fwd_slash = true;
			break;
		}
			
		default : {
			
			if(cur_fchar >= 97 && cur_fchar <= 122 ||  //check if char is any lower case letter
			   cur_fchar >= 65 && cur_fchar <= 90 ||   //check if char is any upper case letter
			   cur_fchar >= 48 && cur_fchar <= 57 ||   //check if char is any number 0 - 9
			   cur_fchar == 95                     ) { //check if char is _
				   
				
				   
			} else {
			
				printf("%c",(char)cur_fchar);
			
			}
			prev_fwd_slash = false;
			break;
		}
			
		}
		
	}
	printf("\n");
	
	bob_exit(0);
	
}

bool missing_var(void) {
	
	return  main_src == NULL ||
	        src_paths->size == 0 ||
		    inc_paths->size == 0 ||
		    exe_path == NULL ||
			obj_path == NULL;
	
}

void parse_chfile(void) {
	
	//Read file change file if exists and full build is false
	struct _stat buf;
	if(_stat(ch_file,&buf) == 0 && !fullbuild) {
		if(dmode == 2)
			printf("%sChange file exists!\n", INFO_H);
		if(ch_file == NULL){
			printf("%sNull change file!\n", ERROR_H);
			bob_exit(1);
		}
		FILE *chfile = fopen(ch_file,"r");
		if(chfile == NULL){
			printf("%sCan't open change file!\n", ERROR_H);
			bob_exit(1);
		}
		char line[1024];
		while(fgets(line, sizeof line, chfile) != NULL) {
			char *filename = strtok(line,SEPARATOR);
			char *unconvhash = strtok(NULL,SEPARATOR);
			unsigned long filehash = strtoul(unconvhash, (char**)NULL, 10);
			
			gvalue filename_gv;
			filename_gv._str = (char*)malloc(strlen(filename) + 1);
			strcpy(filename_gv._str,filename);
			ladd(prev_sources,gvtnew(filename_gv,T_STR));
			
			gvalue filehash_gv;
			filehash_gv._ulong = filehash;
			ladd(prev_hashes,gvtnew(filehash_gv,T_ULONG));
			
		}
		fclose(chfile);
		if(dmode == 2) {
			printf("%sChange file contents:\n", INFO_H);
			printf("Prev files:  ");
			lprint(prev_sources);
			printf("Prev hashes: ");
			lprint(prev_hashes);
		}
	}
	
}

void create_chfile(void) {
	
	//Creating this builds change file.
	node_t *cur = sources->head;
	for(int i = 0; i < sources->size; i++){
		gvalue src_hash;
		src_hash._ulong = bob_hashfile(cur->value->value._str);
		ladd(hashes,gvtnew(src_hash,T_ULONG));
		cur = cur->next;
	}
	
	if(dmode == 2) {
		printf("%sCurrent build stats:\n", INFO_H);
		printf("Files:  ");
		lprint(sources);
		printf("Hashes: ");
		lprint(hashes);
	}
	
	{ //Write new change file
		
		FILE *chfile = fopen(ch_file,"w");
		if(chfile == NULL){
			printf("%sCan't open change file!\n", ERROR_H);
			bob_exit(1);
		}
		node_t *curs = sources->head;
		node_t *curh = hashes->head;
		
		if(dmode == 2)
			printf("-[NEW CHANGE FILE BEGIN]-\n");
		
		for(int i = 0; i < sources->size; i++){
			if(dmode == 2)
				printf("%s%s%lu\n", curs->value->value._str, SEPARATOR, curh->value->value._ulong);
			fprintf(chfile, "%s%s%lu\n", curs->value->value._str, SEPARATOR, curh->value->value._ulong);
			curs = curs->next;
			curh = curh->next;
		}
		fclose(chfile);
		
		if(dmode == 2)
			printf("--[NEW CHANGE FILE END]--\n");
		
	}
	
}

void get_src_diff(void) {
	
	node_t *cur = sources->head;
	for(int i = 0; i < sources->size; i++){
		int index = lhas(prev_sources,cur->value);
		if(index != -1){
			node_t *prevh = lgetat(prev_hashes,index);
			node_t *curh = lgetat(hashes,i);
			if(ncmp(prevh,curh) && strcmp(cur->value->value._str,main_src) != 0){
				cur->value->value._str = NULL;
			}
		}
		cur = cur->next;
	}
	
}

void create_compile(void) {
	
	compile_command = bob_strcat(vcvarsall," & cl /c ");
	//Concat additional parameters
	if(ad_param) {
		char *spaced_param = bob_strcat(ad_param," ");
		compile_command = bob_strcat(compile_command,spaced_param);
		free(spaced_param);
	}
	
	{ //Concatenate all the sources to the compile command
		
		node_t *cur = sources->head;
		for(int i = 0; i < sources->size; i++){
			if(!cur->value->value._str)
				continue;
			
			char *spaced_src = bob_strcat(cur->value->value._str," ");
			compile_command = bob_strcat(compile_command, spaced_src);
			free(spaced_src);
			cur = cur->next;
		}
		
	}

	//Concat obj path
	char *opath_arg = bob_strcat("/Fo.\\", obj_path);
	compile_command = bob_strcat(compile_command,opath_arg);
	free(opath_arg);
	
	{ //Concatenate all the include directorys to the compile command
		
		node_t *cur = inc_paths->head;
		for(int i = 0; i < inc_paths->size; i++){
			char *include_arg = bob_strcat(" /I.\\", cur->value->value._str);
			compile_command = bob_strcat(compile_command, include_arg);
			free(include_arg);
			cur = cur->next;
		}
		
	}
	
}

void create_link(void) {
	
	link_command = (char*)malloc(26 + strlen(vcvarsall) + strlen(subsys) + strlen(obj_path));
	snprintf(link_command, 26 + strlen(vcvarsall) + strlen(subsys) + strlen(obj_path), "%s & link /SUBSYSTEM:%s %s*.obj ", vcvarsall, subsys, obj_path);
	
	{ //Concatenate all libpaths
		
		node_t *cur = lib_paths->head;
		for(int i = 0; i < lib_paths->size; i++){
			char *libpath_tmp = bob_strcat(" /LIBPATH:.\\", cur->value->value._str);
			link_command = bob_strcat(link_command, libpath_tmp);
			cur = cur->next;
			free(libpath_tmp);
		}
	}
	
	{ //Concatenate all libs
		
		node_t *cur = libs->head;
		for(int i = 0; i < libs->size; i++){
			char *lib_tmp = bob_strcat(" ", cur->value->value._str);
			link_command = bob_strcat(link_command, lib_tmp);
			cur = cur->next;
			free(lib_tmp);
		}
	}
	
	//Concat exe path
	char *extra_params = bob_strcat(" /DEBUG /OUT:", exe_path);
	link_command = bob_strcat(link_command,extra_params);
	free(extra_params);
	
}

int main (int argc, char** argv) {
	
	parse_args(argc,argv);
	
	if(dmode == 2)
		printf("%sbob v%u\n", INFO_H, VERSION);
	
	src_paths = lnew();
	inc_paths = lnew();
	lib_paths = lnew();
	libs      = lnew();
	
	parse_conf();
	//parse_conf2(); WIP :D
	
	//If subsystem was not set use CONSOLE subsystem
	if(!subsys){
		subsys = (char*)malloc(8);
		strcpy(subsys,"CONSOLE");
	}
	
	//If target arch was not set use x86
	if(!trgt_arch){
		trgt_arch = (char*)malloc(4);
		strcpy(trgt_arch,"x86");
	}
	
	//If vcvars not set in config try to get it from env variable.
	if(!vcvarsall) {
		char *vcinstalldir = getenv("VS140COMNTOOLS");
		if(vcinstalldir) {
			char *quoted_vcdir = bob_strcat("\"",vcinstalldir);
			vcvarsall = bob_strcat(quoted_vcdir, "..\\..\\VC\\vcvarsall.bat\" ");
			vcvarsall = bob_strcat(vcvarsall, trgt_arch);
			free(quoted_vcdir);
		} else {
			if(dmode >= 1)
				printf("%sVCVARSALL not set, execution will continue but commands may fail.\n", WARN_H);
		}
	}
	
	if(missing_var()) {

		printf("%sVariable not set in build config file!.\n", ERROR_H);
		bob_exit(1);
		
	}
	
	prev_sources = lnew();
	prev_hashes = lnew();
	
	parse_chfile();
	
	sources = bob_sources(src_paths);
	hashes = lnew();
	
	create_chfile();
	
	get_src_diff();
	
	create_compile();
	create_link();
	
	printf("\n%sCompiling...\n\t%s\n\n", INFO_H, compile_command+strlen(vcvarsall)+3);
#ifndef MEMCHECK_BUILD
	int comp_error = system(compile_command);
	if(comp_error != 0) {
		
		printf("\n%sError compiling, aborting build...\n", ERROR_H);
		bob_exit(3);
		
	}
#endif
	printf("\n%sLinking...\n\t%s\n\n", INFO_H, link_command+strlen(vcvarsall)+3);
#ifndef MEMCHECK_BUILD
	int link_error = system(link_command);
	if(link_error != 0) {
		
		printf("\n%sError linking, aborting build...\n", ERROR_H);
		bob_exit(4);
		
	}
#endif
	
	bob_exit(0);
	
}

list_t* bob_sources(list_t *src_paths) {
	
	list_t *src_list = lnew();
	
	node_t *cur = src_paths->head;
	for (int i = 0; i < src_paths->size; i++) {
	
		DIR *dp;
		struct dirent *ep;
		
		dp = opendir(cur->value->value._str);
		if (dp == NULL)
			return NULL;
		
		while (ep = readdir(dp)) {
			bool is_src = false;
			char *ext = strrchr(ep->d_name,'.');
			for(int i = 0; i < 3; i++){
				if(ext && strcmp(ext,SOURCE_EXTENSIONS[i]) == 0){
					is_src = true;
					break;
				}
			}
			if(is_src) {
				gvalue src;
				char *slashed_name = bob_strcat("\\",ep->d_name);
				src._str = bob_strcat(cur->value->value._str, slashed_name);
				free(slashed_name);
				ladd(src_list,gvtnew(src,T_STR));
			}
		}
			
		closedir(dp);
		cur = cur->next;
	
	}
	
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
		if(dmode >= 1)
			printf("%sError getting file hash! %s\n", WARN_H, path);
		return 0;
	}
	if(fseek(fileptr,0,SEEK_END)) {
		if(dmode >= 1)
			printf("%sfseek Error! %s\n", WARN_H, path);
		return 0;
	}
	long fsize = ftell(fileptr);
	if(dmode == 2)
		printf("%sHashing file of size: %ld\n", INFO_H, fsize);
	if(fseek(fileptr,0,SEEK_SET)) {
		if(dmode >= 1)
			printf("%sfseek Error! %s\n", WARN_H, path);
		return 0;
	}
	
	char *filecontent = (char*)calloc(1,fsize + 1);
	fread(filecontent, fsize, 1, fileptr);
	fclose(fileptr);
	filecontent[fsize] = '\0';
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
	if(exe_path)
		free(exe_path);
	if(obj_path)
		free(obj_path);
	free(subsys);
	
	if(src_paths)
		lfree(src_paths);
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
	
	if(compile_command)
		free(compile_command);
	if(link_command)
		free(link_command);
	
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