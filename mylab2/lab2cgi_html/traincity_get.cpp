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

char sqlbuffer[2048];


int main ()
{
	Cgicc formData;

	cout << "Content-type:text/html\n\n";
	cout << "<html>\n";
cout << "<meta charset='utf-8' />"<<endl;
	cout << "<head>\n";
	cout << "<title>Search result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	char typebuffer[100]={0};
	char sty[7][30];
	sprintf(sty[0], "%s", "hard_seat");
	sprintf(sty[1], "%s", "soft_seat");
	sprintf(sty[2], "%s", "hard_berth_upper");
	sprintf(sty[3], "%s", "hard_berth_middle");
	sprintf(sty[4], "%s", "hard_berth_lower");
	sprintf(sty[5], "%s", "soft_berth_upper");
	sprintf(sty[6], "%s", "soft_berth_lower");	
	
	int flag=0;
	int returnflag=0;

	char from[20] = {0};
   form_iterator fvalue7 = formData.getElement("from");
   if( !fvalue7->isEmpty() && fvalue7 != (*formData).end()) {
			cout << "from: " << (**fvalue7).c_str() << endl;
			strcpy(from,(**fvalue7).c_str());
			
			if(flag==1)
				cout << "from location wrong! :" << (**fvalue7).c_str() << endl;
		}
	
	cout << p();	


	char des[20] = {0};
   form_iterator fvalue8 = formData.getElement("destination");
   if( !fvalue8->isEmpty() && fvalue8 != (*formData).end()) {
			cout << "destination: " << (**fvalue8).c_str() << endl;
			strcpy(des,(**fvalue8).c_str());
			
			
			if(flag==1)
				cout << "destination location wrong! :" << (**fvalue8).c_str() << endl;
		}
	
	cout << p();


	char date[11]={0};

    char year[5] ={0};
	form_iterator fvalue4 = formData.getElement("year");
	if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {
		if((**fvalue4).length()!= 4 ){
			cout << "year wrong! :" << (**fvalue4).c_str() << endl;
			flag++;
		}
		else {
//			cout << "year: " << (**fvalue4).c_str() << endl;
			strcpy(year,(**fvalue4).c_str());
			year[4]=0;
			if(!isdigit(year[2])|| !isdigit(year[3])|| year[0] != '2' || year[1] != '0') 
				cout << "year wrong! :" << (**fvalue4).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for year" << endl;
		flag++;
	}
	cout << p();

    char month[3] ={0};
	form_iterator fvalue5 = formData.getElement("month");
	if( !fvalue5->isEmpty() && fvalue5 != (*formData).end()) {
		if((**fvalue5).length()> 2 ){
			cout << "month wrong! :" << (**fvalue5).c_str() << endl;
			flag++;
		}
		else {
			// cout << "month: " << (**fvalue5).c_str() << endl;
			strcpy(month,(**fvalue5).c_str());
			month[2]=0;
			if(!isdigit(month[0])||!isdigit(month[1])) 
				cout << "month wrong! :" << (**fvalue5).c_str() << endl;
			else if((**fvalue5).length() == 1){
				month[1] = month[0];
				month[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for month" << endl;
		flag++;
	}
	cout << p();

    char day[3] ={0};
	form_iterator fvalue6 = formData.getElement("day");
	if( !fvalue6->isEmpty() && fvalue6 != (*formData).end()) {
		if((**fvalue6).length()> 2 ){
			cout << "day wrong! :" << (**fvalue6).c_str() << endl;
			flag++;
		}
		else {
			// cout << "day: " << (**fvalue6).c_str() << endl;
			strcpy(day,(**fvalue6).c_str());
			day[2]=0;
			if(!isdigit(day[0])|| !isdigit(day[1])) 
				cout << "day wrong! :" << (**fvalue6).c_str() << endl;
			else if((**fvalue6).length() == 1){
				day[1] = day[0];
				day[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for day" << endl;
		flag++;
	}
	cout << p();

		strcat(typebuffer,"(");

	bool hard_seat = formData.queryCheckbox("hard_seat");
	// cout << "hard_seat: " << hard_seat << endl;
	// cout << p();

 	if(hard_seat==true)
 		strcat(typebuffer,"'hard_seat',");

	bool soft_seat = formData.queryCheckbox("soft_seat");
	// cout << "soft_seat: " << soft_seat << endl;
	// cout << p();

	if(soft_seat==true)
 		strcat(typebuffer,"'soft_seat',");

	bool hard_berth_upper = formData.queryCheckbox("hard_berth_upper");
	// cout << "hard_berth_upper: " << hard_berth_upper << endl;
	// cout << p();

	if(hard_berth_upper==true)
 		strcat(typebuffer,"'hard_berth_upper',");

	bool hard_berth_middle = formData.queryCheckbox("hard_berth_middle");
	//cout << "hard_berth_middle: " << hard_berth_middle << endl;
	//cout << p();

	if(hard_berth_middle==true)
 		strcat(typebuffer,"'hard_berth_middle',");

	bool hard_berth_lower = formData.queryCheckbox("hard_berth_lower");
	//cout << "hard_berth_lower: " << hard_berth_lower << endl;
	// cout << p();

	if(hard_berth_lower==true)
 		strcat(typebuffer,"'hard_berth_lower',");

	bool soft_berth_upper = formData.queryCheckbox("soft_berth_upper");
	// cout << "soft_berth_upper: " << soft_berth_upper << endl;
	// cout << p();

	if(soft_berth_upper==true)
 		strcat(typebuffer,"'soft_berth_upper',");

	// bool soft_berth_middle = formData.queryCheckbox("soft_berth_middle");
	// cout << "soft_berth_middle: " << soft_berth_middle << endl;
	// cout << p();

	// if(soft_berth_middle==true)
 // 		strcat(typebuffer,"'soft_berth_middle',");

	bool soft_berth_lower = formData.queryCheckbox("soft_berth_lower");
	// cout << "soft_berth_lower: " << soft_berth_lower << endl;
	// cout << p();

	if(soft_berth_lower==true)
 		strcat(typebuffer,"'soft_berth_lower',");

 	if(strlen(typebuffer)!=1){
 		typebuffer[strlen(typebuffer)-1]='\0';
 	}
	strcat(typebuffer,")");

	// char order[20] = {0};
   form_iterator fvalue1 = formData.getElement("Order");
 //   if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {
	// 		cout << "Order by: " << (**fvalue1).c_str() << endl;
	// 		strcpy(order,(**fvalue1).c_str());
	// 		int i;
	// 		for(i=0;i<20;i++){
	// 			if(order[i] == '\0')
	// 				break;
	// 			else if(!isdigit(order[i]))
	// 				flag=1;
	// 		}
	// 		if(flag==1)
	// 			cout << "order wrong! :" << (**fvalue1).c_str() << endl;
	// 	}
	
	cout << p();

    char return_date[11]={0};

    char return_year[5] ={0};
	form_iterator fvalue2 = formData.getElement("return_year");
	if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
		if((**fvalue2).length()!= 4 ){
			cout << "return year wrong! :" << (**fvalue2).c_str() << endl;
			flag++;
		}
		else {
			// cout << "return year: " << (**fvalue2).c_str() << endl;
			strcpy(return_year,(**fvalue2).c_str());
			return_year[4]=0;
			if(!isdigit(return_year[2])|| !isdigit(return_year[3])|| return_year[0] != '2' || return_year[1] != '0') 
				cout << "return year wrong! :" << (**fvalue2).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for return year" << endl;
		returnflag++;
	}
	cout << p();

    char return_month[3] ={0};
	form_iterator fvalue3 = formData.getElement("return_month");
	if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {
		if((**fvalue3).length()> 2 ){
			cout << "return month wrong! :" << (**fvalue3).c_str() << endl;
			flag++;
		}
		else {
			// cout << "return month: " << (**fvalue3).c_str() << endl;
			strcpy(return_month,(**fvalue3).c_str());
			return_month[2]=0;
			if(!isdigit(return_month[0])||!isdigit(return_month[1])) 
				cout << "return month wrong! :" << (**fvalue3).c_str() << endl;
			else if((**fvalue3).length() == 1){
				return_month[1] = return_month[0];
				return_month[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for return month" << endl;
		returnflag++;
	}
	cout << p();

    char return_day[3] ={0};
	form_iterator fvalue9 = formData.getElement("return_day");
	if( !fvalue9->isEmpty() && fvalue9 != (*formData).end()) {
		if((**fvalue9).length()> 2 ){
			cout << "return day wrong! :" << (**fvalue9).c_str() << endl;
			flag++;
		}
		else {
			// cout << "return day: " << (**fvalue9).c_str() << endl;
			strcpy(return_day,(**fvalue9).c_str());
			return_day[2]=0;
			if(!isdigit(return_day[0])|| !isdigit(return_day[1])) 
				cout << "return day wrong! :" << (**fvalue9).c_str() << endl;
			else if((**fvalue9).length() == 1){
				return_day[1] = return_day[0];
				return_day[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for return day" << endl;
		returnflag++;
	}
	cout << p();


	
	sprintf(date,"%s-%s-%s",year,month,day);
	sprintf(return_date,"%s-%s-%s",return_year,return_month,return_day);
	
	cout<<"Search for date:"<<date<<endl;
	cout<<p();
	if (returnflag==3)
	{
		cout<< "you don't input your return date, so we won't show the return trains"<<endl;
		cout<<p();
	}else if(returnflag!=0 )
	{
		cout<< "you wrongly input your return date "<<endl;
		cout<<p();
	}else if(returnflag==0)
	{
		cout<<"Search for return date:"<<return_date<<endl;
		cout<<p();
	}

	cout<<"Search seat type: "<<typebuffer<<endl;
	cout<<p();

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

	PGresult *res;
	ExecStatusType status;
	sprintf(sqlbuffer,"delete from temp");
	res=PQexec(conn,sqlbuffer);
	status=PQresultStatus(res);
	if(status != PGRES_COMMAND_OK){
		cout<<"delete from temp wrong !"<<endl;
		cout<<p();
	}			
	PQclear(res);

	int i;
	for(i=0;i<7;i++)
	{
		sprintf(sqlbuffer,"insert into temp(train_id,dep_station,dest_station,seat_type,one_price,real_date,dep_date,dep_station_order,dest_station_order,leaving_time,duration) \
select rs.train_id,t1.station_name,t2.station_name,'%s',t2.%s_price-t1.%s_price,rs.real_date,'%s',t1.station_order,t2.station_order,t1.leaving_time,t2.duration-t1.duration \
from remaining_seat as rs, train as t1, train as t2, station as s1, station as s2 \
where rs.real_date = date'%s'-t1.leaving_days_offset \
and s1.city_name = '%s' \
and s2.city_name = '%s' \
and rs.train_id = t1.train_id \
and t1.train_id = t2.train_id \
and rs.station_order between t1.station_order + 1 and t2.station_order \
and t1.station_order < t2.station_order \
and t1.station_name = s1.station_name \
and t2.station_name = s2.station_name \
and (t1.%s_price != 0 or t1.station_order=1) \
and t2.%s_price != 0 \
group by rs.train_id,t1.station_name,t2.station_name,t2.%s_price-t1.%s_price,rs.real_date,t1.station_order,t2.station_order,t1.leaving_time,t2.duration,t1.duration \
having min(rs.%s) > 0",sty[i],sty[i],sty[i],date,date,from,des,sty[i],sty[i],sty[i],sty[i],sty[i]);
	
		res=PQexec(conn,sqlbuffer);
		status=PQresultStatus(res);
		if(status != PGRES_COMMAND_OK){
			cout<<"    no info insert"<<i<<endl;
			cout<<sqlbuffer<<endl;
			cout<<"<br/>"<<endl;
		}			
		PQclear(res);
	}
	
	sprintf(sqlbuffer,"select * from temp where seat_type in %s order by %s",typebuffer,(**fvalue1).c_str());
	
	res=PQexec(conn,sqlbuffer);
	status=PQresultStatus(res);
	if(status != PGRES_TUPLES_OK){
		cout<<"no info in temp"<<endl;
		cout<<p();
	}else{
		cout<<"<table border=1><caption align=\"top\">Train Search Result</caption>"<<endl;
        cout<<"<tr><th align=\"right\">Train ID</th>"<<
        "<th align=\"right\">Dep Station</th>"<<
        "<th align=\"right\">Dest Station</th>"<<
        "<th align=\"right\">Seat Type</th>"<<
        "<th align=\"right\">Ticket Price</th>"<<
        "<th align=\"right\">Dep Date</th>"<<
        "<th align=\"right\">Leaving Time</th>"<<
        "<th align=\"right\">Duration</th></tr>"<<endl;
        int num_rows=PQntuples(res);
        int num_cols=PQnfields(res);
        int r;
        char *val;
        for (r=0; r<num_rows; r++) {
            cout<<"<tr>";
            for (i=0; i<num_cols; i++) {
            	if(i==5 || i==7 || i==8)
            		continue;
                val= PQgetvalue(res, r, i);
                cout<<"<td align=\"right\">"<<val<<"</td>";
                }

            cout<<"<td><form action=\"orderinit_get.cgi\" method=\"get\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,0)<<"\" name=\"train_id\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,1)<<"\" name=\"dep_station\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,2)<<"\" name=\"dest_station\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,3)<<"\" name=\"seat_type\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,4)<<"\" name=\"one_price\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,5)<<"\" name=\"real_date\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,6)<<"\" name=\"dep_date\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,7)<<"\" name=\"dep_station_order\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,8)<<"\" name=\"dest_station_order\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,9)<<"\" name=\"leaving_time\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<PQgetvalue(res,r,10)<<"\" name=\"duration\">"<<endl;
			cout<<"<input type=\"submit\" value=\"Buy it\">"<<endl;
			cout<<"</form></td>"<<endl; 
            cout<<"</tr>"<<endl;
        }
        cout<<"</table>"<<endl;
        cout<<p();

        if(returnflag==0)
        {
        	cout<<"<form action=\"traincity_get.cgi\" method=\"get\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<des<<"\" name=\"from\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<from<<"\" name=\"destination\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<return_year<<"\" name=\"year\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<return_month<<"\" name=\"month\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<return_day<<"\" name=\"day\">"<<endl;
			if(hard_seat==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"hard_seat\">"<<endl;
			if(soft_seat==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"soft_seat\">"<<endl;
			if(hard_berth_upper==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"hard_berth_upper\">"<<endl;
			if(hard_berth_middle==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"hard_berth_middle\">"<<endl;
			if(hard_berth_lower==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"hard_berth_lower\">"<<endl;
			if(soft_berth_upper==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"soft_berth_upper\">"<<endl;
			if(soft_berth_lower==true)
				cout<<"<input type=\"hidden\" value=\"on\" name=\"soft_berth_lower\">"<<endl;

			cout<<"<input type=\"hidden\" value=\""<<**fvalue1<<"\" name=\"Order\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<year<<"\" name=\"return_year\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<month<<"\" name=\"return_month\">"<<endl;
			cout<<"<input type=\"hidden\" value=\""<<day<<"\" name=\"return_day\">"<<endl;


			cout<<"<input type=\"submit\" value=\"Return Trains\">"<<endl;
			cout<<"</form>"<<endl; 
        }

	}
		
	PQclear(res);
	PQfinish(conn);

	}
	cout<<p();

	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
		return 0;
	
}
