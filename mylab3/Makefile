CC=gcc
LD=ld
CFLAGS=-g -c -O2 -Wall -std=c99
SQL_OBJS =  list.o mycmp.o sql_create.o sql_drop.o sql_insert.o sql_select.o new_interpreter.o sql_main.o
all:ucasdb

ucasdb: $(SQL_OBJS)
	$(CC) -o ucasdb $(SQL_OBJS)

test_meta: list.o test_meta.o
	$(CC) -o create_meta list.o test_meta.o
	./create_meta
	mv metadata ./db/
	rm create_meta test_meta.o
	
%.o : %.c 
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f ucasdb