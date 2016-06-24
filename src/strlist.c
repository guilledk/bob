#include "strlist.h"
#include <stdio.h>

strnode* sn_new(char *str) {
	
	strnode *node = (strnode*) malloc(sizeof(strnode));
	
	node->str = (char*)malloc(strlen(str));
	strcpy(node->str, str);
	node->hash = hash(str);
	
	return node;
	
}

strlist* sl_new(void) {
	
	strlist *list = (strlist*)malloc(sizeof(strlist));
	list->head = NULL;
	list->size = 0;
	
	return list;
	
}

void sl_add(strlist *list, char *str) {
	
	strnode *link = sn_new(str);
	
	if(list->head == NULL) {
		list->head = link;
	} else {
		
		strnode *cur = list->head;
		while(cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = link;
		
	}
	list->size++;
	
}

void sl_addf(strlist *list, char *str) {
	
	strnode *link = sn_new(str);
	
	link->next = list->head;
	
	list->head = link;
	
	list->size++;
	
}

strnode* sl_del(strlist *list, char *str) {
	
	if(list->head == NULL) {
		return NULL;
	}
	
	strnode *cur = list->head;
	strnode *prev = NULL;
	
	unsigned long strhash = hash(str);
	while(cur->hash != strhash) {
		
		if(cur->next == NULL){
			
			return NULL;
			
		} else {
			
			prev = cur;
			cur = cur->next;
			
		}
		
		return cur;
	
	}
	
	if(cur == list->head) {
		
		list->head = list->head->next;
		
	} else {
		
		prev->next = cur->next;
		
	}
	
	list->size--;
	return cur;
	
}

strnode* sl_delf(strlist *list) {
	
	strnode *temp = list->head;
	list->head = list->head->next;
	if(temp != NULL) 
		list->size--;
	return temp;
	
}

strnode* sl_get(strlist *list, char *str) {
	
	if(list->head == NULL) {
		return NULL;
	}
	strnode *cur = list->head;
	unsigned long strhash = hash(str);
	
	while(cur->hash != strhash) {
		
		if(cur->next == NULL){
			
			return NULL;
			
		} else {
			
			cur = cur->next;
			
		}
	
	}
	
	return cur;
	
}

bool sl_empty(strlist *list) {
	
	return list->head == NULL;
	
}

const unsigned long hash(const char *str){
	
	unsigned long hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
	
}