#ifndef _LIST_H_
#define _LIST_H_

#define T_CHAR   0
#define T_STR    1
#define T_SCHAR  2
#define T_SHORT  3
#define T_USHORT 4
#define T_INT    5
#define T_UINT   6
#define T_LONG   7
#define T_ULONG  8
#define T_FLOAT  9

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//GENERIC VALUE IMPLEMENTATION

union generic_value {       //REPRESENTATION
	char           _char;   //0
	char*          _str;    //1
	signed char    _schar;  //2
	short          _short;  //3
	unsigned short _ushort; //4
	int            _int;    //5
	unsigned int   _uint;   //6
	long           _long;   //7
	unsigned long  _ulong;  //8
	float          _float;  //9
} typedef gvalue;

struct tagged_generic_value {
	
	gvalue value;
	char   type;
	
} typedef gvalue_t;

gvalue_t* gvt_new (gvalue value, char type);
void      gvt_free(gvalue_t *gvt);

//LINKED LIST IMPLEMENTATION

struct node {
	
	gvalue_t *value;
	struct node* next;
	
} typedef node_t;

node_t* nnew (gvalue_t *val);
void    nfree(node_t *node);

struct list {
	
	node_t *head;
	unsigned short size;
	
} typedef list_t;

list_t* lnew  (void);
void    lfree (list_t *list);
void    ladd  (list_t *list, gvalue_t *val);
void    laddf (list_t *list, gvalue_t *val);
void    lprint(list_t *list);
node_t* ldelf (list_t *list);
bool    lempty(list_t *list);

#endif