-- correct: create table with 1 column  
create table col1 (id int);

-- correct: create table with 2 columns
create table col2 (id int, dis varchar);

-- correct: create table with 4 columns
create table col4 (id int, dis varchar, frequency int, token varchar);

-- correct
insert into col2 values (1, 'a');
insert into col2 values (2, 'b');
insert into col2 values (3, 'c');
insert into col2 values (4, 'd');
insert into col2 values (5, 'e');
insert into col2 values (6, 'f');
insert into col2 values (7, 'g');
insert into col2 values (8, 'h');

insert into col4 values (1, 'a', 10, 'a10');
insert into col4 values (2, 'b', 20, 'b20');
insert into col4 values (3, 'c', 30, 'c30');
insert into col4 values (4, 'd', 40, 'd40');
insert into col4 values (5, 'e', 50, 'e50');
insert into col4 values (6, 'f', 60, 'f60');
insert into col4 values (7, 'g', 70, 'g70');
insert into col4 values (8, 'h', 80, 'h80');
