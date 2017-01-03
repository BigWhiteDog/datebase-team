#include"mycmp.h"
#include "mysql.h"
#include "list.h"

int (*filter_cmp_func[11])(void *,void*)={
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

uint32_t bkdr_hash(const char *key,int len)
{
	uint32_t hash = 0;
	for (uint32_t i = 0; i < len; ++i)
		hash = 131 * hash + key[i];
	return hash;
}

typedef struct link_node
{
	struct link_node * next;
	char * tuple_base[2];
	int32_t s;
	int32_t c;
}join_node;


char * in_tuple(table_head* t_head,char * tuple_base,int col_no,int *len)
{
	int total_var=0;
	int var_index=0;
	int int_index=0;
	short total_size = *(short *)tuple_base;
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
			*len=((short*)tuple_base)[0]-((short*)tuple_base)[total_var]
		
		return tuple_base+((short*)tuple_base)[1+var_index]
	}

}

/*typedef struct
{
	char table_name[128];
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;
*/


int sol_select_query()
{
	table_head *t_head[2];
	t_head[0] = (table_head *)(table_heads.elem)+temp_select_query.use_table[0];
	t_head[1] = (table_head *)(table_heads.elem)+temp_select_query.use_table[1];
	
	FILE *fp[2];
	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",t_head[0]->table_name);
	fp[0]=fopen(name_buffer,"rb");
	if(fp[0]==NULL)
		return 0;
	fseek(fp[0],0,SEEK_SET);
	if(temp_select_query.join_sign)
	{
		sprintf(name_buffer,"./db/%s.tbl",t_head[1]->table_name);
		fp[1]=fopen(name_buffer,"rb");
		if(fp[1]==NULL)
			return 0;
		fseek(fp[1],0,SEEK_SET);
	}
	
	SqList tab0_filter;
	SqList tab1_filter;
	ListInit(&tab0_filter,sizeof(char*));
	ListInit(&tab1_filter,sizeof(char*));
	char temp_page[4096];
	page_header * p_head=(page_header *) temp_page;
	
	while(!feof(fp[0]))
	{
		fread(temp_page,4096,1,fp[0]);
		short * slot_base=temp_page+4096-p_head->slot.listsize*2;
		int i;
		char *tuple_base;
		short tuple_size;
		int len;
		for(i=0;i<p_head->slot.length;i++)
		{
			tuple_base=temp_page+slot_base[i];
			tuple_size=*(short *)tuple_base;
			if(temp_select_query.filter_0_sign)
			{
				char *val_to_cmp;
				val_to_cmp=in_tuple(t_head[0],tuple_base,temp_select_query.filter_0_table_col_no,&len);
				if(temp_select_query.filter_0_sign>=7)
				{
					char* temp=malloc(len+1);
					memset(temp,0,len+1);
					memcpy(temp,val_to_cmp,len);
					if((filter_cmp_func[temp_select_query.filter_0_sign])(temp,temp_select_query.filter_0_const_char)==0)					
					{	
						free(temp);
						continue;
					}
					free(temp);
				}
				else
				{
					if((filter_cmp_func[temp_select_query.filter_0_sign])(val_to_cmp,&temp_select_query.filter_0_const_int)==0)
						continue;
				}
			}
			char * temp_tuple=malloc(tuple_size);
			memcpy(temp_tuple,tuple_base,tuple_size);
			ListInsert(&tab0_filter,&temp_tuple);
		}
	}
	if(temp_select_query.join_sign)
		while(!feof(fp[1]))
		{
			fread(temp_page,4096,1,fp[1]);
			short * slot_base=temp_page+4096-p_head->slot.listsize*2;
			int i;
			char *tuple_base;
			short tuple_size;
			int len;
			for(i=0;i<p_head->slot.length;i++)
			{
				tuple_base=temp_page+slot_base[i];
				tuple_size=*(short *)tuple_base;
				if(temp_select_query.filter_1_sign)
				{
					char *val_to_cmp;
					val_to_cmp=in_tuple(t_head[1],tuple_base,temp_select_query.filter_1_table_col_no,&len);
					if(temp_select_query.filter_1_sign>=7)
					{
						char* temp=malloc(len+1);
						memset(temp,0,len+1);
						memcpy(temp,val_to_cmp,len);
						if((filter_cmp_func[temp_select_query.filter_1_sign])(temp,temp_select_query.filter_1_const_char)==0)					
						{	
							free(temp);
							continue;
						}
						free(temp);
					}
					else
					{
						if((filter_cmp_func[temp_select_query.filter_1_sign])(val_to_cmp,&temp_select_query.filter_1_const_int)==0)
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

		ListInsert
	}
	else
	{
		int i;
		for(i=0;i<tab1_filter.length;i++)
		{
			join_node temp_join;
			temp_join.next=NULL;
			temp_join.tuple_base[0]=((char**)tab0_filter.elem)[i];
			temp_join.tuple_base[1]=NULL;
			temp_join.s=0;
			temp_join.c=0;
		}
		
	}

	return 1;
}