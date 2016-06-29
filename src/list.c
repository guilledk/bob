#include "list.h" 

gvalue_t* gvtnew(gvalue value, char type) {
	
	gvalue_t *new = (gvalue_t*)malloc(sizeof(gvalue_t));
	
	new->value = value;
	new->type = type;
	
	return new;
	
}

void      gvtfree(gvalue_t *gvt) {
	
	if(gvt)
		free(gvt);
	
}

bool      gvtcmp(gvalue_t *a, gvalue_t *b) {
	
	if(a->type == b->type){
		switch (a->type) {
			
		case T_CHAR : {
			return (a->value._char == b->value._char);
			break;
		}
		case T_STR : {
			return (strcmp(a->value._str,b->value._str) == 0);
			break;
		}
		case T_SCHAR : {
			return (a->value._schar == b->value._schar);
			break;
		} 
		case T_SHORT : {
			return (a->value._short == b->value._short);
			break;
		}
		case T_USHORT : {
			return (a->value._ushort == b->value._ushort);
			break;
		}
		case T_INT : {
			return (a->value._int == b->value._int);
			break;
		}
		case T_UINT : {
			return (a->value._uint == b->value._uint);
			break;
		}
		case T_LONG : {
			return (a->value._long == b->value._long);
			break;
		}
		case T_ULONG : {
			return (a->value._ulong == b->value._ulong);
			break;
		}
		case T_FLOAT : {
			return (fabs(a->value._float - b->value._float) < 0.00001);
			break;
		}
		default : {
			return false;
			break;
		}
		
		}
	}
	return false;
	
}

node_t*   nnew(gvalue_t *val) {
	
	node_t *node = (node_t*) malloc(sizeof(node_t));
	
	node->value = val;
	node->next = NULL;
	
	return node;
	
}

void      nfree(node_t *node) {
	
	if(node->value)
		gvtfree(node->value);
	if(node)
		free(node); //:D
	
}

bool      ncmp (node_t *a, node_t *b) {
	
	return gvtcmp(a->value,b->value);
	
}

list_t*   lnew(void) {
	
	list_t *list = (list_t*)malloc(sizeof(list_t));
	list->head = NULL;
	list->size = 0;
	
	return list;
	
}

void      lfree (list_t *list) {
	
	if(!list)
		return;
	
	node_t *temp = list->head;
	while(temp != NULL) {
		list->head = list->head->next;
		nfree(temp);
		temp = list->head;
	}
	free(list);
	
}

void      ladd(list_t *list, gvalue_t *val) {
	
	node_t *link = nnew(val);
	
	if(list->head == NULL) {
		list->head = link;
	} else {
		
		node_t *cur = list->head;
		while(cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = link;
		
	}
	list->size++;
	
}

void      laddf(list_t *list, gvalue_t *val) {
	
	node_t *link = nnew(val);
	
	link->next = list->head;
	list->head = link;
	
	list->size++;
	
}

void      lprint(list_t *list) {
	
	node_t *cur = list->head;
	printf("{ ");
	for (int i = 0; i < list->size; i++) {
		switch(cur->value->type){
		
		case T_CHAR   : {
			printf("%c",cur->value->value._char);
			break;
		}
		case T_STR    : {
			printf("%s",cur->value->value._str);
			break;
		}
		case T_SCHAR  : {
			printf("%d",cur->value->value._schar);
			break;
		}
		case T_SHORT  : {
			printf("%d",cur->value->value._short);
			break;
		}
		case T_USHORT : {
			printf("%u",cur->value->value._ushort);
			break;
		}
		case T_INT    : {
			printf("%d",cur->value->value._int);
			break;
		}
		case T_UINT   : {
			printf("%u",cur->value->value._uint);
			break;
		}
		case T_LONG   : {
			printf("%d",cur->value->value._long);
			break;
		}
		case T_ULONG  : {
			printf("%u",cur->value->value._ulong);
			break;
		}
		case T_FLOAT  : {
			printf("%f",cur->value->value._float);
			break;
		}
		
		}
		cur = cur->next;
		if(cur != NULL)
			printf(", ");
	}
	printf(" }\n");
	
}

node_t*   ldelf(list_t *list) {
	
	node_t *temp = list->head;
	list->head = list->head->next;
	if(temp != NULL) 
		list->size--;
	return temp;
	
}

node_t*   ldell(list_t *list) {

	node_t *temp = list->head;
	node_t *prev_temp = list->head;
	for (int i = 0; i < list->size - 1; i++) {
		if(i < list->size - 2)
			prev_temp = prev_temp->next;
		temp = temp->next;
	}
	prev_temp->next = NULL;
	
	return temp;
	
}

bool      lempty(list_t *list) {
	
	return list->head == NULL;
	
}

int       lhas  (list_t *list, gvalue_t *val) {
	
	int result = -1;
	node_t *cur = list->head;
	for(int i = 0; i < list->size; i++){
		if(gvtcmp(cur->value,val)) {
			result = i;
			break;
		}
		cur = cur->next;
	}
	return result;
	
}

node_t*   lgetat(list_t *list, unsigned short index) {
	
	if(index > list->size - 1)
		return NULL;
	
	if(index == 0)
		return list->head;
	
	node_t *cur = list->head;
	for(int i = 0; i < index; i++){
		cur = cur->next;
	}
	return cur;
	
}