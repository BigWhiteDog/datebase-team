#include <stdio.h>
#include "mysql.h"
#include <string.h>
#include <stdlib.h>
int get_shell(char* s, FILE* sql);
void interperter(FILE* sql);
int pattern_match(char* s, char* pat);
char* get_middle(char* result, char*s, char*im1, char* im2);

char s[4096];
int i;
int ind1, ind2, ind3, ind4, ind5, ind6, ind7, ind8, ind9;
int res, err;
int table_head_ind;
int col_head_ind;
char cname_list[4096];
char table_name[128];
char select_table_name[2][128];//record the two tname in select
char col_value[4096];
int col_num = 0;
int c_num = 0;
int res1 = 1;
int res2 = 1;
char condition[3][1024];
char temp_table_name[2][128];
char temp_col_name[2][128];
char sub_str[2048];
char condition_str[2048];
char el_type[10];
elem_type e_type[2];
filter_sign op[2];
char cop[2][20];
char char_const[2][100];
char agg_op[2048][20];
aggr_op agg[2048];
char op_table[2048];
char op_col[2048];
char select_name[2048][256];
char select_str[2048];
char se_col[2048];
char se_table[2048];
char sl_str[2048][512];
char group_table_name[2048];
char group_col_name[2048];
char op_str[2048];
char group_name[2048];


int tran_col(char * col_c , int tab0 ,int tab1 ,int tab_num ,int *col_no)
{

}



void interperter(FILE* sql){
	
	int indl, indle, indg, indge, inde, indne, indlike, indnlike;
	while (1){
		//bzero
		memset(s, '\0', sizeof(s));
		memset(col_value, '\0', sizeof(col_value));
		memset(table_name, '\0', sizeof(table_name));

		memset(&temp_drop_query,0,sizeof(temp_drop_query));
		memset(&temp_create_query,0,sizeof(temp_create_query));
		memset(&temp_select_query, 0, sizeof(temp_select_query));
		memset(&temp_insert_query,0,sizeof(temp_insert_query));
		
		memset(cname_list, '\0', sizeof(cname_list));
		memset(select_table_name, '\0', sizeof(select_table_name));
		memset(temp_table_name, '\0', sizeof(temp_table_name));
		memset(temp_col_name, '\0', sizeof(temp_col_name));
		memset(condition, '\0', sizeof(condition));
		memset(sub_str, '\0', sizeof(sub_str));
		memset(el_type, '\0', sizeof(el_type));
		memset(e_type, '\0', sizeof(e_type));
		memset(op, '\0', sizeof(op));
		memset(cop, '\0', sizeof(cop));
		memset(char_const, '\0', sizeof(char_const));
		memset(agg_op, '\0', sizeof(agg_op));
		memset(agg,0, sizeof(agg));
		memset(op_table, '\0', sizeof(op_table));
		memset(op_col, '\0', sizeof(op_col));
		memset(select_name, '\0', sizeof(select_name));
		memset(select_str, '\0', sizeof(select_str));
		memset(sl_str, '\0', sizeof(sl_str));
		memset(se_table, '\0', sizeof(se_table));
		memset(se_col, '\0', sizeof(se_col));
		memset(group_table_name, '\0', sizeof(group_table_name));
		memset(group_col_name, '\0', sizeof(group_col_name));
		memset(op_str, '\0', sizeof(op_str));
		memset(group_name, '\0', sizeof(group_name));
		indl = 0; indle = 0; indg = 0; indge = 0; inde = 0; indne = 0; indlike = 0; indnlike = 0;
		col_num = 0;
		c_num = 0;
		table_head_ind = 0; col_head_ind = 0;
		ind1 = 0; ind2 = 0; ind3 = 0; ind4 = 0; ind5 = 0; ind6 = 0; ind7 = 0; ind8 = 0; ind9 = 0;
		res = 1;
		err = 0;
		res1 = 1; res2 = 1;

		//get shell
		if(get_shell(s, sql)==0)
			break;

		//create
		//create table tname(cname1 type,cname2 type);
		if (!strncmp("create table ", s, 13)){//strncmp return 0 if they are the same
			//table name
			ind1 = pattern_match(s, "(");//in exq: ind = 18 ind2 = 25, ind3 =30 ;ind2 = 37 ind3 = 42
			if (ind1>0){
				get_middle(temp_create_query.table_name, s, "create table ", "(");
				for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
					if (!strcmp(table_head_p[table_head_ind].table_name, temp_create_query.table_name)){
						res = 0;
						break;
					}
				}
			}
			else err = 1;

			if (!res){
				printf("Can't create table %s\n", temp_create_query.table_name);
				continue;
			}
			if (err){
				printf("Syntax error\n");
				continue;
			}

			//col name and type
			strcpy(sub_str, s + ind1 + 1);
			while (strchr(sub_str, ',') != NULL){//did not concern about when typing the same col once;
				ind2 = pattern_match(sub_str, " ");
				if (ind2 > 0){
					get_middle(temp_create_query.col_name[col_num], sub_str, "\0", " ");
					strcpy(sub_str, sub_str + ind2 + 1);
					ind3 = pattern_match(sub_str, ",");
					if (ind3 > 0){
						get_middle(el_type, sub_str, "\0", ",");
						if (!strcmp("varchar", el_type))
							temp_create_query.e_type[col_num] = varchar;
						else if (!strcmp("int", el_type))
							temp_create_query.e_type[col_num] = int32;
						else { err = 1; break; }
						col_num++;
						strcpy(sub_str, sub_str + ind3 + 1);
					}
					else { err = 1; break; }
				}
				else { err = 1; break; }
			}
			if (err){
				printf("Syntax error\n");
				continue;
			}
			//last one
			ind2 = pattern_match(sub_str, " ");
			if (ind2 > 0){
				get_middle(temp_create_query.col_name[col_num], sub_str, "\0", " ");
				strcpy(sub_str, sub_str + ind2 + 1);
				ind3 = pattern_match(sub_str, ")");
				if (ind3 > 0){
					get_middle(el_type, sub_str, "\0", ")");
					if (!strcmp("varchar", el_type))
						temp_create_query.e_type[col_num] = varchar;
					else if (!strcmp("int", el_type))
						temp_create_query.e_type[col_num] = int32;
					else err = 1;
					col_num++;
				}
				else err = 1;
			}
			else err = 1;
			if (sub_str[ind3 + 1] != ';') err = 1;
			if (err){
				printf("Syntax error\n");
				continue;
			}

			//call sol_query
			temp_create_query.col_num = col_num;
			res = sol_query(my_create);
			if (res)//successful
				printf("Successfully create table %s\n", temp_create_query.table_name);
			else
				printf("Can't create table %s\n", temp_create_query.table_name);

			continue;
		}


		//drop
		//drop table tname;
		else if (!strncmp("drop table ", s, 11)){
			ind1 = pattern_match(s, ";");// ind1 = 16
			if (ind1>0){
				get_middle(table_name, s, "drop table ", ";");
				if (strchr(table_name, ' ') || strchr(table_name, '\n') || strchr(table_name, ',') || strchr(table_name, ';'))//drop one table each time
					err = 1;
				else{
					for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if (!strcmp(table_head_p[table_head_ind].table_name, table_name)){
							res = 0;
							break;
						}
					}
					temp_drop_query.table_no = table_head_ind;
				}
			}
			else err = 1;
			if (err){
				printf("Syntax error\n");
				continue;
			}
			else if (res){
				printf("Can’t drop table %s\n", table_name);
				continue;
			}

			res = sol_query(my_drop);
			if (res)
				printf("Successfully dropped table %s\n", table_name);
			else
				printf("Can’t drop table %s\n", table_name);
			continue;
		}


		//insert
		//insert into tname values(vec,vec,vec);
		else if (!strncmp("insert into ", s, 12)){
			//table name
			ind1 = pattern_match(s, " values(");// ind1 = 17, ind3 = 24 ind2 = 28
			if (ind1>0){
				get_middle(table_name, s, "insert into ", " values(");
				if (strchr(table_name, ' ') || strchr(table_name, '\n') || strchr(table_name, ',') || strchr(table_name, ';'))//drop one table each time
					err = 1;
				else{
					for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if (!strcmp(table_head_p[table_head_ind].table_name, table_name)){
							res = 1;
							break;
						}
					}
					temp_insert_query.table_no = table_head_ind;
				}
			}
			else err = 1;
			if (err){
				printf("Syntax error\n");
				continue;
			}
			else if (!res){
				printf("Can’t find table %s\n", table_name);
				continue;
			}

			//vaules
			strcpy(sub_str, s + ind1 + 8);
			while (c_num < table_head_p[table_head_ind].col_num - 1){
				ind2 = pattern_match(sub_str, ",");
				if (ind2 > 0){
					get_middle(col_value, sub_str, "\0", ",");
					if (table_head_p[table_head_ind].e_type[c_num] == int32 && col_value[0] != '\'')
						temp_insert_query.int32[c_num] = atoi(col_value);
					else if (table_head_p[table_head_ind].e_type[c_num] == varchar && col_value[0] == '\'')
						strncpy(temp_insert_query.varchar[c_num], col_value + 1, strlen(col_value) - 2);
					else{
						printf("Value and column type mismatch\n");
						err = 1;
						break;
					}
					c_num++;
					strcpy(sub_str, sub_str + ind2 + 1);
				}
				else { printf("Wrong number of columns\n"); err = 1; break; }
			}
			if (err == 1) continue;

			//last one
			ind2 = pattern_match(sub_str, ");");
			if (ind2 > 0){
				get_middle(col_value, sub_str, "\0", ");");
				if (table_head_p[table_head_ind].e_type[c_num] == int32 && col_value[0] != '\'')
					temp_insert_query.int32[c_num] = atoi(col_value);
				else if (table_head_p[table_head_ind].e_type[c_num] == varchar && col_value[0] == '\'')
					strncpy(temp_insert_query.varchar[c_num], col_value + 1, strlen(col_value) - 2);
				else{
					printf("Value and column type mismatch\n");
					continue;
				}
				c_num++;
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
		else if (!strncmp("select", s, 6)){
			//table name
			ind1 = pattern_match(s, "from");//index of 'f'rom
			ind2 = pattern_match(s, "where");//index of 'w'here
			if (ind1 > 0){//in exq5:ind1 = 24,ind2 = 40
				//table_no
				if (ind2>0)
					get_middle(cname_list, s, "from ", " where");//came_list:"tname,tname" in exq5
				else
					get_middle(cname_list, s, "from ", ";");
				ind3 = pattern_match(cname_list, ",");
				if (ind3>0){//at most two table	
					temp_select_query.join_sign = 1;
					get_middle(select_table_name[0], cname_list, "\0", ",");
					for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if (!strcmp(table_head_p[table_head_ind].table_name, select_table_name[0])){
							res1 = 0;
							temp_select_query.use_table_no[0] = table_head_ind; break;
						}
					}
					if (res1){ printf("Table %s doesn’t exist\n", select_table_name[0]); continue; }
					
					get_middle(select_table_name[1], cname_list, ",", "\0");
					for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if (!strcmp(table_head_p[table_head_ind].table_name, select_table_name[1])){
							res2 = 0;
							temp_select_query.use_table_no[1] = table_head_ind; break;
						}
					}

				}
				else{
					strcpy(select_table_name[0], cname_list);
					for (table_head_ind = 0; table_head_ind < table_heads.length; table_head_ind++){
						if (!strcmp(table_head_p[table_head_ind].table_name, select_table_name[0])){
							res2 = 0;
							temp_select_query.use_table_no[0] = table_head_ind;
						}
					}
				}

				
				if (res2){ printf("Table %s doesn’t exist\n", select_table_name[1]); continue; }

				int ind10=0;
				int ind11=0;
				char condition_first_member[256]={0};
				char condition_second_member[256]={0};
				int link_sign=0;
				char cond_f_table_name[256]={0};
				char cond_f_col_name[256]={0};
				char cond_s_table_name[256]={0};
				char cond_s_col_name[256]={0}; 
				char char_const[256]={0};

				//condition				
				if (ind2>0){
					res1 = 1; res2 = 1;
					ind4 = pattern_match(s, "group by");//index of 'g'roup
					ind5 = pattern_match(s, ";");
					ind6 = pattern_match(s, "and");//first 'a'nd, ind6 = 57 in exq5
					if (ind4>0){//in exq6:ind2 = 46, ind4 = 61, len(condition_str = 9)
						get_middle(condition_str, s, "where ", " group by");//condition_str="cname='a'"in exq6

						//*******************************************
					}
					else
						get_middle(condition_str, s, "where ", ";");
					ind10 = pattern_match(condition_str,"and");
					if(ind10 > 0){
						get_middle(condition[0], condition_str, "\0", " and");
						ind11 = pattern_match(condition_str+ind10+4,"and");//c1 and {ind10+4}c2 and {ind11+4}c3
						if(ind11>0){
							get_middle(condition[1],condition_str+ind10+4,"\0"," and");
							get_middle(condition[2], condition_str+ind10+4,"and ","\0");
						}

						else
							get_middle(condition[1],condition_str,"and ","\0");//two condition
					} 
					else
						strcpy(condition[0],condition_str);//one condition

					for (i = 0; i <= 2; i++){
						memset(condition_first_member,0,sizeof(condition_first_member));
						memset(condition_second_member, 0 ,sizeof(condition_second_member));
						memset(cond_f_table_name,0,sizeof(cond_f_table_name));
						memset(cond_f_col_name,  0,sizeof(cond_f_col_name));
						memset(cond_s_table_name,0,sizeof(cond_s_table_name));
						memset(cond_s_col_name,  0,sizeof(cond_s_col_name));
						memset(char_const,0,sizeof(char_const));
						if (condition[i][0] != '\0'){
							indl = pattern_match(condition[i], "<");
							indle = pattern_match(condition[i], "<=");
							indg = pattern_match(condition[i], ">");
							indge = pattern_match(condition[i], ">=");
							inde = pattern_match(condition[i], "=");
							indne = pattern_match(condition[i], "!=");
							indlike = pattern_match(condition[i], "like");
							indnlike = pattern_match(condition[i], "not like");
							if ((indl<0) && (indg<0) && (inde<0) && (indlike<0)) { printf("Syntax error\n"); continue; }
							if (indle>0) { op[i] = int_le; strcpy(cop[i], "<="); }
							else if (indl>0) { op[i] = int_l; strcpy(cop[i], "<"); }
							else if (indge>0) { op[i] = int_ge; strcpy(cop[i], ">="); }
							else if (indg>0) { op[i] = int_g; strcpy(cop[i], ">"); }
							else if (indnlike>0) { op[i] = varchar_nlike; strcpy(cop[i], " not like"); }
							else if (indlike>0) { op[i] = varchar_like; strcpy(cop[i], " like"); }
							else if (indne>0) { op[i] = int_ne; strcpy(cop[i], "!="); }
							else { op[i] = int_e; strcpy(cop[i], "="); }

							//if(op[i] == int_e && temp_select_query.join_sign == 1 &&(pattern_match))
							get_middle(condition_first_member, condition[i], "\0" ,cop[i]);
							get_middle(condition_second_member,condition[i], cop[i],"\0");
							if(strchr(condition_first_member, '.')){//first member with "tname.cname"
								get_middle(cond_f_table_name, condition_first_member,"\0",".");
								get_middle(cond_f_col_name, condition_first_member,".","\0");
							}
							else{//first member with "cname"
								strcpy(cond_f_col_name, condition_first_member);
							}
							if(strchr(condition_second_member,'\'')||isdigit(condition_second_member[0])){
								link_sign = 0;
							}
							else link_sign = 1;
							if(link_sign){
								if(strchr(condition_second_member, '.')){//second member with "tname.cname"
									get_middle(cond_s_table_name, condition_second_member,"\0",".");
									get_middle(cond_s_col_name, condition_second_member,".","\0");
								}
								else{//second member with "cname"
									strcpy(cond_s_col_name, condition_second_member);
								}
							}
							else strcpy(char_const, condition_second_member);

							
							if(link_sign){
								res1 = 1; res2 = 1;
								//for member 1
								if(cond_f_table_name[0]!= '\0'){//tname.cname
									if (!strcmp(cond_f_table_name, select_table_name[0])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
												res1 = 0;
												temp_select_query.join_table_0_col_no = col_head_ind;
												e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
											}
										}
										if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); break; }
									}
									else if (!strcmp(cond_f_table_name, select_table_name[1])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
												res1 = 0;
												temp_select_query.join_table_1_col_no = col_head_ind;
												e_type[0] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
											}
										}
										if (res1){ printf("Column %s doesn’t exist\n",cond_f_col_name); break; }
									}
									else { printf("Syntax error\n"); break; }
								}
								else{//cname
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
											res1 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
											strcpy(cond_f_table_name, table_head_p[temp_select_query.use_table_no[0]].table_name);
											e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];		

										}
									}
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
											if (res1 == 0){ printf("Ambiguous column%s\n", cond_f_col_name); err = 1; break; }
											else{
												res1 = 0;
												temp_select_query.join_table_1_col_no = col_head_ind;
												strcpy(cond_f_table_name, table_head_p[temp_select_query.use_table_no[1]].table_name);
												e_type[0] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];		
											}
										}
									}
									if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); break; }
									if (err) break;
								}

								//for member 2
								if(cond_s_table_name[0]!='\0'){//tname.cname
									if (!strcmp(cond_s_table_name, select_table_name[0])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_s_col_name)){
												res2 = 0;
												temp_select_query.join_table_0_col_no = col_head_ind;
												e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
											}
										}
										if (res2){ printf("Column %s doesn’t exist\n", cond_s_col_name); break; }
									}
									else if (!strcmp(cond_s_table_name, select_table_name[1])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_s_col_name)){
												res2 = 0;
												temp_select_query.join_table_1_col_no = col_head_ind;
												e_type[1] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
											}
										}
										if (res2){ printf("Column %s doesn’t exist\n", cond_s_col_name); break; }
									}
									else { printf("Syntax error\n"); break; }
									if (!strcmp(cond_s_table_name, cond_f_table_name)){ printf("Syntax error\n"); break; }
									if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); break; }

								}
								else{//cname
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_s_col_name)){
											res2 = 0;
											temp_select_query.join_table_0_col_no = col_head_ind;
											strcpy(cond_s_table_name, table_head_p[temp_select_query.use_table_no[0]].table_name);
											e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];		
		

										}
									}
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_s_col_name)){
											if (res2 == 0){ printf("Ambiguous column%s\n", cond_s_col_name); err = 1; break; }
											else{
												res2 = 0;
												temp_select_query.join_table_1_col_no = col_head_ind;
												strcpy(cond_s_table_name, table_head_p[temp_select_query.use_table_no[1]].table_name);
												e_type[1] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];		
		

											}
										}
									}
									if (res2){ printf("Column %s doesn’t exist\n", cond_s_col_name); continue; }
									if (err) break;
									if (!strcmp(cond_f_table_name, cond_s_table_name)){ printf("Syntax error\n"); continue; }
									if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); continue; }
								}
							}
							else{// not link condition
								
								if(cond_f_table_name[0]!='\0'){//has table name
									if (!strcmp(cond_f_table_name, select_table_name[0])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
												res1 = 0;
												temp_select_query.filter_0_table_col_no = col_head_ind;
												e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
												if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
												else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
												else if (e_type[i] == varchar && (op[i]==int_e||op[i]==int_ne)) { op[i] = op[i] + 2; }
												temp_select_query.filter_0_sign = op[i];
												if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const + 1, strlen(char_const) - 2); }
												else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const); }
											}
										}
										if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); err = 1; continue; }
									}
									else if (!strcmp(cond_f_table_name, select_table_name[1])){
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
												res1 = 0;
												temp_select_query.filter_1_table_col_no = col_head_ind;
												e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
												if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
												else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
												else if (e_type[i] == varchar && (op[i]==int_e||op[i]==int_ne)) { op[i] = op[i] + 2; }
												temp_select_query.filter_1_sign = op[i];
												if (e_type[i] == varchar){ strncpy(temp_select_query.filter_1_const_char, char_const + 1, strlen(char_const) - 2); }
												else if (e_type[i] == int32){ temp_select_query.filter_1_const_int = atoi(char_const); }
											}
										}
										if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); err = 1; continue; }
									}
									else { printf("Syntax error\n"); err = 1; continue; }
									if (err){ continue; }					
								}
								else{//has no table name
									res1 = 1; res2 = 1;
									//first col of equal link
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
											res1 = 0;
											temp_select_query.filter_0_table_col_no = col_head_ind;
											strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[0]].table_name);
											e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
											if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
											else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
											else if (e_type[i] == varchar && (op[i]==int_e||op[i]==int_ne)) { op[i] = op[i] + 2; }
											temp_select_query.filter_0_sign = op[i];
											if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const + 1, strlen(char_const) - 2); }
											else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const); }
										}
									}
									if(temp_select_query.join_sign)
										for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
											if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
												if (res1 == 0){ printf("Ambiguous column%s\n", cond_f_col_name); err = 1; break; }
												else{
													res1 = 0;
													strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[1]].table_name);
													temp_select_query.filter_1_table_col_no = col_head_ind;
													e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
													if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
													else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
													else if (e_type[i] == varchar && (op[i]==int_e||op[i]==int_ne)) { op[i] = op[i] + 2; }
													temp_select_query.filter_1_sign = op[i];
													if (e_type[i] == varchar){ strncpy(temp_select_query.filter_1_const_char, char_const + 1, strlen(char_const) - 2); }
													else if (e_type[i] == int32){ temp_select_query.filter_1_const_int = atoi(char_const); }
												}
											}
										}
									if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); continue; }
									if (err) continue;
								}	
							}
						}							
					}//for 3 condition
				}//has where
				else{//no where
					temp_select_query.filter_0_sign = 0;
					temp_select_query.filter_1_sign = 0;
					temp_select_query.join_sign = 0;
				}
					
				//group by
				if (pattern_match(s, "group by") > 0){
					get_middle(group_name, s, "group by ", ";");

					if (strchr(group_name, '.')){//tname.cname
						//first col of equal link
						get_middle(group_table_name, group_name, "\0", ".");
						get_middle(group_col_name, group_name, ".", "\0");
						if (!strcmp(group_table_name, select_table_name[0])){
							temp_select_query.group_table_no = 0;
							for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], group_col_name)){
									res1 = 0;
									temp_select_query.group_table_col_no = col_head_ind;
								}
							}
							if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
						}
						else if (!strcmp(group_table_name, select_table_name[1])){
							temp_select_query.group_table_no = 1;
							for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], group_col_name)){
									res1 = 0;
									temp_select_query.group_table_col_no = col_head_ind;
								}
							}
							if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
						}
						else { printf("Syntax error\n"); continue; }
					}
					else{//cname
						res1 = 1; res2 = 1;
						strcpy(group_col_name, group_name);
						for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], group_col_name)){
								res1 = 0;
								temp_select_query.group_table_col_no = col_head_ind;
								temp_select_query.group_table_no = 0;
							}
						}
						if(temp_select_query.join_sign)
							for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], group_col_name)){
									if (res1 == 0){ printf("Ambiguous column%s\n", temp_col_name[0]); err = 1; break; }
									else{
										res1 = 0;
										temp_select_query.group_table_col_no = col_head_ind;
										temp_select_query.group_table_no = 1;
									}
								}
							}
						if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
						if (err) continue;
						//second col of equal link
					}

				}//group by
				else
				{//no group
					temp_select_query.group_table_no=2;
				}


				//select
				if (pattern_match(s, "select")<0){ printf("Syntax error\n"); continue; }
				// if (pattern_match(s, "count(*)")>0){ temp_select_query.total_select_no = 1; }	
				get_middle(select_str, s, "select ", "from");

				strcpy(sub_str, select_str);
				if(sub_str[strlen(sub_str)-1]==' ')
					sub_str[strlen(sub_str)-1]='\0';

				if(strncmp(s,"select*from",11)==0)
				{
					temp_select_query.select_table_all_col=1;
				}
				else
				{
					strcat(sub_str,",");
				}
				char * select_sub=sub_str;
				i = 0;
				while (strchr(select_sub, ',')){
					get_middle(select_name[i], select_sub, "\0", ",");
					select_sub = select_sub + pattern_match(select_sub,",") + 1 ;
					if (pattern_match(select_name[i], "(")>0){//aggr op
						get_middle(agg_op[i], select_name[i], "\0", "(");
						if (!strcmp(agg_op[i], "sum")) agg[i] = aggr_sum;
						else if (!strcmp(agg_op[i], "count")) agg[i] = aggr_count;
						else if (!strcmp(agg_op[i], "avg")) agg[i] = aggr_avg;
						else if (!strcmp(agg_op[i], "min")) agg[i] = aggr_min;
						else if (!strcmp(agg_op[i], "max")) agg[i] = aggr_max;

						get_middle(op_str, select_name[i], "(", ")");
						memset(op_table, '\0', sizeof(op_table));
						memset(op_col, '\0', sizeof(op_col));
						if (pattern_match(op_str, ".")>0){//op(a.col)
							get_middle(op_table, op_str, "\0", ".");
							get_middle(op_col, op_str, ".", "\0");
							if (!strcmp(op_table, select_table_name[0])){
								for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
									if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], op_col)){
										res1 = 0;
										temp_select_query.select_table[i] = 2 * agg[i];
										temp_select_query.select_table_col_no[i] = col_head_ind;
									}
								}
								if (res1){ printf("Column %s doesn’t exist\n", op_col); continue; }
							}
							else if (!strcmp(op_table, select_table_name[1])){
								for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
									if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], op_col)){
										res1 = 0;
										temp_select_query.select_table[i] = 2 * agg[i] + 1;
										temp_select_query.select_table_col_no[i] = col_head_ind;
									}
								}
								if (res1){ printf("Column %s doesn’t exist\n", op_col); continue; }
							}
							else { printf("Syntax error\n"); continue; }
						}
						else{//cname
							res1 = 1; res2 = 1;
							strcpy(op_col, op_str);
							if(strcmp("*",op_col)==0)
							{
								res1=0;res2=0;
								temp_select_query.select_table[i] = 2 * aggr_count;
								temp_select_query.select_table_col_no[i]=0;
							}
							for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], op_col)){
									res1 = 0;
									temp_select_query.select_table[i] = 2 * agg[i];
									temp_select_query.select_table_col_no[i] = col_head_ind;
								}
							}
							if(temp_select_query.join_sign)
								for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
									if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], op_col)){
										if (res1 == 0){ printf("Ambiguous column%s\n", op_col); err = 1; break; }
										else{
											res1 = 0;
											temp_select_query.select_table[i] = 2 * agg[i] + 1;
											temp_select_query.select_table_col_no[i] = col_head_ind;
										}
									}
								}
							if (res1){ printf("Column %s doesn’t exist\n", op_col); continue; }
							if (err) break;
						}
					}
					else{//select a
						strcpy(sl_str[i], select_name[i]);
						memset(se_table, '\0', sizeof(se_table));
						memset(se_col, '\0', sizeof(se_col));
						if (pattern_match(s, "group by") > 0){//has group by
							if (pattern_match(sl_str[i],".")>0){//a.col
								get_middle(se_table, sl_str[i], "\0", ".");
								get_middle(se_col, sl_str[i], ".", "\0");
								if (strcmp(se_table, table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].table_name)){
									printf("Non-group-by column %s in select list\n", se_col);
									err = 1; break;
								}
								if (strcmp(se_col, table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].col_name[temp_select_query.group_table_col_no])){
									printf("Non-group-by column %s in select list\n", se_col);
									err = 1; break;
								}
								// if (table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].e_type[temp_select_query.group_table_col_no] == varchar){
								// 	printf("Column %s is not int and can’t be used in aggregation\n", table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].col_name[temp_select_query.group_table_col_no]);
								// 	err = 1; break;
								// }
								temp_select_query.select_table[i] = temp_select_query.group_table_no;
								temp_select_query.select_table_col_no[i] = temp_select_query.group_table_col_no;
							}
							else{//cname
								strcpy(se_col, sl_str[i]);
								if (strcmp(se_col, table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].col_name[temp_select_query.group_table_col_no])){
									printf("Non-group-by column %s in select list\n", se_col);
									err = 1; break;
								}
								if (table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].e_type[temp_select_query.group_table_col_no] == varchar){
									printf("Column %s is not int and can’t be used in aggregation\n", table_head_p[temp_select_query.use_table_no[temp_select_query.group_table_no]].col_name[temp_select_query.group_table_col_no]);
									err = 1; break;
								}
								temp_select_query.select_table[i] = temp_select_query.group_table_no;
								temp_select_query.select_table_col_no[i] = temp_select_query.group_table_col_no;
							}
						}
						else{//has no group by
							if (pattern_match(sl_str[i],".")>0){//op(a.col)
								get_middle(se_table, sl_str[i], "\0", ".");
								get_middle(se_col, sl_str[i], ".", "\0");
								if (!strcmp(se_table, select_table_name[0])){
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], se_col)){
											res1 = 0;
											temp_select_query.select_table[i] = 0;
											temp_select_query.select_table_col_no[i] = col_head_ind;
										}
									}
									if (res1){ printf("Column %s doesn’t exist\n", se_col); err = 1; break; }
								}
								else if (!strcmp(op_table, select_table_name[1])){
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], se_col)){
											res1 = 0;
											temp_select_query.select_table[i] = 1;
											temp_select_query.select_table_col_no[i] = col_head_ind;
										}
									}
									if (res1){ printf("Column %s doesn’t exist\n", se_col); err = 1; break; }
								}
								else { printf("Syntax error\n"); err = 1; break; }
							}
							else{//cname
								res1 = 1; res2 = 1;
								strcpy(se_col, sl_str[i]);
								for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[0]].col_num; col_head_ind++){
									if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], se_col)){
										res1 = 0;
										temp_select_query.select_table[i] = 0;
										temp_select_query.select_table_col_no[i] = col_head_ind;
									}
								}
								if(temp_select_query.join_sign)
									for (col_head_ind = 0; col_head_ind < table_head_p[temp_select_query.use_table_no[1]].col_num; col_head_ind++){
										if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], se_col)){
											if (res1 == 0){ printf("Ambiguous column%s\n", se_col); err = 1; break; }
											else{
												res1 = 0;
												temp_select_query.select_table[i] = 1;
												temp_select_query.select_table_col_no[i] = col_head_ind;
											}
										}
									}
								if (res1){ printf("Column %s doesn’t exist\n", se_col); continue; }
								if (err) break;
							}

						}
					}
					i++;
				}//while
				if (err) break;

				temp_select_query.total_select_no = i;

				//call
				sol_query(my_select);
			}//no from
			else{ printf("Syntax error\n"); err = 1; }

		}//not sentence with key words
		else
			printf("Syntax error\n");

	}
}
//auto choose the first im2 after im1
char* get_middle(char* result, char*s, char*im1, char* im2){//at the end of im1,beign of im2
	int ind1, ind2;
	int shf1, shf2;

	ind1 = pattern_match(s, im1);
	ind2 = pattern_match(s, im2);
	shf1 = strlen(im1);
	shf2 = strlen(im2);

	
	if (im1[0] == '\0'){
		if(ind2>=0){
			strncpy(result, s, ind2);
			result[ind2] = '\0';
		}
		else
			result[0]='\0';
		return result;
	}
	else if (im2[0] == '\0'){
		if(ind1>=0)
			strcpy(result, s + ind1 + shf1);
		else
			result[0]='\0';
		return result;
	}
	else if(ind1<0|| ind2<0 )
	{
		result[0]='\0';
		return result;
	}
	else{
		while (ind2<ind1){
			s = s + ind2 + 1;
			ind1 = pattern_match(s, im1);
			ind2 = pattern_match(s, im2);
			if(ind2<0){
				result[0] = '\0';
				return result;
			}
			
		}
		strncpy(result, s + ind1 + shf1, ind2 - shf1 - ind1);
		result[ind2 - shf1 - ind1] = '\0';
		return result;

	}
}

//return the index of the pat[0] in s[ ], if not find , return -1
int pattern_match(char* s, char* pat){
	char * sub;
	sub = strstr(s, pat);
	if (sub)
		return sub - s;
	else
		return -1;
}

int get_shell(char * shell, FILE * sql){
	int i;
	int in_quort = 0;
	int cont_blank=0;
	int in_bracket=0;

	int enter = 0;

	char blank_buf[4096]={0};
	char *buf=blank_buf;

	char *get_res=NULL;
	char temp[4096]={0};

	char *buf_p=buf;
	while(get_res=fgets(temp,4096,sql))
	{
		if(temp[strlen(temp)-1]=='\n')
			temp[strlen(temp)-1]=' ';
		if(temp[0]=='-'&&temp[1]=='-')
			continue;
		
		// strcat(buf,temp);
		for(i=0;i<strlen(temp);i++){
			if(temp[i]=='\'')
			{
				if(in_quort==0)
					in_quort++;
				else
					in_quort--;
			}

			if(in_quort)
			{
				*buf_p++=temp[i];
			}
			else
			{
				if(temp[i]==';')
				{
					*buf_p++=temp[i];
					*buf_p='\0';
					break;
				}
				else if(temp[i]==' ')
				{
					if(cont_blank==0)
						*buf_p++=temp[i];
					cont_blank++;
				}
				else if(temp[i]=='(')
				{
					*buf_p++=temp[i];
					in_bracket++;
				}
				else if(temp[i]==')')
				{
					*buf_p++=temp[i];
					in_bracket--;
				}
				else
				{
					*buf_p++=temp[i];
					cont_blank=0;
				}
			}

			if(in_bracket<0)
			{
				printf("() mismatch!\n");
				return 0;
			}
		}
		*buf_p=0;
		if(buf_p[-1]==';')
			break;		
	}
	if(get_res==NULL)
		return 0;
	if(in_bracket)
	{
		printf("() mismatch\n");
		return 0;
	}
	if(in_quort)
	{
		printf("'' mismatch\n");
		return 0;
	}
	while(*buf==' ')
		buf++;

	in_quort=0;
	for (i = 0; i < strlen(buf); i++){
		buf[i] = tolower(buf[i]);
		
		if (buf[i] == '\''&& in_quort == 0)
			in_quort = 1;
		else if (buf[i] == '\''&& in_quort == 1)
			in_quort = 0;
		else if (buf[i] == ' ' && in_quort==0 && !(isalnum(buf[i-1]) && isalnum(buf[i+1])) )// only the blank space between 'word' and 'word' will be preseved	
			continue; 
		
		strncat(shell, buf+i,1);
	}

	return 1;
}
