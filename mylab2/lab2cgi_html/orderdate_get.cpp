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
    cout << "<title>Order date search result</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";

    int flag=0;

    char id[19] = {0};
    form_iterator fvalue1 = formData.getElement("id");
    if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {
        if((**fvalue1).length()!=18 ){
            cout << "id: " << (**fvalue1).c_str() << endl;
            flag++;
        }
        else{
            strcpy(id,(**fvalue1).c_str());
            int i;
            for(i=0;i<18;i++){
                if(id[i] == '\0')
                    break;
                else if(!isdigit(id[i]))
                    flag=1;
            }
            if(flag==1)
                cout << "ID wrong! :" << (**fvalue1).c_str() << endl;
            }
	}
    else{
        cout << "No text entered for ID" << endl;
        flag++;
    	}
	
    cout << p();    


    char date[11]={0};

    char year[5] ={0};
    form_iterator fvalue4 = formData.getElement("order_year");
    if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {
        if((**fvalue4).length()!= 4 ){
            cout << "year wrong!:" << (**fvalue4).c_str() << endl;
            flag++;
        }
        else {
            // cout << "year: " << (**fvalue4).c_str() << endl;
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
    form_iterator fvalue5 = formData.getElement("order_month");
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
    form_iterator fvalue6 = formData.getElement("order_day");
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

   sprintf(date,"%s-%s-%s",year,month,day);


    char end_date[11]={0};

    char end_year[5] ={0};
    form_iterator fvalue2 = formData.getElement("end_year");
    if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
        if((**fvalue2).length()!= 4 ){
            cout << "end year wrong! :" << (**fvalue2).c_str() << endl;
            flag++;
        }
        else {
            // cout << "end year: " << (**fvalue2).c_str() << endl;
            strcpy(end_year,(**fvalue2).c_str());
            end_year[4]=0;
            if(!isdigit(end_year[2])|| !isdigit(end_year[3])|| end_year[0] != '2' || end_year[1] != '0') 
                cout << "end year wrong! :" << (**fvalue2).c_str() << endl;
        }
    }
    else{
        cout << "No text entered for end year" << endl;
        flag++;
    }
    cout << p();

    char end_month[3] ={0};
    form_iterator fvalue3 = formData.getElement("end_month");
    if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {
        if((**fvalue3).length()> 2 ){
            cout << "end month wrong! :" << (**fvalue3).c_str() << endl;
            flag++;
        }
        else {
            // cout << "end month: " << (**fvalue3).c_str() << endl;
            strcpy(end_month,(**fvalue3).c_str());
            end_month[2]=0;
            if(!isdigit(end_month[0])||!isdigit(end_month[1])) 
                cout << "end month wrong! :" << (**fvalue3).c_str() << endl;
            else if((**fvalue3).length() == 1){
                end_month[1] = end_month[0];
                end_month[0] = 0;
            }
        }
    }
    else{
        cout << "No text entered for end month" << endl;
        flag++;
    }
    cout << p();

    char end_day[3] ={0};
    form_iterator fvalue7 = formData.getElement("end_day");
    if( !fvalue7->isEmpty() && fvalue7 != (*formData).end()) {
        if((**fvalue7).length()> 2 ){
            cout << "end day wrong! :" << (**fvalue7).c_str() << endl;
            flag++;
        }
        else {
            // cout << "end day: " << (**fvalue7).c_str() << endl;
            strcpy(end_day,(**fvalue7).c_str());
            end_day[2]=0;
            if(!isdigit(end_day[0])|| !isdigit(end_day[1])) 
                cout << "end day wrong! :" << (**fvalue7).c_str() << endl;
            else if((**fvalue7).length() == 1){
                end_day[1] = end_day[0];
                end_day[0] = 0;
            }
        }
    }
    else{
        cout << "No text entered for end day" << endl;
        flag++;
    }
    cout << p();


    sprintf(end_date,"%s-%s-%s",end_year,end_month,end_day);
    

    cout<<"Start date:"<<date<<endl;
    cout<<p();
    cout<<"End date:"<<end_date<<endl;
    cout<<p();
    // cout << "<br/>\n";

    // cout << "</body>\n";
    // cout << "</html>\n";
   

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
        sprintf(sqlbuffer,"select o.order_id,o.dep_date,t.leaving_time,o.dep_station, o.dest_station, o.one_price+5\
 from train as t, orders as o\
 where t.train_id = o.train_id\
 and t.station_name = o.dep_station\
 and '%s' <= o.dep_date+ t.leaving_days_offset\
 and o.dep_date+ t.leaving_days_offset<= '%s'\
 and o.passenger_id = '%s'",date,end_date,id);
    
        cout<<p();
        res=PQexec(conn,sqlbuffer);
        status=PQresultStatus(res);
        if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
            cout<<"no info"<<endl;
        }else{
            cout<<"<table border=1><caption align=\"top\">Order Search Result</caption>"<<endl;
            cout<<"<tr><th align=\"right\">order_id</th>"<<
            "<th align=\"right\">date</th>"<<
            "<th align=\"right\">leaving_time</th>"<<
            "<th align=\"right\">departure station</th>"<<
            "<th align=\"right\">destination</th>"<<
            "<th align=\"right\">total price</th></tr>"<<endl;
            int num_rows=PQntuples(res);
            int num_cols=PQnfields(res);
            int r,i;
            char *val;
            for (r=0; r<num_rows; r++) {
                cout<<"<tr>";
                for (i=0; i<num_cols; i++) {
                    val= PQgetvalue(res, r, i);
                    cout<<"<td align=\"right\">"<<val<<"</td>";
                    }
                cout<<"<td align=\"right\">"<<"<A href=\"orderid_get.cgi?id="<<id<<"&order_id="<<PQgetvalue(res,r,0)<<"\"> Details"<<"</A>"<<"</td>";
                cout<<"</tr>"<<endl;
            }
            cout<<"</table>"<<endl;
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
