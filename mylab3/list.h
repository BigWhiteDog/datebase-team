#ifndef SQLIST 
#define SQLIST

#define LIST_INIT_SIZE 10
#define LISTINCREMENT 10
typedef struct
{
	char *elem;
	int elem_size
	int length;
	int listsize;
}SqList;

void ListInsert(SqList *l,char *e);
void ListDelete(SqList *l,int index);
void ListInit(SqList *l,int elem_size);


#endif