#include"mycmp.h"
#include "mysql.h"

int (*filter_cmp_func[11])(void *,void*,int,int)={
	NULL,
	&int_l_cmp,
    &int_le_cmp,
    &int_g_cmp,
    &int_ge_cmp,
    &int_e_cmp,
    &int_ne_cmp,
    &varchar_e_cmp,
    &varchar_ne_cmp,
    &varchar_like_cmp,
    &varchar_nlike_cmp
};


char * in_tuple(table_head* t_head,char * tuple_base,int col_no,int *len)
{
	int total_var=0;
	int var_index=0;
	int int_index=0;
	int i;
	for(i=0;i<t_head->col_num;i++){
		if(t_head->e_type[i]){
			if(i<col_no)
				var_index++;
			total_var++;
		}
		else{
			if(i<col_no)
				int_index++;
		}
	}
	if(t_head->e_type[col_no]==0)//int
	{
		*len=4;
		return tuple_base+2+2*total_var+int_index*4;
	}
	else
	{
		if(var_index<total_var-1)
			*len=((short*)tuple_base)[1+var_index+1]-((short*)tuple_base)[1+var_index];
		else
			*len=((short*)tuple_base)[0]-((short*)tuple_base)[total_var];
		
		return tuple_base+((short*)tuple_base)[1+var_index];
	}
}



//for hash -----------------------------------------------------------------------
#define HASH_SIZE 1024

typedef struct join_link_node
{
	struct join_link_node* next;
	char *tuple_base;
}hash_node;

typedef struct group_link_node
{
	struct group_link_node * next;
	char * tuple_base[2];
	struct group_link_node * brother;
}join_node;

uint32_t bkdr_hash(const char *key,int len)
{
	uint32_t hash = 0;
	for (uint32_t i = 0; i < len; ++i)
		hash = 131 * hash + key[i];
	return (hash & 0x7FFFFFFF)%HASH_SIZE;
}
void * init_hash(int node_size)
{
	return calloc(HASH_SIZE,node_size);
}
void insert_join_hash(hash_node * hash_tab ,int index ,char* tuple_base)
{
	hash_node *link_start=hash_tab[index].next;
	if(hash_tab[index].tuple_base==NULL)
	{
		hash_tab[index].tuple_base=tuple_base;
		hash_tab[index].next=NULL;
	}
	else
	{
		hash_tab[index].next=calloc(1,sizeof(hash_node));
		(hash_tab[index].next)->next=link_start;
		(hash_tab[index].next)->tuple_base=tuple_base;
	}
}
void free_join_hash(hash_node * hash_tab)
{
	int i;
	for(i=0;i<HASH_SIZE;i++)
	{
		hash_node *link_start=hash_tab[i].next;
		while(link_start!=NULL)
		{
			void *temp=link_start;
			link_start=link_start->next;
			free(temp);
		}
	}
	free(hash_tab);
}
void insert_group_hash(const join_node ** group_hash_tab,join_node * insert_node, const table_head* t_head)
{
	int group_filter_sign;
	group_filter_sign = (t_head->e_type[temp_select_query.group_table_col_no])? varchar_e : int_e;
	
	char* insert_tuple_base=insert_node->tuple_base[temp_select_query.group_table_no];
	
	int insert_key_len;
	char* insert_key_base;
	
	insert_key_base=in_tuple(t_head,insert_tuple_base,temp_select_query.group_table_col_no,&insert_key_len);

	int hash_key_index;
	hash_key_index=bkdr_hash(insert_key_base,insert_key_len);

	join_node * link_start = group_hash_tab[hash_key_index];

	if(link_start==NULL)//new bucket
	{
		group_hash_tab[hash_key_index]=insert_node;
	}
	else
	{
		join_node *link_pre = link_start;

		while(link_start!=NULL)
		{
			link_pre= link_start;

			char * cmp_tuple_base=link_start->tuple_base[temp_select_query.group_table_no];
			int cmp_key_len;
			char * cmp_key_base;

			cmp_key_base = in_tuple(t_head,cmp_tuple_base,temp_select_query.group_table_col_no,&cmp_key_len);

			//if find brother
			if((filter_cmp_func[group_filter_sign])(insert_key_base,cmp_key_base,insert_key_len,cmp_key_len))
			{
				insert_node->brother = link_start->brother;
				link_start->brother = insert_node;
				break;
			}
			link_start=link_start->next;
		}
		if(link_start==NULL)//no brother,need new node in bucket
		{
			link_pre->next=insert_node;
		}
	}
}

//end hash -----------------------------------------------------------------------


//for aggr ----------------------------------------------------------------------

void aggr_func_nosign(join_node * big_bro, table_head* t_head,int s_table,int s_col_no)
{
	char *x;
	int len;
	x=in_tuple(t_head,big_bro->tuple_base[s_table],s_col_no,&len);
	if(t_head->e_type[s_col_no])//varchar
	{
		char temp[len+1];
		memcpy(temp,x,len);
		temp[len]='\0';
		printf("%s", temp);
	}
	else
	{
		printf("%d",*(int32_t *)x);
	}
}

void aggr_func_sum(join_node * big_bro, table_head* t_head,int s_table,int s_col_no)
{
	int32_t s=0;
	while(big_bro!= NULL)
	{
		int32_t *x;
		int len;
		x=(int32_t *)in_tuple(t_head,big_bro->tuple_base[s_table],s_col_no,&len);
		
		s+=*x;

		big_bro=big_bro->brother;
	}
	printf("%d", s);
}
void aggr_func_count(join_node * big_bro, table_head* t_head, int s_table,int s_col_no)
{
	int32_t c=0;
	while(big_bro!= NULL)
	{	
		c++;
		big_bro=big_bro->brother;
	}
	printf("%d", c);
}
void aggr_func_avg(join_node * big_bro, table_head* t_head, int s_table,int s_col_no)
{
	int32_t s=0;
	int32_t c=0;
	while(big_bro!= NULL)
	{
		int32_t *x;
		int len;
		x=(int32_t *)in_tuple(t_head,big_bro->tuple_base[s_table],s_col_no,&len);
		
		s+=*x;
		c++;

		big_bro=big_bro->brother;
	}
	printf("%d", s/c);
}
void aggr_func_min(join_node * big_bro, table_head* t_head, int s_table,int s_col_no)
{
	int32_t m=0x7FFFFFFF;
	while(big_bro!=NULL)
	{
		int32_t *x;
		int len;
		x=(int32_t *)in_tuple(t_head,big_bro->tuple_base[s_table],s_col_no,&len);
		if(*x<m)
			m=*x;
		big_bro=big_bro->next;
	}
	printf("%d", m);
}
void aggr_func_max(join_node * big_bro, table_head* t_head, int s_table,int s_col_no)
{
	int32_t m=0x80000000;
	while(big_bro!=NULL)
	{
		int32_t *x;
		int len;
		x=(int32_t *)in_tuple(t_head,big_bro->tuple_base[s_table],s_col_no,&len);
		if(*x>m)
			m=*x;
		big_bro=big_bro->next;
	}
	printf("%d", m);
}

void (*aggr_funcs[6])(join_node *,table_head*,int,int)={
	&aggr_func_nosign,
	&aggr_func_sum,
	&aggr_func_count,
	&aggr_func_avg,
	&aggr_func_min,
	&aggr_func_max
};

char * aggr_str[6]={
	"",
	"sum",
	"count",
	"avg",
	"min",
	"max"
};


int sol_select_query()
{
	if(temp_select_query.select_table_all_col)
	{
		int i,j;
		int total_col;
		total_col=table_head_p[temp_select_query.use_table_no[0]].col_num;
		temp_select_query.total_select_no=total_col;
		for(i=0;i<total_col;i++)
		{
			temp_select_query.select_table[i]=0;
			temp_select_query.select_table_col_no[i]=i;
		}
		if(temp_select_query.join_sign)
		{
			total_col=table_head_p[temp_select_query.use_table_no[1]].col_num;
			temp_select_query.total_select_no+=total_col;
			for(j=0;j<total_col;j++)
			{
				temp_select_query.select_table[i]=1;
				temp_select_query.select_table_col_no[i]=j;
				i++;
			}
		}
	}

	table_head *t_head[2];
	t_head[0] = table_head_p+temp_select_query.use_table_no[0];
	t_head[1] = table_head_p+temp_select_query.use_table_no[1];
	
	FILE *fp[2];
	int fp_end[2];
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",t_head[0]->table_name);
	fp[0]=fopen(name_buffer,"rb");
	if(fp[0]==NULL)
		return 0;

	fseek(fp[0],0,SEEK_END);
	fp_end[0]=ftell(fp[0]);
	fseek(fp[0],0,SEEK_SET);
	
	if(temp_select_query.join_sign)
	{
		sprintf(name_buffer,"./db/%s.tbl",t_head[1]->table_name);
		fp[1]=fopen(name_buffer,"rb");
		if(fp[1]==NULL)
			return 0;
	
		fseek(fp[1],0,SEEK_END);
		fp_end[1]=ftell(fp[1]);
		fseek(fp[1],0,SEEK_SET);
	}
	
	SqList tab0_filter;
	SqList tab1_filter;
	ListInit(&tab0_filter,sizeof(char*));
	if(temp_select_query.join_sign)
	{
		ListInit(&tab1_filter,sizeof(char*));
	}
	char temp_page[4096];
	page_header * p_head=(page_header *) temp_page;
	

	while(fp_end[0]!=ftell(fp[0]))
	{
		fread(temp_page,4096,1,fp[0]);
		short * slot_base=temp_page+4096-p_head->slot.listsize*2;
		int i;
		char *tuple_base;
		short tuple_size;
		int len;
		int const_len;
		const_len=(temp_select_query.filter_0_sign>=varchar_e)?strlen(temp_select_query.filter_0_const_char):4;
		for(i=0;i<p_head->slot.length;i++)
		{
			tuple_base=temp_page+slot_base[i];
			tuple_size=*(short *)tuple_base;
			if(temp_select_query.filter_0_sign)
			{
				char *val_to_cmp;
				val_to_cmp=in_tuple(t_head[0],tuple_base,temp_select_query.filter_0_table_col_no,&len);

				if(temp_select_query.filter_0_sign>=varchar_e)
				{
					if((filter_cmp_func[temp_select_query.filter_0_sign])(val_to_cmp,temp_select_query.filter_0_const_char,len,const_len)==0)	
						continue;
				}
				else
				{
					if((filter_cmp_func[temp_select_query.filter_0_sign])(val_to_cmp,&temp_select_query.filter_0_const_int,len,const_len)==0)
						continue;
				}
			}
			char * temp_tuple=malloc(tuple_size);
			memcpy(temp_tuple,tuple_base,tuple_size);
			ListInsert(&tab0_filter,&temp_tuple);
		}
	}
	if(temp_select_query.join_sign)
		while(fp_end[1]!=ftell(fp[1]))
		{
			fread(temp_page,4096,1,fp[1]);
			short * slot_base=temp_page+4096-p_head->slot.listsize*2;
			int i;
			char *tuple_base;
			short tuple_size;
			int len;
			int const_len;
			const_len=(temp_select_query.filter_1_sign>=7)?strlen(temp_select_query.filter_1_const_char):4;
			for(i=0;i<p_head->slot.length;i++)
			{
				tuple_base=temp_page+slot_base[i];
				tuple_size=*(short *)tuple_base;
				if(temp_select_query.filter_1_sign)
				{
					char *val_to_cmp;
					val_to_cmp=in_tuple(t_head[1],tuple_base,temp_select_query.filter_1_table_col_no,&len);
					if(temp_select_query.filter_1_sign>=varchar_e)
					{
						if((filter_cmp_func[temp_select_query.filter_1_sign])(val_to_cmp,temp_select_query.filter_1_const_char,len,const_len)==0)
							continue;
					}
					else
					{
						if((filter_cmp_func[temp_select_query.filter_1_sign])(val_to_cmp,&temp_select_query.filter_1_const_int,len,const_len)==0)
							continue;
					}
				}
				char * temp_tuple=malloc(tuple_size);
				memcpy(temp_tuple,tuple_base,tuple_size);
				ListInsert(&tab1_filter,&temp_tuple);
			}
		}
	SqList tab_join;
	ListInit(&tab_join,sizeof(join_node));
	
	if(temp_select_query.join_sign)
	{
		//create hash
		hash_node *hash_tab=init_hash(sizeof(hash_node));
		int i;
		for(i=0;i<tab0_filter.length;i++)
		{
			int len;
			char *tuple_0_base=((char**)tab0_filter.elem)[i];
			char * cmp_0_key=in_tuple(t_head[0],tuple_0_base,temp_select_query.join_table_0_col_no,&len);
			uint32_t hash_key_index=bkdr_hash(cmp_0_key,len);
		
			insert_join_hash(hash_tab,hash_key_index,((char**)tab0_filter.elem)[i]);
		}
		//use hash
		for(i=0;i<tab1_filter.length;i++)
		{
			int len0,len1;
			char *tuple_1_base=((char**)tab1_filter.elem)[i];
			char * cmp_1_key=in_tuple(t_head[1],tuple_1_base,temp_select_query.join_table_1_col_no,&len1);
			uint32_t hash_key_index=bkdr_hash(cmp_1_key,len1);
			hash_node * res=hash_tab+hash_key_index;
			if(res->tuple_base==NULL)//nothing in this bucket
				continue;
			while(res!=NULL)
			{
				char *tuple_0_base=res->tuple_base;
				char * cmp_0_key=in_tuple(t_head[0],tuple_0_base,temp_select_query.join_table_0_col_no,&len0);
				if(filter_cmp_func[temp_select_query.join_sign](cmp_0_key,cmp_1_key,len0,len1))
				{
					join_node temp_join;
					temp_join.next=NULL;
					temp_join.tuple_base[0]=tuple_0_base;
					temp_join.tuple_base[1]=tuple_1_base;
					temp_join.brother=NULL;
					ListInsert(&tab_join,&temp_join);
				}
				res=res->next;
			}
		}
		free_join_hash(hash_tab);
	}
	else
	{
		int i;
		for(i=0;i<tab0_filter.length;i++)
		{
			join_node temp_join;
			temp_join.next=NULL;
			temp_join.tuple_base[0]=((char**)tab0_filter.elem)[i];
			temp_join.tuple_base[1]=NULL;
			temp_join.brother=NULL;
			ListInsert(&tab_join,&temp_join);
		}
	}
	
	//output the header
	int t_select_no=temp_select_query.total_select_no;
	
	int k;
	int kcount=0;
	for(k=0;k<t_select_no;k++)
	{
		int aggr_mode=temp_select_query.select_table[k]>>1;
		int s_tab=temp_select_query.select_table[k]&1;
		int s_col_no=temp_select_query.select_table_col_no[k];
		
		if(aggr_mode==0)
			kcount+=printf("%s",t_head[s_tab]->col_name[s_col_no]);
		else if(aggr_mode==aggr_count)
			kcount+=printf("count");
		else
			kcount+=printf("%s(%s)",aggr_str[aggr_mode],t_head[s_tab]->col_name[s_col_no]);	
		if(k!=t_select_no-1)
			putchar('|');
		kcount++;
	}
	putchar('\n');
	for(k=0;k<kcount;k++)
		putchar('-');
	putchar('\n');


	join_node * join_node_base = (join_node *)tab_join.elem;
	if(temp_select_query.group_table_no!=2)//need group
	{
		join_node ** group_hash_tab=init_hash(sizeof(join_node*));
		// join_node * group_hash_tab[HASH_SIZE]={NULL};
		int i;
		for(i=0;i<tab_join.length;i++)
			insert_group_hash(group_hash_tab,join_node_base+i,t_head[temp_select_query.group_table_no]);

		//output
		int j;
		for(i=0;i<HASH_SIZE;i++)
		{
			join_node * link_start=group_hash_tab[i];
			while(link_start!=NULL)
			{
				for(j=0;j<t_select_no;j++){
					int aggr_mode=temp_select_query.select_table[j]>>1;
					int s_tab=temp_select_query.select_table[j]&1;
					int s_col_no=temp_select_query.select_table_col_no[j];
					aggr_funcs[aggr_mode]( link_start ,t_head[s_tab],s_tab,s_col_no);
				
					if(j!=t_select_no-1)
						putchar('|');
				}
				putchar('\n');
				link_start=link_start->next;
			}
		}
		free(group_hash_tab);
	}
	else //normal output
	{
		int i,j;
		for(i=0;i<tab_join.length;i++)
		{
			for(j=0;j<t_select_no;j++){
				int s_tab=temp_select_query.select_table[j]&1;
				int s_col_no=temp_select_query.select_table_col_no[j];
				aggr_funcs[0]( join_node_base+i ,t_head[s_tab],s_tab,s_col_no);
				
				if(j!=t_select_no-1)
					putchar('|');
			}
			putchar('\n');
		}

	}
	int i;
	for(i=0;i<tab0_filter.length;i++)
		free(((char**)tab0_filter.elem)[i]);
	free(tab0_filter.elem);
	fclose(fp[0]);

	if(temp_select_query.join_sign){
		for(i=0;i<tab1_filter.length;i++)
			free(((char**)tab1_filter.elem)[i]);
		free(tab1_filter.elem);
		fclose(fp[1]);
	}
	free(tab_join.elem);
	// printf("select finish!\n");
	return 1;
}