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
	strcpy(temp_create_query.col_name[1],"grade");
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
// select * from tab1, tab2 where tab1.id = tab2.id and grade <= 30;

	temp_select_query.select_table_all_col=1;
	// temp_select_query.total_select_no=1;
	// temp_select_query.select_table[0]=0;
	// temp_select_query.select_table_col_no[0]=1;
	
	temp_select_query.use_table_no[0]=0;
	temp_select_query.use_table_no[1]=1;

	temp_select_query.join_sign=int_e;
	temp_select_query.join_table_0_col_no=0;
	temp_select_query.join_table_1_col_no=0;

	temp_select_query.filter_1_sign=int_le;
	temp_select_query.filter_1_table_col_no=1;
	temp_select_query.filter_1_const_int=30;
	// strcpy(temp_select_query.filter_1_const_char,"first");

	temp_select_query.group_table_no=2;
	sol_query(my_select);

	memset(&temp_select_query,0,sizeof(select_query));

	
	//test drop
	
	temp_drop_query.table_no=0;
	sol_query(my_drop);

	temp_drop_query.table_no=0;
	sol_query(my_drop);