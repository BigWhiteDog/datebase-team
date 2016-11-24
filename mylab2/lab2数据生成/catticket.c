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
	char *ti,*to,*t1,*t2;
	outcsv=fopen("ticket.csv","w");
	if(outcsv==NULL)
		return 1;
	for(i=1;i<=30;i++)
	{
		
		csv=fopen("train.csv","r");
		if(csv==NULL)
			continue;
		while(fgets(mybuffer,512,csv))
		{
			int comma=0;
			ti=mybuffer;
			to=myready;
			while(*ti!='\n')
			{
				if(*ti==','){
					comma++;
					if(comma==2)
						break;
				}
				if (comma<=1){
					switch(*ti){
						case ' ':
							ti++;break;
						case '/':
							*to++=',';
							ti++;break;
						case '-':
							*to++='0';
							ti++;break;
						default:
							*to++=*ti++;break;
					}
				}
			}
			sprintf(to,",2016-11-%02d",i);
			while(*to!='\0')
				to++;
			int j;
			for(j=0;j<7;j++){
				*to++=',';
				*to++='5';
			}
			*to++='\n';
			*to++='\0';
			fputs(myready,outcsv);
		}
		fclose(csv);
	}
	fclose(outcsv);
	return 0;
}
