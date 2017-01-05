-- join SQL statements demo
--
--   usage:
--   $ ucasdb ./05_join.sql



-- prepare: create table and insert  -- 

create table tab1 (id int, name varchar); 
create table tab2 (id int, grade int, text varchar); 

insert into tab1 values (1, 'first record');
insert into tab1 values (2, 'second record');
insert into tab1 values (3, 'third record');
insert into tab1 values (1, 'first again');

insert into tab2 values (1, 10, 'tab 2 data');
insert into tab2 values (3, 20, 'tab 2 data');
insert into tab2 values (5, 30, 'tab 2 data');



-- correct statement -- 

select * from tab1, tab2 where tab1.id = tab2.id and grade <= 30;

drop table tab1;
drop table tab2;


-- -- wrong statement -- 

-- -- 0. ambiguous column -- 
-- select * from tab1, tab2 where id = grade and grade <= 30;

-- -- 1. data type mismatch -- 
-- select * from tab1, tab2 where name = grade and grade <= 30;


