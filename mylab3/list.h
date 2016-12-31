#ifndef SQLIST 
#define SQLIST

#define LIST_INIT_SIZE 10
#define LISTINCREMENT 10
typedef table_head ElemType;
typedef struct
{
	ElemType *elem;
	int length;
	int listsize;
}SqList;

void ListInsert(SqList *l,ElemType e);
void ListDelete(SqList *l,int index);

extern SqList table_heads;
#endif