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
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;
*/

SqList table_heads;//use table_heads.elem[i] to get the ith table_head.



int sol_query(query_type q)
{

}

one_query get_query(FILE *sql)
{
	char query_buffer[4096];//for get_query
	char line_buffer[4096];

	int end_of_query=0;
	int start_of_query=0;
	one_query res;
	char *get_res;
	while(!start_of_query)
	{
		get_res=fgets(line_buffer,4096,sql);
		if(get_res==NULL)
			return  res;
		if(line_buffer[0])
	}
	while(!end_of_query){
		fgets(line_buffer,4096,sql);


	}
	
}

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
	
	// table_heads.elem=calloc(LIST_INIT_SIZE,sizeof(ElemType));
	// table_heads.length=0;
	// table_heads.listsize=LIST_INIT_SIZE;


	int i;
	while(!feof(sql)){
		temp_query=get_query(sql);
		sol_query(temp_query);
		for(i=0;i<)
	}


	fclose(sql);

	return 0;
}