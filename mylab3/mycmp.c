#include "mycmp.h"
int int_l_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1<a2)?1:0;

}
int int_le_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1<=a2)?1:0;
}
int int_g_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1>a2)?1:0;
}
int int_ge_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1>=a2)?1:0;
}
int int_e_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1==a2)?1:0;
}
int int_ne_cmp(void * p1,void * p2,int len1,int len2)
{
	int a1=*(int *)p1;
	int a2=*(int *)p2;
	return (a1!=a2)?1:0;
}
int varchar_e_cmp(void * p1,void * p2,int len1,int len2)
{
	char temp1[len1+1];
	char temp2[len2+1];
	memcpy(temp1,p1,len1);
	memcpy(temp2,p2,len2);
	temp1[len1]='\0';
	temp2[len2]='\0';

	return (strcmp(temp1,temp2)==0)?1:0;
}
int varchar_ne_cmp(void * p1,void * p2,int len1,int len2)
{
	char temp1[len1+1];
	char temp2[len2+1];
	memcpy(temp1,p1,len1);
	memcpy(temp2,p2,len2);
	temp1[len1]='\0';
	temp2[len2]='\0';
	
	return (strcmp(temp1,temp2)==0)?0:1;
}
int varchar_like_cmp(void * p1,void * p2,int len1,int len2)
{
	char temp1[len1+1];
	char temp2[len2+1];
	memcpy(temp1,p1,len1);
	memcpy(temp2,p2,len2);
	temp1[len1]='\0';
	temp2[len2]='\0';
	if(strstr(temp1,temp2)==NULL)
		return 0;
	else
		return 1;
}
int varchar_nlike_cmp(void * p1,void * p2,int len1,int len2)
{
	char temp1[len1+1];
	char temp2[len2+1];
	memcpy(temp1,p1,len1);
	memcpy(temp2,p2,len2);
	temp1[len1]='\0';
	temp2[len2]='\0';

	if(strstr(temp1,temp2)==NULL)
		return 1;
	else
		return 0;
}