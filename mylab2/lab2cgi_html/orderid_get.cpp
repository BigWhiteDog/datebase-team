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
    cout << "<title>Order Id search result</title>\n";
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


    char order_id[10] = {0};
    form_iterator fvalue2 = formData.getElement("order_id");
    if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
        {
            strcpy(order_id,(**fvalue2).c_str());
            int i;
            for(i=0;i<10;i++){
                if(order_id[i] == '\0')
                    break;
                else if(!isdigit(order_id[i]))
                    flag=1;
            }
            if(flag==1)
                cout << "order id wrong! :" << (**fvalue2).c_str() << endl;
	    }        
	}
    else{
        cout << "No text entered for order id" << endl;
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

    if(flag==0){
        PGresult *res;
        ExecStatusType status;
        char *thedate;
        sprintf(sqlbuffer,
    "select o.order_id, o.train_id, o.dep_station, o.dest_station,o.seat_type,o.one_price, o.dep_date, t.leaving_time, t2.arriving_time\
 from train as t,train as t2, orders as o\
 where o.order_id= %s\
 and t.train_id = o.train_id\
 and t2.train_id = o.train_id\
 and t.station_name = o.dep_station\
 and t2.station_name = o.dest_station\
 and o.passenger_id = '%s'",order_id,id);
        
        res=PQexec(conn,sqlbuffer);
        status=PQresultStatus(res);
        if(status != PGRES_TUPLES_OK){
            cout<<"no info"<<endl;
        }else{
            PQprintOpt po={0};
            po.header=1; po.align=1; po.fieldSep="|";
        po.html3=1; po.caption="Order Information"; po.tableOpt="border=1";
            PQprint(stdout,res,&po);
            cout<<p();
            cout<<"<A href=\"orderdelete_get.cgi?order_id="<<order_id<<"&passenger_id="<<id<<"\"> delete order :"<<**fvalue2<<"</A><br/>"<<endl;
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
