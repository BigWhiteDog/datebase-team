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

	//test select
	memset(&temp_select_query,0,sizeof(select_query));
// select * from crtDemo;
	temp_select_query.select_table_all_col=1;
	temp_select_query.total_select_no=0;
	temp_select_query.use_table_no[0]=0;
	temp_select_query.join_sign=0;
	temp_select_query.filter_0_sign=0;
	temp_select_query.filter_1_sign=0;
	temp_select_query.group_table_no=2;
	sol_query(my_select);

	memset(&temp_select_query,0,sizeof(select_query));


// select id from crtDemo;
	temp_select_query.select_table_all_col=0;
	temp_select_query.total_select_no=1;
	temp_select_query.select_table[0]=0;
	temp_select_query.select_table_col_no[0]=0;
	
	temp_select_query.use_table_no[0]=0;
	temp_select_query.join_sign=0;
	temp_select_query.filter_0_sign=0;
	temp_select_query.filter_1_sign=0;
	temp_select_query.group_table_no=2;
	sol_query(my_select);

	memset(&temp_select_query,0,sizeof(select_query));

// select id from crtDemo where id = 1;
	temp_select_query.select_table_all_col=0;
	temp_select_query.total_select_no=1;
	temp_select_query.select_table[0]=0;
	temp_select_query.select_table_col_no[0]=0;
	
	temp_select_query.use_table_no[0]=0;
	temp_select_query.join_sign=0;
	temp_select_query.filter_0_sign=int_e;
	temp_select_query.filter_0_const_int=1;
	
	temp_select_query.filter_1_sign=0;
	temp_select_query.group_table_no=2;
	sol_query(my_select);

	memset(&temp_select_query,0,sizeof(select_query));


// select name from crtDemo where name like 'first';
	temp_select_query.select_table_all_col=0;
	temp_select_query.total_select_no=1;
	temp_select_query.select_table[0]=0;
	temp_select_query.select_table_col_no[0]=1;
	
	temp_select_query.use_table_no[0]=0;
	temp_select_query.join_sign=0;
	temp_select_query.filter_0_sign=varchar_like;
	temp_select_query.filter_0_table_col_no=1;
	strcpy(temp_select_query.filter_0_const_char,"first");
	
	temp_select_query.filter_1_sign=0;
	temp_select_query.group_table_no=2;
	sol_query(my_select);

	memset(&temp_select_query,0,sizeof(select_query));

	
	//test drop
	
	temp_drop_query.table_no=0;
	sol_query(my_drop);
	printf("%d %d %d \n", table_heads.elem_size,table_heads.length,table_heads.listsize);
	
