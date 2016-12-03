#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
char sql_buffer[4096];

enum keyword_type{a,create,table,};
enum elem_type{int32,varchar};
enum sign{int_l=0,int_le,int_g,int_ge,int_e,int_ne,
	varchar_e,varchar_ne,varchar_like,varchar_nlike};
typedef struct query_elem
{
		
}query_elem;
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




	return 0;
}