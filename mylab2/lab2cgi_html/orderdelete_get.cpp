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
cout << "<meta charset='utf-8' />"<<endl;
	cout << "<head>\n";
	cout << "<title>Order Delete result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	int flag=0;
	char order_id[10] = {0};
	form_iterator fvalue1 = formData.getElement("order_id");
   	if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {	
			strcpy(order_id,(**fvalue1).c_str());
			int i;
			for(i=0;i<10;i++){
				if(order_id[i] == '\0')
					break;
				else if(!isdigit(order_id[i]))
					flag=1;
			}
			if(flag==1)
				cout << "order_id wrong! :" << (**fvalue1).c_str() << endl;
	}
	else{
		cout << "No text entered for order_id" << endl;
		flag++;
	}
	char passenger_id[19];
	form_iterator fvalue2 = formData.getElement("passenger_id");
   	if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
   		if((**fvalue2).length()!=18 ){
			cout << "Passenger id: " << (**fvalue2).c_str() << endl;
			flag++;
		}
		else{
			strcpy(passenger_id,(**fvalue2).c_str());
			int i;
			for(i=0;i<18;i++){
				if(passenger_id[i] == '\0')
					break;
				else if(!isdigit(passenger_id[i]))
					flag=1;
			}
			if(flag==1)
				cout << "passenger_id wrong! :" << (**fvalue2).c_str() << endl;
		    }
	}
	else{
		cout << "No text entered for passenger_id" << endl;
		flag++;
	}
	cout << p();
	
	PGconn* conn= PQconnectdb("dbname=train12306");
	if(PQstatus(conn)!=CONNECTION_OK){
		cout<<"Connection to train12306 fail";
		cout << p();
		cout << "<br/>\n";
		cout << "</body>\n";
		cout << "</html>\n";
		return 1;
	}

	PGresult *res;
	ExecStatusType status;

	sprintf(sqlbuffer,"delete from orders where order_id= %s and passenger_id = '%s' ",order_id,passenger_id);
	res=PQexec(conn,sqlbuffer);
	status=PQresultStatus(res);
	if(status != PGRES_COMMAND_OK){
		cout<<"no info"<<endl;
	}else{
		cout<<"Delete OK!!!"<<endl;
	}

	PQclear(res);
	
	cout<<p();

	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
	PQfinish(conn);
	return 0;
	
}
