#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <postgresql/libpq-fe.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  

using namespace std;
using namespace cgicc;

char sqlbuffer[1024];

int mystr_isdigit(char *s)
{
	if(*s=='\0')
		return 0;
	while(*s!='\0')
	{
		if(!isdigit(*s))
			return 0;
		s++;
	}
	return 1;
	
}

int main ()
{
	Cgicc formData;

   
	cout << "Content-type:text/html\n\n";
	cout << "<html>\n";
cout << "<meta charset='utf-8' />"<<endl;
	cout << "<head>\n";
	cout << "<title>Train ID search result</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	int flag=0;
	int date_flag=0;
    char trainid[6]={0};
	form_iterator fvalue1 = formData.getElement("trainid");
	if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {
		if((**fvalue1).length()> 5 ){
			cout << "trainid too long! :" << (**fvalue1).c_str() << endl;
			flag++;
		}
		else {
			cout << "trainid: " << (**fvalue1).c_str() << endl;
			strcpy(trainid,(**fvalue1).c_str());
			trainid[5]=0;
			if(!isdigit(trainid[0]) && trainid[0]!='C' && trainid[0]!= 'D' && trainid[0]!= 'G' &&  trainid[0]!= 'K' && trainid[0]!= 'T' &&  trainid[0]!= 'Y' &&  trainid[0]!= 'Z')
				cout << "trainid wrong! :" << (**fvalue1).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for trainid" << endl;
		flag++;
	}
	cout << p();


    //char in_a_specific_day[1]={0}; // in_a_specific_day=1 -> in a specific day; date vaild
	// bool in_a_specific_day = formData.getElement("nondate");
	// 	cout << "in a specific: " << in_a_specific_day << endl;
		//strcpy(trainid,(**fvalue1).c_str());
		//in_a_specific_day=0;
	cout << p();



    char date[11]={0};

    char year[5] ={0};
	form_iterator fvalue2 = formData.getElement("year");
	if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
		if((**fvalue2).length()!= 4 ){
			cout << "year wrong! :" << (**fvalue2).c_str() << endl;
			flag++;
		}
		else {
			cout << "year: " << (**fvalue2).c_str() << endl;
			strcpy(year,(**fvalue2).c_str());
			year[4]=0;
			if(!mystr_isdigit(year) || year[0] != '2' || year[1] != '0') 
				cout << "year wrong! :" << (**fvalue2).c_str() << endl;
		}
	}
	else{
		cout << "No text entered for year" << endl;
		date_flag++;
	}
	cout << p();

    char month[3] ={0};
	form_iterator fvalue3 = formData.getElement("month");
	if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {
		if((**fvalue3).length()> 2 ){
			cout << "month wrong! :" << (**fvalue3).c_str() << endl;
			flag++;
		}
		else {
			cout << "month: " << (**fvalue3).c_str() << endl;
			strcpy(month,(**fvalue3).c_str());
			month[2]=0;
			if(!mystr_isdigit(month)) 
				cout << "month wrong! :" << (**fvalue3).c_str() << endl;
			else if((**fvalue3).length() == 1){
				month[1] = month[0];
				month[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for month" << endl;
		date_flag++;
	}
	cout << p();

    char day[3] ={0};
	form_iterator fvalue4 = formData.getElement("day");
	if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {
		if((**fvalue4).length()> 2 ){
			cout << "day wrong! :" << (**fvalue4).c_str() << endl;
			flag++;
		}
		else {
			cout << "day: " << (**fvalue4).c_str() << endl;
			strcpy(day,(**fvalue4).c_str());
			day[2]=0;
			if(!mystr_isdigit(day)) 
				cout << "day wrong! :" << (**fvalue4).c_str() << endl;
			else if((**fvalue4).length() == 1){
				day[1] = day[0];
				day[0] = 0;
			}
		}
	}
	else{
		cout << "No text entered for day" << endl;
		date_flag++;
	}
	cout << p();

	if(date_flag==0)
		sprintf(date,"%s-%s-%s",year,month,day);
	else{
		time_t timep;
    	struct tm *p;
    	time(&timep);
   		p = localtime(&timep); //取得当地时间
    	sprintf(date,"%d-%02d-%02d ", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
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

	if(flag==0){
		PGresult *res;
		ExecStatusType status;
		sprintf(sqlbuffer,"select t.station_order, t.station_name, t.arriving_time, t.leaving_time, rs.hard_seat, t.hard_seat_price, rs.soft_seat, t.soft_seat_price, rs.hard_berth_upper, t.hard_berth_upper_price, rs.hard_berth_middle, t.hard_berth_middle_price, rs.hard_berth_lower, t.hard_berth_lower_price, rs.soft_berth_upper, t.soft_berth_upper_price, rs.soft_berth_lower, t.soft_berth_lower_price \
		from train as t, remaining_seat as rs \
		where t.train_id = '%s' \
		and t.train_id=rs.train_id \
		and t.station_order=rs.station_order \
		and rs.real_date = '%s'",trainid,date);
		res=PQexec(conn,sqlbuffer);
		status=PQresultStatus(res);
		if(status != PGRES_TUPLES_OK){
			cout<<"no info"<<endl;
		}else{
			PQprintOpt po={0};
			po.header=1; po.align=1; po.fieldSep="|";
		po.html3=1; po.caption="Train Information"; po.tableOpt="border=1";
			PQprint(stdout,res,&po);
		}
		PQclear(res);
	}
	
	
	cout<<p();

	cout << "<br/>\n";

	cout << "</body>\n";
	cout << "</html>\n";
   
	PQfinish(conn);
	return 0;
	
}
