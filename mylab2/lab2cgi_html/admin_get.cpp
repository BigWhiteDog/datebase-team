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
	cout << "<title>Admin result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	
	PGconn* conn= PQconnectdb("dbname=train12306");
	if(PQstatus(conn)!=CONNECTION_OK){
		cout<<"Connection to train12306 fail";
		cout << p();
		cout << "<br/>\n";
		cout << "</body>\n";
		cout << "</html>\n";
		return 1;
	}

	PGresult *res= PQexec(conn,"select count(*) from orders");
	ExecStatusType status=PQresultStatus(res);
	
	cout << p();
	if(status != PGRES_TUPLES_OK){
		cout<<"no info"<<endl;

	}else{
		cout<<"total order count:"<<PQgetvalue(res,0,0)<<endl;
	}
	char ordernumc[10]={0};
	int ordern=0;
	strcpy(ordernumc,PQgetvalue(res,0,0));
	ordern=atoi(ordernumc);
	PQclear(res);
	cout << p();

	res=PQexec(conn,"select sum(one_price) from orders");
	status=PQresultStatus(res);
	if(status != PGRES_TUPLES_OK){
		cout<<"no info"<<endl;
	}else{
		cout<<"total ticket price:"<<PQgetvalue(res,0,0)<<endl;
	}
	PQclear(res);
	cout<<p();

	cout<<"total order ticket price:"<<ordern*5<<endl;
	cout<<p();
	

	cout << p();



	res=PQexec(conn,"select orders.train_id, count(*)as Number from orders group by train_id order by Number desc limit 10");
	status=PQresultStatus(res);
	if(status != PGRES_TUPLES_OK){
		cout<<"no info"<<endl;
	}else{
		PQprintOpt po={0};
		po.header=1; po.align=1; po.fieldSep="|";
		po.html3=1; po.caption="Admin Information"; po.tableOpt="border=1";
		PQprint(stdout,res,&po);
	}
		
	PQclear(res);
	
	cout<<p();

	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
	PQfinish(conn);
	return 0;
	
}
