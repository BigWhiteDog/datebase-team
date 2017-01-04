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


void sol_query(query_type q)
{
	switch(q){
		case my_create:
			sol_create_query();
			break;
		case my_insert:
			sol_insert_query();
			break;
		case my_drop:
			sol_drop_query();
			break;
		case my_select:
			sol_select_query();
			break;
	}

}


int main(int argc, char const *argv[])
{

	// if(argc!=2){
	// 	fprintf(stderr,"Usage: ./ucasdb xxx.sql\n");
	// 	return 1;
	// }
	// FILE *sql;
	// sql=fopen(argv[1],"r");
	// if (sql==NULL)
	// {
	// 	fprintf(stderr,"Error! can't open file %s\n",argv[1]);
	// 	return 1;
	// }
	FILE *metadata;
	metadata=fopen("./db/metadata","rb+");
	fseek(metadata,0,SEEK_SET);
	//printf("%d %d %d \n", table_heads.elem_size,table_heads.length,table_heads.listsize);
	fread(&table_heads,sizeof(table_heads),1,metadata);
	table_heads.elem=malloc(table_heads.elem_size*table_heads.listsize);
	fread(table_heads.elem,table_heads.elem_size,table_heads.listsize,metadata);
	table_head_p = (table_head *)table_heads.elem;

	//test create
	strcpy(temp_create_query.table_name,"tab1");
	temp_create_query.col_num=2;
	strcpy(temp_create_query.col_name[0],"id");
	strcpy(temp_create_query.col_name[1],"name");
	temp_create_query.e_type[0]=int32;
	temp_create_query.e_type[1]=varchar;
	sol_query(my_create);
	memset(&temp_create_query,0,sizeof(create_query));

	strcpy(temp_create_query.table_name,"tab2");
	temp_create_query.col_num=3;
	strcpy(temp_create_query.col_name[0],"id");
	strcpy(temp_create_query.col_name[1],"count");
	strcpy(temp_create_query.col_name[2],"text");
	temp_create_query.e_type[0]=int32;
	temp_create_query.e_type[1]=int32;
	temp_create_query.e_type[2]=varchar;
	sol_query(my_create);
	memset(&temp_create_query,0,sizeof(create_query));

	//test insert
	temp_insert_query.table_no=0;
	temp_insert_query.int32[0]=1;
	strcpy(temp_insert_query.varchar[1],"first record");
	sol_query(my_insert);

	temp_insert_query.table_no=0;
	temp_insert_query.int32[0]=2;
	strcpy(temp_insert_query.varchar[1],"second record");
	sol_query(my_insert);

	temp_insert_query.table_no=0;
	temp_insert_query.int32[0]=3;
	strcpy(temp_insert_query.varchar[1],"third record");
	sol_query(my_insert);

	temp_insert_query.table_no=0;
	temp_insert_query.int32[0]=1;
	strcpy(temp_insert_query.varchar[1],"first again");
	sol_query(my_insert);

	memset(&temp_insert_query,0,sizeof(insert_query));

	temp_insert_query.table_no=1;
	temp_insert_query.int32[0]=1;
	temp_insert_query.int32[1]=10;
	strcpy(temp_insert_query.varchar[2],"tab 2 data");
	sol_query(my_insert);

	temp_insert_query.table_no=1;
	temp_insert_query.int32[0]=3;
	temp_insert_query.int32[1]=20;
	strcpy(temp_insert_query.varchar[2],"tab 2 data");
	sol_query(my_insert);

	temp_insert_query.table_no=1;
	temp_insert_query.int32[0]=5;
	temp_insert_query.int32[1]=30;
	strcpy(temp_insert_query.varchar[2],"tab 2 data");
	sol_query(my_insert);

	//test select
	memset(&temp_select_query,0,sizeof(select_query));
// select id, count(*) from tab1 where id > 0 group by id;
	temp_select_query.select_table_all_col=0;
	temp_select_query.total_select_no=2;
	temp_select_query.select_table[0]=0;
	temp_select_query.select_table_col_no[0]=0;
	temp_select_query.select_table[1]=2*aggr_count+0;
	temp_select_query.select_table_col_no[1]=1;

	temp_select_query.use_table_no[0]=0;
	// temp_select_query.use_table_no[1]=1;

	// temp_select_query.join_sign=int_e;
	// temp_select_query.join_table_0_col_no=0;
	// temp_select_query.join_table_1_col_no=0;

	temp_select_query.filter_0_sign=int_g;
	temp_select_query.filter_0_table_col_no=0;
	temp_select_query.filter_0_const_int=0;

	temp_select_query.group_table_no=0;
	temp_select_query.group_table_col_no=0;

	sol_query(my_select);
// select tab1.id, max(count) from tab1, tab2 where tab1.id = tab2.id and count <= 30 group by tab1.id;
	memset(&temp_select_query,0,sizeof(select_query));

	temp_select_query.select_table_all_col=0;
	temp_select_query.total_select_no=2;
	temp_select_query.select_table[0]=0;
	temp_select_query.select_table_col_no[0]=0;
	temp_select_query.select_table[1]=2*aggr_max+1;
	temp_select_query.select_table_col_no[1]=1;

	temp_select_query.use_table_no[0]=0;
	temp_select_query.use_table_no[1]=1;

	temp_select_query.join_sign=int_e;
	temp_select_query.join_table_0_col_no=0;
	temp_select_query.join_table_1_col_no=0;

	temp_select_query.filter_1_sign=int_le;
	temp_select_query.filter_1_table_col_no=1;
	temp_select_query.filter_1_const_int=30;

	temp_select_query.group_table_no=0;
	temp_select_query.group_table_col_no=0;

	sol_query(my_select);
	

	memset(&temp_select_query,0,sizeof(select_query));

	
	//test drop
	
	temp_drop_query.table_no=0;
	sol_query(my_drop);

	temp_drop_query.table_no=0;
	sol_query(my_drop);
	
	printf("%d %d %d \n", table_heads.elem_size,table_heads.length,table_heads.listsize);
	


	fseek(metadata,0,SEEK_SET);
	fwrite(&table_heads,sizeof(table_heads),1,metadata);
	fwrite(table_heads.elem,table_heads.elem_size,table_heads.listsize,metadata);
	free(table_heads.elem);
	// fclose(sql);
	fclose(metadata);

	return 0;
}
