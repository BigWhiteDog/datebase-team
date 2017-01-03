#include <string.h>
#ifndef MY_CMP
#define MY_CMP

//return 1 if satisfied
int int_l_cmp(void * p1,void * p2);
int int_le_cmp(void * p1,void * p2);
int int_g_cmp(void * p1,void * p2);
int int_ge_cmp(void * p1,void * p2);
int int_e_cmp(void * p1,void * p2);
int int_ne_cmp(void * p1,void * p2);
int varchar_e_cmp(void * p1,void * p2);
int varchar_ne_cmp(void * p1,void * p2);
int varchar_like_cmp(void * p1,void * p2);
int varchar_nlike_cmp(void * p1,void * p2);

#endif