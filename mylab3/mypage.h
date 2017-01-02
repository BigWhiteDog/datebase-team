#ifndef MYPAGE
#define MYPAGE
#include"list.h"
// typedef char one_page[4096];
typedef struct 
{
	int remain_size;
	SqList slot;
}page_header;


#endif