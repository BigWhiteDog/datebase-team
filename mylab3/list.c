#include "list.h"
void ListInit(SqList *l,int elem_size)
{
	l->elem=calloc(LIST_INIT_SIZE,elem_size);
	l->elem_size=elem_size;
	l->length=0;
	l->listsize=LIST_INIT_SIZE;
}
void ListInsert(SqList *l,char* e)
{
	if(l->length >= l->listsize)
	{
		char * newbase;
		newbase=(char*)realloc(l->elem,(l->listsize+LISTINCREMENT)*l->elem_size);
		if(newbase==NULL)
			exit(1);
		l->elem=newbase;
		l->listsize+=LISTINCREMENT;
	}
	memcpy(l->elem+l->length*l->elem_size,e,l->elem_size);
	l->length++;
}
void ListDelete(SqList *l,int index)
{
	if(index<0||index>=l->length)
		return;
	int j;
	for(j=index;j<l->length-1;j++)
		memcpy(l->elem+j*l->elem_size,l->elem+(j+1)*l->elem_size,l->elem_size);
	l->length--;
}