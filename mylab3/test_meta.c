#include"mysql.h"
int main(int argc, char const *argv[])
{
	FILE *fp;
	fp=fopen("metadata","wb");
	SqList table_heads;
	ListInit(&table_heads,sizeof(table_head));
	fwrite(&table_heads,sizeof(SqList),1,fp);
	// memset(table_heads.elem,0,table_heads.elem_size*table_heads.listsize);
	// fclose(fp);
	// return 0;
	fwrite(table_heads.elem,table_heads.elem_size,table_heads.listsize,fp);
	fclose(fp);
	return 0;
}