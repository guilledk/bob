#ifndef _STR_LIST_H_
#define _STR_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

struct strnode {
	
	char *str;
	unsigned long hash;
	struct strnode *next;
	
} typedef strnode;

strnode* sn_new(char *str);

struct strlist {
	
	strnode *head;
	unsigned int size;
	
} typedef strlist;

strlist* sl_new(void);

void sl_add(strlist *list, char *str);
void sl_addf(strlist *list, char *str);
strnode* sl_del(strlist *list, char *str);
strnode* sl_delf(strlist *list);
strnode* sl_get(strlist *list, char *str);
bool sl_empty(strlist *list);

const unsigned long hash(const char *str);

#endif