#include <stdio.h>
#include "mysql.h"
#include <string.h>
#include <stdlib.h>
void get_shell(char* s, FILE* sql);
void interperter(FILE* sql);
int pattern_match(char* s, char* pat);

//may be delete
int main(int argc, char const *argv[]){
	FILE *sql;
	sql=fopen(argv[1],"r");
	interperter(sql);

	return 0;
}

void interperter(FILE* sql){
	char s[4096] = {'\0'};
	int ind1, ind2, ind3, ind4, ind5, ind6,ind7, ind8, ind9;
	int res,err;
	int table_head_ind;
	int col_head_ind;
	char cname_list[4096] = {'\0'};
	char table_name[128] = {'\0'};
	char select_table_name[2][128] = {'\0'};//record the two tname in select
	char col_value[4096] = {'\0'};
	int col_num = 0;
	int c_num = 0;
	int res1 = 1;
	int res2 = 1;
	char condition[3][1024] = {'\0'};
	char temp_table_name[2][128] = {'\0'};
	char temp_col_name[2][128] = {'\0'};
	while (1){ 
		//bzero
 		memset(s, '\0', sizeof(s));
 		memset(temp_create_query.table_name , '\0', sizeof(temp_create_query.table_name));
 		memset(temp_create_query.col_name, '\0', sizeof(temp_create_query.col_name));
 		memset(temp_create_query.e_type , '0', sizeof(temp_create_query.e_type));
 		temp_create_query.col_num = 0;
 		temp_drop_query.table_no = 0;
 		memset(col_value,'\0',sizeof(col_value));
 		memset(table_name,'\0',sizeof(table_name));
 		temp_insert_query.table_no = 0;
 		memset(temp_insert_query.varchar, '\0',sizeof(varchar));
 		memset(temp_insert_query.int32,'\0',sizeof(int32));
 		temp_select_query.select_table_all_col = 0;
 		temp_select_query.total_select_no = 0;
 		memset(temp_select_query.select_table,'0',sizeof(temp_select_query.select_table));
 		memset(temp_select_query.use_table_no,'0',sizeof(temp_select_query.use_table_no));
 		temp_select_query.join_sign = 0;
 		temp_select_query.join_table_0_col_no = 0;
 		temp_select_query.join_table_1_col_no = 0;
 		temp_select_query.filter_0_sign = 0;
 		temp_select_query.filter_1_sign = 0;
 		temp_select_query.filter_0_table_col_no = 0;
 		temp_select_query.filter_1_table_col_no = 0;
 		temp_select_query.filter_0_const_int = 0; 
 		temp_select_query.filter_1_const_int = 0;
 		memset(temp_select_query.filter_0_const_char, '\0', sizeof(temp_select_query.filter_0_const_char));
 		memset(temp_select_query.filter_1_const_char, '\0', sizeof(temp_select_query.filter_1_const_char));
 		temp_select_query.group_table_no = 0;
 		temp_select_query.group_table_col_no = 0;
 		memset(cname_list,'\0',sizeof(cname_list));
 		memset(select_table_name,'\0',sizeof(select_table_name));
  		memset(temp_table_name,'\0',sizeof(temp_table_name));
   		memset(temp_col_name,'\0',sizeof(temp_col_name));
 		memset(condition,'\0',sizeof(condition));
		col_num = 0;
		c_num = 0;
		table_head_ind = 0; col_head_ind = 0;
		ind1 = 0; ind2 = 0; ind3 = 0; ind4 = 0; ind5 = 0; ind6 = 0; ind7 = 0; ind8 = 0; ind9 = 0;
		res = 1;
		err = 0;
		res1 = 1; res2 = 1;

		//get shell
		get_shell(&s, sql);

		//create
		//create table tname(cname1 type,cname2 type);
		if(!strncmp("create table ", s, 13)){//strncmp return 0 if they are the same
			//table name
			ind1 = pattern_match(s, "(");//in exq: ind = 18 ind2 = 25, ind3 =30 ;ind2 = 37 ind3 = 42
			if(ind1>0){
				strncpy(temp_create_query.table_name, s+13, ind1 - 13);
				for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
					if(!strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), temp_create_query.table_name)){
						res = 0;
						break;
					}
				}
			}
			else err = 1;

			if(!res){
				printf("Can't create table %s\n", temp_create_query.table_name);
				continue;
			}
			if(err){
				printf("Syntax error\n");
				continue;
			}

			//col name and type
			ind3 = ind1;
			while(strchr(s+ind3, ',') != NULL){//did not concern about when typing the same col once;
				ind2 = pattern_match(s+ind3+1, " ") + ind3 + 1;
				if(ind2 > 0){
					strncpy(temp_create_query.col_name + col_num, s+ind3+1, ind2-ind3-1);
					ind3 = pattern_match(s+ind2+1, ",") + ind2 + 1;
					if(ind3 > 0){
						if(!strncmp("varchar", s+ind2+1,ind3-ind2-1))
							temp_create_query.e_type[col_num] = varchar;
						else if(!strncmp("int", s+ind2+1,ind3-ind2-1))
							temp_create_query.e_type[col_num] = int32;
						else {err = 1;break;}
						col_num ++;
					}
					else {err = 1;break;}
				}
				else {err = 1;break;}
			}
			if(err){
				printf("Syntax error\n");
				continue;
			}
			//last one
			ind2 = pattern_match(s+ind3+1, " ") + ind3 + 1;
			if(ind2 > 0){
				strncpy(temp_create_query.col_name + col_num, s+ind3+1, ind2-ind3-1);
				ind3 = pattern_match(s+ind2+1, ")") + ind2 + 1;
				if(ind3 > 0){
					if(!strncmp("varchar", s+ind2+1,ind3-ind2-1))
						temp_create_query.e_type[col_num] = varchar;
					else if(!strncmp("int", s+ind2+1,ind3-ind2-1))
						temp_create_query.e_type[col_num] = int32;
					else err = 1;
					col_num ++;
				}
				else err = 1;
			}
			else err = 1;
			if(s[ind3+1]!= ';') err = 1;
			if(err){
				printf("Syntax error\n");
				continue;
			}

			//call sol_query
			temp_create_query.col_num = col_num;
			res = sol_query(my_create);
			if(res)//successful
				printf("Successfully create table %s\n", temp_create_query.table_name);
			else
				printf("Can't create table %s\n", temp_create_query.table_name);

 			continue;
 		}


 		//drop
 		//drop table tname;
 		else if(!strncmp("drop table ", s, 11)){
 			ind1 = pattern_match(s, ";");// ind1 = 16
 			if(ind1>0){
 				strncpy(table_name, s+11, ind1 - 11);
 				if(strchr(table_name,' ')||strchr(table_name,'\n')||strchr(table_name,',')||strchr(table_name, ';'))//drop one table each time
 					err = 1;
 				else{	
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if(!strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), temp_create_query.table_name)){
							res = 0;
							break;
						}
					}				
					temp_drop_query.table_no = table_head_ind;
				}
			}
			else err = 1;
			if(err){
				printf("Syntax error\n");
				continue;
			}
			else if(!res){
				printf("Can’t drop table %s\n", table_name);
				continue;
			}

			res = sol_query(my_drop);
			if(res)
				printf("Successfully dropped table %s\n",table_name);
			else
				printf("Can’t drop table %s\n", table_name);
			continue;
		}


		//insert
		//insert into tname values(vec,vec,vec);
 		else if(!strncmp("insert into ", s, 12)){
 			//table name
 			ind1 = pattern_match(s, " values(");// ind1 = 17, ind3 = 24 ind2 = 28
 			if(ind1>0){
 				strncpy(table_name, s+12, ind1 - 12);
 				if(strchr(table_name,' ')||strchr(table_name,'\n')||strchr(table_name,',')||strchr(table_name, ';'))//drop one table each time
 					err = 1;
 				else{	
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if(!strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), temp_create_query.table_name)){
							res = 1;
							break;
						}
					}				
					temp_insert_query.table_no = table_head_ind;
				}
			}
			else err = 1;
			if(err){
				printf("Syntax error\n");
				continue;
			}
			else if(!res){
				printf("Can’t find table %s\n", table_name);
				continue;
			}

			//vaules
			ind3 = ind1 + 7;
			while(c_num < (((table_head*)(table_heads.elem) + table_head_ind)->col_num) - 1){
				ind2 = pattern_match(s+ind3 + 1, ",") + ind3 + 1;
				if(ind2 > 0){
					strncpy(col_value, s+ind3+1, ind2-ind3-1);
					if((((table_head*)(table_heads.elem) + table_head_ind)->e_type[c_num]) == int32 && col_value[0] !='\'')
						temp_insert_query.int32[c_num] = atoi(col_value);
					else if((((table_head*)(table_heads.elem) + table_head_ind)->e_type[c_num]) == varchar && col_value[0] =='\'')
						strncpy(temp_insert_query.varchar+c_num, col_value+1,strlen(col_value)-2);
					else{
						printf("Value and column type mismatch\n");
						err = 1;
						break;
					}
					c_num ++;
					ind3 = ind2;
				}
				else {printf("Wrong number of columns\n");err =1; break;}
			}
			if (err == 1) continue;

			//last one
			ind2 = pattern_match(s+ind3 + 1, ");") + ind3 + 1;
			if(ind2 > 0){
				strncpy(col_value, s+ind3+1, ind2-ind3-1);
				if(strchr(table_name,',')){
					printf("Wrong number of columns\n");
					continue;
				}
				else{	
					if((((table_head*)(table_heads.elem) + table_head_ind)->e_type[c_num]) == int32 && col_value[0] !='\'')
						temp_insert_query.int32[c_num] = atoi(col_value);
					else if((((table_head*)(table_heads.elem) + table_head_ind)->e_type[c_num]) == varchar && col_value[0] =='\'')
						strncpy(temp_insert_query.varchar+c_num, col_value+1,strlen(col_value)-2);
					else{
						printf("Value and column type mismatch\n");
						continue;
					}
					c_num ++;
				}
			}
			else {printf("Syntax error\n");continue;}
	
			//call sol_query
			res = sol_query(my_insert);

		}

		//select
		//select*from tname;
		//select*from tnamewhere cname='a';
		//select cname,cname,cnamefrom tname;
		//select cname,cname,cnamefrom tnamewhere cname='a';
		//select cname,cname,cnamefrom tname,tnamewhere c1.i=c2.iand cname='a'; 
		//select cname,cname,cname,aggr(cname)from tnamewhere cname='a'group by cname,cname;
		
		//select {*/cname,aggr(cname)}from {tname,tname}where 
		else if(!strncmp("select", s, 6)){
			//table name
			ind1 = pattern_match(s,"from");//index of 'f'rom
			ind2 = pattern_match(s,"where");//index of 'w'here
			if(ind1 > 0){//in exq5:ind1 = 24,ind2 = 40
				if(ind2>0){
					strncpy(cname_list, s + ind1 + 5, ind2 - ind1 - 5);//came_list:"tname,tname" in exq5
					ind3 = pattern_match(cname_list,",");
					if(ind3>0){//at most two table	
						temp_select_query.join_sign = 1;
						strncpy(select_table_name[0], cname_list, ind3);//ind3 = 5
						for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){					
							if(!strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), select_table_name[0])){
								res1 = 0;
								temp_select_query.use_table_no[0] = table_head_ind;
							}
						}
						if(res1){printf("Table %s doesn’t exist\n", select_table_name[0]); continue;}
					}
					else
						ind3 = ind1+4;//the " "before tname
					strncpy(select_table_name[1], cname_list+ind3+1, ind2-ind3-ind1-6);
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){					
						if(!strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), select_table_name[1])){
							res2 = 0;
							if(temp_select_query.join_sign)
								temp_select_query.use_table_no[1] = table_head_ind;
							else
								temp_select_query.use_table_no[0] = table_head_ind;
						}
					}
					if(res2){printf("Table %s doesn’t exist\n", select_table_name[1]); continue;}

					res1 = 1; res2 = 1;
					
					//where condition
					ind4 = pattern_match(s,"group by");//index of 'g'roup
					ind5 = pattern_match(s,";");
					ind6 = pattern_match(s,"and");//first 'a'nd, ind6 = 57 in exq5
					if(ind4>0){//in exq6:ind2 = 46, ind4 = 61, len(condition_str = 9)
						strncpy(condition_str, s+ind2+6,ind4-ind2-6);//condition_str="cname='a'"in exq6

					}
					else
						strncpy(condition_str, s+ind2+6,ind5-ind2-6);

					if(temp_select_query.join_sign){//link condition
						if(ind6>0){//has and
							strncpy(condition[2],condition_str,ind6-ind2-6);//condition[2] :link condition
							if(strchr(condition[2],'.')){//tname.cname
								ind7 = pattern_match(condition[2],".");
								ind8 = pattern_match(condition[2], "=")
								//first col of equal link
								strncpy(temp_table_name[0],condition[2],ind7);
								strncpy(temp_col_name[0],condition[2]+ind7+1,ind8-ind7-1);
								if(!strcmp(temp_table_name[0],select_table_name[0])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp((((table_head*)(table_heads.elem) + use_table_no[0])->col_name[col_head_ind]), temp_col_name[0])){
											res1 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
										}
									}
									if(res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue;}
								}
								else if(!strcmp(temp_table_name[0],select_table_name[1])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp((((table_head*)(table_heads.elem) + use_table_no[1])->col_name[col_head_ind]), temp_col_name[0])){
											res1 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
									if(res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue;}
								}

								ind9 = pattern_match(condition[2]+ind8+1,".");
								//second col of equal link
								strncpy(temp_table_name[1],condition[2]+ind8+1,ind9);
								strcpy(temp_col_name[1],condition[2]+ind9+1);
								if(!strcmp(temp_table_name[1],select_table_name[0])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp((((table_head*)(table_heads.elem) + use_table_no[0])->col_name[col_head_ind]), temp_col_name[1])){
											res2 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
										}
									}
									if(res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue;}
								}
								else if(!strcmp(temp_table_name[1],select_table_name[1])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp((((table_head*)(table_heads.elem) + use_table_no[1])->col_name[col_head_ind]), temp_col_name[1])){
											res2 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
									if(res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue;}
								}
							}
							else//cname

						}


					}


					}//has where
					else{//no where
						temp_select_query.filter_0_sign = 0;
						temp_select_query.filter_1_sign = 0;
						temp_select_query.join_sign = 0;
					}

			}//no from
			else{printf("Syntax error\n"); continue;}













			//first item in select
			if(s[6] == '*'){
				temp_select_query.select_table_all_col = 1;
			}
			else
				ind2 = 6;
				while(strchr(s+ind2+1, ',') != NULL)
					ind3 = pattern_match(s+ind3+1, ",") + ind3 + 1;
				if(ind2 > 0)


/*


	 				strncpy(table_name, s+12, ind1 - 12);
	 				if(strchr(table_name,' ')||strchr(table_name,'\n')||strchr(table_name,',')||strchr(table_name, ';'))//drop one table each time
	 					err = 1;
	 				else{	
						for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
							if(strcmp((((table_head*)(table_heads.elem) + table_head_ind)->table_name), temp_create_query.table_name)){
								res = 1;
								break;
							}
						}				
						temp_insert_query.table_no = table_head_ind;
					}
				}
				else err = 1;
				if(err){
					printf("Syntax error\n");
					continue;
				}


*/







		}
		else
			printf("Syntax error\n");

	}
}


//return the index of the pat[0] in s[ ], if not find , return -1
int pattern_match(char* s, char* pat){
	char * sub;
	sub = strstr(s ,pat);
	if(sub)
		return sub - s;
	else 
		return -1;
}

void get_shell(char * shell, FILE * sql){
	char buf [4096];
	int i;
	while(1){
		//gets(&s);
		//memset(buf, '\0', sizeof(buf));
		fgets(&buf,4096, sql);
		for (i = 0; i < strlen(buf); i++){
			if (buf[i] == '\t' || buf[i] == '\n')
				continue;
			else if (!(buf[i] == ' '&& ((buf[i-1]>='A' && buf[i-1]<= 'Z') || (buf[i-1] >='a' && buf[i-1] <='z')) && ((buf[i+1]>='A' && buf[i+1]<= 'Z') || (buf[i+1] >='a' && buf[i+1] <='z')) ))// only the blank space between 'word' and 'word' will be preseved
				continue; //in this way , the colname and table name must write in words(not digit or other)
			else if (buf[i] == '-' && buf[i+1] == '-'){
				if(shell[0] != '\0')
					printf("Syntax error\n");
				break;
			}
			else if (buf[i] == ';')
				break;
			else
				shell = strcat(shell, buf[i]); 
		}	
		if (strchr(buf, ';') != NULL){
			break;
		}
	}

}
