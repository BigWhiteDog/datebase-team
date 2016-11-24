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
	outcsv=fopen("train2.csv","w");
	if(outcsv==NULL)
		return 1;
		
		csv=fopen("train1.csv","r");
		if(csv==NULL)
			return 1;
		while(fgets(mybuffer,512,csv))
		{
			int comma=0;
			ti=mybuffer;
			to=myready;
			while(*ti!='\n')
			{
				if(*ti==','){
					comma++;
					if(ti[1]==',')
						*to++='0';
				}
				if (comma!=5){
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
			*to++='\n';
			*to++='\0';
			fputs(myready,outcsv);
		}
		fclose(csv);
	
	fclose(outcsv);
	return 0;
}
