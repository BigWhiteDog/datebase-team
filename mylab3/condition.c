/***********************************************************************************************************************/
	//int ind10 ind11;
	//char condition_first_member[128];
	//char condition_second_member[128];
	//int link_sign;
	//char cond_f_table_name[128];
	//char cond_f_col_name[128];
	//char cond_s_table_name[128];
	//char cond_s_col_name[128]; 
	//char char_const[128];


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
				get_middle(condition[2], condition_str+in10+4,"and ","\0");
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
				if ((indl>0) && (indg>0) && (inde>0) && (indlike>0)) { printf("Syntax error\n"); continue; }
				if (indle>0) { op[i] = int_le; strcpy(cop[i], "<="); }
				else if (indl>0) { op[i] = int_l; strcpy(cop[i], "<"); }
				else if (indge>0) { op[i] = int_ge; strcpy(cop[i], ">="); }
				else if (indg>0) { op[i] = int_g; strcpy(cop[i], ">"); }
				else if (indnlike>0) { op[i] = varchar_nlike; strcpy(cop[i], " not like "); }
				else if (indlike>0) { op[i] = varchar_like; strcpy(cop[i], " like "); }
				else if (indne>0) { op[i] = int_ne; strcpy(cop[i], "!="); }
				else { op[i] = int_e; strcpy(cop[i], "!="); }

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
					else{//first member with "cname"
						strcpy(cond_s_col_name, condition_second_member);
					}
				}
				else strcpy(char_const, condition_second_member);

				
				if(link_sign){
					res1 = 1; res2 = 1;
					//for member 1
					if(cond_f_table_name[0]!= '\0'){//tname.cname
						if (!strcmp(cond_f_table_name, select_table_name[0])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
									res1 = 0;
									temp_select_query.join_table_0_col_no = col_head_ind;
									e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
								}
							}
							if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); continue; }
						}
						else if (!strcmp(cond_f_table_name, select_table_name[1])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
									res1 = 0;
									temp_select_query.join_table_1_col_no = col_head_ind;
									e_type[0] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
								}
							}
							if (res1){ printf("Column %s doesn’t exist\n",cond_f_col_name); continue; }
						}
						else { printf("Syntax error\n"); continue; }
					}
					else{//cname
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
								res1 = 0;
								temp_select_query.join_table_0_col_no = col_head_ind;
								strcpy(cond_f_table_name, table_head_p[temp_select_query.use_table_no[0]].table_name);
								e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];		

							}
						}
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
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
						if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); continue; }
						if (err) continue;
					}

					//for member 2
					if(cond_s_table_name[0]!='\0'){//tname.cname
						if (!strcmp(cond_s_table_name, select_table_name[0])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_s_col_name)){
									res2 = 0;
									temp_select_query.join_table_0_col_no = col_head_ind;
									e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
								}
							}
							if (res2){ printf("Column %s doesn’t exist\n", cond_s_col_name); continue; }
						}
						else if (!strcmp(cond_s_table_name, select_table_name[1])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_s_col_name)){
									res2 = 0;
									temp_select_query.join_table_1_col_no = col_head_ind;
									e_type[1] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
								}
							}
							if (res2){ printf("Column %s doesn’t exist\n", cond_s_col_name); continue; }
						}
						else { printf("Syntax error\n"); continue; }
						if (!strcmp(cond_s_table_name, cond_f_table_name)){ printf("Syntax error\n"); continue; }
						if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); continue; }

					}
					else{//cname
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_s_col_name)){
								res2 = 0;
								temp_select_query.join_table_0_col_no = col_head_ind;
								strcpy(cond_s_table_name, table_head_p[temp_select_query.use_table_no[0]].table_name);
								e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];		


							}
						}
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
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
						if (err) continue;
						if (!strcmp(cond_f_table_name, cond_s_table_name)){ printf("Syntax error\n"); continue; }
						if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); continue; }
					}
				}
				else{// not link condition
					
					if(cond_f_table_name[0]!='\0'){//has table name
						if (!strcmp(cond_f_table_name, select_table_name[0])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
									res1 = 0;
									temp_select_query.filter_0_table_col_no = col_head_ind;
									e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
									if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
									temp_select_query.filter_0_sign = op[i];
									if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const + 1, strlen(char_const) - 2); }
									else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const); }
								}
							}
							if (res1){ printf("Column %s doesn’t exist\n", cond_f_col_name); err = 1; continue; }
						}
						else if (!strcmp(cond_f_table_name, select_table_name[1])){
							for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
								if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
									res1 = 0;
									temp_select_query.filter_1_table_col_no = col_head_ind;
									e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
									if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
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
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], cond_f_col_name)){
								res1 = 0;
								temp_select_query.filter_0_table_col_no = col_head_ind;
								strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[0]].table_name);
								e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
								if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
								else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
								else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
								temp_select_query.filter_0_sign = op[i];
								if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const + 1, strlen(char_const) - 2); }
								else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const); }
							}
						}
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], cond_f_col_name)){
								if (res1 == 0){ printf("Ambiguous column%s\n", cond_f_col_name); err = 1; break; }
								else{
									res1 = 0;
									strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[1]].table_name);
									temp_select_query.filter_1_table_col_no = col_head_ind;
									e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
									if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", cond_f_col_name, cop[i], char_const); break; err = 1; }
									else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
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




//old

if (temp_select_query.join_sign){//link condition
			if (ind6>0){//has and
				get_middle(condition[2], condition_str, "\0", " and");
				get_middle(sub_str, condition_str, "and ", "\0");
				if (pattern_match(sub_str, "and")>0){//3 conditions in total
					get_middle(condition[0], sub_str, "\0", " and");
					get_middle(condition[1], sub_str, "and ", "\0");
				}
				else{//only 2 conditions
					strcpy(condition[0], sub_str);
				}
			}
			else{//only one condition
				strcpy(condition[2], condition_str);
				temp_select_query.filter_0_sign = filter_nosign;
				temp_select_query.filter_1_sign = filter_nosign;
			}

			//condition[2] :link condition
			if (strchr(condition[2], '.')){//tname.cname
				//ind7 = pattern_match(condition[2],".");
				ind8 = pattern_match(condition[2], "=");
				if (ind8<0) { printf("Syntax error\n"); continue; }
				//first col of equal link
				get_middle(temp_table_name[0], condition[2], "\0", ".");
				get_middle(temp_col_name[0], condition[2], ".", "=");
				if (!strcmp(temp_table_name[0], select_table_name[0])){
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[0])){
							res1 = 0;
							temp_select_query.join_table_0_col_no = col_head_ind;
							e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
						}
					}
					if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
				}
				else if (!strcmp(temp_table_name[0], select_table_name[1])){
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[0])){
							res1 = 0;
							temp_select_query.join_table_1_col_no = col_head_ind;
							e_type[0] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
						}
					}
					if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
				}
				else { printf("Syntax error\n"); continue; }
				ind9 = pattern_match(condition[2] + ind8 + 1, ".");
				if (ind9<0) { printf("Syntax error\n"); continue; }
				//second col of equal link
				get_middle(temp_table_name[1], condition[2] + ind8 - 1, "=", ".");
				get_middle(temp_col_name[1], condition[2] + ind8 - 1, ".", "\0");
				if (!strcmp(temp_table_name[1], select_table_name[0])){
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[1])){
							res2 = 0;
							temp_select_query.join_table_0_col_no = col_head_ind;
							e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
						}
					}
					if (res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue; }
				}
				else if (!strcmp(temp_table_name[1], select_table_name[1])){
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[1])){
							res2 = 0;
							temp_select_query.join_table_1_col_no = col_head_ind;
							e_type[1] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
						}
					}
					if (res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue; }
				}
				else { printf("Syntax error\n"); continue; }
				if (!strcmp(temp_table_name[1], temp_table_name[0])){ printf("Syntax error\n"); continue; }
				if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); continue; }
			}
			else{//cname
				res1 = 1; res2 = 1;
				//first col of equal link
				get_middle(temp_col_name[0], condition[2], "\0", "=");
				for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
					if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[0])){
						res1 = 0;
						temp_select_query.join_table_0_col_no = col_head_ind;
						strcpy(temp_table_name[0], table_head_p[temp_select_query.use_table_no[0]].table_name);
						e_type[0] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];

					}
				}
				for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
					if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[0])){
						if (res1 == 0){ printf("Ambiguous column%s\n", temp_col_name[0]); err = 1; break; }
						else{
							res1 = 0;
							temp_select_query.join_table_1_col_no = col_head_ind;
							strcpy(temp_table_name[0], table_head_p[temp_select_query.use_table_no[1]].table_name);
							e_type[0] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];


						}
					}
				}
				if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[0]); continue; }
				if (err) continue;
				//second col of equal link
				get_middle(temp_col_name[1], condition[2], "=", "\0");
				for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
					if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[1])){
						res2 = 0;
						temp_select_query.join_table_0_col_no = col_head_ind;
						strcpy(temp_table_name[1], table_head_p[temp_select_query.use_table_no[0]].table_name);
						e_type[1] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];


					}
				}
				for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
					if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[1])){
						if (res2 == 0){ printf("Ambiguous column%s\n", temp_col_name[1]); err = 1; break; }
						else{
							res2 = 0;
							temp_select_query.join_table_1_col_no = col_head_ind;
							strcpy(temp_table_name[1], table_head_p[temp_select_query.use_table_no[1]].table_name);
							e_type[1] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];


						}
					}
				}
				if (res2){ printf("Column %s doesn’t exist\n", temp_col_name[1]); continue; }
				if (err) continue;
				if (!strcmp(temp_table_name[0], temp_table_name[1])){ printf("Syntax error\n"); continue; }
				if (e_type[0] != e_type[1]){ printf("Value and column type mismatch\n"); continue; }
			}
		}
		else{//no link condition					
			memset(temp_col_name, '\0', sizeof(temp_col_name));
			memset(temp_table_name, '\0', sizeof(temp_table_name));
			memset(el_type, '\0', sizeof(el_type));
			if (ind6>0){//has and
				get_middle(condition[0], condition_str, "\0", " and");
				get_middle(condition[1], condition_str, "and ", "\0");
			}
			else{//only one condition
				strcpy(condition[0], condition_str);
			}
		}
		//due with condition[0] and condition[1]
		for (i = 0; i <= 1; i++){
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
				else if (indnlike>0) { op[i] = varchar_nlike; strcpy(cop[i], " not like "); }
				else if (indlike>0) { op[i] = varchar_like; strcpy(cop[i], " like "); }
				else if (indne>0) { op[i] = int_ne; strcpy(cop[i], "!="); }
				else { op[i] = int_e; strcpy(cop[i], "!="); }


				if (strchr(condition[i], '.')){//tname.cname
					get_middle(temp_table_name[i], condition[i], "\0", ".");
					get_middle(temp_col_name[i], condition[i], ".", cop[i]);
					get_middle(char_const[i], condition[i], cop[i], "\0");
					if (!strcmp(temp_table_name[i], select_table_name[0])){
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[i])){
								res1 = 0;
								temp_select_query.filter_0_table_col_no = col_head_ind;
								e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
								if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
								temp_select_query.filter_0_sign = op[i];
								if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const[i] + 1, strlen(char_const[i]) - 2); }
								else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const[i]); }
							}
						}
						if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[i]); err = 1; continue; }
					}
					else if (!strcmp(temp_table_name[i], select_table_name[1])){
						for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
							if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[i])){
								res1 = 0;
								temp_select_query.filter_1_table_col_no = col_head_ind;
								e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
								if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
								temp_select_query.filter_1_sign = op[i];
								if (e_type[i] == varchar){ strncpy(temp_select_query.filter_1_const_char, char_const[i] + 1, strlen(char_const[i]) - 2); }
								else if (e_type[i] == int32){ temp_select_query.filter_1_const_int = atoi(char_const[i]); }
							}
						}
						if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[i]); err = 1; continue; }
					}
					else { printf("Syntax error\n"); err = 1; continue; }
					if (err){ continue; }
				}
				else{//cname
					res1 = 1; res2 = 1;
					//first col of equal link
					get_middle(temp_col_name[i], condition[i], "\0", cop[i]);
					get_middle(char_const[i], condition[i], cop[i], "\0");
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[0]].col_name[col_head_ind], temp_col_name[i])){
							res1 = 0;
							temp_select_query.filter_0_table_col_no = col_head_ind;
							strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[0]].table_name);
							e_type[i] = table_head_p[temp_select_query.use_table_no[0]].e_type[col_head_ind];
							if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
							else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
							else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
							temp_select_query.filter_0_sign = op[i];
							if (e_type[i] == varchar){ strncpy(temp_select_query.filter_0_const_char, char_const[i] + 1, strlen(char_const[i]) - 2); }
							else if (e_type[i] == int32){ temp_select_query.filter_0_const_int = atoi(char_const[i]); }
						}
					}
					for (col_head_ind = 0; col_head_ind < MAX_COL_NUM; col_head_ind++){
						if (!strcmp(table_head_p[temp_select_query.use_table_no[1]].col_name[col_head_ind], temp_col_name[i])){
							if (res1 == 0){ printf("Ambiguous column%s\n", temp_col_name[i]); err = 1; break; }
							else{
								res1 = 0;
								strcpy(temp_table_name[i], table_head_p[temp_select_query.use_table_no[1]].table_name);
								temp_select_query.filter_1_table_col_no = col_head_ind;
								e_type[i] = table_head_p[temp_select_query.use_table_no[1]].e_type[col_head_ind];
								if (e_type[i] == varchar && ((indle>0) || (indl>0) || (indge>0) || (indg>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == int32 && ((indlike>0) || (indnlike>0))) { printf("Predicate %s%s%s error\n", temp_col_name[i], cop[i], char_const[i]); break; err = 1; }
								else if (e_type[i] == varchar) { op[i] = op[i] + 2; }
								temp_select_query.filter_1_sign = op[i];
								if (e_type[i] == varchar){ strncpy(temp_select_query.filter_1_const_char, char_const[i] + 1, strlen(char_const[i]) - 2); }
								else if (e_type[i] == int32){ temp_select_query.filter_1_const_int = atoi(char_const[i]); }
							}
						}
					}
					if (res1){ printf("Column %s doesn’t exist\n", temp_col_name[i]); continue; }
					if (err) continue;
				}
			}
		}
		if (!strcmp(temp_table_name[0], temp_table_name[1])){ printf("Syntax error\n"); err = 1; }
	}//has where
	else{//no where
		temp_select_query.filter_0_sign = 0;
		temp_select_query.filter_1_sign = 0;
		temp_select_query.join_sign = 0;
	}
