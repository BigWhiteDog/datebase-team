CC=g++
CFLAGS=-g -Wall 
LIBS=/usr/local/cgicc/lib/libcgicc.a -lpq

TARGET= orderinit_get.cgi admin_get.cgi signup_get.cgi orderdate_get.cgi traincity_get.cgi orderdelete_get.cgi trainid_get.cgi orderid_get.cgi orderinsert_get.cgi

all: ${TARGET}

%.cgi: %.cpp
	${CC} ${CFLAGS} -o $@ $< ${LIBS}


#cgi_get.cgi:   cgi_get.cpp 
#cgi_post.cgi:  cgi_post.cpp


clean:
	-rm -f ${TARGET}

