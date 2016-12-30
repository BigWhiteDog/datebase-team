#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#ifndef MYSQL
#define MYSQL

#define MAX_COL_NUM 128

typedef enum{
	create=0,
	insert,
	drop,
	select
}query_type;
typedef enum{
	int32=0,
	varchar
}elem_type;
typedef enum{
	nosign=0,
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
	nosign=0,
	sum,
	count,
	avg,
	min,
	max
}aggr_op;

//assume max column number = 128
//try to use a list structure to manage this table_heads[]
// and we try to use a list to save the headers
typedef struct
{
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;


//create query
typedef struct
{
	// query_type q_type;
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}create_query;

//drop query
typedef struct {
	// query_type q_type;
	//char table_name[128];
//need to translate the table_name into table_no
	int table_no;
}drop_query;

//insert query
typedef struct
{
	// query_type q_type;
	//char table_name[128];
//need to translate the table_name into table_no
	int table_no;
	char varchar[MAX_COL_NUM][4096];
	int int32[MAX_COL_NUM];
}insert_query;

//select query
typedef struct
{
	// query_type q_type;
	//char table_name[128];
//need to translate the table_name into table_no
	int select_table_all_col;//1 for select * , 2 for count(*)
	int select_table_no[2048];//0 for tab0 , 1 for tab1 ,  2*i + 0/1 : i present aggr_op  present the 0/1 table
	//eg: when that value is 3,mean this col will be sum(one col in tab 1) 
	
	int select_table_col_no[2048];
	int use_table[2];//0 for used table1 , 1 for used table2
	int join_sign;
	int join_table_0_col_no;
	int join_table_1_col_no;
	
	int filter_1_sign;
	int filter_1_table_no;//0 for tab0, 1 for tab 1
	int filter_1_table_col_no;
	int filter_1_const_int;
	char filter_1_const_char[4096];
	
	int filter_2_sign;
	int filter_2_table_no;//0 for tab0, 1 for tab 1
	int filter_2_const_int;
	char filter_2_const_char[4096];

	int group_table_no[MAX_COL_NUM];//0 for tab0 , 1 for tab1
	int group_table_col_no[MAX_COL_NUM];
}select_query;
#endif