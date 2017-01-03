#include"list.h"
#include"mysql.h"

/*typedef struct
{
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;


//drop query
typedef struct {
	// query_type q_type;
	//char table_name[128];
//need to translate the table_name into table_no
	int table_no;
}drop_query;
*/
// extern SqList table_heads;

int sol_drop_query()
{
	ListDelete(&table_heads,temp_drop_query.table_no);
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",temp_create_query.table_name);
	if(remove(name_buffer)<0)
		return 0;
	return 1;
}