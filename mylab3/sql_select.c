#include"mycmp.h"
#include "mysql.h"
#include "list.h"
int (*filter_cmp_func[11])(void *,void*)={
	NULL,
	int_l_cmp,
    int_le_cmp,
    int_g_cmp,
    int_ge_cmp,
    int_e_cmp,
    int_ne_cmp,
    varchar_e_cmp,
    varchar_ne_cmp,
    varchar_like_cmp,
    varchar_nlike_cmp
};



/*typedef struct
{
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;
*/

int sol_select_query()
{
	table_head *t_head[2];
	t_head[0] = (table_head *)(table_heads.elem)+temp_select_query.use_table[0];
	t_head[1] = (table_head *)(table_heads.elem)+temp_select_query.use_table[1];
	
	FILE *fp[2];
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",t_head[0]->table_name);
	fp[0]=fopen(name_buffer,"rb");
	if(fp[0]==NULL)
		return 0;
	fseek(fp[0],0,SEEK_SET);
	if(temp_select_query.join_sign)
	{
		sprintf(name_buffer,"./db/%s.tbl",t_head[1]->table_name);
		fp[1]=fopen(name_buffer,"rb");
		if(fp[1]==NULL)
			return 0;
		fseek(fp[1],0,SEEK_SET);
	}
	
	SqList 


	return 1;
}