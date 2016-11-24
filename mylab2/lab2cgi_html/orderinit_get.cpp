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
	cout << "<title>Order Initial result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	int flag=0;
	form_iterator fvalue1 = formData.getElement("train_id");
   	if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {	
		cout<<"Train ID: "<<**fvalue1<<endl;
	}
	else{
		cout << "No text entered for train_id" << endl;
		flag++;
	}
	cout<<p();

	form_iterator fvalue2 = formData.getElement("dep_station");
   	if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {	
		cout<<"Departure from: "<<**fvalue2<<endl;
	}
	else{
		cout << "No text entered for dep_station" << endl;
		flag++;
	}
	cout<<p();

	form_iterator fvalue3 = formData.getElement("dest_station");
   	if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {	
		cout<<"Arriving in: "<<**fvalue3<<endl;
	}
	else{
		cout << "No text entered for dest_station" << endl;
		flag++;
	}
	cout<<p();

	form_iterator fvalue4 = formData.getElement("seat_type");
   	if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {	
		cout<<"Seat Type: "<<**fvalue4<<endl;
	}
	else{
		cout << "No text entered for seat_type" << endl;
		flag++;
	}
	cout<<p();


	form_iterator fvalue5 = formData.getElement("one_price");
   	if( !fvalue5->isEmpty() && fvalue5 != (*formData).end()) {	
		cout<<"One Price: "<<**fvalue5<<endl;
	}
	else{
		cout << "No text entered for one_price" << endl;
		flag++;
	}
	cout<<p();
	// form_iterator fvalue6 = formData.getElement("total_price");
 //   	if( !fvalue6->isEmpty() && fvalue6 != (*formData).end()) {	
	// 	cout<<"Total Price: "<<**fvalue6<<endl;
	// }
	// else{
	// 	cout << "No text entered for one_price" << endl;
	// 	flag++;
	// }
	// cout<<p();

	form_iterator fvalue7 = formData.getElement("real_date");
 	if( fvalue7->isEmpty() || fvalue7 == (*formData).end()) 
	{
		cout << "No text entered for real_date" << endl;
		flag++;cout<<p();
	}
	
	form_iterator fvalue8 = formData.getElement("dep_date");
	if( !fvalue8->isEmpty() && fvalue8 != (*formData).end()) {	
		cout<<"Departure date: "<<**fvalue8<<endl;
	}
	else{
		cout << "No text entered for dep_date" << endl;
		flag++;
	}
	cout<<p();

	form_iterator fvalue12 = formData.getElement("leaving_time");
	if( !fvalue12->isEmpty() && fvalue12 != (*formData).end()) {	
		cout<<"Leaving time: "<<**fvalue12<<endl;
	}
	else{
		cout << "No text entered for dep_date" << endl;
		flag++;
	}
	cout<<p();

	form_iterator fvalue13 = formData.getElement("duration");
	if( !fvalue13->isEmpty() && fvalue13 != (*formData).end()) {	
		cout<<"Duration "<<**fvalue13<<"  min"<<endl;
	}
	else{
		cout << "No text entered for dep_date" << endl;
		flag++;
	}
	cout<<p();

	// form_iterator fvalue9 = formData.getElement("passenger_id");
 //   	if( !fvalue9->isEmpty() && fvalue9 != (*formData).end()) {
	// 	cout<<"Your Passenger ID:"<<**fvalue9<<endl;
	// }
	// else{
	// 	cout << "No text entered for passenger_id" << endl;
	// 	flag++;
	// }
	// cout << p();



	form_iterator fvalue10 = formData.getElement("dep_station_order");
	if( fvalue10->isEmpty() || fvalue10 == (*formData).end()) {
		cout << "No text entered for dep_station_order" << endl;
		flag++;cout << p();
	}
	form_iterator fvalue11 = formData.getElement("dest_station_order");
	if( fvalue11->isEmpty() || fvalue11 == (*formData).end()) {
		cout << "No text entered for dest_station_order" << endl;
		flag++;cout << p();
	}
	
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
	sprintf(sqlbuffer,"select %s + 5",(**fvalue5).c_str());
	res=PQexec(conn,sqlbuffer);
	status=PQresultStatus(res);
	if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		cout<<"calculate order price wrong!"<<endl;

	}

cout<<p();
cout<<"Total price of this order:"<<PQgetvalue(res,0,0)<<endl;
cout<<p();	
	
cout<<"<form action=\"orderinsert_get.cgi\" method=\"get\">"<<endl;
cout<<"Your Passenger Id:"<<endl;
cout<<"<input type=\"text\" id=\"txtname\" name=\"passenger_id\">"<<endl;

cout<<"<input type=\"hidden\" value=\""<<**fvalue1<<"\" name=\"train_id\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue2<<"\" name=\"dep_station\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue3<<"\" name=\"dest_station\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue4<<"\" name=\"seat_type\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue5<<"\" name=\"one_price\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,0,0)<<"\" name=\"total_price\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue7<<"\" name=\"real_date\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue8<<"\" name=\"dep_date\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue10<<"\" name=\"dep_station_order\">"<<endl;
cout<<"<input type=\"hidden\" value=\""<<**fvalue11<<"\" name=\"dest_station_order\">"<<endl;
cout<<"<input type=\"submit\" value=\"Submit this order\">"<<endl;
cout<<"</form>"<<endl; 
	PQclear(res);
	
	cout<<p();

	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
	PQfinish(conn);
	return 0;
	
}
