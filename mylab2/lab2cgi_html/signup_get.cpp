#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#include <postgresql/libpq-fe.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  

using namespace std;
using namespace cgicc;

char sqlbuffer[1024];


int main ()
{
	Cgicc formData;
   
	cout << "Content-type:text/html\n\n";
	cout << "<html>\n";
	cout << "<head>\n";
cout << "<meta charset='utf-8' />"<<endl;
	cout << "<title>Sign Up result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	int flag=0;
	char username[17]={0};
	form_iterator fvalue1 = formData.getElement("username");
	if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {
		if((**fvalue1).length()>16){
			cout << "username too long! :" << (**fvalue1).c_str() << endl;
			flag++;
		}
		else {
			cout << "username: " << (**fvalue1).c_str() << endl;
			strcpy(username,(**fvalue1).c_str());
			username[17]=0;
		}
	}
	else{
		cout << "No text entered for username" << endl;
		flag++;
	}
	cout << p();

	char id[19]={0};
	form_iterator fvalue2 = formData.getElement("ID");
	if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
		if((**fvalue2).length()!=18 ){
			cout << "ID wrong! :" << (**fvalue2).c_str() << endl;
			flag++;
		}
		else {
			cout << "ID: " << (**fvalue2).c_str() << endl;
			strcpy(id,(**fvalue2).c_str());
			id[18]=0;
			int i;
			for(i=0;i<18;i++)
				if(!isdigit(id[i]))
					flag=1;
			if(flag==1)
				cout << "ID wrong! :" << (**fvalue2).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for ID" << endl;
		flag++;
	}
	cout << p();
	char telephone[11]={0};
	form_iterator fvalue3 = formData.getElement("telephone");
	if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {
		if((**fvalue3).length()!=10 ){
			cout << "telephone wrong! :" << (**fvalue3).c_str() << endl;
			flag++;
		}
		else {
			cout << "telephone: " << (**fvalue3).c_str() << endl;
			strcpy(telephone,(**fvalue3).c_str());
			telephone[10]=0;
			int i;
			for(i=0;i<10;i++)
				if(!isdigit(telephone[i]))
					flag=1;
			if(flag==1)
				cout << "telephone wrong! :" << (**fvalue3).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for telephone" << endl;
		flag++;
	}
	cout << p();

	char bank[100]={0};
	form_iterator fvalue4 = formData.getElement("bank of card");
	if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {
		// Note this is just a different way to access the string class.
		// See the YoLinux GNU string class tutorial.
		cout << "bank of card: " << (**fvalue4).c_str() << endl;
		strncpy(bank,(**fvalue4).c_str(),100);
		bank[99]=0;
	}
	else{
		cout << "No text entered for bank of card" << endl;
		flag++;
	}
    cout << p();
	char card[17]={0};
	form_iterator fvalue5 = formData.getElement("card number");
	if( !fvalue5->isEmpty() && fvalue5 != (*formData).end()) {
		if((**fvalue5).length()!=16 ){
			cout << "card wrong! :" << (**fvalue5).c_str() << endl;
			flag++;
		}
		else {
			cout << "card: " << (**fvalue5).c_str() << endl;
			strcpy(card,(**fvalue5).c_str());
			card[16]=0;
			int i;
			for(i=0;i<16;i++)
				if(!isdigit(card[i]))
					flag=1;
			if(flag==1)
				cout << "card wrong! :" << (**fvalue5).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for card number" << endl;
		flag++;
	}

	sprintf(sqlbuffer,"insert into people values('%s','%s','%s','%s','%s',0)",id,username,telephone,card,bank);
	if(flag==0){
		PGconn* conn= PQconnectdb("dbname=train12306");
		if(PQstatus(conn)!=CONNECTION_OK){
			cout<<"Connection to train12306 fail";
			cout << p();
			cout << "<br/>\n";
			cout << "</body>\n";
			cout << "</html>\n";
			return 1;
		}

		PGresult *res= PQexec(conn,sqlbuffer);
		ExecStatusType status=PQresultStatus(res);
		
		cout << p();
		if(status != PGRES_COMMAND_OK){
			cout<<"insert error!!! please check your info"<<endl;
		}else
			cout << "insert OK!" <<endl;
		PQclear(res);
		PQfinish(conn);
	}
	
	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
	return 0;
}
