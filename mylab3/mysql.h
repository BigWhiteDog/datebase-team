#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#ifndef MYSQL
#define MYSQL

#include "list.h"

#define MAX_COL_NUM 128

typedef enum{
	my_create=0,
	my_insert,
	my_drop,
	my_select
}query_type;
typedef enum{
	int32=0,
	varchar
}elem_type;
typedef enum{
	filter_nosign=0,
	int_l,
	int_le,
	int_g,
	int_ge,
	int_e,
	int_ne,
	varchar_e,
	varchar_ne,
	varchar_like,
	varchar_nlike
}filter_sign;

typedef enum{
	aggr_nosign=0,
	aggr_sum,
	aggr_count,
	aggr_avg,
	aggr_min,
	aggr_max
}aggr_op;

//assume max column number = 128
//try to use a list structure to manage this table_heads[]
// and we try to use a list to save the headers
typedef struct
{
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;

//create query
typedef struct
{
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}create_query;

//drop query
typedef struct {
	int table_no;
}drop_query;

//insert query
typedef struct
{
	int table_no;
	char varchar[MAX_COL_NUM][4096];
	int32_t int32[MAX_COL_NUM];
}insert_query;

//select query
typedef struct
{
	int select_table_all_col;//1 for select * , 2 for count(*)
	
	int total_select_no;//<=2048
	int select_table[2048];//0 for tab0 , 1 for tab1 ,  2*i + 0/1 : i present aggr_op  present the 0/1 table
//eg: when that value is 3,mean this col will be sum(one col in tab 1) 	
	int select_table_col_no[2048];
	
	int use_table_no[2];//no[0] for used table0 , no[1] for used table1
	
	int join_sign;
	int join_table_0_col_no;
	int join_table_1_col_no;
	
	int filter_0_sign;
	int filter_0_table_col_no;
	int32_t filter_0_const_int;
	char filter_0_const_char[4096];
	
	int filter_1_sign;
	int filter_1_table_col_no;
	int32_t filter_1_const_int;
	char filter_1_const_char[4096];

	int group_table_no;//0 for tab0 , 1 for tab1 ,2 for no group
	int group_table_col_no;
}select_query;



extern create_query temp_create_query;
extern drop_query temp_drop_query;
extern insert_query temp_insert_query;
extern select_query temp_select_query;

extern SqList table_heads;
extern table_head * table_head_p;

int sol_create_query();
int sol_drop_query();
int sol_insert_query();
int sol_select_query();

typedef struct 
{
	int remain_size;
	SqList slot;
}page_header;

#endif