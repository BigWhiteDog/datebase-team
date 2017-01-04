CC=gcc
LD=ld
CFLAGS=-g -O2 -Wall
SQL_OBJS = sql_main.o sql_create.o sql_drop.o sql_insert.o sql_select.o list.o mycmp.o 
			##interpreter.o
all:ucasdb

ucasdb: $(SQL_OBJS)
	$(CC) -o ucasdb $(SQL_OBJS)

%.o : %.c 
	$(CC) $(CFLAGS) $<

clean:
	-rm -f *.o
	-rm -f ucasdb