#include"list.h"
#include"mysql.h"
/*typedef struct
{
	int col_num;
	char col_name[MAX_COL_NUM][128];
	elem_type e_type[MAX_COL_NUM];
}table_head;

//insert query
typedef struct
{
	int table_no;
	char varchar[MAX_COL_NUM][4096];
	int int32[MAX_COL_NUM];
}insert_query;

typedef struct 
{
	int remain_size;
	SqList slot;
}page_header;
*/

// extern SqList table_heads;



int sol_insert_query()
{
	table_head *t_head = (table_head *)(table_heads.elem)+temp_insert_query.table_no;
	char temp_page[4096];
	memset(temp_page,0,sizeof(temp_page));
	int16_t t_size=2;
	int i;
	int int_num=0;
	int varchar_num=0;
	for(i=0;i<t_head->col_num;i++)
	{
		if(t_head->e_type[i])//varchar
		{	
			t_size+=2+strlen(temp_insert_query.varchar[i]);
			varchar_num++;
		}
		else
		{
			t_size+=4;
			int_num++;
		}
	}
	char * temp_alloc=calloc(t_size,1);
	if(temp_alloc==NULL)
		return 0;
	int var_index=0;
	int int_index=0;
	int16_t last_var=2+2*varchar_num+4*int_num;

	((int16_t *)temp_alloc)[0]=t_size;

	for(i=0;i<t_head->col_num;i++)
	{
		if(t_head->e_type[i])//varchar
		{
			((int16_t *)temp_alloc)[var_index+1]=last_var;
			memcpy(temp_alloc+last_var,temp_insert_query.varchar[i],strlen(temp_insert_query.varchar[i]));
			last_var+=strlen(temp_insert_query.varchar[i]);
			var_index++;
		}
		else
		{
			memcpy(temp_alloc+2+2*varchar_num+4*int_index,&temp_insert_query.int32[i],4);
			int_index++;
		}
	}


	char name_buffer[256];
	sprintf(name_buffer,"./db/%s.tbl",t_head->table_name);
	FILE *fp;
	int fp_end;
	fp=fopen(name_buffer,"rb+");
	fseek(fp,0,SEEK_SET);
	if(fp==NULL)
		return 0;

	fseek(fp,0,SEEK_END);
	fp_end=ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	int16_t next_tuple=sizeof(page_header);
	page_header * p_head=(page_header *) temp_page;
	while(fp_end!=ftell(fp))
	{
		fread(temp_page,4096,1,fp);
		if(p_head->slot.listsize==p_head->slot.length)//need new space when need realloc list
		{
			if(p_head->remain_size>=t_size+LISTINCREMENT*2)
			{
				fseek(fp,-4096,SEEK_CUR);
				break;
			}
		}
		else if(p_head->remain_size>=t_size)
		{
			fseek(fp,-4096,SEEK_CUR);
			break;
		}
		
	}
	if(fp_end==ftell(fp))
	{
		memset(temp_page,0,4096);
		page_header new_p_header;
		ListInit(&new_p_header.slot,2);
		memcpy(temp_page,&new_p_header,sizeof(page_header));
		p_head->remain_size=4096-sizeof(page_header)-LIST_INIT_SIZE*2;
	}
	else
	{
		p_head->slot.elem=malloc(p_head->slot.listsize*2);
		if(p_head->slot.elem==NULL)
			return 0;
		memcpy(p_head->slot.elem , temp_page+4096-p_head->slot.listsize*2 , p_head->slot.listsize*2);
		next_tuple=((int16_t *)(p_head->slot.elem))[p_head->slot.length-1];
		next_tuple+=((int16_t *)(temp_page+next_tuple))[0];

	}
	if(p_head->slot.listsize==p_head->slot.length)
		p_head->remain_size-=LISTINCREMENT*2;
	p_head->remain_size-=t_size;
	
	ListInsert(&p_head->slot,&next_tuple);
	memcpy(temp_page+next_tuple,temp_alloc,t_size);
	memcpy(temp_page+4096-p_head->slot.listsize*2,p_head->slot.elem,p_head->slot.listsize*2);
	fwrite(temp_page,4096,1,fp);

	free(p_head->slot.elem);
	free(temp_alloc);
	fclose(fp);
	return t_size;
}