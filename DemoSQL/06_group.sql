-- group SQL statements demo
--
--   usage:
--   $ ucasdb ./06_group.sql



-- prepare: create table and insert  -- 

create table tab1 (id int, name varchar); 
create table tab2 (id int, count int, text varchar); 

insert into tab1 values (1, 'first record');
insert into tab1 values (2, 'second record');
insert into tab1 values (3, 'third record');
insert into tab1 values (1, 'first again');

insert into tab2 values (1, 10, 'tab 2 data');
insert into tab2 values (3, 20, 'tab 2 data');
insert into tab2 values (5, 30, 'tab 2 data');



-- correct statement -- 

select id, count(*) from tab1 where id > 0 group by id;

select tab1.id, max(count) from tab1, tab2 where tab1.id = tab2.id and count <= 30 group by tab1.id;




-- 0. aggregation column not int -- 
select id, avg(name) from tab1 where id > 0 group by id;

-- 1. data type mismatch -- 
select id, text from tab1 group by id;


