#include "mysql.h"
#include "list.h"
create_query temp_create_query;
drop_query temp_drop_query;
insert_query temp_insert_query;
select_query temp_select_query;
//when call get_query , set value of one of above four query(use strcpy for varchar , = for int)
//then call sol_query with query_type

/*
MAX_COL_NUM 128
typedef struct
{
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;
*/

SqList table_heads;//use table_heads.elem[i] to get the ith table_head.
table_head * table_head_p;

FILE* metadata;

void interperter(FILE* sql);
int main(int argc, char const *argv[])
{

	if(argc!=2){
		fprintf(stderr,"Usage: ./ucasdb xxx.sql\n");
		return 1;
	}
	FILE *sql;
	sql=fopen(argv[1],"r");
	if (sql==NULL)
	{
		fprintf(stderr,"Error! can't open file %s\n",argv[1]);
		return 1;
	}
	metadata=fopen("./db/metadata","rb+");
	fseek(metadata,0,SEEK_SET);
	//printf("%d %d %d \n", table_heads.elem_size,table_heads.length,table_heads.listsize);
	fread(&table_heads,sizeof(table_heads),1,metadata);
	table_heads.elem=malloc(table_heads.elem_size*table_heads.listsize);
	fread(table_heads.elem,table_heads.elem_size,table_heads.listsize,metadata);
	table_head_p = (table_head *)table_heads.elem;

	interperter(sql);
	
	fseek(metadata,0,SEEK_SET);
	fwrite(&table_heads,sizeof(table_heads),1,metadata);
	fwrite(table_heads.elem,table_heads.elem_size,table_heads.listsize,metadata);
	free(table_heads.elem);
	fclose(sql);
	fclose(metadata);

	return 0;
}
