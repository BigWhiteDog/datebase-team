#include"list.h"
#include"mysql.h"

/*typedef struct
{
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;


//create query
typedef struct
{
	// query_type q_type;
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}create_query;
*/
extern SqList table_heads;

int sol_create_query()
{
	table_head new_table_head;
	memset(&new_table_head,0,sizeof(table_head));
	memcpy(new_table_head.table_name,temp_create_query.table_name,128);
	new_table_head.col_num=temp_create_query.col_num;
	int i;
	for(i=0;i<new_table_head.col_num;i++)
	{
		strcpy(temp_create_query.col_name[i],new_table_head.col_name[i]);
		new_table_head.e_type[i] = temp_create_query.e_type[i];
	}
	ListInsert(&table_heads,&new_table_head);
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",temp_create_query.table_name);
	FILE *fp;
	fp=fopen(name_buffer,"wb");
	if(fp==NULL)
		return 0;
	fclose(fp);
	return 1;
}