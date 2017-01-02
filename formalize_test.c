#include<stdio.h>
#include<string.h>



int formalize(char *sql){
    int i = 0, j = 0;
    char temp_sql[128];
    int quote_mark = 0;
    
    while(sql[i] == ' ') i++;
          
    while(sql[i] != ';'){
        if(quote_mark == 0){                        // out of ''
            if(sql[i] >= 'A' && sql[i] <= 'Z')      // tolower
                temp_sql[j] = sql[i] - 'A' + 'a';
            else if(sql[i] == ' '){
                temp_sql[j] = ' ';
                while(sql[++i] == ' ');
                i--;
            }else if(sql[i] == 39){                 // ascii(')
                temp_sql[j] = 39;
                quote_mark = quote_mark ? 0 : 1;
            }else
                temp_sql[j] = sql[i];
        }else{
            quote_mark = sql[i] == 39 ? 0 : 1;
            temp_sql[j] = sql[i];
        }
        
        i++;
        j++;
    }
    
    temp_sql[j] = ';';
    temp_sql[j+1] = '\0';
    
    i = 0;
    j = 0;
    quote_mark = 0;
    while(temp_sql[j] != ';'){
        if(quote_mark == 0){
            if((temp_sql[j] == ',' || temp_sql[j] == '(') && sql[i-1] == ' '){
                sql[i-1] = temp_sql[j];
                sql[i] = ' ';
                j = temp_sql[j+1] == ' ' ? j+1 : j;
            }else if(temp_sql[j] == ')'){
                if(temp_sql[j-1] != ' ')
                    sql[i++] = ' ';
                sql[i] = ')';
                if(temp_sql[j+1] != ' ')
                    sql[++i] = ' ';
            }else
                sql[i] = temp_sql[j];
        }else{
            quote_mark = temp_sql[j] == 39 ? 0 : 1;
            sql[i] = temp_sql[j];
        }
        
        i++;
        j++;
    }
    
    sql[i] = ';';
    sql[i+1] = '\0';
    
    return 0;
}




int main(){
	char s[100];

	strcpy(s, "create table tbl1(col1 int, col2 varchar);");
	printf("%s\n", s);
	formalize(s);
	printf("%s\n", s);

printf("\n");

	strcpy(s, "CREATE TABLE TBL2(COL1 INT, COL2 VARCHAR);");
	printf("%s\n", s);
	formalize(s);
	printf("%s\n", s);

printf("\n");

	strcpy(s, "CREATE       TABLE TBL3(COL1 INT, COL2 VARCHAR);");
	printf("%s\n", s);
	formalize(s);
	printf("%s\n", s);

printf("\n");

	strcpy(s, "   create TAbLE TBL2    (  COL1 INT, COl2 VARchAR   )  ;");
	printf("%s\n", s);
	formalize(s);
	printf("%s\n", s);

printf("\n");

	strcpy(s, "    insert     iNTO     tAbLe tbl2(23   ,   'CHar_Test   '  );");
	printf("%s\n", s);
	formalize(s);
	printf("%s\n", s);
	return 0;
}


