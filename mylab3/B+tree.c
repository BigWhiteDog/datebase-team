#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#define ORDER 16
#define KEYSIZE 16
#define VALUESIZE 16
#define TESTN 10000000
static inline void clflush(volatile void *__p)
{
//	asm volatile("clflush (%0)" :: "r" (__p));
}
#define my_mfence() asm volatile("mfence":::"memory")
int nodespace=0;
int valspace=0;
typedef char KeyType[KEYSIZE];
typedef char ValueType[VALUESIZE];
int trandom[TESTN*2];
typedef struct BTnode{
	int keynum;//how many keys the node have
	struct BTnode *parent;//to parent
	KeyType key_array[ORDER+1];//record the key, +1 for division
	struct BTnode * son_array[ORDER+1];//record the pointers to sons, +1 for division
	struct BTnode * next;//the next node
}BTnode;
typedef struct Result{//search result
	BTnode *ptr;//to the node found
	int i;//the the_index in the key array
	int tag;//1 for success, 0 for failure
}Result;
typedef struct Build_Result{
	BTnode * BTroot;
	BTnode * BTleaflist;
}Build_Result;
ValueType * Valalloc();
BTnode * BTalloc();
Result Search_BTnode(BTnode *p,KeyType key);
Result Search_BTree(BTnode *BTroot,KeyType key);
void insertBTnode_change1st(BTnode *p,KeyType new_key);
int insertBTnode_leaf(BTnode *p,int the_index,KeyType new_key,ValueType new_val);
void insertBTnode_mid(BTnode *p,int the_index,BTnode *new_son);
int insertBTnode_division(BTnode * p,BTnode **ptr_to_root);
int insertBTnode(KeyType key,ValueType val,BTnode **ptr_to_root);
void BTnode_change1st(BTnode *p,KeyType old_key,KeyType new_key);
void deleteBTnode_leaf(BTnode *p,int the_index);
void deleteBTnode_mid(BTnode *p,int the_index);
void deleteBTnode_merger(BTnode *p,BTnode **ptr_to_root);
int deleteBTnode(BTnode **ptr_to_root,KeyType key);
void free_tree(BTnode* BTroot);
Build_Result BuildBTree(KeyType key_a[],ValueType val_a[],int n);
ValueType * Valalloc(){//get space for data
	return (ValueType *)calloc(1,sizeof(ValueType));
}
BTnode * BTalloc(){//get space for one node
	return (BTnode *)calloc(1,sizeof(BTnode));
}
Result Search_BTnode(BTnode *p,KeyType key){
	Result result;//initial the result
	result.ptr=p;
	result.i=0;
	result.tag=0;
	int i;
	int cmp_result;//to record the result of compare
	if(p==NULL)
		return result;
//start search
	for(i=0;i<p->keynum;i++){
		if((cmp_result=strcmp(key,p->key_array[i]))==0){//find
			result.i=i;
			result.tag=1;//success
			return result;
		}
		if(cmp_result<0)//will not find
			break;
	}
//fail,return the place it should be
	result.i=i;//key_array[i-1]<key<key_array[i]
	return result;
}

Result Search_BTree(BTnode *BTroot,KeyType key){
	BTnode *p=BTroot;//initialize p
	Result result;
	while(p->next==NULL){//mid node's next is NULL
		result=Search_BTnode(p,key);
		if(result.tag==0&&result.i!=0)//go to the previous one(except the first)
			result.i--;
		p=p->son_array[result.i];//move to the son node
	}
	result=Search_BTnode(p,key);
	return result;
}
int insertBTnode_leaf(BTnode *p,int the_index,KeyType new_key,ValueType new_val){//need change
//insert that
	if(the_index==0)
		BTnode_change1st(p,p->key_array[0],new_key);
	ValueType * dup;
	dup=Valalloc();
	valspace++;
	if(dup==NULL){
		printf("malloc fail!\n");
		return 0;
	}
	int j;
	for(j=p->keynum-1;j>=the_index;j--){//everyone behind the_index should move to the next space
		memcpy(p->key_array[j+1],p->key_array[j],KEYSIZE);
		clflush(p->key_array[j+1]);
		p->son_array[j+1]=p->son_array[j];
	}
	//insert the new one
	memcpy(p->key_array[the_index],new_key,KEYSIZE);
	clflush(p->key_array[the_index]);
	p->son_array[the_index]=(BTnode*)dup;
	memcpy(p->son_array[the_index],new_val,VALUESIZE);
	clflush(p->son_array[the_index]);
	p->keynum++;
	return 1;
}
void insertBTnode_mid(BTnode *p,int the_index,BTnode *new_son){
//insert that
	if(the_index==0)
		BTnode_change1st(p,p->key_array[0],new_son->key_array[0]);
	int j;
	for(j=p->keynum-1;j>=the_index;j--){//everyone behind the_index should move to the next space
		memcpy(p->key_array[j+1],p->key_array[j],KEYSIZE);
		clflush(p->key_array[j+1]);
		p->son_array[j+1]=p->son_array[j];
	}
	memcpy(p->key_array[the_index],new_son->key_array[0],KEYSIZE);//insert the new one
	clflush(p->key_array[the_index]);
	p->son_array[the_index]=new_son;
	p->keynum++;
	new_son->parent=p;
}
int insertBTnode_division(BTnode * p,BTnode **ptr_to_root){
	if(p==NULL)//beyond the root
		return 1;
	if(p->keynum<=ORDER)//need not to divide
		return 1;
//need to divide(keynum==ORDER+1)
	int i,j;
	BTnode * div=NULL,*temp_parent=NULL;
	div=BTalloc();
	nodespace++;
	if(div==NULL){
		printf("malloc fail!\n");
		return 0;
	}
	if(p->parent==NULL){
		temp_parent=BTalloc();
		nodespace++;
		if(temp_parent==NULL){
			printf("malloc fail!\n");
			return 0;
		}
		memset(temp_parent,0,sizeof(BTnode));//clear RAM
	}
	for(i=(ORDER+1)/2,j=0;i<=ORDER;i++,j++){
		memcpy(div->key_array[j],p->key_array[i],KEYSIZE);
		clflush(div->key_array[j]);
		memset(p->key_array[i],0,KEYSIZE);//clear RAM
		div->son_array[j]=p->son_array[i];
		p->son_array[i]=NULL;
		if(p->next==NULL)
			 div->son_array[j]->parent=div;//change parent
	}
	if(p->next!=NULL){
		//put into the list
		div->next=p->next;
		p->next=div;	
	}
	//reset keynum
	p->keynum=(ORDER+1)/2;
	div->keynum=j;
//check have parent or not
	if(p->parent==NULL){
		p->parent=temp_parent;
		div->parent=temp_parent;
		memcpy(temp_parent->key_array[0],p->key_array[0],KEYSIZE);
		clflush(temp_parent->key_array[0]);
		memcpy(temp_parent->key_array[1],div->key_array[0],KEYSIZE);
		clflush(temp_parent->key_array[1]);
		temp_parent->son_array[0]=p;
		temp_parent->son_array[1]=div;
		temp_parent->keynum=2;
		*ptr_to_root=temp_parent;
	}
	else{
		Result result;
		result=Search_BTnode(p->parent,div->key_array[0]);//go to parent and find the place
		insertBTnode_mid(result.ptr,result.i,div);//insert into parent
	}
	return insertBTnode_division(div->parent,ptr_to_root);//recursion
}
int insertBTnode(KeyType key,ValueType val,BTnode **ptr_to_root){
	Result result;
	result=Search_BTree(*ptr_to_root,key);
	if(result.tag==1){
		printf("error,there have been a same key.\n");
		return 0;//fail to insert
	}
//insert
	insertBTnode_leaf(result.ptr,result.i,key,val);
//need divide?
	if(insertBTnode_division(result.ptr,ptr_to_root)==0)//this function still need to check
		return 0;
//success
	return 1;
}
void BTnode_change1st(BTnode *p,KeyType old_key,KeyType new_key){
//change the 1st key of p's all parent
	if(p==NULL)
		return ;
	Result temp_result;
	while((p=p->parent)!=NULL){
		temp_result=Search_BTnode(p,old_key);//search his parent
		if(temp_result.tag==0)
			break;
		memcpy(p->key_array[temp_result.i],new_key,KEYSIZE);//replace it
		clflush(p->key_array[temp_result.i]);
	}
}
void deleteBTnode_leaf(BTnode *p,int the_index){
	if(the_index==0&&p->keynum>1)
		BTnode_change1st(p,p->key_array[0],p->key_array[1]);
	int i;
	free((ValueType*)p->son_array[the_index]);
	for(i=the_index;i<p->keynum-1;i++){
		memcpy(p->key_array[i],p->key_array[i+1],KEYSIZE);
		clflush(p->key_array[i]);
		p->son_array[i]=p->son_array[i+1];
	}
	memset(p->key_array[i],0,KEYSIZE);
	p->son_array[i]=NULL;
	p->keynum--;
}
void deleteBTnode_mid(BTnode *p,int the_index){
	if(the_index==0&&p->keynum>1)
		BTnode_change1st(p,p->key_array[0],p->key_array[1]);
	int i;
	for(i=the_index;i<p->keynum-1;i++){
		memcpy(p->key_array[i],p->key_array[i+1],KEYSIZE);
		clflush(p->key_array[i]);
		p->son_array[i]=p->son_array[i+1];
	}
	memset(p->key_array[i],0,KEYSIZE);
	p->son_array[i]=NULL;
	p->keynum--;
}
void deleteBTnode_merger(BTnode *p,BTnode **ptr_to_root){
	if(p->keynum>=((ORDER+1)/2))
		return ;
	if(p==*ptr_to_root){
		if(p->keynum==1){
			free(*ptr_to_root);
			*ptr_to_root=p->son_array[0];
		}
		return ;
	}
	BTnode * temp;
	Result result;
	result=Search_BTnode(p->parent,p->key_array[0]);
	int i;
	if((temp=p->parent->son_array[result.i+1])!=NULL&&temp->keynum>((ORDER+1)/2)){//borrow next
		if(p->next!=NULL){
			insertBTnode_leaf(p,p->keynum,temp->key_array[0],*(ValueType*)temp->son_array[0]);
			deleteBTnode_leaf(temp,0);//delete and free	
		}else{
			insertBTnode_mid(p,p->keynum,temp->son_array[0]);
			deleteBTnode_mid(temp,0);//delete and free
		}
	}else if(result.i!=0&&(temp=p->parent->son_array[result.i-1])!=NULL&&temp->keynum>((ORDER+1)/2)){//borrow pre
		if(p->next!=NULL){
			insertBTnode_leaf(p,0,temp->key_array[temp->keynum-1],*(ValueType*)temp->son_array[temp->keynum-1]);
			deleteBTnode_leaf(temp,temp->keynum-1);//delete and free
		}else{
			insertBTnode_mid(p,0,temp->son_array[temp->keynum-1]);
			deleteBTnode_mid(temp,temp->keynum-1);//delete and free
		}
	}else if((temp=p->parent->son_array[result.i+1])!=NULL){//merger next
		for(i=0;i<temp->keynum;i++){
			memcpy(p->key_array[p->keynum+i],temp->key_array[i],KEYSIZE);
			clflush(p->key_array[p->keynum+i]);
			if(p->next==NULL)
				temp->son_array[i]->parent=p;//reset the parent
			p->son_array[p->keynum+i]=temp->son_array[i];
			temp->son_array[i]=NULL;
			
		}
		p->keynum=p->keynum+temp->keynum;
		p->next=temp->next;//mid node next is null,so it doesn't matter
		deleteBTnode_mid(result.ptr,result.i+1);
		free(temp);
	}else if(result.i!=0&&(temp=p->parent->son_array[result.i-1])!=NULL){//merger pre
		for(i=0;i<p->keynum;i++){
			memcpy(temp->key_array[temp->keynum+i],p->key_array[i],KEYSIZE);
			clflush(temp->key_array[temp->keynum+i]);
			if(p->next==NULL)
				p->son_array[i]->parent=temp;//reset the parent
			temp->son_array[temp->keynum+i]=p->son_array[i];
			p->son_array[i]=NULL;
		}
		temp->keynum=p->keynum+temp->keynum;	
		temp->next=p->next;//mid node next is null,so it doesn't matter
		deleteBTnode_mid(result.ptr,result.i);
		free(p);
	}

	deleteBTnode_merger(result.ptr,ptr_to_root);
}
int deleteBTnode(BTnode **ptr_to_root,KeyType key){
	Result result;
	result=Search_BTree(*ptr_to_root,key);
	if(result.tag==0){
		printf("%s does not exist\n",key);
		return 0;
	}
//delete in leaf
	deleteBTnode_leaf(result.ptr,result.i);
//need merger?
	deleteBTnode_merger(result.ptr,ptr_to_root);
	return 1;
}
void free_tree(BTnode* BTroot){
	if(BTroot==NULL)
		return ;
	int i=0;
	if(BTroot->next!=NULL)
		for(i=0;i<BTroot->keynum;i++)
			free((ValueType*)BTroot->son_array[i]);
	else
		for(i=0;i<BTroot->keynum;i++)
			free_tree(BTroot->son_array[i]);
	free(BTroot);
}
Build_Result new_test_build_tree(void){
	Build_Result res;
	KeyType keybuffer;
	ValueType valbuffer;
	res.BTleaflist=NULL;
	res.BTroot=NULL;
	int i;
	
	struct timeval tpstart,tpend;
	float timeuse;
	gettimeofday(&tpstart,NULL);

	BTnode * BTroot=BTalloc();
	nodespace++;
	BTroot->next=BTroot;//build a circle
	for(i=0;i<TESTN;i++){
		sprintf(keybuffer,"%d",trandom[i]);
		sprintf(valbuffer,"%d",trandom[i]);
		insertBTnode(keybuffer,valbuffer,&BTroot);
	}
	BTnode * temp=BTroot;
	while(temp->next==NULL)//get to the first one
		temp=temp->son_array[0];
	res.BTleaflist=temp;
	res.BTroot=BTroot;

	gettimeofday(&tpend,NULL);
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
	tpend.tv_usec-tpstart.tv_usec;
	timeuse/=1000000;
	printf("Build Used Time:%f\n",timeuse);

	return res;
}
void new_test_search_tree(BTnode *BTroot,int left,int right){
	Result res;
    KeyType keybuffer;
	int i;
    struct timeval tpstart,tpend;
	float timeuse;
	gettimeofday(&tpstart,NULL);

    for(i=left;i<=right;i++){
    	sprintf(keybuffer,"%d",trandom[i]);
		res=Search_BTree(BTroot,keybuffer);
		if(res.tag==0)
			printf("%s is wrong!\n",keybuffer);
    }

    gettimeofday(&tpend,NULL);
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
	tpend.tv_usec-tpstart.tv_usec;
	timeuse/=1000000;
	printf("search Used Time:%f\n",timeuse);
}
void new_test_insert_tree(BTnode ** ptr_to_root){
	int i; 
	KeyType keybuffer;
	ValueType valbuffer;

	struct timeval tpstart,tpend;
	float timeuse;
	gettimeofday(&tpstart,NULL);

	for(i=TESTN;i<TESTN*2;i++){
		sprintf(keybuffer,"%d",trandom[i]);
		sprintf(valbuffer,"%d",trandom[i]);
		insertBTnode(keybuffer,valbuffer,ptr_to_root);
	}

	gettimeofday(&tpend,NULL);
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
	tpend.tv_usec-tpstart.tv_usec;
	timeuse/=1000000;
	printf("Insert Used Time:%f\n",timeuse);
}
void new_test_delete_tree(BTnode** ptr_to_root){
	int i;
	KeyType keybuffer;
	struct timeval tpstart,tpend;
	float timeuse;
	gettimeofday(&tpstart,NULL);

	for(i=TESTN/2;i<TESTN+TESTN/2;i++){
		sprintf(keybuffer,"%d",trandom[i]);
		deleteBTnode(ptr_to_root,keybuffer);
	}

	gettimeofday(&tpend,NULL);
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
	tpend.tv_usec-tpstart.tv_usec;
	timeuse/=1000000;
	printf("Delete Used Time:%f\n",timeuse);
}
void new_test_tree(){
	Build_Result res;
	res=new_test_build_tree();
	new_test_insert_tree(&res.BTroot);
	new_test_search_tree(res.BTroot,TESTN/2,TESTN+TESTN/2-1);
	new_test_delete_tree(&res.BTroot);
	free_tree(res.BTroot);

}
int main(){
	int ti,tn,ttmp;
	for(ti=0;ti<TESTN*2;ti++)
		trandom[ti]=ti;
	srand(time(0));
	for(ti=0;ti<TESTN*2;ti++){
		tn=rand()%(TESTN*2);
		ttmp=trandom[ti];
		trandom[ti]=trandom[tn];
		trandom[tn]=ttmp;
	}
	new_test_tree();
	printf("%d MB was used\n",(sizeof(BTnode)*nodespace+sizeof(ValueType)*valspace)>>20);
	return 0;
}
