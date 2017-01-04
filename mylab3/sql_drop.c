#include"list.h"
#include"mysql.h"
int sol_drop_query()
{
	char the_tab_name[130];
	strcpy(the_tab_name,table_head_p[temp_drop_query.table_no].table_name);
	ListDelete(&table_heads,temp_drop_query.table_no);
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",the_tab_name);
	if(remove(name_buffer)<0){
		printf("Can't drop table %s\n",the_tab_name);
		return 0;
	}
	else
	{
		printf("Succesfully dropped tabel %s\n",the_tab_name);
		return 1;
	}
}