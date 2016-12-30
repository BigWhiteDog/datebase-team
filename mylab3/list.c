#include "list.h"
void ListInsert(SqList *l,ElemType e)
{
	int i,j;
	if(l->length>l->listsize)
	{
		ElemType * newbase;
		newbase=(ElemType*)realloc(l->elem,(l->listsize+LISTINCREMENT)*sizeof(ElemType));
		if(newbase==NULL)
			exit(1);
		l->elem=newbase;
		l->listsize+=LISTINCREMENT;
	}
	l->elem[l->length]=e;
	l->length++;
}
void ListDelete(SqList *l,int index)
{
	if(index<0||index>=l->length)
		return;
	int j;
	for(j=index;j<l->length-1;j++)
		l->elem[j]=l->elem[j+1];
	l->length--;
}