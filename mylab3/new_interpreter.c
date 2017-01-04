#include <stdio.h>
#include "mysql.h"
#include <string.h>
#include <stdlib.h>
void get_shell(char* s, FILE* sql);
void interperter(FILE* sql);
int pattern_match(char* s, char* pat);
char* get_middle(char*s, char*im1, char* im2);

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
	char sub_str[2048];
	char type[10];
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
 		memset(sub_str,'\0',sizeof(sub_str));
 		memset(el_type,'\0',sizeof(el_type));

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
				temp_create_query.table_name = get_middle(s,"create table ","(");
				for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
					if(!strcmp(table_head_p[table_head_ind].table_name, temp_create_query.table_name)){
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
			sub_str = strcpy(s+ind1+1);
			while(strchr(sub_str, ',') != NULL){//did not concern about when typing the same col once;
				ind2 = pattern_match(sub_str, " ");
				if(ind2 > 0){
					temp_create_query.col_name + col_num = get_middle(sub_str,"\0"," ");
					sub_str = strcpy(sub_str+ind2);
					ind3 = pattern_match(sub_str, ",");
					if(ind3 > 0){
						type = get_middle(sub_str,"\0",",");
						if(!strcmp("varchar", type))
							temp_create_query.e_type[col_num] = varchar;
						else if(!strcmp("int", type))
							temp_create_query.e_type[col_num] = int32;
						else {err = 1;break;}
						col_num ++;
						sub_str = strcpy(sub_str+ind3+1);
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
			ind2 = pattern_match(sub_str, " ");
			if(ind2 > 0){					
				temp_create_query.col_name + col_num = get_middle(sub_str,"\0"," ");
				sub_str = strcpy(sub_str+ind2+1);
				ind3 = pattern_match(sub_str, ")");
				if(ind3 > 0){
					type = get_middle(sub_str,"\0",")");
					if(!strcmp("varchar", type))
						temp_create_query.e_type[col_num] = varchar;
					else if(!strcmp("int", type))
						temp_create_query.e_type[col_num] = int32;
					else err = 1;
					col_num ++;
				}
				else err = 1;
			}
			else err = 1;
			if(sub_str[ind3+1]!= ';') err = 1;
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
 				table_name = get_middle(s,"drop table ",";");
 				if(strchr(table_name,' ')||strchr(table_name,'\n')||strchr(table_name,',')||strchr(table_name, ';'))//drop one table each time
 					err = 1;
 				else{	
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if(!strcmp(table_head_p[table_head_ind].table_name, temp_create_query.table_name)){
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
 				table_name = get_middle(s, "insert into ", " values(");
 				if(strchr(table_name,' ')||strchr(table_name,'\n')||strchr(table_name,',')||strchr(table_name, ';'))//drop one table each time
 					err = 1;
 				else{	
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if(!strcmp(table_head_p[table_head_ind].table_name, temp_create_query.table_name)){
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
			sub_str = strcpy(s+ind1+8);
			while(c_num < table_head_p[table_head_ind].col_num - 1){
				ind2 = pattern_match(sub_str, ",");
				if(ind2 > 0){
					col_value = get_middle(sub_str, "\0",",");
					if(table_head_p[table_head_ind].e_type[c_num] == int32 && col_value[0] !='\'')
						temp_insert_query.int32[c_num] = atoi(col_value);
					else if(table_head_p[table_head_ind].e_type[c_num] == varchar && col_value[0] =='\'')
						strncpy(temp_insert_query.varchar+c_num, col_value+1,strlen(col_value)-2);
					else{
						printf("Value and column type mismatch\n");
						err = 1;
						break;
					}
					c_num ++;
					sub_str = strcpy(sub_str+ind2+1);
				}
				else {printf("Wrong number of columns\n");err =1; break;}
			}
			if (err == 1) continue;

			//last one
			ind2 = pattern_match(sub_str, ");");
			if(ind2 > 0){
				col_value = get_middle(sub_str,"\0",");");
				if(table_head_p[table_head_ind].e_type[c_num] == int32 && col_value[0] !='\'')
					temp_insert_query.int32[c_num] = atoi(col_value);
				else if(table_head_p[table_head_ind].e_type[c_num] == varchar && col_value[0] =='\'')
					strncpy(temp_insert_query.varchar+c_num, col_value+1,strlen(col_value)-2);
				else{
					printf("Value and column type mismatch\n");
					continue;
				}
				c_num ++;
			}
			//call sol_query
			res = sol_query(my_insert);

		}

		//select
		//select*from tname;
		//select*from tnamewhere cname='a';
		//select cname,cname,cname from tname;
		//select cname,cname,cname from tname where cname='a';
		//select cname,cname,cname from tname,tname where c1.i=c2.i and cname='a'; 
		//select cname,cname,cname,aggr(cname)from tname where cname='a'group by cname,cname;
		
		//select {*/cname,aggr(cname)}from {tname,tname}where 
		else if(!strncmp("select", s, 6)){
			//table name
			ind1 = pattern_match(s,"from");//index of 'f'rom
			ind2 = pattern_match(s,"where");//index of 'w'here
			if(ind1 > 0){//in exq5:ind1 = 24,ind2 = 40
				if(ind2>0){
					cname_list = get_middle(s,"from "," where")//came_list:"tname,tname" in exq5
					ind3 = pattern_match(cname_list,",");
					if(ind3>0){//at most two table	
						temp_select_query.join_sign = 1;
						select_table_name[0] = get_middle(cname_list, "\0", ",");
						for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){					
							if(!strcmp(table_head_p[table_head_ind].table_name, select_table_name[0])){
								res1 = 0;
								temp_select_query.use_table_no[0] = table_head_ind;
							}
						}
						if(res1){printf("Table %s doesn’t exist\n", select_table_name[0]); continue;}
						select_table_name[1] = get_middle(cname_list,",","\0");
					}
					else
						select_table_name[1] = strcpy(cname_list);
					for(table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){					
						if(!strcmp(table_head_p[table_head_ind].table_name, select_table_name[1])){
							res2 = 0;
							if(temp_select_query.join_sign)
								temp_select_query.use_table_no[1] = table_head_ind;
							else
								temp_select_query.use_table_no[0] = table_head_ind;
						}
					}
					if(res2){printf("Table %s doesn’t exist\n", select_table_name[1]); continue;}

					res1 = 1; res2 = 1;
					
					//condition
					ind4 = pattern_match(s,"group by");//index of 'g'roup
					ind5 = pattern_match(s,";");
					ind6 = pattern_match(s,"and");//first 'a'nd, ind6 = 57 in exq5
					if(ind4>0){//in exq6:ind2 = 46, ind4 = 61, len(condition_str = 9)
						condition_str = get_middle(s,"where "," group by");//condition_str="cname='a'"in exq6

					//*******************************************
					}
					else
						condition_str = get_middle(s,"where ", ";");

					if(temp_select_query.join_sign){//link condition
						if(ind6>0){//has and
							condition[2] = get_middle(condition_str, "\0"," and");//condition[2] :link condition
							sub_str = strcpy(condition_str,"and ","\0");
							condition[1] = get_middle()


							
							if(strchr(condition[2],'.')){//tname.cname
								//ind7 = pattern_match(condition[2],".");
								ind8 = pattern_match(condition[2], "=")
								if(ind8<0) {printf("Syntax error\n"); continue;}
								//first col of equal link
								temp_table_name[0] = get_middle(condition[2],"\0", ".");
								temp_col_name[0] = get_middle(condition[2], ".","=");
								if(!strcmp(temp_table_name[0],select_table_name[0])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp(table_head_p[use_table_no[0]].col_name[col_head_ind], temp_col_name[0])){
											res1 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
										}
									}
									if(res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue;}
								}
								else if(!strcmp(temp_table_name[0],select_table_name[1])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp(table_head_p[use_table_no[1]].col_name[col_head_ind], temp_col_name[0])){
											res1 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
									if(res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue;}
								}
								else {printf("Syntax error\n"); continue;}

								ind9 = pattern_match(condition[2]+ind8+1,".");
								if(ind9<0) {printf("Syntax error\n"); continue;}

								//second col of equal link
								temp_table_name[1] = get_middle(condition[2]+ind8-1,"=",".");
								temp_col_name[1] = get_middle(condition[2]+ind8-1,".", "\0" );

								if(!strcmp(temp_table_name[1],select_table_name[0])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp(table_head_p[use_table_no[0]].col_name[col_head_ind], temp_col_name[1])){
											res2 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
										}
									}
									if(res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue;}
								}
								else if(!strcmp(temp_table_name[1],select_table_name[1])){
									for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
										if(!strcmp(table_head_p[use_table_no[1]].col_name[col_head_ind], temp_col_name[1])){
											res2 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
									if(res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue;}
								}
								else {printf("Syntax error\n"); continue;}
								if(!strcmp(temp_table_name[1],temp_table_name[0])){printf("Syntax error\n"); continue;}
							}
							else{//cname
								res1 =1; res2 =1;
								//first col of equal link
								temp_col_name[0] = get_middle(condition[2], "\0","=");
								for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
									if(!strcmp(table_head_p[use_table_no[0]].col_name[col_head_ind], temp_col_name[0])){
										res1 = 0;
										temp_select_query.join_table_0_col_no = col_head_ind;
									}
								}

								for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
									if(!strcmp(table_head_p[use_table_no[1]].col_name[col_head_ind]), temp_col_name[0]){
										if(res1 = 0){ printf("Ambiguous column%s\n", temp_col_name[0]); err = 1; break;}
										else{
											res1 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
								}
								if(res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue;}
								if(err) continue;

								//second col of equal link
								temp_col_name[1] = get_middle(condition[2],"=","\0" );
								for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
									if(!strcmp(table_head_p[use_table_no[0]].col_name[col_head_ind], temp_col_name[1])){
										res2 = 0;
										temp_select_query.join_table_0_col_no = col_head_ind;
									}
								}

								for(col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
									if(!strcmp(table_head_p[use_table_no[1]].col_name[col_head_ind]), temp_col_name[1]){
										if(res2 = 0){ printf("Ambiguous column%s\n", temp_col_name[1]); err = 1; break;}
										else{
											res2 = 0;
											temp_select_query.join_table_1_col_no = col_head_ind;
										}
									}
								}
								if(res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue;}
								if(err) continue;
								if(!strcmp(table_head_p[use_table_no[0]].col_name[temp_select_query.join_table_0_col_no], table_head_p[use_table_no[0]].col_name[temp_select_query.join_table_1_col_no])){printf("Syntax error\n", );continue;}
								if(!strcmp(table_head_p[use_table_no[1]].col_name[temp_select_query.join_table_0_col_no], table_head_p[use_table_no[1]].col_name[temp_select_query.join_table_1_col_no])){printf("Syntax error\n", );continue;}
							}

							//op condition(of two table, has and)



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
//auto choose the first im2 after im1
char* get_middle(char*s, char*im1, char* im2){//at the end of im1,beign of im2
	int ind1, ind2;
	int shf1, shf2;
	char result[2048];

	ind1 = pattern_match(s, im1);
	ind2 = pattern_match(s, im2);
	shf1 = strlen(im1);
	shf2 = strlen(im2);
	if(im1[0]=='\0'){
		strncpy(result, s, ind2);
		return result;
	}
	else if(im2[0] == '\0'){
		strcpy(result,s+ind1+shf1);
		return result;
	}
	else{
		while(ind2<ind1){
			strcpy(s,s+ind2+1);
			ind1 = pattern_match(s,im1);
			ind2 = pattern_match(s, im2);
		}
		strncpy(result, s+ind1+shf1, ind2-shf1-ind1);
		return result;
		}
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
	int in_quort = 0;
	int enter = 0;
	char b_1 = '0';
	while(1){
		//gets(&s);
		//memset(buf, '\0', sizeof(buf));
		fgets(&buf,4096, sql);
		if(enter){
			buf = strcat(" ",buf);
			enter = 0;
		}
		for (i = 0; i < strlen(buf); i++){
			if (buf[i] == '\t' || buf[i] == '\n'){
				enter = 1;
				b_1 = buf[i-1];
				continue;
			}
			else if(buf[i] == '\''&& in_quort == 0){
				in_quort == 1;
				shell = strcat(shell, buf[i]); 
			}
			else if(buf[i] == '\''&& in_quort == 1){
				in_quort == 0;
				shell = strcat(shell, buf[i]); 
			}
			else if ((!(buf[i] == ' '&& (((buf[i-1]>='A' && buf[i-1]<= 'Z') || (buf[i-1] >='a' && buf[i-1] <='z')) && ((buf[i+1]>='A' && buf[i+1]<= 'Z') || (buf[i+1] >='a' && buf[i+1] <='z')))||(buf[i-1]>='0'&& buf[i-1]<='9') )) && in_quort == 0 && i!=0)// only the blank space between 'word' and 'word' will be preseved
				continue; //in this way , the colname and table name must write in words
			else if ((!(buf[i] == ' '&& (((buf_1>='A' && buf_1<= 'Z') || (b_1 >='a' && b_1 <='z')) && ((buf[i+1]>='A' && buf[i+1]<= 'Z') || (buf[i+1] >='a' && buf[i+1] <='z')))||(b_1>='0'&& b_1<='9') )) && in_quort == 0 && i==0)// only the blank space between 'word' and 'word' will be preseved
				continue; //in this way , the colname and table name must write in words
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
