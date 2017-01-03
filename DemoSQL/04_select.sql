-- select SQL statements demo
--
--   usage:
--   already run 03_insert.sql
--   $ ucasdb ./04_select.sql



-- prepare statement -- 
create table crtDemo (id int, name varchar); 

insert into crtDemo values (1, 'first record');
insert into crtDemo values (2, 'second record');
insert into crtDemo values (3, 'third record');
insert into crtDemo values (1, 'first again');







-- correct statement -- 
select * from crtDemo;

select id from crtDemo;

select id from crtDemo where id = 1;

select id from crtDemo where name like 'first record';





-- wrong statement -- 


-- 0. syntax error  --
select id from crtDemo where ;

-- 1. table does not exist --
select * from TableNotExist;

-- 2. column does not exist --
select ColNotExist from crtDemo;

-- 3. operation mismatch --
select name from crtDemo where id like '1';



