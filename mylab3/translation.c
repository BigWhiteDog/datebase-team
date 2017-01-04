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
table_head * table_head_p = table_heads.elem;


int sol_query(query_type q)
{

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
void select_query_all_change()
{
	if(temp_select_query.select_table_all_col)
	{
		int i,j;
		int total_col;
		total_col=table_head_p[temp_select_query.use_table_no[0]].col_num;
		temp_select_query.total_select_no=total_col;
		for(i=0;i<total_col;i++)
		{
			temp_select_query.select_table[i]=0;
			temp_select_query.select_table_col_no[i]=i;
		}
		if(temp_select_query.join_sign)
		{
			total_col=table_head_p[temp_select_query.use_table_no[1]].col_num;
			temp_select_query.total_select_no+=total_col;
			for(j=0;j<total_col;j++)
			{
				temp_select_query.select_table[i]=1;
				temp_select_query.select_table_col_no[i]=j;
				i++;
			}
		}
	}
}