#ifndef SQLIST 
#define SQLIST

#include <stdlib.h>
#include <string.h>

#define LIST_INIT_SIZE 16
#define LISTINCREMENT 16
typedef struct
{
	char *elem;
	int elem_size;
	int length;
	int listsize;
}SqList;

void ListInsert(SqList *l,char *e);
void ListDelete(SqList *l,int index);
void ListInit(SqList *l,int elem_size);


#endif