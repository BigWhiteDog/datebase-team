#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
char sql_buffer[8192];
char line_buffer[4096];
typedef enum{create,insert,drop,select}keyword_type;
typedef enum{int32,varchar}elem_type;
typedef enum{nosign=0,int_l,int_le,int_g,int_ge,int_e,int_ne,
	varchar_e,varchar_ne,varchar_like,varchar_nlike}filter_sign;
typedef enum{nosign=0,sum,count,avg,min,max}aggr_op;

/*basic agrv:
	create : table_name 0, 

*/
typedef struct query_elem
{
	keyword_type query;
	int argc;
	char* argv[];
	//
	filter_sign cmp_op[3];//0 for nosign
	int cmp_op[6];//0 for nothing




}one_query;


int sol_query(one_query q)
{

}

one_query get_query(FILE *sql)
{
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
	one_query temp_query;
	int i;
	while(!feof(sql)){
		temp_query=get_query(sql);
		sol_query(temp_query);
		for(i=0;i<)
	}


	fclose(sql);

	return 0;
}