#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char train[10];
char csvfn[10];
char mybuffer[512];
char myready[512];
int main()
{
	int i;
	FILE *csv;
	FILE *outcsv;
	outcsv=fopen("my-stations.csv","w");
	if(outcsv==NULL)
		return 1;
	csv=fopen("all-stations.txt","r");
	if(csv==NULL)
		return 1;
	while(fgets(mybuffer,512,csv))
	{
		int comma=0;
		char *ti=mybuffer;
		char *to=myready;
		while(*ti!='\n')
		{
			if(*ti==','){
				if(comma++==0)//skip station id
					ti++;
			}
			if(comma==0)//skip station id
				ti++;
			else{
				switch(*ti){
					case ' ':
						ti++;break;
					default:
						*to++=*ti++;break;
				}
			}
		}
		*to++='\n';
		*to++='\0';
		fputs(myready,outcsv);
	}
	fclose(csv);
	
	fclose(outcsv);
	return 0;
}
