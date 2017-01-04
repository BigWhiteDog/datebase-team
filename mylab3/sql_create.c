#include"list.h"
#include"mysql.h"


int sol_create_query()
{
	table_head new_table_head;
	memset(&new_table_head,0,sizeof(table_head));
	memcpy(new_table_head.table_name,temp_create_query.table_name,128);
	new_table_head.col_num=temp_create_query.col_num;
	int i;
	for(i=0;i<new_table_head.col_num;i++)
	{
		strcpy(new_table_head.col_name[i],temp_create_query.col_name[i]);
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
	printf("Successfully created table %s\n",temp_create_query.table_name);
	return 1;
}